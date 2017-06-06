#ifndef GRAPH1_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <functional>
#include <queue>
#include <functional>

using namespace std;

class DisjointSet
{
  private:
    unordered_map<string, string> graphTable; // <node, itsParent>
    unordered_map<string, int> sentinelSize; // <sentinelNode(set), itsSize>
  
  public:
    friend class Graph1;
   
    DisjointSet();
    void toUnion( string firstNode, string secondNode );
    string find( string theLostNode );

};

class nodePQ
{
  private:
    string movieName;
    int year;
    vector<string> actorList;
  public:
    friend class Graph1;
  
    nodePQ( string movieName, int year, vector<string> aList );

    bool operator<(const nodePQ& other);

};

class nodePQComp
{
  public:
   bool operator()( nodePQ& lhs, nodePQ& rhs )
   {
     return lhs < rhs;
   }


};

class WeightedEdge
{
  private:
    string nM;
    int weight;

  public:
    friend class Graph1;

    WeightedEdge( string neighbors, int w );   

    bool operator<(const WeightedEdge& other );
};

class NodeActor
{
  private:
    string headerActor; 
    string prev;
    int dist;
    string prevMovie;
    vector<string> movieYear; 
    bool done;

  public:
    friend class Graph1;
    
    NodeActor( string hActor);

    
};

class NodeMovieYear
{
  private:
    string headerMovie;
    string headerYear;
    vector<string> listActors;
  public:
    friend class Graph1;

    NodeMovieYear( string hMovie, string hYear );
};

class Graph1
{
  private:
    unordered_map<string, NodeActor * > verticesL;
    unordered_map<string, NodeMovieYear * > moviesL;
    std::priority_queue<nodePQ, vector<nodePQ>, nodePQComp> pq;    
    //DisjointSet mySet;

    bool built;

    void printToOutfile( vector<string> nameOfActors, ofstream& outfile );
    bool runBFSofActorConnection( vector<string> nameOfActors );  

  public:
    Graph1();
    ~Graph1();
    void buildGraph( ifstream& infile );
    void runBFS( vector<string> nameOfActors, ofstream& outfile );
    void runDijkstra( vector<string> nameOfActors, ofstream& outfile );
    void createQueue( ifstream& infile );
    void findActorConnection( vector<string> nameOfActors, ofstream& outfile);
    void findActorConnectionThruDT( vector<string> nameOfActors, ofstream& outfile);
 
   
   int getNumberOfActors()const;
   int getNumberOfMovies() const;
//   unordered_map<string, NodeActor *> getVerticesL() const;
//   unordered_map<string, NodeMovieYear * > getMoviesL() const; 

};

#endif //GRAPH1_H
