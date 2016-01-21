// Jimmy (Sicong) Liu
// Jan 20th 2016
// Project 1
// Implement DoublyLinkedList

#include <string>
#include <iostream>
#include <memory>
#include <iomanip>
#include <fstream>
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

	/** Removes all entries from this bag.
	@post  Bag contains no items, and the count of items is 0. */
	virtual void clear() = 0;

	/** Counts the number of times a given entry appears in bag.
	@param anEntry  The entry to be counted.
	@return  The number of times anEntry appears in the bag. */
	virtual int getFrequencyOf(const T& anEntry) const = 0;

	/** Tests whether this bag contains a given entry.
	@param anEntry  The entry to locate.
	@return  True if bag contains anEntry, or false otherwise. */
	virtual bool contains(const T& anEntry) const = 0;
}; // end BagInterface

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
	friend ostream& operator << (ostream& stream, Node<T>& node)
	{
		stream << node.getItem() << endl;
		return stream;
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
	void clear()
	{
		itemCount = 0;
	}  // end clear
	int getFrequencyOf(const T& anEntry) const
	{
		int frequency = 0;
		int counter = 0;
		shared_ptr<Node<T> > curPtr = headPtr;
		while ((curPtr != nullptr) && (counter < itemCount))
		{
			if (anEntry == curPtr->getItem())
			{
				frequency++;
			} // end if

			counter++;
			curPtr = curPtr->getNext();
		} // end while
		return frequency;
	}  // end getFrequencyOf
	bool contains(const T& anEntry) const
	{
		return (getPointerTo(anEntry) != nullptr);
	}  // end contains
	shared_ptr<Node<T> > getPointerTo(const T& anEntry) const
	{
		bool found = false;
		shared_ptr<Node<T> > curPtr = headPtr;

		while (!found && (curPtr != nullptr))
		{
			if (anEntry == curPtr->getItem())
				found = true;
			else
				curPtr = curPtr->getNext();
		} // end while

		return curPtr;
	} // end getPointerTo
	/*friend ostream& operator << (ostream& stream, DoublyLinkedBag<string>& bag)
	{
		stream << "The bag contains " << bag.getCurrentSize() << " items:" << endl;
		shared_ptr<Node<T> > rover = bag.headPtr;
		while (rover != nullptr)
		{
			cout << *rover << BLANK;
			rover = rover->getNext();
		}  // end for
		cout << endl << endl;
		return stream;
	}  // end displayBag*/
	friend ostream& operator << (ostream& stream, DoublyLinkedBag<string>& bag)
	{
		stream << "The bag contains " << bag.getCurrentSize() << " items:" << endl;
		shared_ptr<Node<T> > rover = bag.tailPtr;
		while (rover != nullptr)
		{
			cout << *rover << BLANK;
			rover = rover->getPrev();
		}  // end for
		cout << endl << endl;
		return stream;
	}  // end displayBag
}; // end DoublyLinkedBag

void bagTester() 
{
	DoublyLinkedBag<string> bag;

	string line;
	ifstream myfile ("UProducts.csv");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			bag.add(line);
		}
	}
	myfile.close();

	/*string numbers[] = { "one", "two", "three", "four", "five"};
	cout << "Add 6 items to the bag: " << endl;
	for (int i = 0; i < sizeof(numbers)/sizeof(numbers[0]); i++)
	{
		bag.add(numbers[i]);
	}  // end for*/

	cout << bag;

}  // end bagTester

int main()
{
	bagTester();
	return 0;
}  // end main