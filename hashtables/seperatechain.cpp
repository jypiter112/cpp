#include <iostream>
#include <list>
#include <cstring>
using std::cout, std::hash, std::list, std::pair, std::string;
/*
  Seperate chain hahstable
  created with help of Coding Jesus' tutorial
  Credits:
  https://www.youtube.com/watch?v=2_3fR-k-LzI
*/


// hashtable to implement phonebook
// 905, jimmy
class HashTable {
private:
	static const int hashGroups = 10007; // prime
	list<pair<int, string>> table[hashGroups]; // List 1, index 0, List 2, index 1
public:
	bool isEmpty() const;
	int hashFunction(int key);
	void insertItem(int key, string value);
	void removeItem(int key);
	string searchTable(int key);
	void printTable();
};

bool HashTable::isEmpty() const {
	int sum{};
	for (int i{}; i < hashGroups; i++) {
		sum += table[i].size();
	}
	if (!sum) {
		return true;
	}
	return false;
}
int HashTable::hashFunction(int key) {
	return hash<int>{}(key) % hashGroups; // Key 905, in return this function spits out 5 (905 % 10 = 5)
}
void HashTable::insertItem(int key, string value) {
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
void HashTable::removeItem(int key) {
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
void HashTable::printTable() {
	for (int i{}; i < hashGroups; i++) {
		if (table[i].size() == 0) continue;
		auto bItr = table[i].begin();
		for (; bItr != table[i].end(); bItr++) {
			cout << "Key: " << bItr->first << "Value: " << bItr->second << "\n";
		}
	}
	return;
}
string HashTable::searchTable(int key) {
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

const bool testhashtable() {
	// allocate to heap to free up headspace
	HashTable* table = new HashTable();
	bool score = true;

	if (table->isEmpty()) {
		cout << "isEmpty works\n";
	}
	else {
		cout << "!!! isEmpty doesnt work?\n";
		score = false;
	}
	table->insertItem(905, "Jim");
	table->insertItem(913, "Bob");
	table->insertItem(933, "Jefferson");
	table->insertItem(102, "Sandy");
	table->insertItem(100, "Rock");
	table->insertItem(909, "Grock");
	table->insertItem(909, "Rick"); // replace grock with rick
	table->printTable();
	table->removeItem(913); // remove Bob
	table->removeItem(90); // random key not in table
	if (table->isEmpty()) {
		cout << "Uh oh. Error\n";
		score = false;
	}
	else {
		cout << "Correct!\n";
	}
	string result = table->searchTable(909);
	if (!result.empty()) {
		cout << "Number for 909 is: " << result << "\n";
	}
	else {
		cout << "Couldnt find 909?\n";
		score = false;
	}
	// printout info about our class
	cout << "HashTable size:" << sizeof(table) << "\n";
	delete table;
	return score;
}

int main() {
	bool score = testhashtable();
	if (!score) {
		cout << "Test failed.\n";
	}
	else {
		cout << "Test passed.\n";
	}
	return 0;
}
