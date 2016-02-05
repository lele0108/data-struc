#include <string>
#include <iostream>
#include <array>
using namespace std;

template <typename T>
class StackInterface
{
public:
	/** Sees whether this stack is empty.
	@return True if the stack is empty, or false if not. */
	virtual bool isEmpty() const = 0;

	/** Adds a new entry to the top of this stack.
	@post If the operation was successful, newEntry is at the top of the stack.
	@param newEntry The object to be added as a new entry.
	@return True if the addition is successful or false if not. */
	virtual bool push(const T& newEntry) = 0;

	/** Removes the top of this stack.
	@post If the operation was successful, the top of the stack
	has been removed.
	@return True if the removal is successful or false if not. */
	virtual bool pop() = 0;

	/** Returns the top of this stack.
	@pre The stack is not empty.
	@post The top of the stack has been returned, and
	the stack is unchanged.
	@return The top of the stack. */
	virtual T peek() const = 0;
}; // end StackInterface

template <typename T>
class Node
{
private:
	T        item; // A data item
	Node<T>* next; // Pointer to next node
public:
	Node<T>::Node() : next(nullptr)
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
};

template <typename T>
class LinkedStack : public StackInterface<T>
{
private:
	Node<T>* topPtr; // Pointer to first node in the chain;
public:
	LinkedStack() : topPtr(nullptr)
	{
	}  // end default constructor
	LinkedStack(const LinkedStack<T>& aStack)
	{
		// Point to nodes in original chain
		Node<T>* origChainPtr = aStack.topPtr;

		if (origChainPtr == nullptr)
			topPtr = nullptr;  // Original stack is empty
		else
		{
			// Copy first node
			topPtr = new Node<T>();
			topPtr->setItem(origChainPtr->getItem());

			// Point to last node in new chain
			Node<T>* newChainPtr = topPtr;

			// Advance original-chain pointer
			origChainPtr = origChainPtr->getNext();

			// Copy remaining nodes
			while (origChainPtr != nullptr)
			{
				// Get next item from original chain
				T nextItem = origChainPtr->getItem();

				// Create a new node containing the next item 
				Node<T>* newNodePtr = new Node<T>(nextItem);

				// Link new node to end of new chain
				newChainPtr->setNext(newNodePtr);

				// Advance pointer to new last node      
				newChainPtr = newChainPtr->getNext();

				// Advance original-chain pointer
				origChainPtr = origChainPtr->getNext();
			}  // end while

			newChainPtr->setNext(nullptr);           // Flag end of chain
		}  // end if
	}  // end copy constructor
	~LinkedStack()
	{
		// Pop until stack is empty
		while (!isEmpty())
			pop();
	}  // end destructor
	bool isEmpty() const
	{
		return topPtr == nullptr;
	}  // end isEmpty
	bool push(const T& newItem)
	{
		Node<T>* newNodePtr = new Node<T>(newItem, topPtr);
		topPtr = newNodePtr;
		newNodePtr = nullptr;

		return true;
	}  // end push
	bool pop()
	{
		bool result = false;
		if (!isEmpty())
		{
			// Stack is not empty; delete top
			Node<T>* nodeToDeletePtr = topPtr;
			topPtr = topPtr->getNext();

			// Return deleted node to system
			nodeToDeletePtr->setNext(nullptr);
			delete nodeToDeletePtr;
			nodeToDeletePtr = nullptr;

			result = true;
		}  // end if

		return result;
	}  // end pop
	T peek() const
	{
		//assert(!isEmpty());  // Enforce precondition
		if (topPtr)
			return topPtr->getItem();
		return 0;

	}  // end peek
};

void copyConstructorTester()
{
	LinkedStack<string> stack;
	array<string,6> items =
	{ 
		string("zero"), 
		string("one"), 
		string("two"), 
		string("three"), 
		string("four"), 
		string("five") 
	};
	for (int i = 0; i < items.size(); i++)
	{
		cout << "Pushing " << items[i] << endl;
		bool success = stack.push(items[i]);
		if (!success)
			cout << "Failed to push " << items[i] << " onto the stack." << endl;
	}
	cout << "Stack contains, from top to bottom, five four three two one zero." << endl;

	LinkedStack<string> copyOfStack(stack);
	cout << "Copy of stack contains, from top to bottom, ";
	while (!copyOfStack.isEmpty())
	{
		cout << " " << copyOfStack.peek();
		copyOfStack.pop();
	}
	cout << "." << endl;

	cout << "Original stack contains, from top to bottom,";
	while (!copyOfStack.isEmpty())
	{
		cout << " " << stack.peek();
		stack.pop();
	}
	cout << "." << endl;
}  // end copyConstructorTester

void stackTester()
{
	StackInterface<string>* stackPtr = new LinkedStack<string>();
	cout << "\nTesting the Link-Based Stack:" << endl;
	array<string, 6> items =
	{
		string("zero"),
		string("one"),
		string("two"),
		string("three"),
		string("four"),
		string("five")
	};
	cout << "Empty: " << stackPtr->isEmpty() << endl;
	for (int i = 0; i < items.size(); i++)
	{
		cout << "Pushing " << items[i] << endl;
		bool success = stackPtr->push(items[i]);
		if (!success)
			cout << "Failed to push " << items[i] << " onto the stack." << endl;
	}

	cout << "Empty?: " << stackPtr->isEmpty() << endl;

	int i = 0;
	while (!stackPtr->isEmpty())  // NEEDS TO BE 5 TO AVOID ASSERT ERROR
	{
		cout << "Loop " << i++ << endl;
		cout << "peek1: " << stackPtr->peek() << endl;
		cout << "pop: " << stackPtr->pop() << endl;
		cout << "Empty: " << stackPtr->isEmpty() << endl;
	}
	cout << "Empty: " << stackPtr->isEmpty() << endl;

	cout << "pop an empty stack: " << endl;
	cout << "pop: " << stackPtr->pop() << endl; // nothing to pop!
}  // end stackTester

void main()
{
	copyConstructorTester();
	stackTester();
} 
