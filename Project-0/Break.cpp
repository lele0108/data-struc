// Jimmy Liu
// Data Structures
// Jan 11th 2016

#include <array>
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <fstream>

using namespace std;

enum { INVALID=-1,DEFAULT_CAPACITY = 10 };	// Small size to test for a full bag

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

template<typename T>
class ArrayBag : public BagInterface<T>
{
private:
	unique_ptr<T[]> items;      		   // Array of bag items
	int itemCount;                         // Current count of bag items 
	int maxItems;                          // Max capacity of the bag
public:
	ArrayBag(int capacity = DEFAULT_CAPACITY) : itemCount(0), maxItems(capacity)
	{	
		items = unique_ptr<T[]>(new T[capacity]);
	}  // end default constructor
	int getCurrentSize() const
	{
		return itemCount;
	}  // end getCurrentSize
	bool isEmpty() const
	{
		return itemCount == 0;
	}  // end isEmpty
	T& get(int index)
	{
		return items.get()[index];
	}
	bool add(const T& newEntry)
	{
		bool hasRoomToAdd = (itemCount < maxItems);
		if (hasRoomToAdd)
		{
			get(itemCount) = move(newEntry);
			itemCount++;
		}  // end if
		return hasRoomToAdd;
	}  // end add
	bool remove(const T& anEntry)
	{
		int locatedIndex = getIndexOf(anEntry, 0);
		bool canRemoveItem = !isEmpty() && (locatedIndex > INVALID);
		if (canRemoveItem)
		{
			itemCount--;
			get(locatedIndex) = move(get(itemCount));
		}  // end if

		return canRemoveItem;
	}  // end remove
	void clear()
	{
		itemCount = 0;
	}  // end clear
	bool contains(const T& anEntry) const
	{
		return getIndexOf(anEntry, 0) > INVALID;
	}  // end contains
	int getFrequencyOf(const T& anEntry) const
	{
		return countFrequency(anEntry, 0);
	}  // end getFrequencyOf
	int countFrequency(const T& anEntry, int searchIndex) const
	{
		int frequency = 0;
		if (searchIndex < itemCount)
		{
			if (items.get()[searchIndex] == anEntry)
			{
				frequency = 1 + countFrequency(anEntry, searchIndex + 1);
			}
			else
			{
				frequency = countFrequency(anEntry, searchIndex + 1);
			}  // end if
		}  // end if

		return frequency;
	}  // end countFrequency
	int getIndexOf(const T& target, int searchIndex) const
	{
		int result = -1;
		if (searchIndex < itemCount)
		{
			if (items.get()[searchIndex] == target)
			{
				result = searchIndex;
			}
			else
			{
				result = getIndexOf(target, searchIndex + 1);
			}  // end if
		}  // end if

		return result;
	}  // end getIndexOf
	T& getIndexValue(int index)
	{
		return items.get()[index];
	}
	friend ostream& operator << (ostream& stream, ArrayBag<T>& bag)
	{
		stream << "The bag contains " << static_cast<int>(bag.getCurrentSize()) << " items:" << endl;
		for (int i = 0; i < bag.getCurrentSize(); i++)
		{
			stream << bag.getIndexValue(i) << " ";
		}  // end for
		stream << endl << endl;
		return stream;
	}  // end displayBag
	void readFile() {
		string line;
		ifstream myfile ("Words.txt");
	  	if (myfile.is_open()) {
	  		while ( getline (myfile,line) ) {
	  			//cout << line << endl;
		  		if (itemCount < 10) {
		  			cout << line << endl;
		  			add(line);
		  		}

		  	}
		} else {
			cout << "Unable to open file" << endl;
		}
		myfile.close();
		for (int i = 0; i < 10; i++) {
			cout << items[i] << endl;
		}
	}
};

void bagTester(ArrayBag<string>& bag)
{
	bag.readFile();

	cout << bag;
	cout << endl;

}  // end bagTester

int main()
{
	ArrayBag<string> bag;
	bagTester(bag);
	cout << "All done!" << endl;
	return 0;
}  // end main

