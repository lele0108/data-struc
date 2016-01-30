// Jimmy (Sicong) Liu
// Jan 20th 2016
// Project 2
// Implement DoublyLinkedList, Insertion Sort

#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <fstream>
#include <cstring>
using namespace std;

#define BLANK " "

template<typename T>
class BagInterface
{
public:
	/** Gets the current number of entries in this bag.
	@return The integer number of entries currently in the bag. */
	virtual int getCurrentSize() const = 0;

	/** Sees whether this bag is empty.
	@return True if the bag is empty, or false if not. */
	virtual bool isEmpty() const = 0;

	/** Adds a new entry to this bag.
	@post  If successful, newEntry is stored in the bag and
	the count of items in the bag has increased by 1.
	@param newEntry  The object to be added as a new entry.
	@return  True if addition was successful, or false if not. */
	virtual bool add(const T& newEntry) = 0;

	/** Removes one occurrence of a given entry from this bag,
	if possible.
	@post  If successful, anEntry has been removed from the bag
	and the count of items in the bag has decreased by 1.
	@param anEntry  The entry to be removed.
	@return  True if removal was successful, or false if not. */
	//virtual bool remove(const T& anEntry) = 0;
	virtual bool remove(const T& anEntry) = 0;

	/** Removes all entries from this bag.
	@post  Bag contains no items, and the count of items is 0. */
	virtual void clear() = 0;
	virtual void sort() = 0;
}; // end BagInterface

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
    		return (price < one.price); 
    	}
    	bool operator> (const Product &one) {
    		return (price > one.price); 
    	}
};

template <typename T>
class Node
{
private:
	T item; // A data item
	shared_ptr<Node<T> > next; // Pointer to next node
	shared_ptr<Node<T> > prev; // Pointer to the previous node
public:
	Node() : next(nullptr)
	{
	} // end default constructor
	Node(const T& anItem) : item(anItem), next(nullptr)
	{
	} // end constructor
	Node(const T& anItem, unique_ptr<Node<T> > nextNodePtr) : item(anItem), next(nextNodePtr)
	{
	} // end constructor
	void setItem(const T& anItem)
	{
		item = anItem;
	} // end setItem
	void setNext(shared_ptr<Node<T> > nextNodePtr)
	{
		next = nextNodePtr;
	} // end setNext
	void setPrev(shared_ptr<Node<T> > prevNodePtr) {
		prev = prevNodePtr;
	}
	T getItem() const
	{
		return item;
	} // end getItem
	shared_ptr<Node<T> > getNext() const
	{
		return next;
	} // end getNext
	shared_ptr<Node<T> > getPrev() const {
		return prev;
	}
	T operator *()
	{
		return getItem();
	}  // end operator *
	friend ostream& operator << (ostream& os, Node<T>& node)
	{
		os << node.getItem() << endl;
		return os;
	}  // end operator <<
}; // end Node

template<typename T>
class DoublyLinkedBag : public BagInterface<T>
{
private:
	shared_ptr<Node<T> > headPtr; // Pointer to first node
	shared_ptr<Node<T> > tailPtr; // Pointer to the last node
	int itemCount;           // Current count of bag items
							 // Returns either a pointer to the node containing a given entry
							 // or the null pointer if the entry is not in the bag.
public:
	DoublyLinkedBag() : headPtr(nullptr), itemCount(0)
	{
	}  // end default constructor
	bool isEmpty() const
	{
		return itemCount == 0;
	}  // end isEmpty
	int getCurrentSize() const
	{
		return itemCount;
	}  // end getCurrentSize
	bool add(const T& newEntry)
	{
		// Add to beginning of chain: new node references rest of chain;
		// (headPtr is null if chain is empty)    
		shared_ptr<Node<T> > nextNodePtr(new Node<T>(newEntry));
		if (headPtr != nullptr) {
			headPtr->setPrev(nextNodePtr);
		}
		nextNodePtr->setNext(headPtr);  // New node points to chain
										//   Node<ItemType>* nextNodePtr = new Node<ItemType>(newEntry, headPtr); // alternate code
		if (itemCount == 0) {
			tailPtr = nextNodePtr;
		} else if(itemCount == 1) {
			tailPtr = headPtr;
		}
		headPtr = nextNodePtr;          // New node is now first node
		itemCount++;

		return true;
	}  // end add
	shared_ptr<Node<T> > getPointerTo(const T& anEntry) const
	{
		bool found = false;
		shared_ptr<Node<T> > curPtr = headPtr;

		while (!found && (curPtr != nullptr))
		{
			if (curPtr->getItem() == anEntry)
				found = true;
			else
				curPtr = curPtr->getNext();
		} // end while

		return curPtr;
	} // end getPointerTo
	bool remove(const T& anEntry)
	{
		shared_ptr<Node<T> > entryNodePtr = shared_ptr< Node<T> >(getPointerTo(anEntry));
		bool canRemoveItem = !isEmpty() && (entryNodePtr != nullptr);
		if (canRemoveItem)
		{
			if (entryNodePtr == headPtr) {
				headPtr = entryNodePtr->getNext();
				headPtr->setPrev(nullptr);
			} else if (entryNodePtr == tailPtr) {
				tailPtr = entryNodePtr->getPrev();
				tailPtr->setNext(nullptr);
			} else {
				entryNodePtr->getPrev()->setNext(entryNodePtr->getNext());
				entryNodePtr->getNext()->setPrev(entryNodePtr->getPrev());
			}
			itemCount--;
		} // end if
		return canRemoveItem;
	}  // end remove
	bool removePtr(shared_ptr<Node<T> > entryNodePtr)
	{
		bool canRemoveItem = !isEmpty() && (entryNodePtr != nullptr);
		if (canRemoveItem)
		{
			if (entryNodePtr == headPtr) {
				headPtr = entryNodePtr->getNext();
				headPtr->setPrev(nullptr);
			} else if (entryNodePtr == tailPtr) {
				tailPtr = entryNodePtr->getPrev();
				tailPtr->setNext(nullptr);
			} else {
				entryNodePtr->getPrev()->setNext(entryNodePtr->getNext());
				entryNodePtr->getNext()->setPrev(entryNodePtr->getPrev());
			}
			itemCount--;
		} // end if
		return canRemoveItem;
	}  // end remove
	shared_ptr<Node<T> > search(const T& anEntry) const
	{
		return getPointerTo(anEntry);
	}
	void sort() {
		shared_ptr<Node<T> > sorted = headPtr;
		shared_ptr<Node<T> > rover = sorted;
		shared_ptr<Node<T> > compare = headPtr->getNext();

		while (sorted != tailPtr) {
			while (rover > compare) {
				if (rover->getPrev() != nullptr)
					rover = rover->getPrev();
			}
			swap(sorted, compare);
			compare = sorted->getNext();
		}
	}
	void insertAt(shared_ptr<Node<T> > before, shared_ptr<Node<T> > insert) {
		insert->setPrev(before);
		insert->setNext(before->getNext());
		before->getNext()->setPrev(insert);
		before->setNext(insert);
	}
	void swap(shared_ptr<Node<T> > one, shared_ptr<Node<T> > two) {
		shared_ptr<Node<T> > beforeOne = one->getPrev();
		shared_ptr<Node<T> > afterOne = one->getNext();
		shared_ptr<Node<T> > beforeTwo = two->getPrev();
		shared_ptr<Node<T> > afterTwo = two->getNext();
		if (beforeOne != nullptr)
			beforeOne->setNext(two);
		if (afterTwo != nullptr)
			afterTwo->setPrev(one);

		two->setNext(afterOne);
		afterOne->setPrev(two);

		one->setPrev(beforeTwo);
		beforeTwo->setNext(one);

		one->setNext(afterTwo);
		two->setPrev(beforeOne);
	}

	void clear()
	{
		itemCount = 0;
	}  // end clear
	friend ostream& operator << (ostream& os, DoublyLinkedBag<T>& bag)
	{
		os << "The bag contains " << bag.getCurrentSize() << " items:" << endl;
		shared_ptr<Node<T> > rover = bag.headPtr;
		while (rover != nullptr)
		{
			cout << *rover << BLANK;
			rover = rover->getNext();
		}  // end for
		cout << endl << endl;
		return os;
	}  // end displayBag
}; // end DoublyLinkedBag

void bagTester() 
{
	DoublyLinkedBag<Product> bag;

	string line;
	ifstream myfile ("UProducts.csv");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp[2];
			istringstream ss(line);
			getline(ss, temp[0], ',');
			getline(ss, temp[1], ',');
			Product hold (temp[0], atof(temp[1].c_str()));
			bag.add(hold);
		}
	}
	myfile.close();
	Product one("Tuna", 19.81);
	Product two("Cats", 5.00);
	shared_ptr<Node<Product> > three = bag.search(one);
	shared_ptr<Node<Product> > four(new Node<Product>(two));
	bag.insertAt(three, four);
	//bag.sort();
	
	cout << bag;

}  // end bagTester

int main()
{
	bagTester();
	return 0;
}  // end main