
/* COP 3503C Assignment 5 
This program is written by: Jonathan Cirillo */ 

import java.util.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.File;
import java.io.IOException;
import java.lang.*;
import java.util.Arrays;
 
public class Main {

   //Method utilizing the dynamic programming strategy of tabulation
   public static int tabulation(int[][] arr, int[][] selected, int numStudents, int count)
    {
      int top = 0;
      int bottom = 1;
  
      for(int index = 1; index < numStudents; index++)
        {

          int left = index - 1;

          if(selected[top][left] != 1)
          {
            if(selected[bottom][index] != 1)
              {
                selected[top][index] = 1;
                count = count + arr[top][index];
              }
            
          }
            
          else if( arr[top][index] > arr[top][left])
          {
              if(selected[bottom][index] != 1)
              {
                 selected[top][index] = 1;
                 selected[top][left] = 0;
                 count = count + arr[top][index];
                 count = count - arr[top][left];
              }
          }
          
           if(selected[bottom][left] != 1)
           {
              if(selected[top][index] != 1)
                {
                  selected[bottom][index] = 1;
                  count = count + arr[bottom][index];
                }

           }

            else if( arr[bottom][index] > arr[bottom][left])
            {
                if(selected[top][index] != 1)
                {
                   selected[bottom][index] = 1;
                   selected[bottom][left] = 0;
                   count = count + arr[bottom][index];
                   count = count - arr[bottom][left];
                }
    
          }
     }
    
  //Total number of problems
  return count;   
}
    

    //FOR DEBUGGING
    /*
               for(int i = 0; i < 2; i++)
                {
                    
                   for(int j = 0 ; j < numStudents; j++)
                      {
                          System.out.print(selected[i][j] + " ");
                      }
                            System.out.print("\n");

                }

    */

    //Driver Method
    public static void main(String[] args) throws IOException
    {
      
          File myFile = new File("in.txt");
          Scanner stdin = new Scanner(myFile);

          int count = 0;
          int numStudents = stdin.nextInt();

         //Scanning in info and creating rows
          int[][] arr = new int[2][numStudents];
          int[][] selected = new int[2][numStudents]; 
      
          for(int i = 0; i < 2; i++)
            {
                 for(int j = 0; j < numStudents; j++)
                  {
                    arr[i][j] = stdin.nextInt();
                    selected[i][j] = 0;
                  }          
            }


      //Initialzing array
      if(arr[0][0] >= arr[1][0])
      {
         count = count + arr[0][0];
         selected[0][0] = 1;
      }
        
      else if(arr[1][0] > arr[0][0])
      {
         count = count + arr[1][0];
         selected[1][0] = 1;
      }

      //Calls tabulation method and returns total # of problems
      System.out.println(tabulation(arr,selected,numStudents,count));

    }

  
}


