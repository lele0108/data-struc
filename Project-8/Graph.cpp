// Jimmy Liu
// Data Structures
// Mar 11th 2016

#include <iostream>
#include <list>
#include <array>
#include <stdexcept>
#include <string>
#include <iostream>
#include <cassert>
#include <sstream>
#include <memory>
#include <iomanip>
#include <fstream>
#include <cstring>
using namespace std;

class Vertex {
	int vertex;
public:
	Vertex(int v) { vertex = v; }
	operator int() { return vertex; }
};

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
	int finCount;
public:
	LinkedQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;
		count = 0;
		finCount = 0;
	}  // end default constructor
	~LinkedQueue()
	{
		while (!empty())
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
		if (empty())
			frontPtr = newNodePtr;        // Insertion into empty queue
		else
			backPtr->setNext(newNodePtr); // Insertion into nonempty queue

		backPtr = newNodePtr;            // New node is at back
		count++;
		finCount++;
		return true;
	}  // end enqueue
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
			finCount--;
			result = true;
		}  // end if

		return result;
	}  // end dequeue
	T peekFront()
	{
		if (isEmpty()) {
			cout << "HASH" << endl;
			return Vertex(-1);
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
	bool empty() {
		if (finCount == 0)
			return true;
		return false;
	}
	   // End of implementation file.
}; // end LinkedQueue

template <typename T>
class Adjacency
{
	LinkedQueue<T>* adjacency;
	int size;
public:
	Adjacency(int n = 0)
	{
		//adjacency = new list<T>[n];
		adjacency = new LinkedQueue<T>[n];
		size = n;
	}
	void Add(int v, int w)
	{
		//adjacency[v].push_back(w);
		adjacency[v].enqueue(w);
	}
	int getSize() { return size; }
	LinkedQueue<T>& getList(int v) { return adjacency[v]; }
	LinkedQueue<T>& operator [] (int v) { return getList(v); }
};

// Graph adjacency list representation
template <typename T>
class Graph
{
	Adjacency<T>* adj;
public:
	Graph(int V)
	{
		adj = new Adjacency<T>(V);
	}
	void addEdge(int v, int w)
	{
		adj->Add(v, w);
	}
	void DFSHelper(int v, bool* visited) {
		visited[v] = true;
		for (int i = 0; i < adj->getList(v).getCount(); i++) {
		//for (auto pos = (*adj)[v].begin(); pos != (*adj)[v].end(); ++pos) {
			int pos = adj->getList(v).peekFront();
			adj->getList(v).dequeue();
			if (!visited[pos]) {
				cout << pos << " ";
				DFSHelper(pos, visited);
			}
		}
	}
	void newDFS(int v) {
		bool *visited = new bool[adj->getSize()];
		for (int i = 0; i < adj->getSize(); i++)
			visited[i] = false;

		DFSHelper(v, visited);
	}
	/*void DFS(int v, bool* visited)
	{
		visited[v] = true;
		cout << v << " ";

		for (auto pos = (*adj)[v].begin(); pos != (*adj)[v].end(); ++pos)
			if (!visited[*pos])
				DFS(*pos, visited);
	}
	void DepthFirstSearch(int v)
	{
		bool *visited = new bool[adj->getSize()];
		for (int i = 0; i < adj->getSize(); i++)
			visited[i] = false;

		DFS(v, visited);
	}*/
	/*void BreadthFirstSearch(int v)
	{
		bool *visited = new bool[adj->getSize()];
		for (int x = 0; x < adj->getSize(); x++)
			visited[x] = false;

		//LinkedQueue<int> queue;
		list<int> queue;
		visited[v] = true;
		queue.enqueue(v);

		while (!queue.empty())
		{
			v = queue.front();
			cout << v << " ";
			queue.pop_front();

			for (auto pos = (*adj)[v].begin(); pos != (*adj)[v].end(); ++pos)
			{
				if (!visited[*pos])
				{
					visited[*pos] = true;
					queue.push_back(*pos);
				}
			}
		}
	}*/
	void newBFS(int v) {
		bool *visited = new bool[adj->getSize()];
		for (int x = 0; x < adj->getSize(); x++) {
			visited[x] = false;
		}

		LinkedQueue<int> queue;
		//list<int> queue;
		visited[v] = true;
		queue.enqueue(v);
		while (!queue.empty()) {
			v = queue.peekFront();
			queue.dequeue();
			for (int i = 0; i < adj->getList(v).getCount(); i++) {
				int pos = adj->getList(v).peekFront();
				adj->getList(v).dequeue();
				if (!visited[pos]) {
					visited[pos] = true;
					cout << pos << " ";		
					queue.enqueue(pos);
				}
			}
		}
		cout << endl;
	}
};

void Depth()
{
	Vertex SFO(0);
	Vertex LAX(1);
	Vertex DFW(2);
	Vertex ORD(3);
	Vertex JFK(4);
	Vertex BOS(5);
	Vertex MIA(6);
	Graph<int> gdepth(7);
	gdepth.addEdge(SFO, BOS);
	gdepth.addEdge(SFO, LAX);
	gdepth.addEdge(SFO, DFW);
	gdepth.addEdge(SFO, ORD);
	gdepth.addEdge(LAX, SFO);
	gdepth.addEdge(LAX, MIA);
	gdepth.addEdge(LAX, DFW);
	gdepth.addEdge(DFW, SFO);
	gdepth.addEdge(DFW, LAX);
	gdepth.addEdge(DFW, MIA);
	gdepth.addEdge(DFW, ORD);
	gdepth.addEdge(ORD, SFO);
	gdepth.addEdge(ORD, DFW);
	gdepth.addEdge(ORD, JFK);
	gdepth.addEdge(ORD, BOS);
	gdepth.addEdge(JFK, ORD);
	gdepth.addEdge(JFK, BOS);
	gdepth.addEdge(JFK, MIA);
	gdepth.addEdge(BOS, SFO);
	gdepth.addEdge(BOS, ORD);
	gdepth.addEdge(BOS, JFK);
	gdepth.addEdge(BOS, MIA);
	gdepth.addEdge(MIA, BOS);
	gdepth.addEdge(MIA, JFK);
	gdepth.addEdge(MIA, DFW);
	gdepth.addEdge(MIA, LAX);
	cout << "\nDepth First Traversal" << endl;

	gdepth.newDFS(2);
}

void Breadth()
{
	Vertex SFO(0);
	Vertex LAX(1);
	Vertex DFW(2);
	Vertex ORD(3);
	Vertex JFK(4);
	Vertex BOS(5);
	Vertex MIA(6);
	Graph<Vertex> gbreadth(7);
	gbreadth.addEdge(SFO, BOS);
	gbreadth.addEdge(SFO, LAX);
	gbreadth.addEdge(SFO, DFW);
	gbreadth.addEdge(SFO, ORD);
	gbreadth.addEdge(LAX, SFO);
	gbreadth.addEdge(LAX, MIA);
	gbreadth.addEdge(LAX, DFW);
	gbreadth.addEdge(DFW, SFO);
	gbreadth.addEdge(DFW, LAX);
	gbreadth.addEdge(DFW, MIA);
	gbreadth.addEdge(DFW, ORD);
	gbreadth.addEdge(ORD, SFO);
	gbreadth.addEdge(ORD, DFW);
	gbreadth.addEdge(ORD, JFK);
	gbreadth.addEdge(ORD, BOS);
	gbreadth.addEdge(JFK, ORD);
	gbreadth.addEdge(JFK, BOS);
	gbreadth.addEdge(JFK, MIA);
	gbreadth.addEdge(BOS, SFO);
	gbreadth.addEdge(BOS, ORD);
	gbreadth.addEdge(BOS, JFK);
	gbreadth.addEdge(BOS, MIA);
	gbreadth.addEdge(MIA, BOS);
	gbreadth.addEdge(MIA, JFK);
	gbreadth.addEdge(MIA, DFW);
	gbreadth.addEdge(MIA, LAX);

	cout << "\nBreadth First Traversal" << endl;
	gbreadth.newBFS(2);
}

int main()
{
	Depth();
	Breadth();
	system("pause");
	return 0;
}