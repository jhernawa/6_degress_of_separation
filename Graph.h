#ifndef GRAPH_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

typedef pair< string, int > thePair;


class Node
{
  private:
    string nameActor;
    int index;
    int dist;
    int prev;
    vector<int> adj;
    vector<thePair> list; // movies list < movies title, year > 
  
  public:
    friend class Graph;
    
    Node( string nameOfActor, vector<thePair> movieList, int indx );

};

class Graph
{
  private:
    vector<Node*> verticesL; // list of vertices
    int vertices; // number of vertices
    int edgesDirected; //number of edgesDirected

    void listDownAdjList();
    void runBFSHelper(vector<int> indices, ofstream& out);
  public:
    Graph(); 
    ~Graph();
    vector<Node*> buildGraph( ifstream& infile );
    void runBFS( /* vector<Node*> theVertices ,*/ vector<string> nameOfActors, ofstream& outfile );

   /////
   int getNumberOfVertices()const;
   int getDirectedEdges() const;

  /////











};



#endif // GRAPH_H
