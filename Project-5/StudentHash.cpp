#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

using namespace std;

struct Student
{
	string sfirst;
	string slast;
	Student(string l, string f) : sfirst(f), slast(l) { }
};

struct Pair
{
	string key;
	Student value;  
	Pair(string n="0", string f="F", string l="L") : key(n), value(l,f) { }
};


Pair studentlist[] =
{
	Pair("20170936","Bango","Herman"),
	Pair("20162142","Braun","Edward"),
	Pair("11269053","Brewer","Ken"),
	Pair("10548356","Byars","Kathy"),
	Pair("20150567","Chan","Hong"),
	Pair("11380053","Chang","Amy"),
	Pair("20148859","Chaw","Ken Sing"),
	Pair("20230962","Chia","Qing"),
	Pair("20217161","Chukhlebov","Alex"),
	Pair("20132217","Dong","Hung"),
	Pair("10242737","Facio","Robert"),
	Pair("10643692","Garcelon","Robert"),
	Pair("20028675","Grekhov","Chekov"),
	Pair("11197729","Halley","Berry"),
	Pair("20169129","Hung Liang","Denny"),
	Pair("11052122","Jeng","Jeff"),
	Pair("10128518","Lambright","Lydia"),
	Pair("20207513","Lee","Frank"),
	Pair("20232989","Lim","Artimus"),
	Pair("20194743","Man","Kai"),
	Pair("20194681","Nguyen","Rammon"),
	Pair("11182688","Phan","Don"),
	Pair("20258135","Pine","Stephen"),
	Pair("20252502","Rosa","Arbute"),
	Pair("11161078","Shah","Kasam"),
	Pair("20185646","Shaw","Mian"),
	Pair("20018772","Tang","Thomas"),
	Pair("20177176","Tatum","Elphonse"),
	Pair("20206541","Uppuri","Ron"),
	Pair("11265973","Wang","Angela"),
	Pair("11083739","Wang","Shen Ting"),
	Pair("20054095","Wang","Yu"),
	Pair("20194002","Wilson","Jack"),
	Pair("20167765","Wojeck","Vince"),
	Pair("20112993","Woods","Tiger"),
	Pair("11255446","Yang","Tang"),
	Pair("20265054","Yost","Robert"),
	Pair("20157439","Zhang","Alice"),
	Pair("20218691","Zuccar","Ben"),
};

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
	int size;
	int collisions;
public:
	HashTable(int number) : size(number), collisions(0)
	{
		hashTable = new T[size+1];
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
			int index = HashGenerator(data.key);
			if (hashTable[index].key.compare("0") != 0)
				collisions++;
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
	int HashGenerator(string str)
	{
		int hash = 0;
		vector<int> vlist;
		for (char c : str)
			vlist.push_back(c - '0');
		int multiply = 1;
		for (int n = 0; n < vlist.size(); n += 2)
		{
			int n1 = vlist[n];
			int n2 = vlist[n + 1];
			int sum = (n1 + n2) * multiply;
			hash += sum;
			multiply *= 10;
		}
		hash %= (size - 1);
		return hash;
	}
	int HashIndex(string sdata)
	{
		return HashGenerator(sdata);
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
			if (pair.key.compare("0") != 0)
				cout << pair.value.slast << ", " << pair.value.sfirst << endl;
			Traverse(hashTable[index],index);
		}
	}
	int hashInsert(string key) {
		int i = 0;
		while (i != size) {
			int j = HashGenerator(key);
			if (hashTable[j].key.compare("0") == 0) {
				hashTable[j] = key;
				return j;
			} else {
				i++;
			}
		}
		cout << "Hash Table Not Found" << endl;
		return -1;
	}
	bool hashRemove(string key) {
		int i = 0;
		while (i != size) {
			int j = HashGenerator(key);
			if (hashTable[j].key.compare("0") == 0) {
				hashTable[j] = 0;
				return true;
			} else {
				i++;
			}
		}
		return false;
	}
	Pair operator[] (int index)
	{
		return hashTable[index];
	}
};

int main()
{
	int size = sizeof(studentlist) / sizeof(studentlist[0]);
	DataSource<Pair> data(studentlist,size);
	HashTable<Pair> hashTable(500);
	hashTable.GetData( data.GetData() );
	string sfindID = "20252502";  // Pair("20252502","Rosa","Arbute"),
	Pair foundStudent = hashTable.Find(sfindID);
	cout << foundStudent.value.slast << "\t" << foundStudent.value.sfirst << endl;
	//hashTable.Traverse(hashTable[0],0);
	return 0;
}