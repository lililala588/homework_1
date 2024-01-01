#include "node.h"
#include "generate_graph.h"
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <filesystem>


int main(int argc, char*argv[]){
  if(argc<3){
    printf("not enough argument\n");
    return -1;
  }
  const std::string tmpFileName = "tmp.txt";
  std::string filePath = argv[1];
  std::string methodName = argv[2];
  std::stringstream jarCmdStream; 
  jarCmdStream << "java -jar cfgparser.jar ";
  jarCmdStream << filePath << " " << methodName << " > " << tmpFileName;
  std::string jarCmd = jarCmdStream.str();
  int systemRet = std::system(jarCmd.c_str());

  std::ifstream file(tmpFileName);
  if(!file){
    printf("open file failed\n");
    return -1;
  }

  std::vector<Graph> graphArray;
  bool ret = GenerateMultiGraph(file,methodName,graphArray);
  if(!ret){
    printf("generate graph failed\n");
    return -1;
  }
  for (const auto& graph : graphArray) {
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
    printf("\n");
  }
  file.close();
  std::filesystem::remove(tmpFileName);
  return 0;
}