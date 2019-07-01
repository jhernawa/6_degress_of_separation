/**
 * CSE 100 PA4 C++ Graph
 *
 * Author: Jesslyn Hernawan
 * Date: 6/5/2017
 * Assignment: PA4
 */
#include "Graph1.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main( int argc, char * argv[] )
{
  // argument is not exactly 4
  if( argc != 5 )
  {
    cout << "invalid argument";
    return -1;
  }

  
  // opening file for the input 
  ifstream infile( argv[1] );
  if( !infile.is_open() )
  {
    cout << "input file cannot be opened ";
    return -1;
  }
  
  // opening file for the test pair
  ifstream infilePair( argv[3] );
  if(!infilePair.is_open() )
  {
    cout << "test pair of file cannot be opened ";
    return -1;
  }
  
  // opening file for the output file
  ofstream outfile( argv[4] );
  if(!outfile.is_open())
  {
    cout<<"output file cannot be opened";
    return -1;
  }
  
  // check for emptiness in the input file and file for test pair simultaneousl
  if( infile.peek() == ifstream::traits_type::eof() )
  {
    cout<< "file input is empty";
    infile.close();
    infilePair.close();
    outfile.close();
    
    return -1;
  }
  if( infilePair.peek() == ifstream::traits_type::eof() )
  {
    cout<< "test pair file is empty";
    infile.close();
    infilePair.close();
    outfile.close();
    
    return -1;
  }

  // start creating the graph
  
  Graph1 theGraph; 

  theGraph.buildGraph(infile);
  
  outfile << "(actor)--[movie#@year]-->(actor)--..."<< endl;
  bool have_header = false;
  while( infilePair ) //start reading the file that contains the pair of actors
  {
    string s;
    
    if(!getline(infilePair, s ))
    {
      break;
    }
    if(!have_header)
    {
      have_header= true;
      continue;
    }
   
    istringstream ss(s);
    vector<string> nameOfActors;
    while(ss)
    {
      string next;
     
      if(!getline(ss, next,'\t'))
      {
        break;
      }
      
      nameOfActors.push_back(next);
    }
    
    if( nameOfActors.size() != 2)
    {
      continue;
    }

    
    
    if( string(argv[2]) == "u" )
    {
      
      theGraph.runBFS( nameOfActors, outfile ); 
    }
    else if( string(argv[2]) == "w" )
    {
      theGraph.runDijkstra( nameOfActors, outfile );
    }
    else
    {
     cout << "invalid input"<< endl;
     return -1;
    }

  } 

  int numOfActors = theGraph.getNumberOfActors();
  int numMovies = theGraph.getNumberOfMovies();
  cout<< "#of Actors is " << numOfActors << endl;
  cout<< "#of movies is " << numMovies << endl;
  
  
  infile.close();
  infilePair.close();
  outfile.close();
  
//  delete theGraph;
}
