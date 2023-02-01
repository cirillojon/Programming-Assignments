/* COP 3503C Assignment 3
This program is written by: Jonathan Cirillo */

import java.util.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.File;
import java.io.IOException;
import java.lang.*;
 
public class Main {
 
  //Driver Program
	public static void main(String[] args) throws IOException {
 
  File myFile = new File("in.txt");
  Scanner stdin = new Scanner(myFile);
 
		
    //Scan in from file
		int numComputers = stdin.nextInt()+1;
    int numConnections = stdin.nextInt();
    int numDestroy = stdin.nextInt();
    int[] destroy = new int[numDestroy];
    int[] item1Arr = new int[numConnections];
    int[] item2Arr = new int[numConnections];
 
		Main mySet = new Main(numComputers); 

    //Performs the union operation for given connections and also stores 
    //the connections in 2 seperate 1D arrays
		for(int i = 0; i < numConnections; i++)
    {
 
			int item1 = stdin.nextInt();
			int item2 = stdin.nextInt();

      item1Arr[i] = item1;
      item2Arr[i] = item2;

      mySet.union(item1 , item2);

		}

    //Function call to calculate Connectivity
    long connectivity = 0;
    connectivity = calculateConnectivity(numComputers, mySet);
    System.out.println(connectivity);

    //Scan in connections to destroy
    for(int i = 0; i < numDestroy; i++)
      {
        destroy[i] = stdin.nextInt();
      }

        int[] setArr;

        //Method to recalculate Connectivity after connections are destroyed
        for(int j = 0; j < destroy.length; j++)
          {

           //These are the computers whose connection will be destroyed
           int first = item1Arr[destroy[j]-1];
           int second = item2Arr[destroy[j]-1];

          
          int root = parents[first].getID();

          //Call to a function that calulcates how many leaf nodes belong to this root
          setArr = findAll(root, mySet,numComputers);

          //Call to a function that returns whether a leaf is directly touching the root
          boolean firstLink = connectedToRoot(item1Arr,item2Arr,root, first);
          boolean secondLink = connectedToRoot(item1Arr,item2Arr,root, second);

          //If Neither leaf is directly touching the root
          if(!firstLink || !secondLink)
             {
                //If the leafs of the connection to be destroyed have the same root and neither are root   
                if((parents[first].getID() == parents[second].getID()) && !(isRoot(first,mySet)) && (!(isRoot(second,mySet))) )
                  {
                     //Loops through the leafs belonging to the mutual root and changes the root of the leafs 
                     //that were only connected through the leaf whose connection is to be destroyed to that leaf itself
                     for(int x = 0; x < setArr.length; x++)
                      {
                          if(setArr[x] >= second)
                          {
                              parents[setArr[x]].setID(second);
                          }
                      }  
                  }
                //If one of leafs of the connection to be destroyed is the root
                if((parents[first].getID() == parents[second].getID()) && (isRoot(first,mySet)))
                {
                  //If there are more leaves than just the root and the given leaf 
                  if( getNonZeroCount(setArr) <= 2)
                  {
                      parents[second].setID(second);
                  }
                  //If it is just the root and the leaf
                  else 
                  {
                     parents[second].setNumDeletion(1);
                     parents[first].setNumDeletion(1);
                  }
                }
                 //Same thing but if the other leaf is the root
                 if((parents[first].getID() == parents[second].getID()) && (isRoot(second,mySet)))
                {
                  if( getNonZeroCount(setArr) <= 2)
                  {
                      parents[first].setID(first);
                  }
                  else 
                  {
                    parents[second].setNumDeletion(1);
                     parents[first].setNumDeletion(1);
                  }
                }
              }
            
            //If one of the leafs was touching root or was root  
            else
            {
               //If neither leaf has had a connection deleted yet 
               if((parents[first].numDeletion() == 0) && (parents[second].numDeletion() == 0) )
               {
                 //If neither leaf is root but both are touching root
                 if(!isRoot(second,mySet) && !isRoot(first,mySet))
                 {
                    parents[first].setNumDeletion(1);
                    parents[second].setNumDeletion(1);
                 }
               //If the second leaf was root         
                 if(isRoot(second,mySet))
                 {
                    parents[first].setNumDeletion(1);
                    parents[second].setNumDeletion(1);
                   
                 }
                //If the first leaf was root
                if(isRoot(first,mySet))
                 {
                    parents[second].setNumDeletion(1);
                    parents[first].setNumDeletion(1);
                 }
               }
                //If one or both of the leaves has a connection that has already been deleted
               else
               {
                 //If the first leaf has already had a connection deleted
                 if(parents[first].numDeletion() == 1)
                 {
                      parents[first].setID(first);
                 }
                  //If the second leaf has had a connection deleted
                 else
                 {
                    parents[second].setID(second);   
                 }
               }
           }    
      //Calculates and outputs updated connectivity after each connection is deleted
      connectivity = calculateConnectivity(numComputers, mySet);
      System.out.println(connectivity);   
    }      
	}

  /* HELPER METHODS */

  //Method that calulates and returns the connectivity of a given set
  public static long calculateConnectivity(int numComputers, Main mySet)
  {
    
    int count[] = new int[numComputers];
    int newArr[];
 
    for( int i = 1; i < numComputers; i++)
    {
      count[i] = mySet.find(i);
    }
 
    newArr = removeDuplicates(count);
    long connectivity = 0;
 
      for(int i = 0; i < newArr.length; i++)
      {
        if(newArr[i] > 0)
        {
          connectivity = (getCount(count,newArr[i]) * getCount(count,newArr[i])) + connectivity;
        }
      }
     return connectivity;
  }
 

    //Method that returns the total number of non-zero values in an arrray
    public static int getNonZeroCount(int[] nums)
    {
      int count = 0;
      for (int i = 0; i < nums.length; i++)
        {
           if(nums[i] > 0)
           {
             count++;
           }
        }    
    return count;   
  }

 //Method that returns the total number a given int occurs in an array
  public static int getCount(int[] nums, int n)
  {
    int count = 0;
    for ( int i = 1; i < nums.length; i++)
      {
         if(nums[i] == n)
         {
           count++;
         }
      }
    return count;
  }

  //Method that returns whether a leaf is touching the root or not
  public static boolean connectedToRoot( int[] item1, int[] item2, int n, int x)
  {

    if( x == n)
      return true;
    
    for(int i = 0; i < item1.length; i++)
      {
        if(item1[i] == n)
        {
          if(item2[i] == x)
          return true;
        }

        if(item1[i] == x)
        {
          if(item2[i] == n)
          return true;
        }
      }
    return false; 
  }

  //Method that finds all the leafs who are children of a given root
public static int[] findAll(int n, Main mySet, int numComputers)
  {
    int[] nums = new int[numComputers]; 
    for(int i = 0; i < numComputers; i++)
      {
        if(mySet.find(i) == n)
        {
          nums[i] = i;
        }
      }
    return nums; 
  }
    //Method to remove duplicates from an array
    public static int[] removeDuplicates(int[] n)
    {
      LinkedHashSet<Integer> noDup = new LinkedHashSet<Integer>();
 
        for (int i = 1; i < n.length; i++)
        {
              noDup.add(n[i]);
        }
 
       int[] arr = new int[noDup.size()];
       Iterator<Integer> i=noDup.iterator();  
       int x = 0;
      
       while(i.hasNext())
       {
          arr[x] = i.next();
          x++;
       }
      return arr;
    }

  //Method that determines whether a leaf is root
  public static boolean isRoot(int n, Main mySet)
  {
    if( mySet.find(n) == n)
      return true;

    return false;
  }

  
  
	private static pair[] parents;
 
	// Create the initial state of a disjoint set of n elements, 0 to n-1.
	public Main(int n) {
 
		// All nodes start as leaf nodes.
		parents = new pair[n];
		for (int i=0; i<n; i++)
		parents[i] = new pair(i, 0); //0 is height 0. parent[i]'s parent is i now
	}
 
	// Returns the root node of the tree storing id.
	public int find(int id) {
// I am the root of the tree)
		if (id == parents[id].getID()) 
      return id;
		// Find my parent's root.
		 int res = find(parents[id].getID());
 
		// if res is not my existing parent, make it parent
    if (res != parents[id].getID())
    {
      // Attach me directly to the root of my tree.
      parents[id].setID(res);
 
      parents[res].decHeight(); //decrease height as id is leveled up
    }
 
		return res;
	}

  //Method that performs the union operation
	public boolean union(int id1, int id2) {
 
		// Find the parents of both nodes.
		int root1 = find(id1);
		int root2 = find(id2);
 
		// No union needed.
		if (root1 == root2)
			return false;
 
		// Attach tree 2 to tree 1
		if (parents[root1].getHeight() > parents[root2].getHeight()) {
			parents[root2].setID(root1);
		}
 
		// Attach tree 1 to tree 2
		else if (parents[root2].getHeight() > parents[root1].getHeight() ) {
			parents[root1].setID(root2);
		}
 
		// Same height case - just attach tree 2 to tree 1, adjust height.
		else {
			parents[root2].setID(root1);
			parents[root1].incHeight();
		}
 
		// We successfully did a union.
		return true;
	}

}


class pair {

  private int numDeletion = 0;
	private int ID; 
	private int height;
 
	public pair(int myNum, int myHeight) {
		ID = myNum;
		height = myHeight;
	}
 
	public int getHeight() {
		return height;
	}
 
	public int getID() {
		return ID;
	}

  	public int numDeletion() {
		return numDeletion;
	}
 
	public void incHeight() {
		height++;
	}
 
  public void decHeight() {
		height--;
	}
 
	public void setID(int newID) {
		ID = newID;
	}

  	public void setNumDeletion(int newNumDeletion) {
		numDeletion = newNumDeletion;
	}
}

