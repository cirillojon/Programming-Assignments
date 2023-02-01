#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"
#include "fa20_cop3502_as1.h"
#include <math.h>

//clear_region/monster and dispose_monster/region_array
//are functions used to free the space allocated for the members of the structs

void clear_region(region *r)
{
  
    if(r->name != NULL)
    {
        free(r->name);
        r->name = NULL;
    }

    free(r->monsters);
    
    r->nmonsters = 0;
    r->total_population = 0;
    
}

void clear_monster(monster *m)
{
    if(m->name != NULL)
    {
        free(m->name);
        m->name = NULL;
    }

    if(m->element!= NULL)
    {
        free(m->element);
        m->element= NULL;
    }

    m->population = 0;
}

void dispose_monster_array(monster *m, int nmonsters)
{
    int i;

    for( i = 0; i < nmonsters; i++)
    {
        clear_monster(m + i);
    }

    free(m);
}
void dispose_region_array(region *r, int nregions)
{
   int i;

    for( i = 0; i < nregions; i++)
    {
        clear_region(r + i);
  
    }
    
    free(r);

}

//These next 4 functions are used to allocate memory 
//for the arrays used to store each individual struct

itinerary *new_itinerary_array(int nitinerary)
{
   itinerary *i = calloc(nitinerary,sizeof(itinerary));

   return i;
}
monster *new_monster_array(int nmonsters)
{
    monster *m = calloc(nmonsters, sizeof(monster));

    return m;
}
region *new_region_array(int nregions)
{
    region *r = calloc(nregions, sizeof(region));

    return r;
}
trainer *new_trainer_array(int ntrainers)
{
    trainer *t = calloc(ntrainers, sizeof(trainer));

    return t;
}

//remove_crlf and get_next_nonblank_line are commands
//copied from Gerber's filio.c template and
//are used to help scan strings from the input file

//NOTE - in the filio.c file it explicitly gave permission to copy these functions

void remove_crlf(char *s)
{
    char *t = s + strlen(s);
    t--;
    while ((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0'; 
        t--;      
    }
}
void get_next_nonblank_line(FILE *ifp, char *s, int max_length)
{
    s[0] = '\0';

    while (s[0] == '\0')
    {
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}

//get_next_number is used to scan in the next number from the
//input file to assign the number of monsters/regions/trainers

int get_next_number(FILE *ifp)
{
    char s[128];
    int num;

    get_next_nonblank_line(ifp, s, 127);

    sscanf(s, "%d", &num);

    return num;
}

//read_monsters and read_regions scan and assign 
//the values for their desginated struct array

void read_monster(FILE *ifp, monster *m)
{
    //Defining variables
    char name[128];
    char element[128];
    char population_string[128];
    int population;

    //Scans in the values from the input file
    fscanf(ifp,"%s %s %s", name,element,population_string);
    population = atoi(population_string);

    //Stores the values in the monster array
    m->name = strdup(name);
    m->element= strdup(element);
    m->population = population;
    
}

//read_regions also returns a struct array that will be used in a later 
//function to assign the values corresponding to each trainer in the
//itinerary struct regions double pointer 

 static region *read_regions(FILE *ifp,region *r,monster *m,int apm,int nregions)
{
    //defining variables 
    char name[128];
    char s[128];
    int nmonsters; 
    int test = 30;
    char temp[128];
    int i; int j; int q;

for(q =0; q<nregions; q++)
{

  //Scanning from input file

  get_next_nonblank_line(ifp, name, 127);   
  get_next_nonblank_line(ifp, s, 127);
  sscanf(s, "%d", &nmonsters);

//Assigning values

  (r+q)->name = strdup(name);
  (r+q)->nmonsters = nmonsters;

  //allocates the memory for the double pointer
  (r+q)->monsters = malloc(nmonsters * sizeof(monster *));
  
//This loop assigns the values of each pointer in the pointer array

  for( i = 0; i < nmonsters ; i++)
  {
      get_next_nonblank_line(ifp, temp, 127);
      for ( j = 0; j < apm ; j++)
      {
          //apm is "all possible monsters"

          //Comapres the strings scanned from the input file to 
          //the name of each monster to assign which monsters 
          //are in each region 

        test=(strcmp((m+j)->name,temp));
        if (test ==0)
         {
           (r+q)->monsters[i]=(m+j);
           (r+q)->total_population = (r+q)->total_population  + (m+j)->population;
         }
        }
     }      
  }
    return r;
}

/*     DISCLAIMER     */

//I understand that the following function certainly 
//for the sake of proper coding, readability, and practicality 
//should have been split up into seperate functions
//but, for the sake of time and ease I crammed it into one

//do_it_all is a multi-purposed function that: assigns the trainer and itinerary values,
//calculates each capture, outputs in the correct format
//and frees the memory allocated for each trainer and itinerary 


void do_it_all(FILE *ifp, FILE * ofp, trainer *t, region *r,int ntrainers,int totalReg,itinerary *visits,int nmonsters)
{
    //Defining variables 
    char name[128];
    char s[128];
    int nregions;
    int captures;

    //These integers are used to iterate through my abundant for loops
    int z; int j; int q; int k; 
  
    //Allocates memory for the double pointer
    visits->regions = malloc(sizeof(region*));

    for ( z =0 ; z< ntrainers ; z++)
    {
        //Scans in values from the input file and assigns them to each applicable trainer/itinerary 

        get_next_nonblank_line(ifp, name, 127);
        (t+z)->name = name;
   
        get_next_nonblank_line(ifp, s, 127);
        sscanf(s, "%d", &captures);
        (visits+z)->captures = captures;
         
         get_next_nonblank_line(ifp, s, 127);
         sscanf(s, "%d", &nregions);
        (visits+z)->nregions = nregions;

        //Prints each trainer name to the output file

         fprintf(ofp, "%s  \n", (t)->name);

        for ( j = 0; j < nregions ;j++)
         {
           //Prints each region name to the output file
           get_next_nonblank_line(ifp, s, 127);
           fprintf(ofp,"%s  \n", s);

            for (q = 0; q<totalReg; q++)
            {
                //Compares the name of each region scanned to the original region
                //array (r) from read_regions to assign the values into the pointer array (regions)

                 if (strcmp(s,(r+q)->name)== 0)
                 {
                 visits->regions[z]=(r+q);
                 }
            }
                for(k = 0 ;k < visits->regions[z]->nmonsters; k++)
                  {
                    //Defining the variables used for calculation
                    double mon_pop = visits->regions[z]->monsters[k]->population;
                    double region_pop = visits->regions[z]->total_population;
                    double intended_captures = (visits+z)->captures;

                    //Calculates each capture 
                    int calculation = round(((mon_pop)/region_pop)*intended_captures);

                        if( calculation != 0)
                        {
                            //Outputs each monster and the number of times it was captured
                            fprintf(ofp," %d %s \n",calculation,visits->regions[z]->monsters[k]->name);
                        }
                  }
        }    fprintf(ofp,"\n");
            //Deallocates the memory stored for each itinerary/trainer/and the double pointer (regions**)
    }       free(visits->regions);  free(visits);  free(t);
}

/* Main function. */

int main(void)
{
    atexit(report_mem_leak); 

    //defining variables
    FILE *ifp;
    FILE *ofp;
    int nmonsters;
    int nregions; 
    int ntrainers;
    int i;
    monster *monsters;
    region *reg;
    trainer *trainers;
    itinerary *visits;


    //Assigns the input/output files 
    ifp = fopen("cop3502-as1-input.txt", "r");
    ofp = fopen("cop3502-as1-output-cirillo-jonathan.txt", "w");

    //Grabs the number of monsters and creates a new array of that size 
    nmonsters = get_next_number(ifp);
    monsters = new_monster_array(nmonsters);

    //Reads in the values for each monster 
    for (i = 0; i < nmonsters; i++)
    {
        read_monster(ifp, monsters + i);
    }

    //Grabs the number of regions and creates a new array of that size 
    nregions = get_next_number(ifp);
    reg = new_region_array(nregions);
   
    //Stores the values of each region returned by the read_regions function 
    //to be used by the do_it_all function
    reg =  read_regions(ifp,reg ,monsters,nmonsters,nregions);
    
    //Grabs the number of trainers and creates and array of that size
    ntrainers = get_next_number(ifp);
    trainers = new_trainer_array(ntrainers);

    //Creates a new array of visits. NOTE ~ ntrainers works here because 
    //itineraries and trainers have a 1-1 ratio
    visits = new_itinerary_array(ntrainers);

    //Calling this function here creates the output and frees visits and trainers
    do_it_all(ifp,ofp,trainers,reg,ntrainers,nregions,visits,nmonsters);
  
    //These functions free the memory stored in the monster/regions arrays
    dispose_monster_array(monsters, nmonsters);
    dispose_region_array(reg, nregions);

    fclose(ifp);
    fclose(ofp);

    return 0;
}