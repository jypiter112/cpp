#pragma once
#include <iostream>
#include <string>
using std::cout, std::endl, std::out_of_range;
/*
	Implementing a nullptr terminated singly linked list
*/
template<typename T>
class LinkedList {
private:
	struct Node {
		T item;
		Node* next = nullptr;
		Node(const T& value) : item(value) {} // constructor for Node struct
	};
	Node* head = nullptr; // first
	Node* tail = nullptr; // last
	size_t size = 0;
public:
	~LinkedList() {
		clear();
	}
	bool is_empty() {
		if (!head) {
			return true;
		}
		return false;
	}
	void push_back(const T& value);
	void push_front(const T& value);
	void clear();
	void insert(int i, const T& value);
	size_t length();
	void print();
	T get(int i);
};
template<typename T>
void LinkedList<T>::push_back(const T& value) {
	Node* new_node = new Node(value);
	if (is_empty()) {
		head = tail = new_node;
	}
	else {
		tail->next = new_node;
		tail = new_node;
	}
	++size;
}

template<typename T>
void LinkedList<T>::push_front(const T& value) {
	Node* new_node = new Node(value);
	if (is_empty()) {
		head = tail = new_node;
	}
	else {
		new_node->next = head;
		head = new_node;
	}
	++size;
}

template<typename T>
size_t LinkedList<T>::length() {
	return size;
}

template<typename T>
T LinkedList<T>::get(int i) {
	Node* itr = head;
	if (!head) {
		throw out_of_range("Calling nullptr");
	}
	int t = 0;
	while (itr) {
		if (t == i) {
			return itr->item;
		}
		itr = itr->next;
		t++;
	}
	// change to std::optinal later
	throw out_of_range("Index out of range");
}

/*
	Actually just replaces the value
*/
template<typename T>
void LinkedList<T>::insert(int i, const T& value) {
	Node* itr = head;
	if (!head) {
		throw out_of_range("Calling nullptr");
	}
	int t = 0;
	while (itr != nullptr) {
		if (t == i) {
			itr->item = value;
			return;
		}
		itr = itr->next;
		t++;
	}
	throw out_of_range("Index out of range");
}

template<typename T>
void LinkedList<T>::clear() {
	Node* itr = head;
	if (!head) {
		throw out_of_range("Calling nullptr");
	}
	while (itr != nullptr) { // head != nullptr
		Node* curr = itr;
		itr = itr->next;
		delete curr;
	}
	head = tail = nullptr;
	size = 0;
}

template<typename T>
void LinkedList<T>::print() {
	Node* itr = head;
	while (itr != nullptr) {
		cout << itr->item << endl;
		itr = itr->next;
	}
}
