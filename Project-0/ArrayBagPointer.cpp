#include <array>
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <fstream>
#include <sstream>

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
	T* items;								// Array of bag items
	int itemCount;                         // Current count of bag items 
	int maxItems;                          // Max capacity of the bag
public:
	ArrayBag(int capacity = DEFAULT_CAPACITY) : itemCount(0), maxItems(capacity)
	{
		items = new T[capacity];
	}  // end default constructor
	~ArrayBag()
	{
		//if (items)
		//	delete[] items;
	}
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
		return items[index];
	}
	bool add(const T& newEntry)
	{
		bool hasRoomToAdd = (itemCount < maxItems);
		if (hasRoomToAdd)
		{
			items[itemCount] = newEntry;
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
			items[locatedIndex] = items[itemCount];
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
			if (items[searchIndex] == anEntry)
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
			if (items[searchIndex] == target)
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
		return items[index];
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
};

void bagTester(ArrayBag<string>& bag)
{
	cout << "isEmpty: returns " << boolalpha << bag.isEmpty() << "; should be 1 (true)" << endl;
	cout << bag;


ifstream myfile ("Words.txt");

  	int currSize = 10;
  	int count = 0;

  	if (myfile.is_open()) {
  		string line;
  		while ( getline (myfile,line) ) {

  			if (count < currSize) {
  				cout << line << endl;
  				bag.add(line);
  				count++;
  			}
  			//else if (count == currSize) {
  			//	ArrayBag<string> temp(bag);
  			//	count++;
  				//temp = move(bag);
  			//}
  		}
  	}

	string numbers[] = 
	{ 
		"one", 
		"two", 
		"three", 
		"four", 
		"five", 
		"one", 
		"six",
		"seven",
		"eight"
	};
	cout << "Add items to the bag container: " << endl;
	for (int i = 0; i < sizeof(numbers)/sizeof(numbers[0]); i++)
	{
		bag.add( numbers[i] );
	}  // end for

	cout << bag;

	cout << "isEmpty: returns " << boolalpha << bag.isEmpty() << "; should be 0 (false)" << endl;
	cout << "getCurrentSize: returns " << bag.getCurrentSize() << "; should be 9" << endl;
	cout << "Try to add another entry: add(\"nine\") returns " << bag.add("nine") << endl;
	cout << "contains(\"three\"): returns " << boolalpha << bag.contains("three") << "; should be 1 (true)" << endl;
	cout << "contains(\"ten\"): returns " << boolalpha << bag.contains("ten") << "; should be 0 (false)" << endl;
	cout << "getFrequencyOf(\"one\"): returns " << bag.getFrequencyOf("one") << " should be 2" << endl;
	cout << "remove(\"one\"): returns " << boolalpha << bag.remove("one") << "; should be 1 (true)" << endl;
	cout << "getFrequencyOf(\"one\"): returns " << bag.getFrequencyOf("one") << " should be 1" << endl;
	cout << "remove(\"one\"): returns " << boolalpha << bag.remove("one") << "; should be 1 (true)" << endl;
	cout << "remove(\"one\"): returns " << boolalpha << bag.remove("one") << "; should be 0 (false)" << endl;
	cout << endl;

	cout << bag;

	cout << "After clearing the bag, ";
	bag.clear();

	cout << "isEmpty: returns " << boolalpha << bag.isEmpty() << "; should be 1 (true)" << endl;
}  // end bagTester

int main()
{
	ArrayBag<string> bag;
	cout << "Testing the Array-Based Bag:" << endl;
	cout << "The initial bag is empty." << endl;
	bagTester(bag);
	cout << "All done!" << endl;
	return 0;
}  // end main

