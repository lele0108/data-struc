// Jimmy (Sicong) Liu
// Jan 20th 2016
// Project 2
// Implement DoublyLinkedList, Insertion Sort

#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

#define BLANK " "

#define MIN_SIZE 3

class Product {
   private:
      double price;
      string name; 
   public:
      Product() {
         price = -1.0;
      }
      Product(string a, double b) {
         name = a;
         price = b;
      }
      void setPrice(double p) {
         price = p;
      }
      void setName(string n) {
         name = n;
      }
      friend ostream& operator<<(ostream& os, const Product& pd) {
         os << pd.name << " " << pd.price;
         return os;
      }
      bool operator== (const Product &one) {
         return (price == one.price && name == one.name); 
      }
      Product& operator= (const Product &one) {
         price = one.price;
         name = one.name;
         return *this;
      }
      bool operator< (const Product &one) {
         return (name < one.name); 
      }
      bool operator<= (const Product &one) {
         return (name < one.name || name == one.name); 
      }
      bool operator> (const Product &one) {
         return (name > one.name); 
      }
};

template<typename T>
int partition(vector<T> & theArray, int first, int last)
{  
   T x = theArray.at(last);
   int i = first - 1; // problem this line
   for (int j = first; j < last; j++) {
      if (theArray.at(j) <= x) {
         i++;
         T hold = theArray.at(i);
         theArray.at(i) = theArray.at(j);
         //theArray[i] = theArray[j];
         theArray.at(j) = hold;
         //theArray[j] = hold;
      }
   }
   T hold = theArray.at(i+1);
   theArray.at(i+1) = theArray.at(last);
   //theArray[i+1] = theArray[last];
   theArray.at(last) = hold;
   //theArray[last] = hold;
   return i+1;
}  // end quickSort

template<typename T>
void quickSort(vector<T> & theArray, int first, int last)
{
   if (last - first + 1 < MIN_SIZE)
   {
      insertionSort(theArray, last - first + 1);
   }
   else
   {
      // Create the partition: S1 | Pivot | S2
      int pivotIndex = partition(theArray, first, last);
      
      // Sort subarrays S1 and S2
      quickSort(theArray, first, pivotIndex - 1);
      quickSort(theArray, pivotIndex + 1, last);
   }  // end if
}  // end quickSort

template<typename T>
void insertionSort(vector<T> & theArray, int n)
{
   // unsorted = first index of the unsorted region,
   // loc = index of insertion in the sorted region,
   // nextItem = next item in the unsorted region.
   // Initially, sorted region is theArray[0],
   // unsorted region is theArray[1..n-1].
   // In general, sorted region is theArray[0..unsorted-1],
   // unsorted region theArray[unsorted..n-1]
   for (int unsorted = 1; unsorted < n; unsorted++)
   {
      // At this point, theArray[0..unsorted-1] is sorted.
      // Find the right position (loc) in theArray[0..unsorted]
      // for theArray[unsorted], which is the first entry in the
      // unsorted region; shift, if necessary, to make room
      T nextItem = theArray.at(unsorted);
      int loc = unsorted;
      while ((loc > 0) && (theArray.at(loc - 1) > nextItem))
      {
         // Shift theArray[loc - 1] to the right
         //theArray[loc] = theArray[loc - 1];
         theArray.at(loc) = theArray.at(loc-1);
         loc--;
      }  // end while
      
      // At this point, theArray[loc] is where nextItem belongs
      //theArray[loc] = nextItem; // Insert nextItem into sorted region
      theArray.at(loc) = nextItem;
   }  // end for
}  // end insertionSort


void quickTester() 
{
   vector<Product> flex_Store; 
   string line;
   ifstream myfile ("UProducts.csv");
   if (myfile.is_open()) {
      while (getline(myfile, line)) {
         string temp[2];
         istringstream ss(line);
         getline(ss, temp[0], ',');
         getline(ss, temp[1], ',');
         Product hold (temp[0], atof(temp[1].c_str()));
         flex_Store.push_back(hold);
      }
   }
   myfile.close();
   quickSort(flex_Store, 0, flex_Store.size() - 1);
   for (int i = 0; i < flex_Store.size(); i++) {
      //cout << storage[i] << endl;
      cout << flex_Store.at(i) << endl;
   }

}  // end bagTester

int main()
{
   quickTester();
   system("pause");
   return 0;
}  // end main