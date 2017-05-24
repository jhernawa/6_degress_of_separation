#include "Graph.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int pathfinder( int argc, char * argv[] )
{
  if( argc != 5 )
  {
    cout << "invalid argument";
    return -1;
  }  
  // opening file for the input 
  ifstream infile( argv[1] );
  if( !infile.isOpen() )
  {
    cout << "input file cannot be opened ";
    return -1;
  }
  
  // opening file for the test pair
  ifstream infilePair( argv[4] );
  if(!infilePair.isOpen() )
  {
    cout << "test pair of file cannot be opened ";
    return -1;
  }
  
  // opening file for the output file
  ofstream outfile( argv[5] );
  if(!outfile.isOpen())
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

  // start creting the graph
  
  Graph theGraph; // HOW DO YOU CONSTRUCT THIS??
 //Graph theGraph(); hmm..

  vector<Node*> theVertices = theGraph.build();  //kalo ga d pake buildGraph() ga usah return pa2
  
  bool have_header = false;
  while( infilePair )
  {
    string s;
    
    if(!getline(infilePair, s )
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
     
      if(!getline(ss, next,'\t')
      {
        break;
      }
      
      nameOfActor.push_back(next);
    }

    // if(nameOfActors.size() != 2 ) // ga perlu since it makes no sense
    // {
    //   continue;
    // }
    theGraph.runBST( /*theVertices,*/ nameOfActors, outfile ); // per line  

  } 

  infile.close();
  infilePair.close();
  outfile.close();
  

}
