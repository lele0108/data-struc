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
using namespace std;

#define BLANK " "

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
int partition(T theArray[], int first, int last)
{
   T x = theArray[last];
   int i = first - 1;
   for (int j = first; j < last - 1; j++) {
      if (theArray[j] <= x) {
         i++;
      }
      T hold = theArray[i];
      theArray[i] = theArray[j];
      theArray[j] = hold;
   }
   T hold = theArray[i+1];
   theArray[i+1] = theArray[last];
   theArray[last] = hold;
   return i+1;
}  // end quickSort

template<typename T>
void quickSort(T theArray[], int first, int last)
{
   if (last - first + 1 < 1000)
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
void insertionSort(T theArray[], int n)
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
      T nextItem = theArray[unsorted];
      int loc = unsorted;
      while ((loc > 0) && (theArray[loc - 1] > nextItem))
      {
         // Shift theArray[loc - 1] to the right
         theArray[loc] = theArray[loc - 1];
         loc--;
      }  // end while
      
      // At this point, theArray[loc] is where nextItem belongs
      theArray[loc] = nextItem; // Insert nextItem into sorted region
   }  // end for
}  // end insertionSort


void quickTester() 
{
   Product storage[1000];
   string line;
   ifstream myfile ("UProducts.csv");
   int count = 0;
   if (myfile.is_open()) {
      while (getline(myfile, line)) {
         string temp[2];
         istringstream ss(line);
         getline(ss, temp[0], ',');
         getline(ss, temp[1], ',');
         Product hold (temp[0], atof(temp[1].c_str()));
         storage[count] = hold;
         count++;
      }
   }
   myfile.close();
   quickSort(storage, 0, count-1);
   for (int i = 0; i < count; i++) {
      cout << storage[i] << endl;
   }

}  // end bagTester

int main()
{
   quickTester();
   return 0;
}  // end main