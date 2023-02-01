#include<stdlib.h>
#include<stdio.h>
#include <string.h>
//#include "leak_detector_c.h"

struct item_node_struct
{
char name[32];
int count;
struct item_node_struct *left, *right;
};

typedef struct item_node_struct item_node;

struct tree_name_node_struct {
char treeName[32];
struct tree_name_node_struct * right, * left;
item_node *theTree;
};

typedef struct tree_name_node_struct tree_name_node;

/* ============================== 
  Functions for creating base tree
  ================================ */

//Creates new tree node
 tree_name_node* newNode(char data[32]) 
{ 
    tree_name_node *node =  malloc(sizeof(tree_name_node)); 
    strcpy(node->treeName,data);
    node->left = NULL; 
    node->right = NULL; 
    node->theTree = NULL;
  
    return(node); 
} 

//Creates our base tree from a sorted array
tree_name_node* createBaseTree(tree_name_node** nodes, int start, int end)  
{  
    if (start > end)  
    return NULL;  
  
    //Creates node from the middle element 
    int mid = (start + end)/2;   
    tree_name_node *temp =  malloc(sizeof(tree_name_node)); 
    strcpy(temp->treeName,nodes[mid]->treeName);
    temp->left = NULL; 
    temp->right = NULL; 
    temp->theTree = NULL;
  
    //Creates left subtree
    temp->left = createBaseTree(nodes, start, mid - 1);  
    //creates right subtree
    temp->right = createBaseTree(nodes, mid + 1, end);  

    return temp;  
}  

//Tells quick-sort what to comapre
 int compareFunction(const void* a, const void* b) 
{ 
    return strcmp(*(const char**)a, *(const char**)b); 
} 

//Sorts array alphabetically
void sort(tree_name_node** nodes, int n) 
{ 
    //Uses built in quick-sort
    qsort(nodes, n, sizeof(tree_name_node*), compareFunction); 
} 

 /* ========================================
  Functions for adding items to correct trees
 =========================================== */

//Creates new item
item_node * newItem (char iName[32], int count)
{
    item_node *temp = NULL;
    temp = malloc(sizeof(tree_name_node));
    temp->left = NULL;
    temp->right = NULL;
    strcpy(temp->name,iName);
    temp->count = count; 

    return temp;
}

//Creates a BST by inserting in alphabetical order
item_node *insertItem( item_node * the_tree, item_node * item)
{
  
    if(strcmp(item->name,the_tree->name)  < 0)
    {
 
        if(the_tree->left != NULL)
        {
            return insertItem(the_tree->left,item);
        }
        else
        {
            the_tree->left = item;
            return the_tree->left; 
        }
    }

    else
    {
        if ( the_tree->right != NULL)
        {
            return insertItem(the_tree->right,item);
        }
        else
        {
            the_tree->right = item;
            return the_tree->right;
        }
    }

}

//Recurs through trees until it finds the correct tree to insert at
void *findTreeToInsert(tree_name_node * tree, char val[32],item_node * item)
{
    if (tree == NULL)
    {
        return NULL;
    }
    
    findTreeToInsert(tree->left,val,item);
        
         if(strcmp(tree->treeName,val) == 0)
        {
            if(tree->theTree != NULL)
            {
                insertItem(tree->theTree,item);
            }
            else
            {
                tree->theTree = item;
            }
        }  
    
    findTreeToInsert(tree->right,val,item);    

    return NULL;
}

 /* ====================================================
  Functions to print trees and their items to the output
 ======================================================= */

//Used to output the trees that were scanned in 
void print_trees(FILE *ofp, tree_name_node * tree)
{
    if (tree)
    {
         
        print_trees(ofp,tree->left);
      
            fprintf(ofp," %s",tree->treeName);  

        print_trees(ofp,tree->right);
    }
}

//Outputs each item in a given tree
void itemOrder(FILE *ofp,item_node * root) 
{ 
    if(root == NULL)
    {
        return;
    }

    if (root->left != NULL)
    {
        itemOrder(ofp,root->left); 
    }

    if(root != NULL)
    {
        fprintf(ofp, "%s ",root->name);
    }

    if(root->right != NULL)
    {
        itemOrder(ofp,root->right);
    }
} 

//Outputs each tree and calls function to print each tree's items
void printItems(FILE *ofp, tree_name_node * root,item_node * item) 
{ 
    if (root != NULL) 
    { 
        printItems(ofp,root->left,item); 

                fprintf(ofp, "===%s===\n", root->treeName); 
              
        itemOrder(ofp,root->theTree);
         
            fprintf(ofp,"\n");

        printItems(ofp,root->right,item); 
    } 
} 


 /* =========================================================
  Functions to free memory allocated for trees and their items
 ============================================================ */

//Frees memory allocated for the trees
void deltrees(tree_name_node * tree)
{
    if (tree)
    {
        deltrees(tree->left);
        deltrees(tree->right);
        free(tree);
    }
}

//Frees memory allocated for each item
void dispose_item_array(item_node** items,int nItems)
{
    int i;

    for(i = 0 ; i < nItems; i++)
    {
        free(items[i]);
    }

free(items);
}

//Frees memory allocated for the tree nodes stored in the array
void dispose_tree_array(tree_name_node** nodes,int nTrees)
{
    int i;

    for(i = 0 ; i < nTrees; i++)
    {
        free(nodes[i]);
    }

free(nodes);
}

 /*==========================================
  Recursive functions for processing commands 
 ============================================*/

/* ItemBefore 1&2 calculate how many items come before the given item */

//Counts items until the given item is find and then outputs the count
void itemBefore2(FILE *ofp,item_node * root, char item[32],int *numBefore) 
{ 
    if (root->left != NULL)
    {
        itemBefore2(ofp,root->left,item,numBefore);    
    }

     if (strcmp(root->name,item)==0)
    {
        fprintf(ofp,"item before %s: %d\n",root->name,*numBefore);
        return;
    }

    else
    {
        (*numBefore)++;
    }
 
    if(root->right != NULL)
    {
        itemBefore2(ofp,root->right,item,numBefore);
    }
} 

//Finds which tree to find the item before the given item in 
void itemBefore1(FILE *ofp, tree_name_node * root,char item[32], char treeString[32]) 
{ 
    int numBefore = 0; 
   
    if (root != NULL) 
    { 
        itemBefore1(ofp,root->left,item,treeString); 
        
          if((strcmp(root->treeName,treeString ) == 0) )
          {
                itemBefore2(ofp,root->theTree,item,&numBefore);
          }

        itemBefore1(ofp,root->right,item,treeString); 
    } 
} 

/* CountTree 1&2 sum up the counts of each item in the given tree */

//Adds up the counts of each item in a given tree
void countTree2(FILE *ofp,item_node * root, int * count) 
{ 
    if (root->left != NULL)
    {
        countTree2(ofp,root->left,count);    
    }

        (*count) = (*count) + root->count;
 
    if ( root->right != NULL)
    {
       countTree2(ofp,root->right,count);
    }
} 

/* Finds which tree to count the items and then outputs the total count */
void countTree1(FILE *ofp, tree_name_node * root, char treeString[32]) 
{ 
    int count = 0;
    
    if (root != NULL) 
    { 
        countTree1(ofp,root->left,treeString); 
        
          if((strcmp(root->treeName,treeString ) == 0) )
          {
            countTree2(ofp,root->theTree,&count);
            fprintf(ofp,"%s count %d\n",root->treeName,count);
          }

        countTree1(ofp,root->right,treeString); 
    } 
} 

/* SearchFor, searchTree, and searchItem are used to search through a given tree for a given item */

//Looks for a given item in a given tree, outputs how many of that item were found where
item_node *searchItem(FILE *ofp, char lookFor[32], item_node  *root, char treeString[32], int *itemFlag, int *treeFlag)
{  
  if( root != NULL )
  {
      if((strcmp(lookFor, root->name) == 0) )
      {
         (*itemFlag)++;
          fprintf(ofp,"%d %s found in %s\n", root->count, lookFor,treeString);
          return root;
      }
      else if(strcmp(lookFor, root->name) < 0)
      {
          return searchItem(ofp,lookFor, root->left,treeString, itemFlag,treeFlag );
      }
      else if(strcmp(lookFor, root->name) > 0)
      {

          return searchItem(ofp, lookFor, root->right,treeString, itemFlag, treeFlag);
      }  
  }
   return NULL;
}

//Locates which tree to search for the item in 
void searchTree(FILE *ofp, tree_name_node * root, char lookFor[32],int *itemFlag, char treeString[32],int *treeFlag) 
{ 
    if (root != NULL) 
    { 
        searchTree(ofp,root->left,lookFor, itemFlag,treeString,treeFlag); 
        
            if((strcmp(root->treeName,treeString ) == 0) )
            {
                (*treeFlag)++;
                searchItem(ofp,lookFor,root->theTree,root->treeName,itemFlag,treeFlag);
            }
    
        searchTree(ofp,root->right, lookFor, itemFlag,treeString,treeFlag); 
    } 
} 

//Outputs if the item doesn't exist in that tree or if that tree doesn't exist
void searchFor( FILE *ofp, tree_name_node* root, char treeString[32], char lookFor[32])
{
    int exists = 0;
    int notFound = 0;

    searchTree(ofp,root,lookFor,&notFound,treeString,&exists);

    if(exists == 0)
    {
        fprintf(ofp,"%s does not exist\n",treeString);
    }

    else if (notFound == 0)
    {
        fprintf(ofp,"%s not found in %s\n",lookFor,treeString);
    }
}

/* DeleteTree, deleteNode, deleteItem 1&2 are used to delete a given tree or item */

//Deletes a given tree
void deleteTree(FILE *ofp, tree_name_node * root, char treeString[32]) 
{ 
    if (root != NULL) 
    { 
        deleteTree(ofp,root->left,treeString); 

            if((strcmp(root->treeName,treeString ) == 0) )
            {
                fprintf(ofp, "%s deleted\n",root->treeName);
                memset(&root->treeName[0], 0, sizeof(root->treeName));
                root->theTree = NULL;
                //free(root);
            }
      
        deleteTree(ofp,root->right,treeString); 
    }  
} 

//Deletes a given item
 void deleteNode(FILE *ofp,item_node* root, char treeString[32])
{
    fprintf(ofp,"%s deleted from %s\n", root->name,treeString);

    if(root == NULL)
    {
      return;
    }

    root->count = 0;
    memset(&root->name[0], 0, sizeof(root->name));
    root = NULL;
    return;
}

//Finds item in tree to delete
item_node *deleteItem2(FILE *ofp, char itemString[32], item_node  *root, char treeString[32])
{
  if( root != NULL )
  {
      if((strcmp(itemString, root->name) == 0) )
      {
        deleteNode(ofp, root,treeString);
      }

      else if(strcmp(itemString, root->name) < 0)
      {
        return deleteItem2(ofp,itemString, root->left,treeString );
      }

      else if(strcmp(itemString, root->name) > 0)
      {
        return deleteItem2(ofp, itemString, root->right,treeString);
      }
  }

   return NULL;
}

//Finds which tree to delete item in
void deleteItem1(FILE *ofp, tree_name_node * root, char lookingFor[32], char treeString[32]) 
{ 
    if (root != NULL) 
    { 
        deleteItem1(ofp,root->left,lookingFor, treeString); 
        
            if((strcmp(root->treeName,treeString) == 0))
            {
                deleteItem2(ofp,lookingFor,root->theTree,root->treeName);
            }
      
        deleteItem1(ofp,root->right,lookingFor,treeString); 
    } 
} 

/* ReturnMaxValue, findHeight, printHeights, isBalanced, and checkBalance
are used to calcualte the heights of the left and right subtrees
and to determine and output whether the base trees are balanced */

//Returns the larger of two numbers
int returnMaxValue(int x, int y) 
{ 
    if(y > x)
    {
        return y;
    }

    else
    {
        return x;
    }
} 

//Determines the hieght of left and right tree
int findHeight(item_node* node) 
{ 
    if (node == NULL) 
    {
        return -1; 
    }
        
    return 1 + returnMaxValue(findHeight(node->left), findHeight(node->right)); 
} 

//Outputs the height of left and right tree
void printHeights(FILE *ofp, item_node* root) 
{ 
    int leftHeight,rightHeight; 
  
    if (root == NULL) 
        return;
  
    leftHeight = findHeight(root->left); 
    rightHeight = findHeight(root->right); 
    
    fprintf(ofp,"left height %d, right height %d, difference %d, ",leftHeight,rightHeight,abs(rightHeight-leftHeight));
}

//Determines whether a tree is balanced
int isBalanced(item_node* root) 
{ 
   int leftHeight,rightHeight; 

    if(root == NULL) 
    {
        return 1; 
    }
        
    leftHeight = findHeight(root->left); 
    rightHeight = findHeight(root->right); 

    if (abs(leftHeight - rightHeight) <= 1 && isBalanced(root->left) && isBalanced(root->right)) 
    {
        return 1; 
    }

        return 0; 
} 
  
//Outputs whether or not the given tree is balanced  
void checkBalance(FILE *ofp, tree_name_node * root, char treeString[32]) 
{ 
    if (root != NULL) 
    { 
      checkBalance(ofp,root->left,treeString); 

        if((strcmp(root->treeName,treeString ) == 0) )
        {
            if (isBalanced(root->theTree) == 1)
            {
                fprintf(ofp,"%s: ",root->treeName);
                printHeights(ofp,root->theTree);
                fprintf(ofp,"balanced\n");
            }

           else
            {
                fprintf(ofp,"%s: ",root->treeName);
                printHeights(ofp,root->theTree);   
                fprintf(ofp,"not balanced\n");
            }
        }
      checkBalance(ofp,root->right,treeString); 
    } 
} 

/* Main function. */

int main(void)
{
    //atexit(report_mem_leak); 

    //Intializing variables 

    tree_name_node *root;
 
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("cop3502-as4-input.txt", "r");
    ofp = fopen("cop3502-as4-output-cirillo-jonathan.txt", "w");

    root = NULL;

    int i;

    int nTrees;
    int nItems;
    int nCommands;

    char tree_string[32];
    char item_string[32];
    char command_string[32];

    fscanf(ifp, "%s %s %s", tree_string, item_string,command_string);

    nTrees = atoi(tree_string);
    nItems = atoi(item_string);
    nCommands = atoi(command_string);

    //Scans in and creates array used to assign base tree values 
    tree_name_node** nodes = malloc(sizeof(tree_name_node*)*nTrees);
    char arr[32];

    //Creates a tree node for each element in array 
    for (i = 0; i < nTrees; i++) 
    { 
      fscanf(ifp,"%s", arr); 
      nodes[i]= newNode(arr); 
    }

    //Sorts array holding base trees 
    sort(nodes,nTrees);

    //Creates tree from sorted array
    root = createBaseTree(nodes, 0, nTrees-1);

    //Prints out the trees before adding any items
    print_trees(ofp, root); 
    fprintf(ofp,"\n");

    //Scans in item infomartion and creates an array to hold items
    item_node** items = malloc(sizeof(item_node*)*nItems);
    for ( i = 0 ; i < nItems; i++)
    {
        char treeString[32];
        char iName[32];
        int count = 0; 

        fscanf(ifp,"%s %s %d",treeString,iName,&count);

        items[i] = newItem(iName,count);

        //Inserts the item into the tree it belongs to 
        findTreeToInsert(root,treeString, items[i]);
 }
    //Prints out trees after adding items
    printItems(ofp,root,root->theTree);

    //Begin Processing Commands
    fprintf(ofp, "=====Processing Commands=====\n");

    //Initializing varibles 
    char command[32];
    char treeString[32];
    char itemString[32];
  
    //Scans in commands and calls the necessary functions to process them 
    for (i = 0; i < nCommands; i ++)
    {
        fscanf(ifp, "%s", command);
        
        if (strcmp(command,"search") == 0)
        {  
            fscanf(ifp, " %s %s",  treeString,itemString);
            searchFor(ofp,root,treeString,itemString);
        }

        if (strcmp(command,"item_before") == 0)
        {
            fscanf(ifp, " %s %s",  treeString,itemString);
            itemBefore1(ofp,root,itemString,treeString);
        }

        if (strcmp(command,"height_balance") == 0)
        {
            fscanf(ifp, " %s",  treeString);
            checkBalance(ofp,root, treeString);
        }

        if (strcmp(command,"count") == 0)
        {
            fscanf(ifp, " %s",  treeString);
            countTree1(ofp,root, treeString);
        }
          
        if (strcmp(command,"delete") == 0)
        {
            fscanf(ifp, " %s %s",  treeString,itemString);
            deleteItem1(ofp,root,itemString,treeString);
        }
              
        if (strcmp(command,"delete_tree") == 0)
        {
            fscanf(ifp, " %s",  treeString);
            deleteTree(ofp,root,treeString); 
        }
    }

    //Frees memory allocated for trees and items 
    dispose_item_array(items,nItems);
    dispose_tree_array(nodes,nTrees);
    deltrees(root);
}