#ifndef GRAPH1_H
/**
 * CSE 100 PA4 C++ Graph
 * Authors: Melvin Wijaya, Jesslyn Hernawan
 * Date: 6/5/2017
 * Assignment: PA4
 *
 */

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

/**
 * The class for inside the Graph1 class to
 * solve the Disjoint Set problem
 *
 */
class DisjointSet
{
  private:
    unordered_map<string, string> graphTable; // <node, itsParent>
    unordered_map<string, int> sentinelSize; // <sentinelNode(set), itsSize>
  
  public:
    friend class Graph1;
   
    /*Create a Disjoint Set*/  
    DisjointSet();

    /* union two sets where the first node and second node belongs to*/
    void toUnion( string firstNode, string secondNode );

    /*find the name of the set in which a node belongs to*/
    string find( string theLostNode );

};

/**
 * The class for inside the Graph1 class
 * to help creating the priority queue
 * with earliest year as the priority
 */
class nodePQ
{
  private:
    string movieName;  //name of the movie
    int year;  // year of that movie
    vector<string> actorList;  //list of that actors that play in that movie
  public:
    friend class Graph1;
 
    /* create the nodePQ */
    nodePQ( string movieName, int year, vector<string> aList );

    /* overload the < operator */
    bool operator<(const nodePQ& other);

};

/**
 * Comparison class for the nodePQ
 * to work in priority queue as
 * a min heap
 *
 */
class nodePQComp
{
  public:
   bool operator()( nodePQ& lhs, nodePQ& rhs )
   {
     return lhs < rhs;
   }


};

/**
 * The class for edge with weight
 *
 */
class WeightedEdge
{
  private:
    string nM; //name of the neighboor
    int weight; //weight of the edge

  public:
    friend class Graph1;

    /* create the weighted edge */
    WeightedEdge( string neighbors, int w );   

    /* overload the < operator */
    bool operator<(const WeightedEdge& other );
};

/**
 * The class for vertex in a graph
 *
 *
 */
class NodeActor
{
  private:
    string headerActor; //name of the actor
    string prev; // name of the actor before this vertex
    int dist; // distance from the source to this vertex
    string prevMovie; // name of the movie before this vertex
    vector<string> movieYear;  //list of movies in which this actor plays
    bool done; 

  public:
    friend class Graph1;
    
    /*create the NodeActor */
    NodeActor( string hActor);
   
};

/**
 * The class NodeMovieYear that keep tracks
 * with the movies and their year and the
 * list of actors who play in these movies
 */
class NodeMovieYear
{
  private:
    string headerMovie; // name of the movie
    string headerYear;  // the year of the movie
    vector<string> listActors; // list of actors who play in this movie
  public:
    friend class Graph1;

    /* create the NodeMovieYear */
    NodeMovieYear( string hMovie, string hYear );
};

/**
 * The class that represent the whole graph
 *
 */
class Graph1
{
  private:
    unordered_map<string, NodeActor * > verticesL;  // list of actors
    unordered_map<string, NodeMovieYear * > moviesL; //list of movies
    std::priority_queue<nodePQ, vector<nodePQ>, nodePQComp> pq; // list of movies sorted ascending    

    bool built;

    /* printing to outfile */
    void printToOutfile( vector<string> nameOfActors, ofstream& outfile ); 

    /* return true if two actors are connected. False, otherwise ) */
    bool runBFSofActorConnection( vector<string> nameOfActors );  

  public:
   
    /*create the Graph*/
    Graph1();

    /*destructor for the Graph*/
    ~Graph1();

    /*build the graph from an input file*/
    void buildGraph( ifstream& infile );

    /*run BFS for two vertices */
    void runBFS( vector<string> nameOfActors, ofstream& outfile );

    /* run dijkstra for two vertices */
    void runDijkstra( vector<string> nameOfActors, ofstream& outfile );

    /* create a list of movies sorted in ascending order */
    void createQueue( ifstream& infile );
  
    /* find the earliest year in which two actors are connected through bfs */
    void findActorConnection( vector<string> nameOfActors, ofstream& outfile);

    /* find the earliest year in which two actors are connected throught disjoint set*/
    void findActorConnectionThruDT( vector<string> nameOfActors, ofstream& outfile);
 
    /* get the number of vertices */
    int getNumberOfActors()const;
  
    /* get the number of movies */
    int getNumberOfMovies() const;
};

#endif //GRAPH1_H
