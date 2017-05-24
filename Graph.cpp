#include "Graph.h"

#include <limits>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

typedef pair< string, int > thePair;


Node:: Node( string nameOfActor, vector<thePair> movieList, int idx )
{ 
  nameActor = nameOfActor;
  index = idx;
  dist = 0;
  prev = 0;
  //adj = NULL;
  list = movieList;  
 
}

Graph:: Graph()
{
  //verticesL = NULL;
  vertices = 0;
  edgesDirected = 0;

}

Graph::~Graph()
{
  //mestinya bnr and cek valgrind
  for( int i = 0 ; i < verticesL.size() ; i++ )
  {
    delete( verticesL[i] ); //clear all the pointers to the nodes in the graph
  }  

}
vector< Node*> Graph::buildGraph( ifstream& infile )
{
  string prev = "";
  int index = 0;
  
  bool have_header = false;
  while(infile)
  {
    string s;
    if(!getline(infile,s))
    {
      break;
    }
    if(!have_header)
    {
      have_header = true;
      continue;
    }

    istringstream ss(s);
    vector<string> record;
    while(ss)
    { 
      string next;
      if(!getline(ss,next, '\t')
      {
        break;
      }
      record.push_back(next);
    }
      /////////////////////
     // if(record.size()!=3)
     // {
     //   continue;
     // }
      ////////////////////
         
     string actorName( argv[0] );
     string movie_title( record[1] );
     int movie_year =  stoi( record[2] );
   
     if( prev == "" ) // first entry
     {  
       prev = record[0];
     }
      
     if( prev != record[0] )
     {
       Node* newNode =  new Node( prev, list, index );
       verticesL.push_back(newNode);
       index++;
       prev = record[0];
       list = list.clear();
       vertices++;
     }
     thePair movieYear;
     movieYear.first = record[1];
     movieYear.second = record[2];
     list.push_back(movieYear);
    
 
  }
  Node * newNode = new Node( prev, list, index );     
  verticesL.push_back( newNode );
  vertices++;
  listDownAdjList();
  
  return verticesL;

}
Graph:: listDownAdjList()
{
  for( int i = 0; i< verticesL.size(); i++ )
  {
    for( int j = 0 ; j< verticesL[i]->list.size() ; j++ )
    {
      thePair currentMovieYear = vertices[i]->list[j];

      for( int k = 0 ; k < verticesL.size(); k++ )
      {
        if(i == k )
        {
          continue;
        }
       
        for( int l = 0; l < vertices[k]->list.size(); l++ )
        {
          if( currentMovieYear == vertices[k]->list[l] )
          {
            vertices[i]->adj.push_back( vertices[k]->index );
            edgesDirected = edgesDirected + 2;
          }
        }
      }
    }

  }

}

void Graph::runBST( /*vector<Node*> theVertices ,*/ vector<string> nameOfActors, ofstream& outfile )
{
  vector<int> indices;
  for( int i = 0; i < nameOfActors.size() ; i++ )
  {
    for( int j = 0; j< verticesL.size(); j++ )
    {
      if( nameOfActors[i] == vertices[j]->nameActor )
      {
        indices.push_back( vertices[j]->index );
        break;     //// mestinya udah bener
      }
    }
    if( indices.size() == 2 )
    {
      break;
    }
  } 

  runBSTHelper( /*theVertices,*/ nameOfActors, outfile );
  


}

void Graph::runBSTHelper( /* vector< Node* > theVertices, */ vector<int> indices , ofstream& outfile )
{
  for( int i = 0; i < verticesL.size(); i++ )
  { 
    verticesL[i]->dist = std::numeric_limits<int>::max() ;
    verticesL[i]->prev = -1;
  }
  
  queue<Node*> toExplore;
 //vertex * start = verticesL[ indices[0] ];
  toExplore.push( vertices[ indices[0] ] );
  vertices[ indices[0] ]->dist = 0;
  Node * curr;

  while(!toExplore.empty() )
  { 
    curr = toExplore.front();
    toExplore.pop();
    
    for(int j = 0; j < curr->adj.list(); j++ )
    {
      int adjIndex = curr->adj[j];
      if( verticesL[adjIndex] -> dist == std::numeric_limits<int>::max() )
      {
        verticesL[adjIndex]->dist = curr->dist + 1;
        vertices[adjIndex]->prev = curr->index;
        toExplore.push( verticesL[adjIndex] );
 
      } 

    }
  }

  //START TRACING and print to the output file
  vector<string> thePath;
  Node * curr = verticesL[ indices[1] ];
  while( curr != verticesL[ indices[0] ] )
  {
    thePath.push_back( curr->nameActor );
    int prevNode = curr->prev;
    
    boolean edges_found = false;
    for( int i = 0 ; i < curr->list.size(); i++ )
    {
      for( int j = 0; j < verticesL[prevNode]->list.size(); j++ )
      {
        if( curr->list[i] == vertices[prevNode]->list[j] )
        {
          string theYear = std::to_string(curr->list[i].second);  //convert dulu k string
          thePath.push_back( /*curr->list[i].second*/ theYear );
          thePath.push_back( curr->list[i].first );
          edges_found = true;
          break;  //mestinya udah bnr tpi ga tau gmn break tie
        }
      }
      if(edges_found == true )
      {
        break;
      }
    }
    curr = verticesL[prevNode];
    
  }
  thePath.push_back( verticesL[indices[0]]->actorName );

  //Printing out to the outfile
  boolean edgesTime = false;
  for( int i = thePath.size() -1 ; i >= 0; i-- )
  {
    if( edgesTime == false )
    {
      outfile << "(" << thePath[i] << ")";
      edgesTime = true;
    }
    else
    {
      outfile << "--[" << thePath[i] << "#@" << thePath[i-1] << "]-->";
      i--; 
      edgesTime = false;   
    }
  }
  outfile << "\n"; //new line for the next entry 

}
