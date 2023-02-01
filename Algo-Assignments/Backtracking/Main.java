/* COP 3503C Assignment 2 
This program is written by: Jonathan Cirillo */ 

import java.util.Scanner;
import java.util.*;
import java.util.stream.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.File;

class Main {

  //Driver Code
  public static void main(String[] args)throws IOException {

    int numCases;
    int rows;
    int cols; 
    int numWords;
    boolean res;

  //Creating file variable and scanner
  File myFile = new File("in.txt");
  Scanner sc = new Scanner(myFile);

 
  numCases = sc.nextInt();

  for(int x = 0; x  < numCases; x++)
  {
          System.out.println("Test#" + (x+1));
          
          //Scanning in values
          rows = sc.nextInt();
          cols = sc.nextInt();
          numWords = sc.nextInt();
          
              char[][] arr = new char[rows+1][cols+1];
              String[] words = new String[numWords];
              

            //Storing values in array
            for(int i = 0; i < rows; i++)
            {
              for(int j = 0; j < cols; j++)
              {
                arr[i][j] = sc.next().charAt(0);
            
              }
            
            }
            String empty = sc.nextLine();

            //Storing words 
            for(int i = 0; i < numWords; i++)
            {
              String next = sc.nextLine();
              words[i] = next;

            }

            //Runs checkExist function for each 
            //Word in words array
            for(int i = 0; i < numWords; i++)
            {
            
              System.out.println("Looking for " + words[i]);
              res = checkExist(arr,words[i]);
                  if(res == false)
                  {
                    System.out.println(words[i] + " not found!");
                    System.out.print("\n");
                  }
            }
      
          
    }
  }

   //Method to check if word exists in matrix
   public static boolean checkExist(char[][] arr, String word) 
   {
       
        int r = arr.length;
        int c = arr[0].length;
        
        boolean[][] beenTo = new boolean[r][c];
        
         if (word.length() > r * c) 
         {
            return false;
         }
        
        //Runs search method for each value in array 
       
        for(int i=0; i < r; i++)
        {
            for(int j=0; j < c; j++)
            {
                if(search(arr, i, j, r, c, word, 0, beenTo) == true)
                {
                    return true;
                }
            }
        }
       return false; 
    }

    //Backtracking algorithm
    public static boolean search(char[][] arr, int row, int col, int r, int c, String word, int index, boolean[][] beenTo)
    {

    boolean val = false;

        if(row < 0 || col < 0 )
        {
          return false;
        }
       
        if(beenTo[row][col])
        {
            return false; 
        }
     

        if(word.charAt(index) == arr[row][col]){ 
            
            beenTo[row][col] = true; 
            
            if(index == word.length()-1)
            { 
              
              for(int i = 0; i < r-1; i++)
              {
                 System.out.print("[");
                for(int j = 0; j < c-1; j++)
                {
                 
                  if(beenTo[i][j] == true)
                  {
                     
                      if(j != (c-2))
                      System.out.print(arr[i][j]+ ", ");

                      else
                      System.out.print(arr[i][j]);
                  }
                  else if(j != (c-2))
                  {
                    System.out.print(" , ");
                  }
                  else
                  {
                    System.out.print(" ");
                  }
                }
                 System.out.print("]");
                System.out.print("\n");
              }
              
              System.out.print("\n");
              return true;
            }
         
            //left
            val = val || search(arr, row, col - 1, r, c, word, index+1, beenTo);
            //right
            val = val || search(arr, row, col + 1, r, c, word, index+1, beenTo);
            //up
            val = val || search(arr, row - 1, col, r, c, word, index+1, beenTo);
            //down
            val = val || search(arr, row + 1, col, r, c, word, index+1, beenTo);
            //diagonal down right 
            val = val || search(arr, row + 1 , col + 1, r, c, word, index+1, beenTo);
            //diagonal up right 
            val = val || search(arr, row- 1, col + 1, r, c, word, index+1, beenTo);
            //diagonal up left 
            val = val || search(arr, row - 1 , col - 1, r, c, word, index+1, beenTo);
            //diagonal up right 
            val = val || search(arr, row + 1, col - 1, r, c, word, index+1, beenTo);

            beenTo[row][col] = false; 

            return val;
        }
    return false;
    }

}