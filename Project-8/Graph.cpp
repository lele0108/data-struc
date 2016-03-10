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
};

void Depth()
{
	Vertex v0(0);
	Vertex v1(1);
	Vertex v2(2);
	Vertex v3(3);
	Graph<int> gdepth(4);
	gdepth.addEdge(v0, v1);
	gdepth.addEdge(v0, v2);
	gdepth.addEdge(v1, v2);
	gdepth.addEdge(v2, v0);
	gdepth.addEdge(v2, v3);
	gdepth.addEdge(v3, v3);

	cout << "\nDepth First Traversal" << endl;
	gdepth.DepthFirstSearch(2);
}

void Breadth()
{
	Vertex v0(0);
	Vertex v1(1);
	Vertex v2(2);
	Vertex v3(3);
	Graph<Vertex> gbreadth(4);
	gbreadth.addEdge(v0, v1);
	gbreadth.addEdge(v0, v2);
	gbreadth.addEdge(v1, v2);
	gbreadth.addEdge(v2, v0);
	gbreadth.addEdge(v2, v3);
	gbreadth.addEdge(v3, v3);

	cout << "\nBreadth First Traversal" << endl;
	gbreadth.BreadthFirstSearch(2);
}

int main()
{
	Depth();
	Breadth();
	return 0;
}