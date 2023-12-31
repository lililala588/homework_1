#ifndef _GRAPH_H_
#define _GRAPH_H_

#include<vector>
#include<set>

class Graph{
public:
  void AddEdge(int from, int to);

  bool IsEdge(int from, int to) const;

  int MaxNodeId() const;

  std::vector<std::vector<int>> findAllPaths() const;

  std::vector<std::vector<int>> findMainPaths() const;

  bool IsNodeInGraph(int nodeId) const;

  const std::set<int>& AllNodes() const{
    return nodeSet_;
  }

private:
  void DFS(int start, int current, 
          std::vector<bool>& visited, 
          std::vector<int>& path, 
          std::vector<std::vector<int>>& allPaths) const;

  std::set<int> nodeSet_;
  std::vector<std::vector<int>> graph_;
  int maxNodeId_=-1;
};


#endif // _GRAPH_H_