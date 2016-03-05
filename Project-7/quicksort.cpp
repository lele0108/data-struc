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
      friend ostream& operator<<(ostream& os, const Product& pd) {
         os << pd.name << " " << pd.price;
         return os;
      }
      bool operator== (const Product &one) {
         return (price == one.price && name == one.name); 
      }
      bool operator< (const Product &one) {
         return (price < one.price); 
      }
      bool operator<= (const Product &one) {
         return (price < one.price || price == one.price); 
      }
      bool operator> (const Product &one) {
         return (price > one.price); 
      }
};

template<typename T>
void quickSort(T theArray[], int first, int last)
{
   if (last - first + 1 < MIN_SIZE)
   {
      insertionSort(theArray, first, last);
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


void quickTester() 
{

   string line;
   ifstream myfile ("UProducts.csv");
   if (myfile.is_open()) {
      while (getline(myfile, line)) {
         string temp[2];
         istringstream ss(line);
         getline(ss, temp[0], ',');
         getline(ss, temp[1], ',');
         Product hold (temp[0], atof(temp[1].c_str()));
      }
   }
   myfile.close();

}  // end bagTester

int main()
{
   quickTester();
   return 0;
}  // end main