#include"graph.h"
#include<algorithm>


namespace{

bool isSubset(const std::vector<int>& subset, const std::vector<int>& fullSet) {
    int i = 0, j = 0;
    while (i < subset.size() && j < fullSet.size()) {
        if (subset[i] == fullSet[j]) {
            ++i;
        }
        ++j;
    }
    return i == subset.size();
}

}  // namespace



void Graph::AddEdge(int from, int to){
  if(from==to){
    return;
  }
  if(from>=graph_.size()){
    maxNodeId_ = std::max(from, maxNodeId_);
    graph_.resize(from+1); 
  }
  if(to>=graph_[from].size()){
    maxNodeId_ = std::max(to,maxNodeId_);
    graph_[from].resize(to+1,0);
  }
  nodeSet_.insert(from);
  nodeSet_.insert(to);
  graph_[from][to] = 1;
}

bool Graph::IsEdge(int from, int to) const{
  if(!IsNodeInGraph(from)||graph_.size()<=from){
    return false;
  }
  if(!IsNodeInGraph(to)||graph_[from].size()<=to){
    return false;
  }
  return graph_[from][to];
}


bool Graph::IsNodeInGraph(int nodeId) const{
  return nodeSet_.count(nodeId)>0;
}

int Graph::MaxNodeId() const{
  return maxNodeId_;
}


void Graph::DFS(int start, int current, 
          std::vector<bool>& visited, 
          std::vector<int>& path, 
          std::vector<std::vector<int>>& allPaths) const{
  visited[current] = true;
  path.push_back(current);
  allPaths.push_back(path);

  for (auto & neighbor : nodeSet_) {
      if (IsEdge(current,neighbor) && !visited[neighbor]) {
          DFS(start, neighbor, visited, path, allPaths);
      }
      else if (IsEdge(current,neighbor) && neighbor == start && path.size() > 1) {
          // If a cycle is detected, add the cycle path to allPaths
          auto tmp = path;
          tmp.push_back(neighbor);
          allPaths.push_back(tmp);
      }
  }
  visited[current] = false;
  path.pop_back();
}

std::vector<std::vector<int>> Graph::findAllPaths() const{
      std::vector<bool> visited(MaxNodeId()+1, false);
      std::vector<int> path;
      std::vector<std::vector<int>> allPaths;

      for (auto & start : nodeSet_) {
          DFS(start, start, visited, path, allPaths);
          visited.assign(MaxNodeId()+1, false);
          path.clear();
      }

      return allPaths;
  }

std::vector<std::vector<int>> Graph::findMainPaths() const{
  std::vector<std::vector<int>> allPaths = findAllPaths();
  std::vector<std::vector<int>> mainPaths;
  for (const auto& path : allPaths) {
      bool isMainPath = true;
      for (const auto& otherPath : allPaths) {
          if (&path != &otherPath && isSubset(path, otherPath)) {
              // If the path is included in another path, it's not a main path
              isMainPath = false;
              break;
          }
      }
      if (isMainPath) {
          mainPaths.push_back(path);
      }
  }
  return mainPaths;
}