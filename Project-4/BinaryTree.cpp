#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class PrecondViolatedExcep : public logic_error
{
public:
	PrecondViolatedExcep(const string& message="") : logic_error("Precondition Violated Exception: " + message)
	{
	}  // end constructor
}; // end PrecondViolatedExcep 

class NotFoundException : public logic_error
{
public:
	NotFoundException(const string& message="") : logic_error("Precondition Violated Exception: " + message)
	{
	}  // end constructor
}; // end NotFoundException 

template <typename T>
class BinaryTreeInterface
{
public:
	/** Tests whether this binary tree is empty.
	@return True if the binary tree is empty, or false if not. */
	virtual bool isEmpty() const = 0;

	/** Gets the height of this binary tree.
	@return The height of the binary tree. */
	virtual int getHeight() const = 0;

	/** Gets the number of nodes in this binary tree.
	@return The number of nodes in the binary tree. */
	virtual int getNumberOfNodes() const = 0;

	/** Gets the data that is in the root of this binary tree.
	@pre  The binary tree is not empty.
	@post  The root’s data has been returned, and the binary tree is unchanged.
	@return  The data in the root of the binary tree. */
	virtual T getRootData() const = 0;

	/** Replaces the data item in the root of this binary tree
	with the given data, if the tree is not empty. However, if
	the tree is empty, inserts a new root node containing the
	given data into the tree.
	@pre  None.
	@post  The data in the root of the binary tree is as given.
	@param newData  The data for the root. */
	virtual void setRootData(const T& newData) = 0;

	/** Adds a new node containing the given data to this binary tree.
	@param newData  The data for the new node.
	@post  The binary tree contains a new node.
	@return  True if the addition is successful, or false not. */
	virtual bool add(const T& newData) = 0;

	/** Removes the node containing the given data item from this binary tree.
	@param data  The data value to remove from the binary tree.
	@return  True if the removal is successful, or false not. */
	virtual bool remove(const T& data) = 0;

	/** Removes all nodes from this binary tree. */
	virtual void clear() = 0;

	/** Gets a specific entry in this binary tree.
	@post  The desired entry has been returned, and the binary tree
	is unchanged. If no such entry was found, an exception is thrown.
	@param anEntry  The entry to locate.
	@return  The entry in the binary tree that matches the given entry.
	@throw  NotFoundException if the given entry is not in the tree. */
	virtual T getEntry(const T& anEntry) const
		throw(NotFoundException) = 0;

	/** Tests whether a given entry occurs in this binary tree.
	@post  The binary search tree is unchanged.
	@param anEntry  The entry to find.
	@return  True if the entry occurs in the tree, or false if not. */
	virtual bool contains(const T& anEntry) const = 0;

	/** Traverses this binary tree in preorder (inorder, postorder) and
	calls the function visit once for each node.
	@param visit A client-defined function that performs an operation on
	or with the data in each visited node. */
	virtual void preorderTraverse(void visit(T&)) const = 0;
	virtual void inorderTraverse(void visit(T&)) const = 0;
	virtual void postorderTraverse(void visit(T&)) const = 0;
}; // end BinaryTreeInterface

template <typename T>
class BinaryNode
{
private:
	T              item;           // Data portion
	BinaryNode<T>* leftChildPtr;   // Pointer to left child
	BinaryNode<T>* rightChildPtr;  // Pointer to right child
public:
	BinaryNode() : item(nullptr), leftChildPtr(nullptr), rightChildPtr(nullptr)
	{
	}  // end default constructor
	BinaryNode(const T& anItem) : item(anItem), leftChildPtr(nullptr), rightChildPtr(nullptr)
	{
	}  // end constructor
	BinaryNode(const T& anItem, BinaryNode<T>* leftPtr, BinaryNode<T>* rightPtr) : item(anItem), leftChildPtr(leftPtr), rightChildPtr(rightPtr)
	{
	}  // end constructor
	void setItem(const T& anItem)
	{
		item = anItem;
	}  // end setItem
	T getItem() const
	{
		return item;
	}  // end getItem
	bool isLeaf() const
	{
		return ((leftChildPtr == nullptr) && (rightChildPtr == nullptr));
	}
	void setLeftChildPtr(BinaryNode<T>* leftPtr)
	{
		leftChildPtr = leftPtr;
	}  // end setLeftChildPtr
	void setRightChildPtr(BinaryNode<T>* rightPtr)
	{
		rightChildPtr = rightPtr;
	}  // end setRightChildPtr
	BinaryNode<T>* getLeftChildPtr() const
	{
		return leftChildPtr;
	}  // end getLeftChildPtr		
	BinaryNode<T>* getRightChildPtr() const
	{
		return rightChildPtr;
	}  // end getRightChildPtr		
}; // end BinaryNode

template <typename T>
class BinaryNodeTree : public BinaryTreeInterface<T>
{
private:
	BinaryNode<T>* rootPtr;
public:
	int getHeightHelper(BinaryNode<T>* subTreePtr) const
	{
		if (subTreePtr == nullptr)
			return 0;
		else
			return 1 + max(getHeightHelper(subTreePtr->getLeftChildPtr()),
				getHeightHelper(subTreePtr->getRightChildPtr()));
	}  // end getHeightHelper
	int getNumberOfNodesHelper(BinaryNode<T>* subTreePtr) const
	{
		if (subTreePtr == nullptr)
			return 0;
		else
			return 1 + getNumberOfNodesHelper(subTreePtr->getLeftChildPtr())
			+ getNumberOfNodesHelper(subTreePtr->getRightChildPtr());
	}  // end getNumberOfNodesHelper
	BinaryNode<T>* balancedAdd(BinaryNode<T>* subTreePtr, BinaryNode<T>* newNodePtr)
	{
		if (subTreePtr == nullptr)
			return newNodePtr;
		else
		{
			BinaryNode<T>* leftPtr = subTreePtr->getLeftChildPtr();
			BinaryNode<T>* rightPtr = subTreePtr->getRightChildPtr();

			if (getHeightHelper(leftPtr) > getHeightHelper(rightPtr))
			{
				rightPtr = balancedAdd(rightPtr, newNodePtr);
				subTreePtr->setRightChildPtr(rightPtr);
			}
			else
			{
				leftPtr = balancedAdd(leftPtr, newNodePtr);
				subTreePtr->setLeftChildPtr(leftPtr);
			}  // end if

			return subTreePtr;
		}  // end if
	}  // end balancedAdd
	BinaryNode<T>* moveValuesUpTree(BinaryNode<T>* subTreePtr)
	{
		BinaryNode<T>* leftPtr = subTreePtr->getLeftChildPtr();
		BinaryNode<T>* rightPtr = subTreePtr->getRightChildPtr();
		int leftHeight = getHeightHelper(leftPtr);
		int rightHeight = getHeightHelper(rightPtr);
		if (leftHeight > rightHeight)
		{
			subTreePtr->setItem(leftPtr->getItem());
			leftPtr = moveValuesUpTree(leftPtr);
			subTreePtr->setLeftChildPtr(leftPtr);
			return subTreePtr;
		}
		else
		{
			if (rightPtr != nullptr)
			{
				subTreePtr->setItem(rightPtr->getItem());
				rightPtr = moveValuesUpTree(rightPtr);
				subTreePtr->setRightChildPtr(rightPtr);
				return subTreePtr;
			}
			else
			{
				//this was a leaf!
				// value not important
				delete subTreePtr;
				return nullptr;
			}  // end if
		}  // end if   
	}  // end moveValuesUpTree
	BinaryNode<T>* removeValue(BinaryNode<T>* subTreePtr, const T target, bool& success)
	{
		if (subTreePtr == nullptr) // not found here
			return nullptr;

		if (subTreePtr->getItem() == target) // found it
		{
			subTreePtr = moveValuesUpTree(subTreePtr);
			success = true;
			return subTreePtr;
		}
		else
		{
			BinaryNode<T>* targetNodePtr = removeValue(subTreePtr->getLeftChildPtr(), target, success);
			subTreePtr->setLeftChildPtr(targetNodePtr);
			if (!success) // no need to search right subTree
			{
				targetNodePtr = removeValue(subTreePtr->getRightChildPtr(), target, success);
				subTreePtr->setRightChildPtr(targetNodePtr);
			}  // end if

			return subTreePtr;
		}  // end if
	}  // end removeValue
	BinaryNode<T>* findNode(BinaryNode<T>* treePtr, const T& target, bool& success) const
	{
		if (treePtr == nullptr) // not found here
			return nullptr;

		if (treePtr->getItem() == target) // found it
		{
			success = true;
			return treePtr;
		}
		else
		{
			BinaryNode<T>* targetNodePtr = findNode(treePtr->getLeftChildPtr(), target, success);
			if (!success) // no need to search right subTree
			{
				targetNodePtr = findNode(treePtr->getRightChildPtr(), target, success);
			}  // end if

			return targetNodePtr;
		}  // end if 
	}  // end findNode
	BinaryNode<T>* copyTree(const BinaryNode<T>* treePtr) const
	{
		BinaryNode<T>* newTreePtr = nullptr;

		// Copy tree nodes during a preorder traversal
		if (treePtr != nullptr)
		{
			// Copy node
			newTreePtr = new BinaryNode<T>(treePtr->getItem(), nullptr, nullptr);
			newTreePtr->setLeftChildPtr(copyTree(treePtr->getLeftChildPtr()));
			newTreePtr->setRightChildPtr(copyTree(treePtr->getRightChildPtr()));
		}  // end if

		return newTreePtr;
	}  // end copyTree
	void destroyTree(BinaryNode<T>* subTreePtr)
	{
		if (subTreePtr != nullptr)
		{
			destroyTree(subTreePtr->getLeftChildPtr());
			destroyTree(subTreePtr->getRightChildPtr());
			delete subTreePtr;
		}  // end if
	}  // end destroyTree
	void preorder(void visit(T&), BinaryNode<T>* treePtr) const
	{
		if (treePtr != nullptr)
		{
			T theItem = treePtr->getItem();
			visit(theItem);
			preorder(visit, treePtr->getLeftChildPtr());
			preorder(visit, treePtr->getRightChildPtr());
		}  // end if
	}  // end preorder
	void inorder(void visit(T&), BinaryNode<T>* treePtr) const
	{
		if (treePtr != nullptr)
		{
			inorder(visit, treePtr->getLeftChildPtr());
			T theItem = treePtr->getItem();
			visit(theItem);
			inorder(visit, treePtr->getRightChildPtr());
		}  // end if
	}  // end inorder
	void postorder(void visit(T&), BinaryNode<T>* treePtr) const
	{
		if (treePtr != nullptr)
		{
			postorder(visit, treePtr->getLeftChildPtr());
			postorder(visit, treePtr->getRightChildPtr());
			T theItem = treePtr->getItem();
			visit(theItem);
		} // end if
	}  // end postorder
	BinaryNodeTree() : rootPtr(nullptr)
	{
	}  // end default constructor
	BinaryNodeTree(const T& rootItem)
	{
		rootPtr = new BinaryNode<T>(rootItem, nullptr, nullptr);
	}  // end constructor
	BinaryNodeTree(const T& rootItem, const BinaryNodeTree<T>* leftTreePtr, const BinaryNodeTree<T>* rightTreePtr)
	{
		rootPtr = new BinaryNode<T>(rootItem, copyTree(leftTreePtr->rootPtr),
			copyTree(rightTreePtr->rootPtr));
	}  // end constructor
	BinaryNodeTree(const BinaryNodeTree<T>& treePtr)
	{
		rootPtr = copyTree(treePtr.rootPtr);
	}  // end copy constructor
	~BinaryNodeTree()
	{
		destroyTree(rootPtr);
	}  // end destructor
	bool isEmpty() const
	{
		return rootPtr == nullptr;
	}  // end isEmpty
	int getHeight() const
	{
		return getHeightHelper(rootPtr);
	}  // end getHeight
	int getNumberOfNodes() const
	{
		return getNumberOfNodesHelper(rootPtr);
	}  // end getNumberOfNodes
	void clear()
	{
		destroyTree(rootPtr);
		rootPtr = nullptr;
	}  // end clear
	T getRootData() const throw(PrecondViolatedExcep)
	{
		if (isEmpty())
			throw PrecondViolatedExcep("getRootData() called with empty tree.");

		return rootPtr->getItem();
	}  // end getRootData
	void setRootData(const T& newItem)
	{
		if (isEmpty())
			rootPtr = new BinaryNode<T>(newItem, nullptr, nullptr);
		else
			rootPtr->setItem(newItem);
	}  // end setRootData
	bool add(const T& newData)
	{
		BinaryNode<T>* newNodePtr = new BinaryNode<T>(newData);
		rootPtr = balancedAdd(rootPtr, newNodePtr);
		return true;
	}  // end add
	bool remove(const T& target)
	{
		bool isSuccessful = false;
		rootPtr = removeValue(rootPtr, target, isSuccessful);
		return isSuccessful;
	}  // end remove
	T getEntry(const T& anEntry) const throw(NotFoundException)
	{
		bool isSuccessful = false;
		BinaryNode<T>* binaryNodePtr = findNode(rootPtr, anEntry, isSuccessful);

		if (isSuccessful)
			return binaryNodePtr->getItem();
		else
			throw NotFoundException("Entry not found in tree!");
	}  // end getEntry
	bool contains(const T& anEntry) const
	{
		bool isSuccessful = false;
		findNode(rootPtr, anEntry, isSuccessful);
		return isSuccessful;
	}  // end contains

	void preorderTraverse(void visit(T&)) const
	{
		preorder(visit, rootPtr);
	}  // end preorderTraverse
	void inorderTraverse(void visit(T&)) const
	{
		inorder(visit, rootPtr);
	}  // end inorderTraverse
	void postorderTraverse(void visit(T&)) const
	{
		postorder(visit, rootPtr);
	}  // end postorderTraverse
	BinaryNodeTree<T>& operator=(const BinaryNodeTree<T>& rightHandSide)
	{
		if (!isEmpty())
			clear();
		this = copyTree(&rightHandSide);
		return *this;
	}  // end operator=
}; // end BinaryNodeTree

void display(string& anItem)
{
	cout << "Displaying item - " << anItem << endl;
}  // end display

void check(bool success)
{
	if (success)
		cout << "Done." << endl;
	else
		cout << " Entry not in tree." << endl;
}  // end check

int main()
{
	BinaryTreeInterface<string>* tree1 = new BinaryNodeTree<string>();

	tree1->add("10");
	tree1->add("20");
	tree1->add("30");
	tree1->add("40");
	tree1->add("50");
	tree1->add("60");
	tree1->add("70");
	tree1->add("80");

	cout << "Tree 1 Preorder: Should be 10 20 40 70 60 30 50 80\n";
	tree1->preorderTraverse(display);

	cout << "Tree 1 Inorder: Should be 70 40 20 60 10 50 30 80\n";
	tree1->inorderTraverse(display);

	cout << "Tree 1 Postorder: Should be 70 40 60 20 50 80 30 10\n";
	tree1->postorderTraverse(display);

	return 0;
}  // end main