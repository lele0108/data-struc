
#include <string>
#include <iostream>
#include <memory>
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
	virtual bool remove(const T& anEntry) = 0;

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
	T getItem() const
	{
		return item;
	} // end getItem
	shared_ptr<Node<T> > getNext() const
	{
		return next;
	} // end getNext
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
class LinkedBag : public BagInterface<T>
{
private:
	shared_ptr<Node<T> > headPtr; // Pointer to first node
	int itemCount;           // Current count of bag items
							 // Returns either a pointer to the node containing a given entry
							 // or the null pointer if the entry is not in the bag.
public:
	LinkedBag() : headPtr(nullptr), itemCount(0)
	{
	}  // end default constructor
	LinkedBag(const LinkedBag<T>& aBag)
	{
		itemCount = aBag.itemCount;
		shared_ptr<Node<T> > origChainPtr = aBag.headPtr;  // Points to nodes in original chain

		if (origChainPtr == nullptr)
			headPtr = nullptr;  // Original bag is empty
		else
		{
			// Copy first node
			headPtr = shared_ptr<Node<T> >(new Node<T>());
			headPtr->setItem(origChainPtr->getItem());

			// Copy remaining nodes
			shared_ptr<Node<T> > newChainPtr = headPtr;      // Points to last node in new chain
			origChainPtr = origChainPtr->getNext();     // Advance original-chain pointer

			while (origChainPtr != nullptr)
			{
				// Get next item from original chain
				T nextItem = origChainPtr->getItem();
				// Create a new node containing the next item
				shared_ptr<Node<T> > newNodePtr(new Node<T>(nextItem));
				// Link new node to end of new chain
				newChainPtr->setNext(newNodePtr);
				// Advance pointer to new last node
				newChainPtr = newChainPtr->getNext();
				// Advance original-chain pointer
				origChainPtr = origChainPtr->getNext();
			}  // end while

			newChainPtr->setNext(nullptr);              // Flag end of chain
		}  // end if
	}  // end copy constructor
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
		nextNodePtr->setNext(headPtr);  // New node points to chain
										//   Node<ItemType>* nextNodePtr = new Node<ItemType>(newEntry, headPtr); // alternate code

		headPtr = nextNodePtr;          // New node is now first node
		itemCount++;

		return true;
	}  // end add
	bool remove(const T& anEntry)
	{
		shared_ptr<Node<T> > entryNodePtr = shared_ptr<Node<T> >(getPointerTo(anEntry));
		bool canRemoveItem = !isEmpty() && (entryNodePtr != nullptr);
		if (canRemoveItem)
		{
			// Copy data from first node to located node
			entryNodePtr->setItem(headPtr->getItem());
			// Delete first node
			shared_ptr<Node<T> > nodeToDeletePtr = headPtr;
			headPtr = headPtr->getNext();
			itemCount--;
		} // end if
		return canRemoveItem;
	}  // end remove
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
	friend ostream& operator << (ostream& stream, LinkedBag<string>& bag)
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
	}  // end displayBag
}; // end LinkedBag

void copyConstructorTester()
{
	LinkedBag<string> bag;
	string numbers[] = { "zero", "one", "two", "three", "four", "five" };
	for (int i = 0; i < bag.getCurrentSize(); i++)
	{
		cout << "Adding " << numbers[i] << endl;
		bool success = bag.add(numbers[i]);
		if (!success)
			cout << "Failed to add " << numbers[i] << " to the bag." << endl;
	}
	cout << bag;

	LinkedBag<string> copyOfBag = bag;
	cout << "Copy of bag: ";
	cout << copyOfBag;

	cout << "The copied bag: ";
	cout << bag;
}  // end copyConstructorTester

void bagTester() 
{
	LinkedBag<string> bag;
	cout << "Testing the Link-Based Bag:" << endl;
	cout << "isEmpty: returns " << bag.isEmpty() << "; should be 1 (true)" << endl;
	cout << bag;

	string numbers[] = { "one", "two", "three", "four", "five", "one" };
	cout << "Add 6 items to the bag: " << endl;
	for (int i = 0; i < sizeof(numbers)/sizeof(numbers[0]); i++)
	{
		bag.add(numbers[i]);
	}  // end for

	cout << bag;

}  // end bagTester

int main()
{
	copyConstructorTester();
	bagTester();
	return 0;
}  // end main