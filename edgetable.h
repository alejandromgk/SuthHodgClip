#ifndef EDGETABLE_H
#define EDGETABLE_H

#include <cstdio>
#include <algorithm>
#include <vector>
#include <list>
#include "point.h"

class Node;

class EdgeTable
{
public:
  EdgeTable();
  ~EdgeTable();
  void buildET (const PointArray & Pts);
  void buildAET(std::list<Node> & AET, std::list<Node> &ET, const int scanline);
  void updateAET(std::list<Node> & AET, const int scanline);
  void printET();
  static bool sortYminmax(Node n1, Node n2);
  
  // Vector formado por una lista de nodos, la tabla de bordes propiamente.
//   std::vector<std::list<Node> > Edges;
  std::list<Node> Edges;
};

class Node
{ 
public:
  Node() : yMax(0), x(0.0), mInversa(0.0) { };
  bool operator<( const Node n1 );
  float x, mInversa;
  int yMax, yMin;
};

#endif // EDGETABLE_H






