#ifndef _GENERATE_GRAPH_H_
#define _GENERATE_GRAPH_H_

#include<sstream>
#include<vector>
#include<string>

#include "graph.h"


using GraphType = Graph;


bool GenerateGraph(
  std::istream& input,
  const std::string& functionName,
  GraphType& graph); 


bool GenerateMultiGraph(
	std::istream& input,
	const std::string& functionName,
	std::vector<GraphType>& graphs);




#endif // _GENERATE_GRAPH_H_