#include "Graph1.h"

#include <limits>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <functional>
#include <queue>
using namespace std;

DisjointSet::DisjointSet() //how to define this?
{

}
string DisjointSet:: find( string theLostNode )
{
  if( graphTable.find( theLostNode ) == graphTable.end() )
  {
    return "";
  }

  vector<string> thePathList; //to keep track with the path for path compression

  if( graphTable[theLostNode] == "") // theLostNode is sentinel node of the set
  {
    return theLostNode;
  }

  string curr = theLostNode;
  thePathList.push_back(curr);
  while( graphTable[curr] != "" )
  {
    curr = graphTable[curr];
    thePathList.push_back(curr);
  }
  
  //path compression
  for( int i = 0 ; i< thePathList.size()-1; i++ )
  {
    graphTable[ thePathList[i] ] = thePathList[ thePathList.size() - 1 ]; 
  }

  return thePathList[thePathList.size() -1];
}

void DisjointSet:: toUnion( string firstNode, string secondNode )
{
  string sentinel1 = find(firstNode);
  string sentinel2 = find(secondNode);

  if( sentinel1 == "" && sentinel2 == "")
  {
    graphTable.insert( {firstNode,""} );
    graphTable.insert( {secondNode, firstNode} );
    sentinelSize.insert( {firstNode, 2 } );
  }
  else if( sentinel1 == sentinel2 )
  {
    return;
  }
  else
  {
    if( sentinel1 == "" || sentinel2 == "" )
    {
      if( sentinel1 == "" )
      {
        graphTable.insert( {firstNode, sentinel2} );
        sentinelSize[sentinel2]++;
      }
      else if( sentinel2 == "")
      {
        graphTable.insert( {secondNode, sentinel1 } );
        sentinelSize[ sentinel1 ]++;
      }
    }
    else //compare sentinel size
    {
      if( sentinelSize[sentinel1] >= sentinelSize[sentinel2] )
      {
        graphTable[sentinel2] = sentinel1;
        sentinelSize[sentinel1] = sentinelSize[sentinel1] + sentinelSize[sentinel2];
        sentinelSize.erase(sentinel2); // removing the smaller sentinel from the sentinelSize list
      }
      else
      {
        graphTable[sentinel1] = sentinel2;
        sentinelSize[sentinel2] = sentinelSize[sentinel2] + sentinelSize[sentinel1];
        sentinelSize.erase(sentinel1);
      }
    }
  }
}
void Graph1:: findActorConnectionThruDT( vector<string> nameOfActors , ofstream& outfile )
{
   DisjointSet mySet;
  
  std::priority_queue< nodePQ, vector<nodePQ>, nodePQComp > pq1 = pq;

  int theYearConnection;
  nodePQ currNodePQ = pq1.top();
  pq1.pop();
  theYearConnection = currNodePQ.year;

  bool connected = false;
 
  while( connected == false )
  {
    int sizeOfActorList = currNodePQ.actorList.size();
    if( sizeOfActorList == 1 )
    {
      if( mySet.graphTable.find( currNodePQ.actorList[0] ) == mySet.graphTable.end() ) //INI DI GANTI biar lbih cpet
      {
        mySet.graphTable.insert( { currNodePQ.actorList[0], ""} );
        mySet.sentinelSize.insert( {currNodePQ.actorList[0] , 1 } );
      } 
      //else
     // {
             // ignore
     // }
    }

    for( int i = 0; i < currNodePQ.actorList.size() - 1; i++ )
    {
      mySet.toUnion( currNodePQ.actorList[i], currNodePQ.actorList[i+1] );
    }

    //check for connection
    string sentinel1 = mySet.find( nameOfActors[0] );
    string sentinel2 = mySet.find( nameOfActors[1] );

    if( sentinel1 == sentinel2 )
    {
      if(!pq1.empty() && (sentinel1 == "" && sentinel2 == "" ) )
      {
        connected = false;
        currNodePQ = pq1.top();
        pq1.pop();
        theYearConnection = currNodePQ.year;
      }
      else if( pq1.empty() && ( sentinel1 == "" && sentinel2 == "" ) )
      {
        connected = true;
        outfile<<nameOfActors[0]<<"\t"<<nameOfActors[1]<<"\t"<<"9999\n";
      }
      else  // sentinel1 = "A" & sentinel2 = "A"
      {
        connected = true;
        outfile<<nameOfActors[0]<<"\t"<<nameOfActors[1]<<"\t"<<theYearConnection<<"\n";
      }
    }
    else // sentinel1 != sentinel2
    {
      if(pq1.empty())
      {
        connected = true;
        outfile<<nameOfActors[0]<<"\t"<<nameOfActors[1]<<"\t"<<"9999\n";
      }
      else
      {
        connected = false;
        currNodePQ = pq1.top();
        pq1.pop();
        theYearConnection = currNodePQ.year;
      }
    }
  }// end of while loop

}

nodePQ::nodePQ( string movieTitle, int yearMade, vector<string> aList )
{
  movieName = movieTitle;
  year = yearMade;
  actorList = aList; 

}
bool nodePQ:: operator<(const nodePQ& other)
{
  if( year != other.year )
  {
    return year > other.year;
  }

  return movieName < other.movieName;

}

void Graph1::createQueue( ifstream & infile)
{
  unordered_map<string, NodeMovieYear*> notSortedTable;

  bool have_header = false;

  while( infile )
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
      if(!getline(ss,next,'\t'))
      {
        break;
      }
     
      record.push_back(next);
    }
  
    if( record.size() != 3 )
    {
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    string movie_year(record[2]);

    string theMovieYear = record[1] + record[2];

    if( notSortedTable.find(theMovieYear) == notSortedTable.end())
    {
      notSortedTable.insert( {theMovieYear, new NodeMovieYear( record[1], record[2]) } );
      notSortedTable[theMovieYear]->listActors.push_back(record[0] );

    }
    else
    {
      notSortedTable[theMovieYear]->listActors.push_back(record[0] );
    }

  }//end of while infile


    //creating the priority queue
    for( auto element : notSortedTable )
    {
      string movieTitle = element.second->headerMovie;
      int yearMade= stoi( element.second->headerYear);
      nodePQ theNodePQ( movieTitle, yearMade, element.second->listActors);
      pq.push(theNodePQ);
    }

  ///////////
 /**  int size = pq.size();
    for( int i = 0; i < size ; i++ )
    {
      nodePQ oi = pq.top();
      pq.pop();

      cout << oi.movieName << " and " << oi.year << endl;
      for(int j = 0; j < oi.actorList.size(); j++ )
      {
        cout<<oi.actorList[j]<<endl;

      }

    }
  */
  ///////////////
}
void Graph1::findActorConnection( vector<string> nameOfActors, ofstream& outfile )
{
  std::priority_queue<nodePQ, vector<nodePQ>, nodePQComp> pq1 = pq;  ////



  int theYearConnection; //earliest year in which those actors are connected

  nodePQ currNodePQ = pq1.top();
  pq1.pop();

  theYearConnection = currNodePQ.year;

  bool connected = false;
  while( connected == false)
  {
    for( int i = 0; i< currNodePQ.actorList.size();i++ )
    { 
      //vertices
      if(verticesL.find(currNodePQ.actorList[i]) == verticesL.end() )
      {
        verticesL.insert( {currNodePQ.actorList[i], new NodeActor(currNodePQ.actorList[i])} );
        string yearr= to_string(currNodePQ.year);
        string movieAndYear = currNodePQ.movieName + yearr;
        verticesL[currNodePQ.actorList[i] ]->movieYear.push_back(movieAndYear);        
      }
      else
      {
        string yearr= to_string(currNodePQ.year);
        string movieAndYear = currNodePQ.movieName + yearr;
        verticesL[currNodePQ.actorList[i] ]->movieYear.push_back(movieAndYear);        
      }

      //moviesL
      string yearr = to_string(currNodePQ.year);
      string movieAndYear = currNodePQ.movieName + yearr;
      if(moviesL.find(movieAndYear) == moviesL.end() )
      {
        moviesL.insert( { movieAndYear, new NodeMovieYear( currNodePQ.movieName, yearr) } );
        moviesL[movieAndYear]->listActors.push_back(currNodePQ.actorList[i]);
      }
      else
      {
       moviesL[movieAndYear]->listActors.push_back(currNodePQ.actorList[i]);
      }
    
    }
   // check for graph
 /**   int count = 0;
    for( auto element : verticesL )
    {
   
      cout<< count << element.second->headerActor<<endl;
      for( int i = 0; i < element.second->movieYear.size(); i++ )
      {
        cout << element.second->movieYear[i] <<endl; 
      }
      count++;
     
    }*/
  /**  cout<<"\nCheck for Edges"<<endl;
   for( auto element : moviesL )
   {
    cout << element.second->headerMovie << " and " << element.second->headerYear << endl;

    for( int i = 0; i < element.second->listActors.size(); i++)
    {
      cout<< element.second->listActors[i]<<endl;

    }
   }*/
////////////////////////////////////

    connected = runBFSofActorConnection(nameOfActors);
    if( connected == false)
    {
      if( pq1.empty())
      {
    //    cout<< "yng ke 2 jatuh d sini"<<endl;
        connected= true;
        outfile<<nameOfActors[0]<<"\t"<<nameOfActors[1]<<"\t"<<"9999\n";
      }
      else
      {
        //theYearConnection = currNodePQ.year;
      // cout<<"yg pertama jatuh d sini"<<endl;
        currNodePQ= pq1.top();
        pq1.pop();
        theYearConnection = currNodePQ.year;
      }
    }
    else
    {
     //  cout<<"yg k dua jatuh d sini"<<endl;
       connected = true;
       outfile<<nameOfActors[0]<<"\t"<<nameOfActors[1]<<"\t"<<theYearConnection<<"\n";
    }
   
  }//end of while connected

  verticesL.clear();
  moviesL.clear();

}

bool Graph1::runBFSofActorConnection( vector<string> nameOfActors )
{
  if(verticesL.find(nameOfActors[0]) == verticesL.end())
  {
 //  cout<<"------------"<<endl;
    return false;
  }
  if(verticesL.find(nameOfActors[1]) == verticesL.end())
  {
  // cout<<"**"<<endl;
    return false;
  }

  //run regular bfs
  
  for( auto element : verticesL )
  {
    element.second->dist = std::numeric_limits<int>::max();
    element.second->prev = "";
    element.second->prevMovie = "";
  }
  
  queue<NodeActor*> toExplore;

  toExplore.push( verticesL[ nameOfActors[0] ] );
  verticesL[ nameOfActors[0] ] -> dist = 0;
  NodeActor* curr;

  while( !toExplore.empty() )
  {
    curr= toExplore.front();
    toExplore.pop();
 
    vector< NodeActor* > theCurrNeighboors;
    vector< NodeMovieYear* > thePrevMovies;
    
    NodeMovieYear* curr1;

    for( int i = 0; i < curr->movieYear.size(); i++ )
    {
      curr1 = moviesL[curr->movieYear[i] ];
   
      for( int j = 0; j < curr1->listActors.size();j++)
      {
        if(curr1->listActors[j] != curr->headerActor )
        {
          theCurrNeighboors.push_back( verticesL[curr1->listActors[j]] );
          thePrevMovies.push_back( curr1 );
        }
      }     
    }
    
    for( int i = 0; i < theCurrNeighboors.size(); i++ )
    {
      NodeActor* temp = theCurrNeighboors[i];
    
      if( temp->dist == std::numeric_limits<int>::max() )
      {
        temp->dist = curr->dist + 1;
        temp->prev = curr->headerActor;
        temp->prevMovie = thePrevMovies[i]->headerMovie + thePrevMovies[i]->headerYear;
        toExplore.push(temp);
      }
    }
 
    theCurrNeighboors.clear();
    thePrevMovies.clear();

  }
   

/**
  //check for status after run bfs
  cout << "CHECKING STATUS AFTER BFS"<<endl;

  for( auto element : verticesL )
  { 
    cout<<element.second->headerActor<<endl;
    cout<<element.second->dist<<endl;
    cout<<element.second->prev<<endl;
    cout<<element.second->prevMovie<<endl;

  }

*/





 

  //end of regular bfs

  //check for connection
  NodeActor * curr1 = verticesL[nameOfActors[1]];
 
  if( curr1->prev == "" )
  {
   //cout<<"nameOfActors is" << nameOfActors [1];
   //cout<<"KOK BISA KESINI"<<endl;
    return false;
  }

  while( curr1->prev != "" )
  {
    curr1 = verticesL[curr1->prev];

    if( curr1->headerActor == nameOfActors[0] )
    {
  //   cout<< "sini trueeeeeeeeeeeeee"<<endl;
      return true;
    }

  }
//   cout<<"malah false"<<endl;
  return false;
}


WeightedEdge:: WeightedEdge( string neighbors, int w )
{ 
  nM = neighbors;
  weight = w;
}
bool WeightedEdge:: operator<( WeightedEdge const & other )
{
  if( weight != other.weight )
  {
    return weight > other.weight;
  }
 
  return nM < other.nM;
} 

class WeightedEdgeComp
{
  public:
    bool operator()(WeightedEdge* &lhs, WeightedEdge* & rhs )const
    {
      return *lhs<*rhs;
    }
};
NodeActor:: NodeActor( string hActor )
{
  headerActor = hActor;
  prev= "";
  dist =0 ;
  prevMovie = "";
//  vector<string> movieYear;
  done = false;

}
NodeMovieYear:: NodeMovieYear( string hMovie, string hYear )
{
  headerMovie = hMovie;
  headerYear = hYear;
}

Graph1:: Graph1()
{
  built = true;

}
Graph1:: ~Graph1()
{
  for( auto element : verticesL )
  {
    delete(element.second);
  }
  for( auto element1 : moviesL )
  {
    delete(element1.second);
  }
}
void Graph1:: buildGraph( ifstream& infile )
{
  string prev = "";

  vector<string> movieListOfEachActor;

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
      have_header= true;
      continue;
    }
    
    istringstream ss(s);
    vector<string> record;
    while(ss)
    {
      string next;
      if(!getline(ss,next,'\t'))
      {
        break;
      }
      record.push_back(next);
    }
   
    if( record.size() != 3 )
    {
     continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    string movie_year(record[2]);

    string theMovieYear = record[1] + record[2];
    if(prev == "")
    {
      prev = record[0];
    }
    if( prev!= record[0] )
    {
     verticesL.insert( {prev, new NodeActor(prev) } );
     verticesL[prev]->movieYear = movieListOfEachActor;
     movieListOfEachActor.clear();
     prev = record[0];
     movieListOfEachActor.push_back(theMovieYear);
    }
    else
    {
     movieListOfEachActor.push_back(theMovieYear);

    }

    if(moviesL.find(theMovieYear) == moviesL.end())
    {
      moviesL.insert( {theMovieYear, new NodeMovieYear( record[1], record[2] ) } );
      moviesL[theMovieYear] -> listActors.push_back(record[0] );
    }
    else
    {
      moviesL[theMovieYear]->listActors.push_back(record[0]);
    }
    

  }//end of while
  verticesL.insert( { prev, new NodeActor(prev) } );
  verticesL[prev]->movieYear = movieListOfEachActor;

}
void Graph1:: runBFS( vector<string> nameOfActors, ofstream& outfile)
{
  for( auto element : verticesL )
  {
    element.second->dist = std::numeric_limits<int>::max();
    element.second->prev = "";
    element.second->prevMovie = "";
  }
  
  queue<NodeActor*> toExplore;

  toExplore.push( verticesL[ nameOfActors[0] ] );
  verticesL[ nameOfActors[0] ] -> dist = 0;
  NodeActor* curr;

  while( !toExplore.empty() )
  {
    curr= toExplore.front();
    toExplore.pop();
 
    vector< NodeActor* > theCurrNeighboors; 
    vector< NodeMovieYear* > thePrevMovies;
    
    NodeMovieYear* curr1;

    for( int i = 0; i < curr->movieYear.size(); i++ )
    {
      curr1 = moviesL[curr->movieYear[i] ];
   
      for( int j = 0; j < curr1->listActors.size();j++)
      {
        if(curr1->listActors[j] != curr->headerActor )
        {
          theCurrNeighboors.push_back( verticesL[curr1->listActors[j]] );
          thePrevMovies.push_back( curr1 );
        }
      }     
    }
    
    for( int i = 0; i < theCurrNeighboors.size(); i++ )
    {
      NodeActor* temp = theCurrNeighboors[i];
    
      if( temp->dist == std::numeric_limits<int>::max() )
      {
        temp->dist = curr->dist + 1;
        temp->prev = curr->headerActor;
        temp->prevMovie = thePrevMovies[i]->headerMovie + thePrevMovies[i]->headerYear;
        toExplore.push(temp);
      }
    }
 
    
    theCurrNeighboors.clear();
    thePrevMovies.clear();
   
  }

  printToOutfile( nameOfActors, outfile);

}
void Graph1:: runDijkstra( vector<string> nameOfActors, ofstream& outfile )
{
  for( auto element : verticesL )
  {
    element.second->dist = std::numeric_limits<int>::max();
    element.second->prev = "";
    element.second->prevMovie = "";
    element.second->done = false;
  }
  
  std::priority_queue<WeightedEdge*, vector<WeightedEdge*>, WeightedEdgeComp > toExplore; 
  verticesL[nameOfActors[0]]->dist = 0;
  
  WeightedEdge * weightedEdgy;
  weightedEdgy = new WeightedEdge( nameOfActors[0], 0 );
  toExplore.push(weightedEdgy);

  NodeActor* curr;
  WeightedEdge* currEdge;

  while(!toExplore.empty())
  {
    currEdge = toExplore.top();
    toExplore.pop();
    curr = verticesL[currEdge->nM];

    if( curr->done == false )
    {
      curr->done = true;
      
      vector<WeightedEdge*> theCurrNeighboors;
      vector<NodeMovieYear*> thePrevMovies;

      NodeMovieYear * curr1;
      //list the curr neighboors
      for( int i = 0 ; i <curr->movieYear.size(); i++ )
      {
        curr1 = moviesL[ curr->movieYear[i] ];
        
        for( int j = 0; j< curr1->listActors.size(); j++)
        {
          if( curr1->listActors[j] != curr->headerActor && verticesL[curr1->listActors[j] ] ->done == false )
          {
            WeightedEdge * weightedEdgyJess;
            int year = stoi( curr1->headerYear );
            weightedEdgyJess = new WeightedEdge( curr1->listActors[j], 1 + (2015 - year)  );
            theCurrNeighboors.push_back(weightedEdgyJess);
            thePrevMovies.push_back(curr1);
          }
        }
      }
      for( int i = 0; i < theCurrNeighboors.size(); i++ )
      {
        NodeActor* temp = verticesL[ theCurrNeighboors[i]->nM ];
         
        int distanceC = curr->dist + theCurrNeighboors[i]->weight;
        if(distanceC < temp->dist )
        {
          temp->prevMovie = thePrevMovies[i]->headerMovie + thePrevMovies[i]->headerYear;
          temp->prev = curr->headerActor;
          temp->dist = distanceC;
    
          WeightedEdge * edgeEdge;
          edgeEdge = new WeightedEdge( theCurrNeighboors[i]->nM , distanceC );
          toExplore.push(edgeEdge);
        }
      } 
    
      theCurrNeighboors.clear();
      thePrevMovies.clear();

    } //if


  }//end of toExplore.empty
  
  printToOutfile( nameOfActors, outfile);


}
void Graph1::printToOutfile( vector<string> nameOfActors, ofstream& outfile )
{
  vector<string> thePath;

  NodeActor* curr = verticesL[nameOfActors[1] ];

  while( curr!= verticesL[nameOfActors[0]] )
  {
    thePath.push_back( curr->headerActor);
 
    string movieYear= curr->prevMovie;
    string year = movieYear.substr( movieYear.length() - 4, 4 );
    string movie = movieYear.substr( 0 , movieYear.length() - 4);

    thePath.push_back(year);
    thePath.push_back(movie);
    curr= verticesL[ curr->prev ];

  }
  thePath.push_back( verticesL[ nameOfActors[0] ]->headerActor ); 

//  for( int i = 0; i< thePath.size(); i++)
//  {
//    cout<< thePath[i]<<endl;
//  }
  //start printing
  bool edgeTime = false;
  for( int i = thePath.size()-1 ; i >= 0 ; i-- )
  {
    if( edgeTime == false)
    {
      outfile<<"("<<thePath[i]<<")"; 
      edgeTime = true;  
    }
    else
    {
      outfile<<"--["<<thePath[i]<<"#@"<<thePath[i-1]<<"]-->";
      edgeTime = false;
      i = i -1;
    }
  }
  outfile<<"\n";

}
int Graph1:: getNumberOfActors() const
{
  return verticesL.size();
}
int Graph1:: getNumberOfMovies() const
{
  return moviesL.size();
}
/**
  cout<<"start printing"<<endl;
  for(auto element : verticesL)
  {
    cout<< element.second->headerActor <<endl;
    for(int i = 0 ; i<element.second->movieYear.size(); i++ )
    {
      cout<<element.second->movieYear[i] << endl;
    }
    cout << "\n" <<endl;
  }
  cout << "vertices end" << endl;
*/
