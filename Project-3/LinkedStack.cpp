// Jimmy Liu
// Project-3
// CIS 22B
// 5 Feb 2016

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
};

template <typename T>
class LinkedStack : public StackInterface<T>
{
private:
	Node<T>* topPtr; // Pointer to first node in the chain;
	int maxSize; // Maximum size of stack
	int top; // Size of stack
public:
	LinkedStack() : topPtr(nullptr)
	{
		maxSize = 10000;
		top = 0;
	}  // end default constructor
	LinkedStack(int max) {
		topPtr = nullptr;
		maxSize = max;
		top = 0;
	}
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
		if (top == maxSize) {
			cout << "Stack is overfilled" << endl;
		} else {
			Node<T>* newNodePtr = new Node<T>(newItem, topPtr);
			topPtr = newNodePtr;
			newNodePtr = nullptr;
			top++;
		}
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
			top--;

			result = true;
		} else {
			cout << "Nothing to pop" << endl;
		}

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

int checkOperator(char hold) {
	if (hold == '*' || hold == '+' || hold == '/' || hold == '-')
		return 0;
	else if (hold == '(' || hold == ')')
		return 1;
	return 2;
}

int precedence(char check) {
	if (check == '*' || check == '/')
		return 1;
	else 
		return 0;
}

void postFixTester() {
	string test = "a-(b+c*d)/e";
	string postFix = "";
	StackInterface<char>* stackPtr = new LinkedStack<char>(test.length());
	for (int i = 0; i < test.length(); i++) {
		char hold = test.at(i);
		if(checkOperator(hold) == 2) {
			postFix.append(string(1, hold));
		} else if (hold == '(') {
			stackPtr->push(hold);
		} else if (checkOperator(hold) == 0) {
			while(!stackPtr->isEmpty() && stackPtr->peek() != '(' && precedence(hold) <= precedence(stackPtr->peek())) {
				postFix.append(string(1, stackPtr->peek()));
				stackPtr->pop();
			}
			stackPtr->push(hold);
		} else if (hold == ')') {
			while (stackPtr->peek() != '(') {
				postFix.append(string(1, stackPtr->peek()));
				stackPtr->pop();
			}
		}
	}
	while (!stackPtr->isEmpty()) {
		postFix.append(string(1, stackPtr->peek()));
		stackPtr->pop();
	}
	cout << postFix << endl;
} // end postFixTester

int main()
{
	postFixTester();
	return 0;
} 
