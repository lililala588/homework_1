#include "node.h"
#include "generate_graph.h"
#include <cstdio>
#include <fstream>


int main(int argc, char*argv[]){
  if(argc<3){
    printf("not enough argument\n");
    return -1;
  }
  std::string filePath = argv[1];
  std::string methodName = argv[2];

  std::ifstream file(filePath);
  if(!file){
    printf("open file failed\n");
    return -1;
  }

  Graph graph;
  bool ret = GenerateGraph(file,methodName,graph);
  if(!ret){
    printf("generate graph failed\n");
    return -1;
  }

  for(int i =0;i<=graph.MaxNodeId();i++){
    int edgeCount = 0;
    for(int j =0;j<=graph.MaxNodeId();j++){
      if(graph.IsEdge(i,j)){
        edgeCount++;
        printf("(%d,%d) ", i, j);
      }
    }
    if(edgeCount>0){
      printf("\n");
    }
  }
  printf("main path:\n");
  std::vector<std::vector<int>> mainPaths = graph.findMainPaths();
  for(auto & path: mainPaths){
    for(auto x: path){
      printf("%d ", x);
    }
    printf("\n");
  }
  return 0;
}