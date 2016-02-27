#include <stdexcept>
#include <string>
#include <iostream>
#include <cassert>
#include <array>
#include <sstream>
#include <memory>
#include <iomanip>
#include <fstream>
#include <cstring>

using namespace std;

template <typename T>
class QueueInterface

{
public:
	/** Sees whether this queue is empty.
	@return True if the queue is empty, or false if not. */
	virtual bool isEmpty() const = 0;

	/** Adds a new entry to the back of this queue.
	@post If the operation was successful, newEntry is at the
	back of the queue.
	@param newEntry  The object to be added as a new entry.
	@return True if the addition is successful or false if not. */
	virtual bool enqueue(const T& newEntry) = 0;

	virtual bool priorityEnqueue(T& newEntry) = 0;

	virtual bool priorityDequeue() = 0;

	/** Removes the front of this queue.
	@post If the operation was successful, the front of the queue
	has been removed.
	@return True if the removal is successful or false if not. */
	virtual bool dequeue() = 0;

	virtual int getCount() = 0;

	/** Returns the front of this queue.
	@pre The queue is not empty.
	@post The front of the queue has been returned, and the
	queue is unchanged.
	@return The front of the queue. */
	virtual T peekFront() = 0;
}; // end QueueInterface

template <typename T>
class Node
{
private: 
	T        item; // A data item
	Node<T>* next; // Pointer to next node

public:
	Node() : next(nullptr)
	{
	} // end default constructor
	Node(const T& anItem) : item(anItem), next(nullptr)
	{
	} // end constructor
	Node(const T& anItem, Node<T>* nextNodePtr) : item(anItem), next(nextNodePtr)
	{
	} // end constructor
	void setItem(const T& anItem)
	{
		item = anItem;
	} // end setItem
	void setNext(Node<T>* nextNodePtr)
	{
		next = nextNodePtr;
	} // end setNext
	T getItem() const
	{
		return item;
	} // end getItem
	Node<T>* getNext() const
	{
		return next;
	} // end getNext
}; // end Node

class Product {
	private:
		double price;
		string name; 
	public:
		Product() {
			price = -1.0;
		}
		Product(string a, double b) {
			name = a;
			price = b;
		}
		friend ostream& operator<<(ostream& os, const Product& pd) {
    		os << pd.name << " " << pd.price;
    		return os;
		}
		bool operator== (const Product &one) {
    		return (price == one.price && name == one.name); 
    	}
    	bool operator< (const Product &one) {
    		return (name < one.name); 
    	}
    	bool operator<= (const Product &one) {
    		return (name < one.name || name == one.name); 
    	}
    	bool operator> (const Product &one) {
    		return (name > one.name); 
    	}
};

template <typename T>
class LinkedQueue : public QueueInterface<T>
{
private:
	// The queue is implemented as a chain of linked nodes that has 
	// two external pointers, a head pointer for front of the queue and
	// a tail pointer for the back of the queue.
	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	LinkedQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;
		count = 0;
	}  // end default constructor
	~LinkedQueue()
	{
		while (!isEmpty())
			dequeue();
		assert((backPtr == nullptr) && (frontPtr == nullptr));
	}  // end destructor
	bool isEmpty() const
	{
		if (count == 0) {
			return true;
		}
		return false;
	}  // end isEmpty
	bool enqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);

		// Insert the new node
		if (isEmpty())
			frontPtr = newNodePtr;        // Insertion into empty queue
		else
			backPtr->setNext(newNodePtr); // Insertion into nonempty queue

		backPtr = newNodePtr;            // New node is at back
		count++;
		return true;
	}  // end enqueue
	bool priorityEnqueue(T& newEntry) {
		Node<T>* newNodePtr = new Node<T>(newEntry);

		if (isEmpty() || newEntry < frontPtr->getItem()) {
			newNodePtr->setNext(frontPtr);
			frontPtr = newNodePtr;
		}
		else {
			Node<T>* tempPtr = frontPtr;
			while (tempPtr->getNext() != nullptr && tempPtr->getNext()->getItem() <= newEntry) {
				tempPtr = tempPtr->getNext();
			}
			newNodePtr->setNext(tempPtr->getNext());
			tempPtr->setNext(newNodePtr);
		}

		count++;
		return true;
	}
	bool dequeue()
	{
		bool result = false;
		if (!isEmpty())
		{
			// Queue is not empty; delete front
			Node<T>* nodeToDeletePtr = frontPtr;
			if (frontPtr == backPtr)
			{  // Special case: one node in queue
				frontPtr = nullptr;
				backPtr = nullptr;
			}
			else
				frontPtr = frontPtr->getNext();

			// Return deleted node to system
			nodeToDeletePtr->setNext(nullptr);
			delete nodeToDeletePtr;
			nodeToDeletePtr = nullptr;

			result = true;
		}  // end if

		return result;
	}  // end dequeue
	bool priorityDequeue() {
		Node<T>* front = frontPtr;
		frontPtr = frontPtr->getNext();
		delete front;
		count--;
		return true;
	}
	T peekFront()
	{
		if (isEmpty()) {
			cout << "HASH" << endl;
			return Product();
		}

		// Queue is not empty; return front
		return frontPtr->getItem();
	}  // end peekFront
	Node<T>* operator [] (int index)
	{
		int count = 0;
		Node<T>* rover = frontPtr;
		while (count++ < index && rover != nullptr) 
			rover = rover->getNext();
		return rover;
	}
	int getCount() {
		return count;
	}
	   // End of implementation file.
}; // end LinkedQueue

void queueTester()
{
	QueueInterface<Product>* queuePtr = new LinkedQueue<Product>();
	
	string line;
	ifstream myfile ("UProducts.csv");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp[2];
			istringstream ss(line);
			getline(ss, temp[0], ',');
			getline(ss, temp[1], ',');
			Product hold (temp[0], atof(temp[1].c_str()));
			queuePtr->priorityEnqueue(hold);
		}
	}
	myfile.close();

	cout << "Total Item Count: " << queuePtr->getCount() << endl;
	for (int i = 0; i < queuePtr->getCount(); i++)
	{	
		cout << queuePtr->peekFront() << endl;
		queuePtr->priorityDequeue();
	}
}  // end queueTester

int main()
{
	queueTester();
	return 0;
}  // end main
