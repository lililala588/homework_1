#include "test_unit.h"

namespace TestUnit{

Graph TestGraph(const std::string& input){
  Graph graph;
  std::istringstream inputStream(input);
  int from = 0;
  int to =0;
  while(inputStream>>from&&inputStream>>to){
    graph.AddEdge(from,to);
  }
  return graph;
}


} // namespace TestUnit