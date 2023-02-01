#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "leak_detector_c.h"

struct failfish_struct {
    int index;
    struct failfish_struct *next;
    struct failfish_struct *prev;
};  typedef struct failfish_struct failfish;

struct failfish_list_struct {
    failfish *head;
    failfish *tail;
};  typedef struct failfish_list_struct failfish_list;

struct pond_struct {
    int pond_number;
    char *name;
    int num_of_fish;
    int eating_counter;
    int min_threshold;
    failfish_list *list;
};  typedef struct pond_struct pond;

/* Creates a new Failfish and assigns it a sequence # */

failfish *new_failfish( FILE *ifp , int sequence_number)
{
    failfish *f;
    f = malloc(sizeof(failfish));
    f->index  = sequence_number+1; 
    f->next = NULL;
    f->prev = NULL;

    return f;
}

/* Destructor for failfish */

void dispose_failfish(failfish *f)
{
    free(f);
}

/* Allocates memory for a failfish list */

failfish_list *new_failfish_list(void)
{
    failfish_list *fl;

    fl = malloc(sizeof(failfish_list));
    fl->head = NULL;
    fl->tail = NULL;

    return fl;
}

/* Disposes of a failfish list */

void dispose_failfish_list(failfish_list *fl)
{
    free(fl);
}

// THESE ARE GERBER'S FUNCTIONS FOR CREATING/MANAGING A CIRCULAR LINKED LIST 
/*-------------------------------------------------------------------------*/

/* Modified for failfish */

void dispose_failfish_list_all(failfish_list *fl)
{
    failfish *f = fl->head;
    failfish *n;
    if(f == NULL) 
    {
      dispose_failfish_list(fl);
      return;
    }
    do {
        n = f->next;
        dispose_failfish(f);
        f = n;
    } while(f != fl->head); 

    dispose_failfish_list(fl);
}

void failfish_list_add(failfish_list *fl, failfish *new_failfish)
{
    if(fl->head == NULL)
    {
        fl->head = new_failfish;
        fl->tail = new_failfish;
        new_failfish->prev = new_failfish;
        new_failfish->next = new_failfish;
    } else {
        new_failfish->prev = fl->tail;
        new_failfish->next = fl->head;
        fl->head->prev = new_failfish;
        fl->tail->next = new_failfish;
        fl->head = new_failfish;
    }
}

void clear_links_or_dispose(failfish *f_to_delete, int dispose)
{
  if(dispose != 0) {
    dispose_failfish(f_to_delete);
  } else {
    f_to_delete->next = NULL;
    f_to_delete->prev = NULL;
  }
}

void failfish_list_delete(failfish_list *fl, failfish *f_to_delete, int dispose)
{
    failfish *f = fl->head;
    failfish *n;

    if(f_to_delete->next == f_to_delete) 
    {
      clear_links_or_dispose(f_to_delete, dispose);
      fl->head = NULL;
      fl->tail = NULL;
      return;
    }
    if(fl->head == f_to_delete)
    {
      fl->head = f_to_delete->next;
    }
    if(fl->tail == f_to_delete) 
    {
      fl->tail = f_to_delete->prev;
    }

    f_to_delete->prev->next = f_to_delete->next;
    f_to_delete->next->prev = f_to_delete->prev;
    clear_links_or_dispose(f_to_delete, dispose);
}

/*--------------------------------------------------------
These are the Functions I created for Assignment 2
---------------------------------------------------------*/

//Gets next number from the input
//Used to assign the number of ponds

int get_next_number(FILE *ifp)
{
    char s[128];
    int num;
    fscanf(ifp, "%d", &num);
    return num;
}

//Creates a new pond and assigns its member's values 
//by scanning from the input file

pond *new_pond( FILE *ifp )
{
    //Initializing variables
    int pond_number;
    char name[128];
    int num_of_fish;
    int eating_counter;
    int min_threshold;

    char pondString[128];
    char numString[128];
    char eatString[128];
    char threshString[128];

    pond *p;

    //Scanning in each individual string in the next line
    fscanf(ifp,"%s %s %s %s %s", pondString,name,numString,eatString,threshString);

    //Converting strings to integers
    pond_number = atoi(pondString);
    num_of_fish = atoi(numString);
    eating_counter = atoi(eatString);
    min_threshold = atoi(threshString);

    //Allocates memory for a pond
    p = malloc(sizeof(pond));

    //Creates a new failfish for the specific pond
    p->list = new_failfish_list();

    //Assigns the values that were scanned in to that pond
    p->pond_number = pond_number;
    p->name = strdup(name);
    p->num_of_fish = num_of_fish;
    p->eating_counter = eating_counter;
    p->min_threshold = min_threshold;

    return p;
}

 //Creates a linked list for each pond, and fills each list 
 //with the correct number of fish

void create_list(FILE *ifp, pond *p, int num_ponds, pond **pondArray, failfish *f)
{
    int x;
    int i;
    
    for (i = 0; i < num_ponds; i++ )
        {
           p = new_pond(ifp);    
           pondArray[i] = p;     //Creates and assigns a filled pond to each element of pondArray

       for (x = 0; x < pondArray[i]->num_of_fish; x++ )
            {
                 f = new_failfish(ifp,x);
                 failfish_list_add(pondArray[i]->list, f);  //Adds a new failfish to the failfish list 
                                                            //belonging to the current pond
                                                            //and assigns it's sequence number
            }
         }
}

/* Disposes of a all the elements of a pond */

void dispose_ponds(pond **pondArray, int num_ponds)
{
    int i;

    for(i = 0; i < num_ponds; i++)
    {
    dispose_failfish_list_all(pondArray[i]->list);
    free(pondArray[i]);
    }

    free(pondArray);
}

//Outputs the current state of each faiflish list
//to the output file

void print_list(FILE *ofp, pond **pondArray, int num_ponds)
{
    int i;
    int x;

    for (i = 0; i < num_ponds; i++ )
         {
          //Outputs each pond number and name
          fprintf(ofp,"%d %s ",pondArray[i]->pond_number, pondArray[i]->name);
          for (x = 0; x < pondArray[i]->num_of_fish; x++ )
            {
              //Outputs the sequence number of every fish in the list of a given pond
              fprintf(ofp,"%d ", pondArray[i]->list->tail->index);
              pondArray[i]->list->tail =   pondArray[i]->list->tail->prev;
            }
              fprintf(ofp,"\n");
         }
}

/* Sorts the ponds into sequential order */
/* utilizing a selection sort algorithm */

void sort(pond **pondArray, int num_ponds)
{
   
      int i;
      int x;
      int min; 

      pond *buffer;

  
 for (i = 0; i < num_ponds-1; i++) //After each iteration, we leave out the 
    {                              //first element once it's been handled
        min = i; 

        for (x = i+1; x < num_ponds; x++) 
        {
          //If the current element is less than the one before, then it will be swapped
          if (pondArray[x]->pond_number < pondArray[min]->pond_number) 
          {
            min = x; 
          }
        }
    //The swapping process
    buffer = pondArray[min]; 
    pondArray[min] = pondArray[i]; 
    pondArray[i] = buffer; 

   
    } 
}

//This function moves forward in the failcircle the 
//amount described by e - 1 which is stored in eat_count

void skip(pond *p, int eat_count)
{
    int x;

    for (x = 0; x <  eat_count; x ++)
    {
         //Moves to the next node 
         p->list->head = p->list->head->prev;  
    }
}

/* Function for the first round of eating */

//Skips forward e - 1 and eats. This process repeats until 
//the threshold has been met

void first_course(FILE *ofp, pond **pondArray,int num_ponds)
{

//Initializing variables
int count;
int threshold;
int eat_count;
int i;

for ( i = 0; i < num_ponds; i ++)
{
    //These variables get reset through each iteration - 
    //I use them to make the code more readable
    count  = pondArray[i]->num_of_fish;
    threshold = pondArray[i]->min_threshold;
    eat_count = pondArray[i]->eating_counter - 1;

    //Moves the head of each list to the first node
    pondArray[i]->list->head = pondArray[i]->list->head->prev; 

    //Outputs the number and name of each pond
    fprintf(ofp, " Pond %d: %s \n", pondArray[i]->pond_number, pondArray[i]->name);

//While the amount of fish in each list is greater than
//the set threshold, they will keep eating
while(count > threshold)
{
    //Skips to the next fish to be eaten
    skip (pondArray[i],eat_count);

    //Outputs which fish will be eaten
    fprintf(ofp, " Failfish %d eaten \n", pondArray[i]->list->head->index);

    //Adjusts the amount of fish
    count = count - 1; 
    pondArray[i]->num_of_fish = count;

    //Deletes the fish from its resepctive linked list
    failfish_list_delete(pondArray[i]->list, pondArray[i]->list->head, pondArray[i]->list->head->index);

    //Moves the list forward one because the 
    //deleting causes it to move backwards one
    pondArray[i]->list->head = pondArray[i]->list->head->prev; 

 }
fprintf(ofp,"\n");

}

}

//Removes a pond from a pond array
int removeFromArray(pond **pondArray, pond *p, int num_ponds) 
{ 
   int i; 
   int x;

   //Looks for the pond in the array
   for (i=0; i<num_ponds; i++) 
   {
      if (pondArray[i] == p) 
      {
         break; 
      }
   }

   //If the pond was found in the array
   if (i < num_ponds) 
   { 
     //Subtracts from the size of num_ponds
     num_ponds--; 

    //Moves each pond in the array forward one
     for (x = i; x <num_ponds; x++) 
     {
        pondArray[x] = pondArray[x+1]; 
     }
   } 
   //Returns the new number of ponds in the array
   return num_ponds; 
} 

/* Function for the second round of eating */

//Compares the index of each head and determines the max
//Eats the first head whose index is equal to max
//Because the ponds are in sequential order, this prevents the need
//to check for ties, beacuse it will eat the correct fish regardless

int course_two(FILE *ofp, pond **pondArray, int num_ponds)
{
    int i;
    int max;
    
    //Sets max to the index of the first head of the first pond
    max= pondArray[0]->list->head->index;

 for( i = 0; i < num_ponds; i++)
    {
        //If a head's index is grater than the current max, it gets set as max
        if(pondArray[i]->list->head->index > max) 
         {
        max= pondArray[i]->list->head->index;
         }
    }

for( i = 0; i < num_ponds;i++)
    {
        if(pondArray[i]->list->head->index == max) 
         {
          //If the pond's head's index is equal to max and there is only
          //one fish left in it's list, it removes that pond from the array, disposes
          //the memory allocated for that pond, and outputs which fish gets eaten
          if(pondArray[i]->num_of_fish == 1 )
            {
              fprintf(ofp,"Eaten: Failfish %d from pond % d\n", 
              pondArray[i]->list->head->index,pondArray[i]->pond_number );
              dispose_failfish_list_all(pondArray[i]->list);
              free(pondArray[i]);
              num_ponds = removeFromArray(pondArray, pondArray[i], num_ponds);
              break;
            }
        //Otherwise, it eats the corresponding head, moves the 
        //list to the next node, adjusts num_of_fish in that pond, 
        //and outputs which fish got eaten
        else
           {
             fprintf(ofp,"Eaten: Failfish %d from pond % d\n", 
             pondArray[i]->list->head->index,pondArray[i]->pond_number);
             failfish_list_delete(pondArray[i]->list, pondArray[i]->list->head, pondArray[i]->list->head->index);
             pondArray[i]->list->head = pondArray[i]->list->head->prev; 
             pondArray[i]->num_of_fish = pondArray[i]->num_of_fish - 1;
            break;
           }
        }
    }
//Returns the updated number of ponds in the array
return num_ponds;
}

//The function "go_to_start" is used to prepare for course 2 by 
//traveling to the beginning of each list

//We know that each list is in sequential order by sequence number, 
//so we can just travel to the head with the smallest #

void go_to_start(pond **pondArray, int num_ponds)
{

int i;
int x;

for (i = 0 ; i < num_ponds; i++)
    {
        for (x = 0 ; x < pondArray[i]->num_of_fish; x ++)  //Iterates through each fish in each pond
        {
            //If the index of a fish is smaller, it travels to that fish
            if (pondArray[i]->list->head->next->index < pondArray[i]->list->head->index)
            {
                pondArray[i]->list->head = pondArray[i]->list->head->next;
            }
        }
    }
}

int main(void) {

    atexit(report_mem_leak); 

    //Initializing variables
    FILE *ifp;
    FILE *ofp;

    int num_ponds;
    failfish *f; 
    pond *p;
    pond **pondArray;

    ifp = fopen("cop3502-as2-input.txt", "r");
    ofp = fopen("cop3502-as2-output-cirillo-jonathan.txt", "w");

    //Scans # of ponds from input and assigns it to num_ponds
    num_ponds = get_next_number(ifp);
    fprintf(ofp, " Initial Pond Status\n");

    //Creates an array to hold ponds
    pondArray = calloc(num_ponds, sizeof(pond*));

    //Creates a linked list for each pond, and fills each list 
    //with the correct number of fish
    create_list(ifp, p,num_ponds, pondArray,f);

    //Sorts the ponds into sequential order
    sort(pondArray,num_ponds);

    //Ouputs the initial status of each pond's list
    print_list(ofp, pondArray, num_ponds);
    fprintf(ofp,"\n");

    /* FIRST COURSE */
    fprintf(ofp, "First Course \n");
    fprintf(ofp,"\n");
    first_course(ofp,pondArray,num_ponds);

    /* Status of the failfish lists at the end of the first course */ 
    fprintf(ofp, " End of Course Pond Status\n");
    print_list(ofp, pondArray, num_ponds);
    fprintf(ofp,"\n");

     /* SECOND COURSE */
    fprintf(ofp, "Second Course \n");
    fprintf(ofp,"\n");
    go_to_start(pondArray,num_ponds);
    while (num_ponds > 1 || pondArray[0]->num_of_fish > 1)  //We are removing a pond from pondArray when it has no fish left, so
    {                                                       //in the end there should always only be one pond left 
    num_ponds = course_two(ofp,pondArray,num_ponds);        //containing the remaining failfish
    }
    fprintf(ofp,"\n");

    //Outputs the fish who survived :)
    fprintf(ofp,"Failfish %d from pond %d remains\n", 
    pondArray[0]->list->head->index,pondArray[0]->pond_number);

    //Free the memory allocated for 
    dispose_ponds(pondArray, num_ponds);

    return 0;
}