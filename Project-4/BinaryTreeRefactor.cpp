#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace std;

class PrecondViolatedExcep : public logic_error
{
public:
	PrecondViolatedExcep::PrecondViolatedExcep(const string& message="") : logic_error("Precondition Violated Exception: " + message)
	{
	}  
};

class NotFoundException : public logic_error
{
public:
	NotFoundException::NotFoundException(const string& message="") : logic_error("Precondition Violated Exception: " + message)
	{
	}  
}; 

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

	/** Gets the number of Nodes in this binary tree.
	@return The number of Nodes in the binary tree. */
	virtual int getNumberOfNodes() const = 0;

	/** Gets the data that is in the root of this binary tree.
	@pre  The binary tree is not empty.
	@post  The root’s data has been returned, and the binary tree is unchanged.
	@return  The data in the root of the binary tree. */
	virtual T getRootData() const = 0;

	/** Replaces the data item in the root of this binary tree
	with the given data, if the tree is not empty. However, if
	the tree is empty, inserts a new root Node containing the
	given data into the tree.
	@pre  None.
	@post  The data in the root of the binary tree is as given.
	@param newData  The data for the root. */
	virtual void setRootData(const T& newData) = 0;

	/** Adds a new Node containing the given data to this binary tree.
	@param newData  The data for the new Node.
	@post  The binary tree contains a new Node.
	@return  True if the addition is successful, or false not. */
	virtual bool add(const T& newData, const string ="") = 0;

	/** Removes the Node containing the given data item from this binary tree.
	@param data  The data value to remove from the binary tree.
	@return  True if the removal is successful, or false not. */
	virtual bool remove(const T& data) = 0;

	/** Removes all Nodes from this binary tree. */
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
	calls the function visit once for each Node.
	@param visit A client-defined function that performs an operation on
	or with the data in each visited Node. */
	virtual void preorderTraverse(void visit(T&)) const = 0;
	virtual void inorderTraverse(void visit(T&)) const = 0;
	virtual void postorderTraverse(void visit(T&)) const = 0;
}; // end BinaryTreeInterface

template <typename T>
class Node
{
private:
	T        Data;           // Data portion
	Node<T>* Left;   // Pointer to left child
	Node<T>* Right;  // Pointer to right child
public:
	Node() : Data(nullptr), Left(nullptr), Right(nullptr)
	{
	}  
	Node(const T& anItem) : Data(anItem), Left(nullptr), Right(nullptr)
	{
	}  
	Node(const T& anItem, Node<T>* leftPtr, Node<T>* rightPtr) : Data(anItem), Left(leftPtr), Right(rightPtr)
	{
	}  
	void Node<T>::setItem(const T& anItem)
	{
		Data = anItem;
	}  
	T getItem() const
	{
		return Data;
	} 
	bool isLeaf() const
	{
		return ((Left == nullptr) && (Right == nullptr));
	}
	void setLeft(Node<T>* leftPtr)
	{
		Left = leftPtr;
	}  
	void setRight(Node<T>* rightPtr)
	{
		Right = rightPtr;
	}  
	Node<T>* getLeft() const
	{
		return Left;
	}  	
	Node<T>* getRight() const
	{
		return Right;
	}  	
};

template <typename T>
class BinaryTree : public BinaryTreeInterface<T>
{
private:
	Node<T>* root;
public:
	int getHeightHelper(Node<T>* nodePtr) const
	{
		if (nodePtr == nullptr)
			return 0;
		else
			return 1 + max(getHeightHelper(nodePtr->getLeft()),getHeightHelper(nodePtr->getRight()));
	}  
	int getNumberOfNodesHelper(Node<T>* nodePtr) const
	{
		if (nodePtr == nullptr)
			return 0;
		else
			return 1 + getNumberOfNodesHelper(nodePtr->getLeft()) + getNumberOfNodesHelper(nodePtr->getRight());
	}  
	Node<T>* balancedAdd(Node<T>* nodePtr, Node<T>* newNodePtr)
	{
		if (nodePtr == nullptr)
			return newNodePtr;
		else
		{
			Node<T>* leftPtr = nodePtr->getLeft();
			Node<T>* rightPtr = nodePtr->getRight();

			if (getHeightHelper(leftPtr) > getHeightHelper(rightPtr))
			{
				rightPtr = balancedAdd(rightPtr, newNodePtr);
				nodePtr->setRight(rightPtr);
			}
			else
			{
				leftPtr = balancedAdd(leftPtr, newNodePtr);
				nodePtr->setLeft(leftPtr);
			}  

			return nodePtr;
		}  
	}  
	Node<T>* orderAdd(Node<T>* nodePtr, Node<T>* newNodePtr)
	{
		if (nodePtr == nullptr)
			return newNodePtr;
		else
		{
			Node<T>* leftPtr = nodePtr->getLeft();
			Node<T>* rightPtr = nodePtr->getRight();

			if (newNodePtr->getItem() > nodePtr->getItem())
			{
				rightPtr = orderAdd(rightPtr, newNodePtr);
				nodePtr->setRight(rightPtr);
			}
			else
			{
				leftPtr = orderAdd(leftPtr, newNodePtr);
				nodePtr->setLeft(leftPtr);
			}  

			return nodePtr;
		}  
	}  
	Node<T>* moveValuesUpTree(Node<T>* nodePtr)
	{
		cout << "Debug:  Entering moveValuesUpTree " << nodePtr->getItem() << endl;
		Node<T>* leftPtr = nodePtr->getLeft();
		Node<T>* rightPtr = nodePtr->getRight();
		int leftHeight = getHeightHelper(leftPtr);
		int rightHeight = getHeightHelper(rightPtr);
		if (leftHeight > rightHeight)
		{
			nodePtr->setItem(leftPtr->getItem());
			leftPtr = moveValuesUpTree(leftPtr);
			nodePtr->setLeft(leftPtr);
			return nodePtr;
		}
		else
		{
			if (rightPtr != nullptr)
			{
				nodePtr->setItem(rightPtr->getItem());
				rightPtr = moveValuesUpTree(rightPtr);
				nodePtr->setRight(rightPtr);
				return nodePtr;
			}
			else
			{
				cout << "Debug: Deleteing " << nodePtr->getItem() << endl;
				delete nodePtr;
				return nullptr;
			}  
		}    
	}  
	Node<T>* removeValue(Node<T>* nodePtr, const T target, bool& success)
	{
		if (nodePtr == nullptr) 
			return nullptr;
		cout << "Debug:  entering removeValue: " << nodePtr->getItem() << "\tTarget = " << target << endl;
		if (nodePtr->getItem() == target) 
		{
			cout << "Debug: nodePtr->getItem == target\n";
			nodePtr = moveValuesUpTree(nodePtr);
			success = true;
			return nodePtr;
		}
		else
		{
			Node<T>* targetNodePtr = removeValue(nodePtr->getLeft(), target, success);
			nodePtr->setLeft(targetNodePtr);
			if (!success) // no need to search right subTree
			{
				targetNodePtr = removeValue(nodePtr->getRight(), target, success);
				nodePtr->setRight(targetNodePtr);
			}  
			return nodePtr;
		}  
	}  
	Node<T>* findNode(Node<T>* treePtr, const T& target, bool& success) const
	{
		if (treePtr == nullptr) 
			return nullptr;

		if (treePtr->getItem() == target) 
		{
			success = true;
			return treePtr;
		}
		else
		{
			Node<T>* targetNodePtr = findNode(treePtr->getLeft(), target, success);
			if (!success) // no need to search right subTree
			{
				targetNodePtr = findNode(treePtr->getRight(), target, success);
			}  
			return targetNodePtr;
		}  
	}  
	Node<T>* copyTree(const Node<T>* treePtr) const
	{
		Node<T>* newTreePtr = nullptr;

		if (treePtr != nullptr)
		{
			newTreePtr = new Node<T>(treePtr->getItem(), nullptr, nullptr);
			newTreePtr->setLeft(copyTree(treePtr->getLeft()));
			newTreePtr->setRight(copyTree(treePtr->getRight()));
		}  
		return newTreePtr;
	}  
	void destroyTree(Node<T>* nodePtr)
	{
		if (nodePtr != nullptr)
		{
			destroyTree(nodePtr->getLeft());
			destroyTree(nodePtr->getRight());
			delete nodePtr;
		}  
	}  
	void preorder(void visit(T&), Node<T>* treePtr) const
	{
		if (treePtr != nullptr)
		{
			T theItem = treePtr->getItem();
			visit(theItem);
			preorder(visit, treePtr->getLeft());
			preorder(visit, treePtr->getRight());
		}  
	}  
	void inorder(void visit(T&), Node<T>* treePtr) const
	{
		if (treePtr != nullptr)
		{
			inorder(visit, treePtr->getLeft());
			T theItem = treePtr->getItem();
			visit(theItem);
			string sdesc = "\tRoot";
			if (treePtr->getItem() < root->getItem())
				sdesc = "\tLeft";
			if (treePtr->getItem() > root->getItem())
				sdesc = "\tRight";
			cout << sdesc << endl;
			inorder(visit, treePtr->getRight());
		}  
	}  
	void postorder(void visit(T&), Node<T>* treePtr) const
	{
		if (treePtr != nullptr)
		{
			postorder(visit, treePtr->getLeft());
			postorder(visit, treePtr->getRight());
			T theItem = treePtr->getItem();
			visit(theItem);
		} 
	}  
	BinaryTree() : root(nullptr)
	{
	}  
	BinaryTree(const T& rootItem)
	{
		root = new Node<T>(rootItem, nullptr, nullptr);
	}  
	BinaryTree(const T& rootItem, const BinaryTree<T>* leftTreePtr, const BinaryTree<T>* rightTreePtr)
	{
		root = new Node<T>(rootItem, copyTree(leftTreePtr->root),copyTree(rightTreePtr->root));
	}  
	BinaryTree(const BinaryTree<T>& treePtr)
	{
		root = copyTree(treePtr.root);
	}  
	~BinaryTree()
	{
		destroyTree(root);
	}  
	bool isEmpty() const
	{
		return root == nullptr;
	}  
	int getHeight() const
	{
		return getHeightHelper(root);
	}  
	int getNumberOfNodes() const
	{
		return getNumberOfNodesHelper(root);
	}  
	void clear()
	{
		destroyTree(root);
		root = nullptr;
	}  
	T getRootData() const throw(PrecondViolatedExcep)
	{
		if (isEmpty())
			throw PrecondViolatedExcep("getRootData() called with empty tree.");

		return root->getItem();
	}  
	void setRootData(const T& newItem)
	{
		if (isEmpty())
			root = new Node<T>(newItem, nullptr, nullptr);
		else
			root->setItem(newItem);
	}  
	bool add(const T& newData,string stype)
	{
		Node<T>* newNodePtr = new Node<T>(newData);
		if (stype.compare("balance") == 0)
			root = balancedAdd(root, newNodePtr);
		else
			root = orderAdd(root, newNodePtr);
		return true;
	}  
	bool remove(const T& target)
	{
		cout << "Debug:  Entering remove: " << target << endl;
		bool isSuccessful = false;
		root = removeValue(root, target, isSuccessful);
		return isSuccessful;
	}  
	T getEntry(const T& anEntry) const throw(NotFoundException)
	{
		bool isSuccessful = false;
		Node<T>* NodePtr = findNode(root, anEntry, isSuccessful);

		if (isSuccessful)
			return NodePtr->getItem();
		else
			throw NotFoundException("Entry not found in tree!");
	}  
	bool contains(const T& anEntry) const
	{
		bool isSuccessful = false;
		findNode(root, anEntry, isSuccessful);
		return isSuccessful;
	}  
	void preorderTraverse(void visit(T&)) const
	{
		preorder(visit, root);
	}  
	void inorderTraverse(void visit(T&)) const
	{
		inorder(visit, root);
	}  
	void postorderTraverse(void visit(T&)) const
	{
		postorder(visit, root);
	}  
	BinaryTree<T>& operator=(const BinaryTree<T>& rightHandSide)
	{
		if (!isEmpty())
			clear();
		this = copyTree(&rightHandSide);
		return *this;
	}  
}; 

template <typename T>
void display(T& anItem)
{
	cout << "Displaying item - " << anItem;
}  

void check(bool success)
{
	if (success)
		cout << "Done." << endl;
	else
		cout << " Entry not in tree." << endl;
}  

void InOrder()
{
	int balance[] = { 42, 68, 35, 1, 70, 25, 79, 59, 63, 65 };
	BinaryTreeInterface<int>* tree = new BinaryTree<int>();

	for (int x = 0; x < 10; x++)
		tree->add(balance[x],"inorder");
	cout << "Order ---" << endl;

	tree->inorderTraverse(display);

	tree->remove(59);
	cout << "---" << endl;
	tree->inorderTraverse(display);
	delete tree;
}  

void Balance()
{
	int balance[] = { 42, 68, 35, 1, 70, 25, 79, 59, 63, 65 };
	BinaryTreeInterface<int>* tree = new BinaryTree<int>();

	for (int x = 0; x < 10; x++)
		tree->add(balance[x], "balance");
	cout << "Balance ---" << endl;

	tree->inorderTraverse(display);

	tree->remove(balance[2]);
	tree->remove(balance[4]);
	tree->remove(balance[6]);
	cout << "---" << endl;
	tree->inorderTraverse(display);

	delete tree;
}

void main()
{
	InOrder();
	Balance();
}