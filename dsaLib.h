/* Created by Nguyen Duc Dung on 2019-09-03.
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - HCMUT
 * Description : The data structure library for Assignment 1
 * Course      : Data Structure and Algorithms - Fall 2019
 * =========================================================================================
 */

#ifndef DSA191_A1_DSALIB_H
#define DSA191_A1_DSALIB_H
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <typename T>
struct L1Item {
	T data;
	L1Item<T> * pNext;
	L1Item(T value) : data(value), pNext(nullptr) {};
	L1Item() {};
};


template <typename T>
class L1List {
public:
	L1Item<T> *pHead;
	L1Item<T> *pTail;
	int size;
	L1List() : pHead(nullptr), pTail(nullptr), size(0) {};

	bool push(const T &value) {
		if (size == 0) {
			pHead = new L1Item<T>(value);
			pTail = pHead;
		}
		else {
			L1Item<T> *pTemp = new L1Item<T>(value);
			pTail->pNext = pTemp;
			pTail = pTemp;
		}

		size++;
		return true;
	}


	bool shift() {
		if (size < 0) return false;
		else if (size == 1) {
			pHead = nullptr;
			pTail = nullptr;
		}
		else {
			pHead = pHead->pNext;

		}
		size--;
		return true;
	}

	bool pop() {
		if (size == 0) return false;
		else if (size == 1) {
			pHead = nullptr;
			pTail = nullptr;
		}
		else {
			L1Item<T> *temp = pHead;
			while (temp->pNext->pNext != nullptr) { temp = temp->pNext; }
			temp->pNext = nullptr;
			pTail = temp;
		}
		size--;
		return true;
	}

	bool unshift(T value) {
		if (size == 0) {
			pHead = new L1Item<T>(value);
			pTail = pHead;
		}
		else {
			L1Item<T> *pTemp = pHead;
			pHead = new L1Item<T>(value);
			pHead->pNext = pTemp;
		}

		size++;
		return true;
	}

	L1Item<T>* get(int index) {
		if (size < 1) return nullptr;
		else {
			if (index < 0) index = 0;
			else if (index > size - 1)  index = size - 1;
		}
		int count = 0;
		L1Item<T>* temp = pHead;
		while (count < index) {
			temp = temp->pNext;
			count++;
		}
		return temp;
	}

	bool remove(int index) {
		if (size < 1) return false;
		else if (index < 0) index = 0;
		else if (index > size - 1) index = size - 1;

		if (index == 0) this->shift();
		else if (index == size - 1) this->pop();
		else {
			L1Item<T>* prev = this->get(index - 1);
			L1Item<T>* next = prev->pNext->pNext;
			prev->pNext = next;
			size--;
		}
		return true;
	}


	bool insert(int index, T value) {
		if (index > size || index < 0) return false; 
		/*if (index > size) index = size; 
		if (index < 0) index = 0; */


		if (index == 0) {
			unshift(value);
			return true;
		}
		if (index == size) {
			push(value);
			return true;
		}

		else {
			L1Item<T> *pNewItem = new L1Item<T>(value);
			L1Item<T> *prev = get(index - 1);
			L1Item<T> *next = prev->pNext;
			prev->pNext = pNewItem;
			pNewItem->pNext = next;
			size++;
		}

		return true;
	
	}

};


#endif //DSA191_A1_DSALIB_H

