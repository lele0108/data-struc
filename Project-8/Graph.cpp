#include <iostream>
#include <list>

using namespace std;

class Vertex
{
	int vertex;
public:
	Vertex(int v) { vertex = v; }
	operator int() { return vertex; }
};

template <typename T>
class Adjacency
{
	list<T>* adjacency;
	int size;
public:
	Adjacency(int n = 0)
	{
		adjacency = new list<T>[n];
		size = n;
	}
	void Add(int v, int w)
	{
		adjacency[v].push_back(w);
	}
	int getSize() { return size; }
	list<T>& getList(int v) { return adjacency[v]; }
	list<T>& operator [] (int v) { return getList(v); }
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
		for (auto pos = (*adj)[v].begin(); pos != (*adj)[v].end(); ++pos) {
			if (!visited[*pos]) {
				cout << *pos << " ";
				DFSHelper(*pos, visited);
			}
		}
	}
	void newDFS(int v) {
		bool *visited = new bool[adj->getSize()];
		for (int i = 0; i < adj->getSize(); i++)
			visited[i] = false;

		DFSHelper(v, visited);
	}
	void DFS(int v, bool* visited)
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
	}
	void BreadthFirstSearch(int v)
	{
		bool *visited = new bool[adj->getSize()];
		for (int x = 0; x < adj->getSize(); x++)
			visited[x] = false;

		list<int> queue;
		visited[v] = true;
		queue.push_back(v);

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
	}
	void newBFS(int v) {
		bool *visited = new bool[adj->getSize()];
		for (int x = 0; x < adj->getSize(); x++) {
			visited[x] = false;
		}

		list<int> queue;
		visited[v] = true;
		queue.push_back(v);

		while (!queue.empty()) {
			v = queue.front();
			queue.pop_front();
			for (auto pos = (*adj)[v].begin(); pos != (*adj)[v].end(); ++pos) {
				if (!visited[*pos]) {
					visited[*pos] = true;
					cout << *pos << " ";
					queue.push_back(*pos);
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
	//gdepth.DepthFirstSearch(2);
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
	return 0;
}