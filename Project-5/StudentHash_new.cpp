#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <functional>

using namespace std;

struct Pair
{
	string key;
	double value;  
	Pair(string n="empty", double p=-1.00) : key(n), value(p) { }
};


Pair studentlist[441];

template <typename T>
class DataSource
{
	vector<T> Data;
public:
	DataSource(T* data,int size)
	{
		for (int index = 0; index < size; index++)
			Data.push_back(data[index]);
	}
	vector<T>& GetData()
	{
		return Data;
	}
};

template <typename T>
class HashTable
{
	T* hashTable;
	T* collStore;
	int size;
	int collisions;
public:
	HashTable(int number) : size(number), collisions(0)
	{
		hashTable = new T[size+1];
		collStore = new T[size+1];
	}
	~HashTable() { delete[] hashTable; }
	void GetData(vector<T>& vdata)
	{
		InitializeHashTable(vdata);
		CheckCollisions();
	}
	int InitializeHashTable(vector<T>& vdata)
	{
		T uninitialized;
		for (int index = 0; index < size; index++)
			hashTable[index] = uninitialized;
		for (Pair data : vdata)
		{
			int index = HashGenerator(data.key, 0);
			if (hashTable[index].key.compare("empty") != 0) {
				collStore[collisions] = data;
				collisions++;
			}
			hashTable[index] = data;
		}
		return collisions;
	}
	double CheckCollisions()
	{
		double percent = double(collisions) / double(size) * 100;
		cout << "Percentage of collisions " << percent << "%" << endl;
		return percent;
	}
	int HashGenerator(string str, int j)
	{	
		unsigned int hash = 0;
		unsigned int prime_one   = 10103;
		unsigned int prime_two    = 49921;
		for (int i = 0; i < str.length(); i++) {
			hash = hash * prime_one + str[i];
			prime_one = prime_one * prime_two;
		}
		string hold = to_string(hash);
		hold = hold.substr(0, 4);
		return atoi(hold.c_str());
	}
	int HashIndex(string sdata)
	{
		return HashGenerator(sdata, 0);
	}
	T& Find(string sfind)
	{
		int hashcode = HashGenerator(sfind);
		return hashTable[hashcode];
	}
	void Traverse(Pair pair, int index)
	{
		if (index < size)
		{
			index++;
			if (pair.key.compare("empty") != 0)
				cout << pair.key << ", " << pair.value << endl;
			Traverse(hashTable[index],index);
		}
	}
	void traverseCollisions() {
		for (int i = 0; i < collisions; i++) {
			cout << collStore[i].key << ", " << collStore[i].value << endl;
		}
	}

	int hashInsert(string key) {
		int i = 0;
		do {
			int j = HashGenerator(key, i);
			if (hashTable[j].key.compare("empty") == 0) {
				hashTable[j] = key;
				return j;
			} else {
				i++;
			}
		} while (i != size);
		cout << "Hash Table Not Found" << endl;
		return -1;
	}
	bool hashRemove(string key) {
		int i = 0;
		do {
			int j = HashGenerator(key);
			if (hashTable[j].key.compare("empty") == 0) {
				hashTable[j].value = -1.00;
				hashTable[j].key = "deleted";
				return true;
			} else {
				i++;
			}
		} while (i != size);
		return false;
	}
	Pair operator[] (int index)
	{
		return hashTable[index];
	}
};

int main()
{
	int size = 441;
	string line;
	ifstream myfile ("UProducts.csv");
	int count = 0;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp[2];
			istringstream ss(line);
			getline(ss, temp[0], ',');
			getline(ss, temp[1], ',');
			Pair hold (temp[0], atof(temp[1].c_str()));
			studentlist[count] = hold;
			count++;
		}
	}
	myfile.close();
	DataSource<Pair> data(studentlist,size);
	HashTable<Pair> hashTable(9999);
	hashTable.GetData( data.GetData() );

	cout << "These are normal items in the hash table: " << endl;
	hashTable.Traverse(hashTable[0],0);

	cout << "These are conflicted items in the hash table" << endl;
	hashTable.traverseCollisions();
	system("pause");
	return 0;
}