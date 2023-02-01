
/* COP 3503C Assignment 5 
This program is written by: Jonathan Cirillo */ 


import java.util.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.File;
import java.io.IOException;
import java.lang.*;
 

 

// A class to store a graph edge
class Edge
{
int source, dest, weight;
public Edge(int source, int dest, int weight)
{
this.source = source;
this.dest = dest;
this.weight = weight;
}
  
}



// A class to store a heap node
class Node
{
int vertex, weight;
public Node(int vertex, int weight)
{
this.vertex = vertex;
this.weight = weight;
}
}

// A class to represent a graph object
class Graph
{
// A list of lists to represent an adjacency list
List<List<Edge>> adjList = null;
// Constructor
Graph(List<Edge> edges, int n)
{
adjList = new ArrayList<>();
for (int i = 0; i < n; i++) {
adjList.add(new ArrayList<>());
}
// add edges to the directed graph
for (Edge edge: edges) {
adjList.get(edge.source).add(edge);
}
}
  
}



class Main {

private static int treasureCount = 0;
private static int inCity = 0;
private static int onRoad = 0;



private static int[] largeArray = new int[50];
  
private static int largeArrayCount = 0;


private static void getRoute(int[] prev, int i, List<Integer> route)
{
if (i >= 0)
{
getRoute(prev, prev[i], route);
route.add(i);
}
}
  public static void main(String[] args) throws IOException
{


    File myFile = new File("in.txt");
  Scanner stdin = new Scanner(myFile);
 


    int numCity = stdin.nextInt();
    int numRoads = stdin.nextInt();
    int capital = stdin.nextInt();

 //   System.out.println("numCity: " + numCity + " numRoads: " + numRoads + " capital: " + capital);


    int[][]arr = new int[numRoads][3];
    for(int i = 0; i < numRoads; i++)
      {
        for(int j = 0; j < 3; j++)
          {
            arr[i][j]  =  stdin.nextInt();

    //        System.out.print(arr[i][j] + " ");
          }

   //       System.out.print("\n");
        
      }
    int dFromCapToT = stdin.nextInt();

    int n = numRoads;
    List<Edge> edges = new ArrayList<Edge>();
  
      for(int i = 0; i < n; i++ )
        {
             
        }


  int s;
  int d;
  int w;
  
      for(int i = 0; i < n; i++)
      {
        int j = 0;

        s = arr[i][j];
        d = arr[i][j+1];
        w = arr[i][j+2];

        Edge temp = new Edge(s, d, w);
          edges.add(temp); 

      //    System.out.print("\n");
        
      }

  
// construct graph
Graph graph = new Graph(edges, n);
// run the Dijkstra’s algorithm from every node just to get shortest paths from source to all the vertices 
//In general, if you have a specific source, call this function by findShortestPaths(graph, source, n);

int iterator = 0;
for (int source = 0; source < n; source++) {
findShortestPaths(graph, source, n,dFromCapToT,arr,iterator);
  iterator++;
}

  System.out.println("In city: " + inCity);

  System.out.println("On the road: " + onRoad);


}

  public static void findShortestPaths(Graph graph, int source, int n, int L, int[][] arr, int iterator)
{

 
// create a min-heap and push source node having distance 0
PriorityQueue<Node> minHeap;
minHeap = new PriorityQueue<>(Comparator.comparingInt(node -> node.weight));
minHeap.add(new Node(source, 0));
// set initial distance from the source to `v` as infinity
List<Integer> dist;
dist = new ArrayList<>(Collections.nCopies(n, Integer.MAX_VALUE));
// distance from the source to itself is zero
dist.set(source, 0);
// boolean array to track vertices for which minimum
// cost is already found
boolean[] done = new boolean[n];
done[source] = true;
// stores predecessor of a vertex (to a print path)
int[] prev = new int[n];
prev[source] = -1;
//Continuing to the right side

// run till min-heap is empty
while (!minHeap.isEmpty())
{
// Remove and return the best vertex
Node node = minHeap.poll();
// get the vertex number
int u = node.vertex;
// do for each neighbor `v` of `u`
for (Edge edge: graph.adjList.get(u))
{
int v = edge.dest;
int weight = edge.weight;
// Relaxation step
  if (!done[v] && (dist.get(u) + weight) < dist.get(v))
  {
  dist.set(v, dist.get(u) + weight);
  prev[v] = u;
  minHeap.add(new Node(v, dist.get(v)));
  }
}
// mark vertex `u` as done so it will not get picked up again
done[u] = true;
}
//int treasureCount = 0;


List<Integer> route = new ArrayList<>();
int j = 2;
for (int i = 0; i < n; i++)
{

  int num = arr[i][j];

  
if (i != source && dist.get(i) != Integer.MAX_VALUE)
{
getRoute(prev, i, route);

  if(dist.get(i) == L )
  {
   
    treasureCount++;
    inCity++;

  }

    if(dist.get(i) > L )
   {
      largeArray[largeArrayCount] = dist.get(i);
      largeArrayCount++;
     
  
   if(dist.get(i) >= (L*2))
   {
   
     treasureCount += 2; 

     onRoad+= 2;

   }
    else
   {

     treasureCount++;
     onRoad++;
   }


  }

  
route.clear();
}
       
        else if( num > L && (iterator == (n-1)) )
        {
         
          
      
          if(!ifLargeNumUsed(num,largeArray))
          {
      
             onRoad++;
      
             largeArray[largeArrayCount] = num;
            largeArrayCount++;
          
          }
         
        }

  
    }



  }


    public static boolean ifLargeNumUsed(int num, int[] arr)
  {
    for(int i = 0; i < arr.length; i++)
      {
        if(arr[i] == num)
        {
          return true;
        }
        
      }

    return false; 
    
  }

}
