/**
 * CSE 100 PA4 C++ Graph
 *
 * Author: Melvin Wijaya, Jesslyn Hernawan
 * Date: 6/5/2017
 * Assignment: PA4
 */
#include "Graph1.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main( int argc, char* argv[] )
{
  // argument is not exactly 4 
  if( argc != 5 )
  {
    cout << "Invalid argument"<< endl;
    return -1;
  }
  
  //opening file to sort based on year
  ifstream infile( argv[1] );
  
  if( !infile.is_open() )
  {
    cout<< "input file cannot be opened" << endl;
    return -1;
  }
  //opening file for the test pair
  ifstream infilePair(argv[2] );
  
  if( !infilePair.is_open())
  {
    cout<< "test pair of file cannot be opened" << endl;
    return -1;
  }

  // opening file for the output
  ofstream outfile(argv[3]);
  
  if(!outfile.is_open())
  {
    cout<<"output file cannot be opened"<<endl;
    return -1;
  }
  //check for the emptiness in the input file and file for test simultaneously
  if( infile.peek() == ifstream::traits_type::eof())
  {
    cout<<"file input is empty";
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

  // create priority queue based on year form the input file
  Graph1 myGraph;

  myGraph.createQueue(infile); // sort the movies list ascending based on year

  outfile<< "Actor1\tActor2\tYear\n";

  bool have_header = false;

  //start reading the file that contains the pair of actors
  while( infilePair )
  {
    string s;

    if(!getline(infilePair,s))
    {
      break;
    }   
    if(!have_header)
    {
      have_header = true;
      continue;
    }

    istringstream ss(s);
  
    vector<string> nameOfActors;
    while(ss)
    {
      string next;

      if(!getline(ss, next, '\t'))
      {
        break;
      }

      nameOfActors.push_back(next);
    }
    if( nameOfActors.size() != 2 )
    {
      continue;
    }


    if( string(argv[4]) == "bfs" )
    {
      myGraph.findActorConnection( nameOfActors, outfile );
    }
    else if( string(argv[4]) == "ufind" )
    {  
      myGraph.findActorConnectionThruDT( nameOfActors, outfile );
    }
    else
    {
      cout<< "invalid comment"<<endl;
      return -1;
    }

  }

  infile.close();
  infilePair.close();
  outfile.close();

  
}
