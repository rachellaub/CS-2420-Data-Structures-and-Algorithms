//Copyright 2017, Bradley Peterson, Weber State University, All rights reserved.

//To prevent those using g++ from trying to use a library
//they don't have
#ifndef __GNUC__
#include <conio.h>
#else
#include <cstdio>
#endif

#include <sstream>
#include <map>
#include <chrono>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

//************************************************************************
//A class I designed to help keep track of how much memory you allocate
//Do not modify, this is not part of your assignment, it just helps test it.
//For this to work, a class needs to inherit off of this one. 
//Then this does the rest of the work, since it 
//overloads new, new[], delete, and delete[].
//************************************************************************
class manageMemory {
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
std::map<void *, std::size_t> manageMemory::mapOfAllocations;



//******************
//The node class
//******************
template <typename T>
class nodeType : public manageMemory {
public:
	T info;
	nodeType *link;
};


//******************
//The linked list base class
//This contains within it a class declaration for an iterator
//******************
template <typename T>
class singlyLinkedList : public manageMemory {
public:

	//public members of the singlyLinkedList class
	singlyLinkedList();
	~singlyLinkedList();
	string getStringFromList();

	void insertFirst(const T&);
	void insertLast(const T&);
	T getAtIndex(const unsigned int index);                          //For your assignment
	T& operator[](const unsigned int index);                         //For your assignment
	void insertAtIndex(const unsigned int index, const T& value);    //For your assignment
	void deleteAtIndex(const unsigned int index);                    //For your assignment
	void deleteAllInstances(const T& value);                      //For your assignment
	void deleteFirst();
	void deleteLast();


protected:
	nodeType<T> *first;
	nodeType<T> *last;
	int count;

};


template <typename T> //constructor
singlyLinkedList<T>::singlyLinkedList() {
	first = NULL;
	last = NULL;
	count = 0;
}

template <typename T>// destructor
singlyLinkedList<T>::~singlyLinkedList() {
	nodeType<T> *temp;
	while (first != NULL) {
		temp = first;
		first = first->link;
		delete temp;
	}
	last = NULL;
	count = 0;
}

template <typename T>
void singlyLinkedList<T>::insertFirst(const T& value) {
	nodeType<T> *temp = new nodeType<T>();
	temp->info = value;
	temp->link = NULL;

	//empty list scenario
	if (first == NULL) {
		last = temp;
	}
	else {
		temp->link = first;
	}

	first = temp;
	count++;
}

template <typename T>
void singlyLinkedList<T>::insertLast(const T& value) {
	nodeType<T> *temp = new nodeType<T>;
	temp->info = value;
	temp->link = NULL;

	if (first == NULL) {
		first = temp;
	}
	else {
		//put it on
		last->link = temp;
	}
	last = temp;
	count++;
}


template <typename T>
void singlyLinkedList<T>::deleteFirst()
{
	if (first == nullptr)
	{
		//empty list scenario
		return;
	}
	if (first == last)
	{
		//one node in list scenario
		delete first;
		first = nullptr;
		last = nullptr;
		count--;
		return;
	}

	nodeType<T>* temp = first; //only want a pointer
	first = first->link;	//forwards a spot. -> is a "dereference and then a "get my member"
	delete temp; //deleting address at temp
	temp = nullptr;
	count--;
}

template <typename T>
void singlyLinkedList<T>::deleteLast()
{
	if (first == nullptr)
	{
		//empty list scenario
		return;
	}
	if (first == last)
	{
		//one node in list scenario
		delete first;
		first = nullptr;
		last = nullptr;
		count--;
		return;
	}

	nodeType<T>* cur = first;
	// dont actually need Node<T>* temp = back;

	//get cur to the second to last node
	while (cur->link != last) {
		cur = cur->link;
	}

	delete last;
	last = cur;
	cur->link = nullptr;
	count--;


}



//TODO: Complete this method
template <typename T>
T singlyLinkedList<T>::getAtIndex(const unsigned int index)
{

	if (count == 0)
	{
		throw 1;
	}
	if (index >= count)
	{
		throw 1;
	}

	nodeType<T>* temp = first;
	for (int i = 0; i < index; i++)
	{
		temp = temp->link;
	}



	return temp->info;
	//This is not what you should return, but this just helps it compile
	return last->info;
}

//TODO: Complete this method
template <typename T>
T& singlyLinkedList<T>::operator[](const unsigned int index)
{
	if (count == 0)
	{
		throw 1;
	}
	if (index >= count)
	{
		throw 1;
	}

	nodeType<T>* temp = first;
	for (int i = 0; i < index; i++)
	{
		temp = temp->link;
	}
	//return temp->info;



	return temp->info;
	//This is not what you should return, but this just helps it compile
	return last->info;
}

//TODO: Complete this method
template <typename T>
void singlyLinkedList<T>::insertAtIndex(const unsigned int index, const T& value)
{
	if (index > count)
	{
		cerr << "Out of bounds." << endl;
		return;
	}

	nodeType<T>* temp = new nodeType<T>();
	temp->info = value;
	if (index == 0 && count == 0)
	{	//empty and index zero
		first = temp;
		last = temp;
	}
	else if (index == 0)
	{	//left end scenario
		temp->link = first;
		first = temp;
	}
	else if (index == count)
	{	//right side scenario
		last->link = temp;
		last = temp;
	}
	else
	{
		nodeType<T>* cur = first;

		for (int i = 0; i < index - 1; i++)
		{
			cur = cur->link;
		}

		temp->link = cur->link;
		cur->link = temp;
	}
	count++;
	return;

}

//TODO: Complete this method
template <typename T>
void singlyLinkedList<T>::deleteAtIndex(const unsigned int index)
{
	if (index >= count)
	{
		cerr << "Error." << endl;
		return;
	}
	else if (index == 0 && first == last)
	{	//one node
		delete first;
		first = nullptr;
		last = nullptr;
		count = 0;
	}
	else if (index == 0)
	{	//left scenario
		deleteFirst();
	}
	else if (count - 1 == index)
	{
		//right scenario
		deleteLast();
	}
	else
	{	//1:04:54
		nodeType<T>* prev = first;
		for (int i = 0; i != index - 1; i++)
		{
			prev = prev->link;
		}
		nodeType<T>* cur = prev->link;
		prev->link = cur->link;
		delete cur;
	}

}

//TODO: Complete this method
template <typename T>
void singlyLinkedList<T>::deleteAllInstances(const T& value)
{
	if (first == nullptr)
	{
		cerr << "Empty" << endl;
		return;
	}
	nodeType<T>* cur = first;
	if (first == last && cur->info == value)
	{
		//one node
		delete first;
		first = nullptr;
		last = nullptr;
		count = 0;
	}
	else
	{
		nodeType<T>* prev = first;
		while (cur != nullptr)
		{
			if (cur->info == value)
			{
				if (cur == first)
				{
					//left side
					deleteFirst();
					cur = first;
					prev = nullptr;
				}
				else if (cur != last)
				{
					//middle
					prev->link = cur->link;
					delete cur;
					cur = prev->link;
					count--;
				}
				else if (cur == last)
				{
					//right side
					deleteLast();
					cur = last;
					prev = nullptr;

				}
			}
			else
			{
				prev = cur;
				cur = cur->link;
			}

		}
	}
	return;

}

//This method helps return a string representation of all nodes in the linked list, do not modify.
template <typename T>
string singlyLinkedList<T>::getStringFromList() {
	stringstream ss;
	if (first == NULL) {
		ss << "The list is empty.";
	}
	else {

		nodeType<T> *currentNode = first;
		ss << currentNode->info;
		currentNode = currentNode->link;

		while (currentNode != NULL) {
			ss << " " << currentNode->info;
			currentNode = currentNode->link;
		};
	}
	return ss.str();
}

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
		cout << "***Failed test " << testName << " *** " << endl << "  You lost track of " << whatItIs << " bytes in memory!" << endl;
		return false;
	}
}

//This helps with testing, do not modify.
void testGetAtIndex() {
	singlyLinkedList<int> *d = new singlyLinkedList<int>;
	for (int i = 10; i < 20; i++) {
		d->insertLast(i);
	}

	//Test just to make sure the data went in the list. 
	checkTest("testGetAtIndex #1", "10 11 12 13 14 15 16 17 18 19", d->getStringFromList());

	//Test retrieving items. 
	int item = d->getAtIndex(0);
	checkTest("testGetAtIndex #2", 10, item);

	item = d->getAtIndex(5);
	checkTest("testGetAtIndex #3", 15, item);

	item = d->getAtIndex(9);
	checkTest("testGetAtIndex #4", 19, item);

	//Make sure the list was undisturbed during this time
	checkTest("testGetAtIndex #5", "10 11 12 13 14 15 16 17 18 19", d->getStringFromList());

	//Try to access out of bounds.  
	string caughtError = "";
	try {
		int item = d->getAtIndex(-1);
	}
	catch (int error) {
		caughtError = "caught";
	}
	checkTest("testGetAtIndex #6", "caught", caughtError);

	try {
		int item = d->getAtIndex(100);
	}
	catch (int error) {
		caughtError = "caught";
	}
	checkTest("testGetAtIndex #7", "caught", caughtError);

	delete d;

	d = new singlyLinkedList<int>;
	d->insertLast(18);
	item = d->getAtIndex(0);
	checkTest("testGetAtIndex #8", 18, item);
	delete d;
}

//This helps with testing, do not modify.
void testSquareBrackets() {
	singlyLinkedList<int> d;
	for (int i = 10; i < 20; i++) {
		d.insertLast(i);
	}

	//Test just to make sure the data went in the list. 
	checkTest("testSquareBrackets #1", "10 11 12 13 14 15 16 17 18 19", d.getStringFromList());

	//Test retrieving items. 
	int item = d[0];
	checkTest("testSquareBrackets #2", 10, item);

	item = d[5];
	checkTest("testSquareBrackets #3", 15, item);

	item = d[9];
	checkTest("testSquareBrackets #4", 19, item);

	//Make sure the list was undisturbed during this time
	checkTest("testSquareBrackets #5", "10 11 12 13 14 15 16 17 18 19", d.getStringFromList());

	//now test the return by reference
	d[1] = 1000;
	checkTest("testSquareBrackets #6", "10 1000 12 13 14 15 16 17 18 19", d.getStringFromList());

	//Try to access out of bounds.  
	string caughtError = "";
	try {
		int item = d[-1];
	}
	catch (int error) {
		caughtError = "caught";
	}
	checkTest("testSquareBrackets #7", "caught", caughtError);

	try {
		int item = d[100];
	}
	catch (int error) {
		caughtError = "caught";
	}
	checkTest("testSquareBrackets #8", "caught", caughtError);

}

//This helps with testing, do not modify.
void testInsertAtIndex() {
	singlyLinkedList<int> *s = new singlyLinkedList<int>;
	for (int i = 10; i < 20; i++) {
		s->insertLast(i);
	}

	//Test just to make sure the data went in the list. 
	checkTest("testInsertAtIndex #1", "10 11 12 13 14 15 16 17 18 19", s->getStringFromList());

	s->insertAtIndex(3, 33);

	checkTest("testInsertAtIndex #2", "10 11 12 33 13 14 15 16 17 18 19", s->getStringFromList());

	s->insertAtIndex(0, 9);

	checkTest("testInsertAtIndex #3", "9 10 11 12 33 13 14 15 16 17 18 19", s->getStringFromList());

	s->insertAtIndex(12, 20);

	checkTest("testInsertAtIndex #4", "9 10 11 12 33 13 14 15 16 17 18 19 20", s->getStringFromList());

	delete s;

	s = new singlyLinkedList<int>;

	s->insertAtIndex(0, 42);
	checkTest("testInsertAtIndex #5", "42", s->getStringFromList());

	s->insertAtIndex(1, 82);
	checkTest("testInsertAtIndex #6", "42 82", s->getStringFromList());

	delete s;

}

//This helps with testing, do not modify.
void testDeleteAtIndex() {
	singlyLinkedList<int> *d = new singlyLinkedList<int>;
	for (int i = 10; i < 17; i++) {
		d->insertLast(i);
	}

	//Test just to make sure the data went in the list. 
	checkTest("testDeleteAtIndex #1", "10 11 12 13 14 15 16", d->getStringFromList());

	//Test deleting front items. 
	d->deleteAtIndex(0);
	checkTest("testDeleteAtIndex #2", "11 12 13 14 15 16", d->getStringFromList());

	d->deleteAtIndex(0);
	checkTest("testDeleteAtIndex #3", "12 13 14 15 16", d->getStringFromList());

	//Test deleting back itmes
	d->deleteAtIndex(4);
	checkTest("testDeleteAtIndex #4", "12 13 14 15", d->getStringFromList());

	d->deleteAtIndex(3);
	checkTest("testDeleteAtIndex #5", "12 13 14", d->getStringFromList());

	//Test deleting a Kth element that doesn't exist.
	d->deleteAtIndex(500);
	checkTest("testDeleteAtIndex #6", "12 13 14", d->getStringFromList());

	//Test deleting middle item
	d->deleteAtIndex(1);
	checkTest("testDeleteAtIndex #7", "12 14", d->getStringFromList());

	//Test deleting item on the back
	d->deleteAtIndex(1);
	checkTest("testDeleteAtIndex #8", "12", d->getStringFromList());

	//Test deleting item on the front
	d->deleteAtIndex(0);
	checkTest("testDeleteAtIndex #9", "The list is empty.", d->getStringFromList());

	//Test attempting to delete from an empty list
	d->deleteAtIndex(0);
	checkTest("testDeleteAtIndex #10", "The list is empty.", d->getStringFromList());

	delete d;
}


//This helps with testing, do not modify.
void testdeleteAllInstances() {

	singlyLinkedList<int> *d = new singlyLinkedList<int>;

	d->insertLast(4);
	d->insertLast(2);
	d->insertLast(6);
	d->insertLast(5);
	d->insertLast(6);
	d->insertLast(9);

	//Do a delete, test it.
	d->deleteAllInstances(6);
	checkTest("testdeleteAllInstances #1", "4 2 5 9", d->getStringFromList());

	delete d;
	d = new singlyLinkedList<int>;
	d->insertLast(4);
	d->insertLast(2);
	d->insertLast(3);
	d->insertLast(4);
	d->insertLast(4);
	d->insertLast(4);
	d->insertLast(9);
	d->deleteAllInstances(4);
	checkTest("testdeleteAllInstances #2", "2 3 9", d->getStringFromList());

	delete d;
	d = new singlyLinkedList<int>;
	d->insertLast(3);
	d->insertLast(3);
	d->insertLast(3);
	d->insertLast(8);
	d->insertLast(2);
	d->insertLast(3);
	d->insertLast(3);
	d->insertLast(3);
	d->deleteAllInstances(3);
	checkTest("testdeleteAllInstances #3", "8 2", d->getStringFromList());

	delete d;
	d = new singlyLinkedList<int>;
	d->insertLast(9);
	d->insertLast(9);
	d->insertLast(4);
	d->insertLast(2);
	d->insertLast(9);
	d->insertLast(9);
	d->insertLast(5);
	d->insertLast(1);
	d->insertLast(9);
	d->insertLast(2);
	d->insertLast(9);
	d->insertLast(9);

	//Do a delete, test it.
	d->deleteAllInstances(9);
	checkTest("testdeleteAllInstances #4", "4 2 5 1 2", d->getStringFromList());

	//Test deleting something that doesn't exist
	d->deleteAllInstances(7);
	checkTest("testdeleteAllInstances #5", "4 2 5 1 2", d->getStringFromList());

	//A few more tests
	d->deleteAllInstances(2);
	checkTest("testdeleteAllInstances #6", "4 5 1", d->getStringFromList());

	d->deleteAllInstances(4);
	checkTest("testdeleteAllInstances #7", "5 1", d->getStringFromList());

	d->deleteAllInstances(5);
	checkTest("testdeleteAllInstances #8", "1", d->getStringFromList());

	d->deleteAllInstances(1);
	checkTest("testdeleteAllInstances #9", "The list is empty.", d->getStringFromList());

	//retest deleting something that doesn't exist.
	d->deleteAllInstances(7);
	checkTest("testdeleteAllInstances #10", "The list is empty.", d->getStringFromList());
	delete d;

	//Now ramp it up and do some huge tests.  Start by timing how long a smaller approach takes.
	d = new singlyLinkedList<int>;
	//Fill the list with a pattern of
	//1 2 2 3 3 3 4 4 4 4 1 2 2 3 3 3 4 4 4 4 ...
	cout << endl << "Preparing for testdeleteAllInstances #11, placing 75,000 numbers into the linked list to see how long things take." << endl;
	for (int i = 0; i < 30000; i++) {
		for (int j = 0; j < i % 4 + 1; j++) {
			d->insertLast(i % 4 + 1);
		}
	}
	cout << "    Calling deleteAllInstances to remove 22,500 3s in the list." << endl;
	//delete all the 3s.
	auto start = std::chrono::high_resolution_clock::now();
	d->deleteAllInstances(3);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::micro> diff = end - start;
	double benchmarkTime = diff.count() / 1000.0;
	cout << "    Removing 22,500 3s took " << benchmarkTime << " milliseconds." << endl;
	cout << "    So we will assume removing 45,000 3s then should be double that..." << endl;
	cout << "	 about " << benchmarkTime << " * 2 = " << (benchmarkTime * 2) << " milliseconds if done correctly." << endl;
	delete d;

	cout << "Starting testdeleteAllInstances #11, filling in 150,000 numbers into the linked list to get it started." << endl;
	d = new singlyLinkedList<int>;
	//Fill the list with a pattern of
	//1 2 2 3 3 3 4 4 4 4 1 2 2 3 3 3 4 4 4 4 ...
	for (int i = 0; i < 60000; i++) {
		for (int j = 0; j < i % 4 + 1; j++) {
			d->insertLast(i % 4 + 1);
		}
	}
	cout << "    Finished inserting 150,000 numbers." << endl;
	cout << "    Calling deleteAllInstances to remove 45,000 3s.  This should take about " << (benchmarkTime * 2) << " milliseconds." << endl;
	//delete all the 3s.
	start = std::chrono::high_resolution_clock::now();
	d->deleteAllInstances(3);
	end = std::chrono::high_resolution_clock::now();
	diff = end - start;
	double actualTime = diff.count() / 1000.0;
	if (actualTime < (benchmarkTime * 2 * 1.5)) { //The 1.5 gives an extra 50% wiggle room
		cout << "Passed testdeleteAllInstances #11, completed deleteAllInstances in " << actualTime << " milliseconds." << endl;

	}
	else {
		cout << "*** Failed testdeleteAllInstances #11, deleteAllInstances took " << actualTime
			<< " milliseconds." << endl;
		cout << "*** This which is much worse than the expected " << (benchmarkTime * 2) << " milliseconds." << endl;
	}
	delete d;

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

}


int main() {


	//Each of these checks how many bytes you have used.
	checkTestMemory("Memory Leak/Allocation Test #1", 0, manageMemory::getTotalSize());

	//For your assignment, write the code to make these three methods work
	//You should not modify the code here in main.
	testGetAtIndex();

	checkTestMemory("Memory Leak/Allocation Test #2", 0, manageMemory::getTotalSize());

	testSquareBrackets();

	checkTestMemory("Memory Leak/Allocation Test #3", 0, manageMemory::getTotalSize());

	testInsertAtIndex();

	checkTestMemory("Memory Leak/Allocation Test #4", 0, manageMemory::getTotalSize());

	testDeleteAtIndex();

	checkTestMemory("Memory Leak/Allocation Test #5", 0, manageMemory::getTotalSize());

	testdeleteAllInstances();

	checkTestMemory("Memory Leak/Allocation Test #6", 0, manageMemory::getTotalSize());

	pressAnyKeyToContinue();


	return 0;
}