#include "node.h"
#include "generate_graph.h"

#include<unordered_map>
#include<cassert>
#include<algorithm>
#include<fstream>
#include<string>
#include<vector>


namespace {

int dfs(Node* node, GraphType& graphic, std::vector<int>& needNext);

Node* GenerateNodeTree(std::istream& input, const std::string& functionName,int& maxNodeId) {
    const int kBufferSize = 1080;
    const char kSeparator = '@';
    // 跳过第一行
    std::string lineData;
    if (!std::getline(input, lineData)) {
        return nullptr;
    }
    maxNodeId = -1;
    Node* root = new Node(-1, kStmt, -1);
    std::unordered_map<int, std::vector<Node*>> nodes;
    nodes[-1].push_back(root);
    char tmpBuffer[kBufferSize] = {'\0'};
    std::string formatStr = functionName + "[%d] %d %d %d %d %d %d %d %s@%s\n";
    while (std::getline(input,lineData)&&!lineData.empty()){
        int methodNum = 0;
        int nodeId = 0;
        int parentId = 0;
        int height = 0;
        int startX = 0;
        int startY = 0;
        int endX = 0;
        int endY = 0;
        int ret = std::sscanf(lineData.c_str(), 
            formatStr.c_str(),
            &methodNum, &nodeId, &parentId, &height, 
            &startX, &startY, &endX, &endY, 
            tmpBuffer);
        if (ret != 9) {
            return nullptr;
        }
        std::string typeAndStmt(tmpBuffer);
        int separatorIndex = typeAndStmt.find(kSeparator);
        if (separatorIndex == std::string::npos) {
            return nullptr;
        }
        std::string typeString = typeAndStmt.substr(0, separatorIndex);
        NodeType type = GetType(typeString);
        Node* node = new Node(nodeId, type, startX);
        nodes[nodeId].push_back(node);
        nodes[parentId].back()->AddChild(node);
        maxNodeId = std::max(maxNodeId, nodeId);
    }
    return root;
}


void connect(int from, int to, GraphType& graphic){
  if(from==to){
    return;
  }
  graphic.AddEdge(from,to);
}

/**
 * @brief 处理语句块
*/
int solveStmt(Node* node, GraphType& graphic,std::vector<int>& needNext){
  assert(node!=nullptr&&node->type()==kStmt);
  std::vector<Node*> children =  node->Children();
  std::sort(children.begin(),children.end(),[](const Node* lh, const Node* rh){
    return lh->LineNumber()<rh->LineNumber();
  });

  std::vector<int> lastNode;
  for(auto& child: children){
    std::vector<int> needNext;
    int id = dfs(child,graphic,needNext);
    for(auto x: lastNode){
      connect(x,id,graphic);
    }
    lastNode.swap(needNext);
  }
  needNext.insert(needNext.end(),lastNode.begin(),lastNode.end());
  return children[0]->Id();
}

/**
 * @brief 处理单个简单语句
*/
int solveNormal(Node* node, GraphType& graphic, std::vector<int>& needNext) {
    assert(node!=nullptr&&node->type()==kNormal);
    needNext.push_back(node->Id());
    return node->Id();
}


/**
 * @brief 处理if语句
*/
int solveIf(Node* node, GraphType& graphic,std::vector<int>& needNext){
  assert(node!=nullptr&&node->type()==kIf);
  std::vector<Node*> children =  node->Children();
  std::sort(children.begin(),children.end(),[](const Node* lh, const Node* rh){
    return lh->LineNumber()<rh->LineNumber();
  });


  std::vector<int> lastNode;
  bool bHasElse = false;
  for(auto& child: children){
    if (child->type() == kThenBody) {
        connect(node->Id(), child->Id(), graphic);
        lastNode = { child->Id() };
        continue;
    }
    // TODO(): else的逻辑处理有点奇怪
    if (child->type() == kElse) {
        bHasElse = true;
        needNext.insert(needNext.end(),lastNode.begin(),lastNode.end());
        connect(node->Id(), child->Id(), graphic);
        lastNode = { child->Id() };
        continue;
    }

    std::vector<int> needNext;
    int id = dfs(child,graphic,needNext);
    for(auto x: lastNode){
      connect(x,id,graphic);
    }
    lastNode.swap(needNext);
  }
  needNext.insert(needNext.end(),lastNode.begin(),lastNode.end());
  if (!bHasElse) {
      needNext.push_back(node->Id());
  }
  return node->Id();
}

/**
 * @brief 处理for语句
*/
int solveFor(Node* node, GraphType& graphic,std::vector<int>& needNext){
  assert(node!=nullptr&&node->type()==kFor);
  std::vector<Node*> children =  node->Children();
  std::sort(children.begin(),children.end(),[](const Node* lh, const Node* rh){
    return lh->LineNumber()<rh->LineNumber();
  });
  Node* forCondition = children[0];
  Node* forUpdate = children[1];
  connect(node->Id(),forCondition->Id(),graphic);

  std::vector<int> lastNode = {forCondition->Id()};
  for(int i = 2; i<children.size();i++){
    Node* child = children[i];
    if (child->type() == kForBody) {
        // 处理forbody
        connect(forCondition->Id(), child->Id(), graphic);
        lastNode = { child->Id() };
        continue;
    }
    std::vector<int> needNext;
    int id = dfs(child,graphic,needNext);
    for(auto x: lastNode){
      connect(x,id,graphic);
    }
    lastNode.swap(needNext);
  }
  for(auto x: lastNode){
    connect(x, forUpdate->Id(), graphic);
  }
  connect(forUpdate->Id(),forCondition->Id(),graphic);
  needNext.push_back(forCondition->Id());
  return node->Id();
}


/**
 * @brief 处理return语句
*/
int solveReturn(Node* node,GraphType& graphic, std::vector<int>& needNext){
  assert(node!=nullptr&&node->type()==kReturn);
  return node->Id();
}

/**
 * @brief 处理while语句
*/
int solveWhile(Node* node, GraphType& graph, std::vector<int>& needNext){
  assert(node!=nullptr&&node->type()==kWhile); 
  std::vector<Node*> children =  node->Children();
  std::sort(children.begin(),children.end(),[](const Node* lh, const Node* rh){
    return lh->LineNumber()<rh->LineNumber();
  });

  std::vector<int> lastNode = {node->Id()};
  for(auto & child: children){
    if(child->type()==kWhildBody){
      connect(node->Id(),child->Id(),graph);
      lastNode = {child->Id()};
      continue;
    }
    std::vector<int> needNext;
    int id = dfs(child,graph,needNext);
    for(auto x: lastNode){
      connect(x,id,graph);
    }
    lastNode.swap(needNext);
  }
  for(auto x: lastNode){
    connect(x,node->Id(),graph);
  }

  needNext.push_back(node->Id());
  return node->Id();
}

int dfs(Node* node, GraphType& graph, std::vector<int>& needNext){
  if(node==nullptr){
    return -1;
  }
  if(node->type()==kStmt){
    return solveStmt(node,graph,needNext);
  }else if(node->type()==kFor){
    return solveFor(node,graph,needNext);
  }else if(node->type()==kIf){
    return solveIf(node,graph,needNext);
  }else if(node->type()==kWhile){
    return solveWhile(node,graph, needNext);
  }else if(node->type() ==kReturn){
    return solveReturn(node,graph,needNext);
  }else if (node->type() == kNormal) {
      return solveNormal(node, graph, needNext);
  }
  return -1;
}

}  // namespace


bool GenerateGraph(
    std::istream& input,
    const std::string& functionName,
    GraphType& graph){
  int maxNodeId=0;
  Node* root = GenerateNodeTree(input,functionName,maxNodeId);
  if(nullptr==root){
    return false;
  }
  std::vector<int> needNext;
  dfs(root, graph,needNext);
  return true;
}
