/*
2DArray
(c) 2016
original authors: David I. Schwartz
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	This tutorial deals with how to create a 2d array with the 
	user-input (or "not-known-until-run-time") rows and cols
*	This tutorial shows how to allocate and deallocate dynamic memory.
*	This tutorial uses CRT Library to find Memory Leaks.


*	see http://www.cplusplus.com/doc/tutorial/pointers/ for reminders on pointers, arrays, pointer arithmetic and delete
*	some comments listed below are cited from https://msdn.microsoft.com

*/



#pragma region Setup	
#include <iostream>
#pragma region Leak Detection			// Leak Detection:  map the malloc and free functions which keep track of memory allocation and deallocation. 

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#ifdef _DEBUG							
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif	// To see file and line numbers in the memory-leak report when using operator new
#endif		// _DEBUG	
			//This block is to make people to see the file and line numbers in the memory-leak report when using new operator
			//This mapping occurs only in a debug build (in which _DEBUG is defined). Release builds use the ordinary malloc and free functions.
#pragma endregion


using namespace std;
typedef unsigned int uint;	// Declare a typedef name, uint.
							// typedef: introduces a name that, within its scope, becomes a synonym for the type given by the type-declaration portion of the declaration.

// saving us some typing for below:
uint rows;
uint cols;
#pragma endregion

#pragma region Array Utilities
void init(int val, int** a) {
	for (uint r=0; r < rows; r++) 
		for (int c=0; c < cols; c++) 
			a[r][c] = val;
}

// give value "val" to every member of the array

void print(int** a) {
	for (int r=0; r < rows; r++) {
		for (uint c=0; c < cols; c++) 
			cout << a[r][c];
		cout << endl;
	}
}

// print the array
#pragma endregion


int main() {
	
	cout << "enter rows: ";
	cin >> rows;
	cout << "enter cols: ";
	cin >> cols;

    // won't compile ... why?
    // int a[rows][cols];
    // int a[][] = new int[rows][cols];
    // int** a = new int[rows][cols];
    
    // the long way to set up the array:
    cout << "\nApproach A:" << endl;
    int** a = new int*[rows];       // a points to an array of pointers to ints		 allocate an array of "rows" adjacent pointers
                                    // a -> [a(0)][a(1)]...[a(rows-1)] 
                                    //         |     |          |
                                    //         V     V          V
                                    //               ?          ?
                                    //         |
                                    //         V
	a[0] = new int[cols*rows];      //       a(0) -> [ rows*cols ints ]				 allocate an array of "cols*rows" adjacent integers

	for (uint r = 1; r < rows; r++)  // skip ahead for a(1)...a(rows-1) based on # of cols:
		a[r] = a[0] + r * cols;     //  *(a+r) = *a + r*cols (pointer arithmetic)

									//  for example, cols=2, rows=3. In this case,
									// a ->[a(0)][a(1)][a(2)]
									//		 |     |     |
									//       V     V     V
									// a(0)->x0,x1,x2,x3,x4,x5 (adjacent integers that a(0) points)
									// the first two elements goes to a(0), the next two goes to a(1), the rest goes to a(2)

    init(0,a);
	print(a);
	delete[] a[0]; //  dellocate storage for array
    delete a;

    // here's the shorter, clearer (and more "normal" approach):
    cout << "\nApproach B:" << endl;
	int** b = new int*[rows]; 
    for(uint i = 0; i < rows; i++)
        b[i] = new int[cols];		//  allocate an array of "cols" adjacent integers
									// still the example, cols=2, rows=3. In this case,
									// b ->[b(0)][b(1)][b(2)]
									//		 |     |     |
									//       V     V     V
									//	x0,x1,	x2,x3,	x4,x5


	init(0,b);
	print(b);
	for(uint i = 0; i < rows; i++)
		delete[] b[i]; // what happens if you don't delete each b[i]?
		delete[] b;		//deallocate dynamic memory 
    // delete b; // does this statement cause a problem? why or why not?

   _CrtDumpMemoryLeaks();	// Dumps all the memory blocks in the debug heap when a memory leak has occurred (debug version only).
   _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
   //system("pause");	
   return 0;
   
}
