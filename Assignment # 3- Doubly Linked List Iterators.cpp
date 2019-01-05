//Copyright 2017, Bradley Peterson, Weber State University, All rights reserved.

#include <iostream>
#include <map>
#include <algorithm>
#include <list>
#include <vector>

//To prevent those using g++ from trying to use a library
//they don't have
#ifndef __GNUC__
#include <conio.h>
#else
#include <stdio.h>
#endif

#include <sstream>

template <typename T> class DoublyLinkedList;

using std::cout;
using std::endl;
using std::string;
using std::vector;

//************************************************************************
//A class I designed to help keep track of how much memory you allocate
//Do not modify, this is not part of your assignment, it just helps test it.
//For this to work, a class needs to inherit off of this one. 
//Then this does the rest of the work, since it 
//overloads new, new[], delete, and delete[].
//************************************************************************
class ManageMemory {
public:

	static std::size_t getTotalSize() {
		std::size_t total = 0;
		std::map<void *, std::size_t>::iterator iter;
		for (iter = mapOfAllocations.begin(); iter != mapOfAllocations.end(); ++iter) {
			total += iter->second;
		}
		return total;
	}

	//I overloaded the new and delete keywords so I could manually track allocated memory.
	void* operator new(std::size_t x) {
		void *ptr = ::operator new(x);
		mapOfAllocations[ptr] = x;
		return ptr;
	}
	void* operator new[](std::size_t x) {
		void *ptr = ::operator new[](x);
		mapOfAllocations[ptr] = x;
		return ptr;
	}
		void operator delete(void* x) {
		mapOfAllocations.erase(x);
		::operator delete(x);
	}
	void operator delete[](void* x) {
		mapOfAllocations.erase(x);
		::operator delete[](x);
	}
private:
	static std::map<void *, std::size_t> mapOfAllocations;
};
std::map<void *, std::size_t> ManageMemory::mapOfAllocations;


//******************
//The node class
//******************
template <typename T>
class NodeType : public ManageMemory {
public:
	T info;
	NodeType *forward;
	NodeType *backward;
};


//******************
//The Iterator class
//******************	
template <typename T>
class Iterator : public ManageMemory, public std::iterator<std::bidirectional_iterator_tag, T> {
	friend class DoublyLinkedList<T>;
public:
	//TODO: Implement the following:
	T& operator*();
	Iterator operator++(); //prefix return *this;
	Iterator operator++(int);// postfix auto temp = *this;  move the "this"...;   return temp;
	Iterator operator--(); //prefix
	bool operator!=(const Iterator& rhs)const; //right hand side 
	bool operator==(const Iterator& rhs)const;

private:
	NodeType<T> *ptr{ nullptr };
	bool offTheRightCliff{ false };
	//You need a pointer to the node and a boolean offTheRightCliff (see lecture video)
};

//***********************************
//The Iterator class methods
//***********************************

//TODO, code the definitions for all the iterator methods. 

template<typename T>
T& Iterator<T>::operator*()
{
	return ptr->info;
}

template<typename T>
Iterator<T> Iterator<T>::operator++()
{
	if (offTheRightCliff == true)
	{
		return *this;
	}

	if (this->ptr->forward == nullptr)
	{
		this->offTheRightCliff = true;
	}

	if (offTheRightCliff == false)
	{
		this->ptr = this->ptr->forward;
	}


	return *this;
}

template<typename T>
Iterator<T> Iterator<T>::operator++(int)
{
	Iterator<T> temp = *this;
	if (offTheRightCliff == true)
	{
		return *this;
	}

	if (this->ptr->forward == nullptr)
	{
		this->offTheRightCliff = true;
	}

	if (offTheRightCliff == false)
	{
		this->ptr = this->ptr->forward;
	}

	return temp;
}

template<typename T>
Iterator<T> Iterator<T>::operator--()
{
	if (this->offTheRightCliff == true)
	{
		this->offTheRightCliff = false;
	}
	else 
	{	
		this->ptr = this->ptr->backward;
		this->offTheRightCliff = false;
		
	}


	return *this;
}

template<typename T>
bool Iterator<T>::operator!=(const Iterator & rhs) const
{
	if ((this->ptr == rhs.ptr) && (this->offTheRightCliff == rhs.offTheRightCliff))
	{
		return false;
	}

	return true;
}

template<typename T>
bool Iterator<T>::operator==(const Iterator & rhs) const
{
	if ((this->ptr == rhs.ptr) && (this->offTheRightCliff == rhs.offTheRightCliff))
	{
		return true;
	}

	return false;
}

//****************************
//The DoublyLinkedList class
//****************************
template <typename T>
class DoublyLinkedList : public ManageMemory {

public:

	//public members of the DoublyLinkedList class
	DoublyLinkedList();
	~DoublyLinkedList();
	string getStringFromList();

	void insertFirst(const T&);
	void insertLast(const T&);
	Iterator<T> begin();
	Iterator<T> end();
	//TODO: Implement a begin() and end() method

protected:
	NodeType<T> *first;
	NodeType<T> *last;
	int count;
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
	count = 0;
	first = nullptr;
	last = nullptr;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {

	NodeType<T> *temp = nullptr;

	while (first != nullptr) {
		temp = first;
		first = first->forward;
		delete temp;
	}
}

template <typename T>
void DoublyLinkedList<T>::insertFirst(const T& info) {
	NodeType<T> *temp = new NodeType<T>();
	temp->info = info;
	temp->forward = nullptr;
	temp->backward = nullptr;

	if (first == nullptr) {
		//this list empty!
		last = temp;
	}
	else {
		//put temp before first
		temp->forward = first;
		first->backward = temp;

	}
	first = temp;
	count++;
}

template <typename T>
void DoublyLinkedList<T>::insertLast(const T& info) {
	NodeType<T> *temp = new NodeType<T>();
	temp->info = info;
	temp->forward = nullptr;

	if (first == nullptr) {
		//this list empty!
		first = temp;
	}
	else {
		//put temp after last
		last->forward = temp;
		temp->backward = last;
	}
	last = temp;
	count++;
}



//This method helps return a string representation of all nodes in the linked list
template <typename T>
string DoublyLinkedList<T>::getStringFromList() {
	stringstream ss;
	if (first == nullptr) {
		ss << "The list is empty.";
	}
	else {

		NodeType<T> *currentNode = first;
		ss << currentNode->info;
		currentNode = currentNode->forward;

		while (currentNode != nullptr) {
			ss << " " << currentNode->info;
			currentNode = currentNode->forward;
		}
	}
	return ss.str();
}

//***********************************
//The DoublyLinkedList class methods
//***********************************
//TODO, implement the  the definitions for the begin() and end() methods
template <typename T>
Iterator<T> DoublyLinkedList<T>::begin()
{
	Iterator<T> iterObj;
	iterObj.offTheRightCliff = false;
	if (first == nullptr)
	{
		iterObj.offTheRightCliff = true;
	}
	else
	{
		iterObj.offTheRightCliff = false;
	}

	iterObj.ptr = first;
	return iterObj;


}

template <typename T>
Iterator<T> DoublyLinkedList<T>::end()
{
	Iterator<T> iterObj;
	iterObj.offTheRightCliff = true;
	iterObj.ptr = last;
	return iterObj;

}


//***********************************
//TODO, complete the ourReverse function.
//Note that begin and end here are are iterators (The T is just saying it can be any kind of iterator).  
//Our tools are ***ONLY***
//++, --, !=, *
//For example,
//auto temp = *end;
//*end = *begin;
//*begin = temp;
//++begin;
//--end;
template <typename T>
void ourReverse(T begin, T end) {
	--end;
	T copy = end;

	while (begin != end)
	{
		copy = end;
		iter_swap(begin, end);
		++begin;
		if (begin == end)
		{
			return;
		}

		--end;
	}

	return;
}


//----------------------------------------------------------------------------------------------------------------------------------------
//This helps with testing, do not modify.
bool checkTest(string testName, string whatItShouldBe, string whatItIs) {

	if (whatItShouldBe == whatItIs) {
		cout << "Passed " << testName << endl;
		return true;
	}
	else {
		cout << "****** Failed test " << testName << " ****** " << endl << "     Output was " << whatItIs << endl << "     Output should have been " << whatItShouldBe << endl;
		return false;
	}
}

//This helps with testing, do not modify.
bool checkTest(string testName, int whatItShouldBe, int whatItIs) {

	if (whatItShouldBe == whatItIs) {
		cout << "Passed " << testName << endl;
		return true;
	}
	else {
		cout << "****** Failed test " << testName << " ****** " << endl << "     Output was " << whatItIs << endl << "     Output should have been " << whatItShouldBe << endl;
		return false;
	}
}

//This helps with testing, do not modify.
bool checkTestMemory(string testName, int whatItShouldBe, int whatItIs) {

	if (whatItShouldBe == whatItIs) {
		cout << "Passed " << testName << endl;
		return true;
	}
	else {
		cout << "***Failed test " << testName << " *** " << endl << "  There are " << whatItIs << " bytes of memory yet to be reclaimed!" << endl;
		return false;
	}
}


//This helps with testing, do not modify.
void testIteratorFundamentals() {

	DoublyLinkedList<int> d;

	//Our test data should have all even numbers from 2 to 20
	for (int i = 2; i <= 20; i += 2) {
		d.insertLast(i);
	}

	//Get an iterator which points at the beginning of the list
	Iterator<int> iter = d.begin();

	//Test the operator* operator
	checkTest("testIteratorFundamentals #1", 2, *iter);

	//Test the ++ prefix increment opreator
	++iter;
	checkTest("testIteratorFundamentals #2", 4, *iter);

	//Test the != operator
	//reset them back to the start
	iter = d.begin();
	Iterator<int> anotherIter = d.begin();
	if (iter != anotherIter) {
		cout << "****** Failed testIteratorFundamentals #3 ****** " << endl << "     The two iteraters held the same data." << endl;
	}
	else {
		cout << "Passed testIteratorFundamentals #3" << endl;
	}

	//Again test the != operator
	++iter;
	if (iter != anotherIter) {
		cout << "Passed testIteratorFundamentals #4" << endl;
	}
	else {
		cout << "****** Failed testIteratorFundamentals #4 ****** " << endl << "     The two iteraters held different data." << endl;
	}

	//Test the ++postfix increment
	iter = d.begin(); //reset it back to the start
	anotherIter = iter++;  //anotherIter should be at the data 2


	checkTest("testIteratorFundamentals #5", 4, *iter);
	checkTest("testIteratorFundamentals #6", 2, *anotherIter);

	cout << "testIteratorFundamentals test #7, this should display 2 4 6 8 10 12 14 16 18 20." << endl;

	for (auto i : d) {
		cout << i << " ";
	}
	cout << endl;

}

//This helps with testing, do not modify.
void testIteratorIncrement() {

	DoublyLinkedList<int> *d = new DoublyLinkedList<int>;

	//Our test data should have all even numbers from 2 to 20
	for (int i = 2; i <= 20; i += 2) {
		d->insertLast(i);
	}

	//Get an iterator which points at the beginning of the list
	Iterator<int> iter = d->begin();

	//Test that it does point to the first
	checkTest("testIteratorsIncrement #1", 2, *iter);

	//Test that our Iterator can move forward;
	++iter;	checkTest("testIteratorsIncrement #2", 4, *iter);


	//Test that our Iterator can move forward again;
	++iter;
	checkTest("testIteratorsIncrement #3", 6, *iter);

	//move it some more
	for (int i = 0; i < 6; i++) {
		++iter;
	}
	checkTest("testIteratorsIncrement #4", 18, *iter);

	//Hit the end
	++iter;
	checkTest("testIteratorsIncrement #5", 20, *iter);

	//Verify we move the iterator past the end without crashing
	++iter;
	checkTest("testIteratorsIncrement #6", "did not crash", "did not crash");


	delete d;
}

//This helps with testing, do not modify.
void testIteratorDecrement() {
	cout << "|||### You need to comment in testIteratorDecrement() when it's ready and remove this###|||" << endl;

	DoublyLinkedList<int> *d = new DoublyLinkedList<int>;

	//Our test data should have all even numbers from 2 to 20
	for (int i = 2; i <= 20; i += 2) {
		d->insertLast(i);
	}

	//Get an Iterator which points at the end of the list
	Iterator<int> iter = d->end();
	--iter;  //We have to do a decrement otherwise it crashes

			 //Test that it does point to the first
	checkTest("testIteratorsDecrement #1", 20, *iter);

	//Test that our Iterator can move forward;
	--iter;
	checkTest("testIteratorsDecrement #2", 18, *iter);

	//move it some more
	for (int i = 0; i < 7; i++) {
		--iter;
	}
	checkTest("testIteratorsDecrement #3", 4, *iter);

	//Hit the end
	--iter;
	checkTest("testIteratorsDecrement #4", 2, *iter);

	//Now go back forward
	++iter;
	checkTest("testIteratorsDecrement #5", 4, *iter);

	delete d;
}

//This helps with testing, do not modify.
void testIterationTricky() {

	cout << "|||### You need to comment in testIterationTricky() when it's ready and remove this###|||" << endl;

	DoublyLinkedList<int> myListOneNode;

	myListOneNode.insertLast(42);
	cout << "TestIterationTricky test #1, the next line should display 42" << endl;
	//see if we can just iterator through one item.
	for (auto i : myListOneNode) {
		cout << i << " ";
	}
	cout << endl;

	DoublyLinkedList<int> myListEmpty;
	cout << "TestIterationTricky test #2, the next line shouldn't display anything" << endl;
	//see if we can just iterator through one item.
	for (auto i : myListEmpty) {
		cout << i << " ";
	}
	cout << endl;

}


//This helps with testing, do not modify.
void testAlgorithms() {


	DoublyLinkedList<int> myList;

	//Our test data should have all even numbers from 2 to 20
	for (int i = 2; i <= 20; i += 2) {
		myList.insertLast(i);
	}

	cout << "testAlgorithms test #1, this should display 2 4 6 8 10 12 14 16 18 20." << endl;
	for (auto i : myList) {
		cout << i << " ";
	}
	cout << endl;

	//Test the STL reverse algorithm on our iterator
	cout << "testAlgorithms test #2, this should display 20 18 16 14 12 10 8 6 4 2." << endl;
	reverse(myList.begin(), myList.end());
	for (auto i : myList) {
		cout << i << " ";
	}
	cout << endl;


	//Test our reverse algorithm
	//get it back in ascending order
	reverse(myList.begin(), myList.end());

	//Now reverse it.
	cout << "testAlgorithms test #3, this should display 20 18 16 14 12 10 8 6 4 2." << endl;
	ourReverse(myList.begin(), myList.end());
	for (auto i : myList) {
		cout << i << " ";
	}
	cout << endl;

	//Test our reverse algorithm on an STL collection
	vector<int> v;
	for (int i = 2; i <= 20; i += 2) {
		v.push_back(i);
	}
	cout << "testAlgorithms test #4, this should display 20 18 16 14 12 10 8 6 4 2." << endl;
	ourReverse(v.begin(), v.end());
	for (auto i : v) {
		cout << i << " ";
	}
	cout << endl;


	//Now test an odd sized list
	DoublyLinkedList<int> oddList;
	for (int i = 2; i <= 18; i += 2) {
		oddList.insertLast(i);
	}
	cout << "testAlgorithms test #5, this should display 18 16 14 12 10 8 6 4 2." << endl;
	ourReverse(oddList.begin(), oddList.end());
	for (auto i : oddList) {
		cout << i << " ";
	}
	cout << endl;

	//Delete all of occurrence using an STL algorithm.
	cout << "testAlgorithms test #6, this should display 4 2 5 1 2." << endl;
	DoublyLinkedList<int> manyNines;
	manyNines.insertLast(9);
	manyNines.insertLast(9);
	manyNines.insertLast(4);
	manyNines.insertLast(2);
	manyNines.insertLast(9);
	manyNines.insertLast(9);
	manyNines.insertLast(5);
	manyNines.insertLast(1);
	manyNines.insertLast(9);
	manyNines.insertLast(2);
	manyNines.insertLast(9);
	manyNines.insertLast(9);
	auto beginIter = manyNines.begin();
	auto endIter = manyNines.end();
	endIter = remove(beginIter, endIter, 9);

	while (beginIter != endIter) {
		cout << *beginIter << " ";
		++beginIter;
	}
	cout << endl;

}

void pressAnyKeyToContinue() {
	cout << "Press any key to continue...";

	//Linux and Mac users with g++ don't need this
	//But everyone else will see this message.
#ifndef __GNUC__
	_getch();

#else
	int c;
	fflush(stdout);
	do c = getchar(); while ((c != '\n') && (c != EOF));
#endif
	cout << endl;
}

int main() {

	cout << "This first test can run forever until you get operators *, != and ++ implemented." << endl;
	pressAnyKeyToContinue();

	testIteratorFundamentals();
	checkTestMemory("Memory Leak/Allocation Test #1", 0, ManageMemory::getTotalSize());
	pressAnyKeyToContinue();

	testIteratorIncrement();
	checkTestMemory("Memory Leak/Allocation Test #2", 0, ManageMemory::getTotalSize());
	pressAnyKeyToContinue();

	testIteratorDecrement();
	checkTestMemory("Memory Leak/Allocation Test #3", 0, ManageMemory::getTotalSize());
	pressAnyKeyToContinue();

	testIterationTricky();
	checkTestMemory("Memory Leak/Allocation Test #4", 0, ManageMemory::getTotalSize());
	pressAnyKeyToContinue();

	testAlgorithms();
	checkTestMemory("Memory Leak/Allocation Test #5", 0, ManageMemory::getTotalSize());
	pressAnyKeyToContinue();

	return 0;
}