#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <cstring>
#include <optional>
using std::cout, std::hash, std::list, std::pair, std::string, std::vector;
/*
	This whole project was for understanding how hashmaps work and
	working with cpp templates and optionals.

	Seperate chaining some functiosn marked as virtual, because at first I thought of using class heritance in 
	the opena addressing hashtable, but soon realised that doesnt that isnt useful at all in this case.
*/


/*
https://en.cppreference.com/w/cpp/utility/unreachable.html
Some compiler opitmization I wanted to try. Im not sure if this even works.
*/
[[noreturn]] inline void _unreachable_() {
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
	__assume(false);
#else // GCC, clang
	__builtin_unreachable();
#endif
}

// hashtable to implement phonebook
// 905, jimmy
namespace HT {
	/*
		Seperate chaining hashtable
		"Primitive"
		Credits:
		https://www.youtube.com/watch?v=2_3fR-k-LzI
	*/
	class HashTable {
	private:
		static const int hashGroups = 10007; // prime
		list<pair<int, string>> table[hashGroups]; // List 1, index 0, List 2, index 1
	public:
		bool isEmpty() const {
			int sum{};
			for (int i{}; i < hashGroups; i++) {
				sum += table[i].size();
			}
			if (!sum) {
				return true;
			}
			return false;
		}
		int hashFunction(int key) {
			return hash<int>{}(key) % hashGroups; // Key 905, in return this function spits out 5 (905 % 10 = 5)
		}
		virtual void insertItem(int key, string value) {
			// findout which list this key, value pair goes into
			int hashValue = hashFunction(key);
			// list
			auto& cell = table[hashValue];
			auto bItr = begin(cell);
			bool keyExists = false;
			// assume key doesn't exist
			// if exists, replace with new value
			for (; bItr != end(cell); bItr++) {
				if (bItr->first == key) {
					keyExists = true;
					// replace value
					bItr->second = value;
					cout << "Key exists: Value replaced.\n";
					break;
				}
			}
			if (!keyExists) {
				cell.emplace_back(key, value);
			}
			return;
		}
		virtual void removeItem(int key) {
			// findout which list this key, value pair goes into
			int hashValue = hashFunction(key);
			// list
			auto& cell = table[hashValue];
			auto bItr = begin(cell);
			bool keyExists = false;
			// assume key doesn't exist
			// if exists, replace with new value
			for (; bItr != end(cell); bItr++) {
				if (bItr->first == key) {
					keyExists = true;
					// remove value
					// return value has to point to something, otherwise seg fault possible
					bItr = cell.erase(bItr);
					cout << "Value erased.\n";
					break;
				}
			}
			if (!keyExists) {
				cout << "Item not found\n";
			}
			return;
		}
		void printTable() {
			for (int i{}; i < hashGroups; i++) {
				if (table[i].size() == 0) continue;
				auto bItr = table[i].begin();
				for (; bItr != table[i].end(); bItr++) {
					cout << "Key: " << bItr->first << "Value: " << bItr->second << "\n";
				}
			}
			return;
		}
		virtual string searchTable(int key) {
			// findout which list this key, value pair goes into
			string value{};
			int hashValue = hashFunction(key);
			// list
			auto& cell = table[hashValue];
			auto bItr = begin(cell);
			bool keyExists = false;
			// assume key doesn't exist
			// if exists, replace with new value
			for (; bItr != end(cell); bItr++) {
				if (bItr->first == key) {
					keyExists = true;
					value = (string)bItr->second;
					cout << "Key found. Returning value\n";
					break;
				}
			}
			if (!keyExists) {
				cout << "Key not found. Value doesnt exist\n";
			}
			return value;
		}
	};
	/*
		Open addressing hash table
		linear probing for collision handling
	*/
	template<typename value_>
	class OAHashTable {
	private:
		struct Cell {
			int key;
			value_ value;
			bool occupied = false;
			bool deleted = false;
			Cell() = default;
			Cell(const int& key_, const value_& val_) : key(key_), value(val_), occupied(true), deleted(false){}
		};
		vector<Cell> table;
		size_t count = 0;
	public:
		// has to be initialized
		explicit OAHashTable(size_t size_ = 107) :
			table(size_), count(0) {}
		/*
			This doesnt work as of yet
			count isnt being tracked well?
		*/
		bool isEmpty() const {
			return (!count);
		}
		int hashFunction(int key) {
			return std::hash<int>{}(key) % table.size();
		}
		void insertItem(int key, value_ value) {
			if (table.empty())
				throw std::runtime_error("Hash table not initialized.");

			// compute hash
			int hash = hashFunction(key);
			if (hash >= table.size()) throw std::out_of_range("Hash index out of range.");
			
			// case: collision
			/*
				Handle collision
				Move forward (i++) until
				- End of table -> wrap around
					Twice: throw exception
				- Empty or deleted slot (insert)
				- Same key (update)
			*/
			// start one index forward
			int start = hash;
			while (true) {
				Cell* c = &table[hash];

				// update existing value
				if (c->occupied && !c->deleted && c->key == key) {
					c->value = value;
					return;
				}
				// empty or deleted or new
				if (!c->occupied || c->deleted) {
					c->key = key;
					c->value = value;
					c->occupied = true;
					c->deleted = false;
					count++; // --- update count here for new item
					return;
				}
				// wrap around
				hash = (hash + 1) % table.size(); // not sure if table.size() is good here or not?
				if (hash == start) {
					throw std::overflow_error("Hash table overflow");
				}
			}
		}
		bool removeItem(int key) {
			if (table.empty())
				throw std::runtime_error("Hash table not initialized.");

			// compute hash
			int hash = hashFunction(key);

			// linear probing
			int start = hash;
			while (true) {
				Cell* c = &table[hash];
				// empty slot so no key found
				if (!c->occupied && !c->deleted) {
					return false;
				}
				// existing value found
				if (c->occupied && c->key == key) {
					c->deleted = true;
					c->occupied = false;
					c->value = value_{};
					c->key = 0;
					count--; // -- update count here
					return true;
				}

				// wrap around
				hash = (hash + 1) % table.size(); // not sure if table.size() is good here or not?
				if (hash == start) {
					return false;
				}
			}
			// std++23 std::unreachable, cpprefe impl:
			_unreachable_();
			return false;
		}
		std::optional<value_> searchTable(int key) {
			if (table.empty())
				throw std::runtime_error("Hash table not initialized.");

			// compute hash
			int hash = hashFunction(key);
			if (hash >= table.size()) throw std::out_of_range("Hash index out of range.");

			// linear probing
			int start = hash;
			while (true) {
				Cell* c = &table[hash];

				// existing value found
				if (c->occupied && !c->deleted && c->key == key) {
					return c->value; // ?
				}
				// empty or deleted
				if (!c->occupied || c->deleted) {
					return std::nullopt;
				}
				// wrap around
				hash = (hash + 1) % table.size(); // not sure if table.size() is good here or not?
				if (hash == start) {
					return std::nullopt;
				}
			}
		}
	};
}
