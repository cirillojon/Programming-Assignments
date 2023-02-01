#include<stdlib.h>
#include<stdio.h>
//#include "leak_detector_c.h"

//Outputs to file the current state of the array
void print(int * array, int count, FILE *ofp)
{
  int i;
  for (i = 0; i < count; i++) 
  {
    fprintf(ofp,"%d ",array[i]); 
  } 
  fprintf(ofp,"\n"); 
} 

//Swaps the values of the given integers 
void siftdown(FILE *ofp, int *x, int *y)
{
   int buffer = *y;
   *y = *x;
   *x = buffer;
}

//Recursively siftdowns an array until its heapified 
void heapify(FILE *ofp, int *array, int count, int x, int built) 
{  
    int leftChild = 2 * x + 1; 
    int rightChild = 2 * x + 2; 
    int max = x; 

    if (leftChild < count && array[leftChild] > array[max]) 
    {
       max = leftChild; 
    }
    
    if (rightChild < count && array[rightChild] > array[max]) 
    {
      max = rightChild; 
    }
    
    if (max != x) 
    { 
      siftdown(ofp, &array[x],&array[max]); 
 
      //Assures that it will only output the array after each 
      //siftdown until it has been heapified 

      if (built == 0)
      {
        print(array,count,ofp);
      }
        
        heapify(ofp, array, count, max,built); 
    } 
} 
  
//Creates heap from array
void build(FILE *ofp, int *array, int count, int built) 
{ 
    int x;
    int first = (count / 2) - 1; 
    
    for ( x = first; x >= 0; x--) 
    { 
      heapify(ofp, array, count, x, built); 
    } 
} 
  
//Deletes the root of the heap
void delete(FILE *ofp, int * array, int * total, int built) 
{ 
    int last = array[*total - 1]; 
    array[0] = last; 
    *total = *total -1;
    heapify(ofp, array, *total, 0, built); 
} 
  
int main()
{ 
   //atexit(report_mem_leak); 

   FILE *ifp;
   FILE *ofp;

   ifp = fopen("cop3502-as5-input.txt", "r");
   ofp = fopen("cop3502-as5-output-cirillo-jonathan.txt", "w");

   //Initializing variables
   char count_string[32];
   char num_string[32];
   int count;
   int num;
   int built = 0;
 
   //Scans in total number of integers
   fscanf(ifp, "%s", count_string);
   count = atoi(count_string);
   int control = count;

   //Allocates dynamic array to hold ints 
   int *main = calloc(count, sizeof(int));
   int *array = main;

   //Scans in ints and stores them in the array
   int i;
   for(i = 0; i < count ; i++)
   {
      fscanf(ifp, "%s", num_string);
      num = atoi(num_string);
      array[i] = num;
   }

   //Prints out filled array before being heapified
   print(array, count,ofp); 

   build(ofp, array, count, built); 
   built = 1;

   //Prints out filled array after being heapified
   print(array, count,ofp); 

   //Assigns variable that will be iterated as each int is deleted 
   int *total = &count;

   //Deletes each int from the heap until there is none left
   for(i = 0; i < control; i++)
   {
      fprintf(ofp,"%d\n", array[0]);
      delete(ofp, array, total, built);
      print(array,*total,ofp); 
   }
   
    //Frees memory allocated for the dynamic array holding the ints
    free(array);
   
    return 0; 
} 