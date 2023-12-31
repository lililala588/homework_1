#include <gmock/gmock.h>

#include "graph.h"
#include "test_unit.h"

using namespace testing;

struct MainPathTestParam{
  Graph inputGraph;
  std::vector<std::vector<int>> expectResult;
};


class MainPathTest:public TestWithParam<MainPathTestParam>{
protected:
  void SetUp() override{
    param_ = GetParam();
  }
  void TearDown() override{

  }
  MainPathTestParam param_;
};

TEST_P(MainPathTest, Normal){
  const Graph& graph = param_.inputGraph; 
  std::vector<std::vector<int>> mainPath = graph.findMainPaths();

  ASSERT_THAT(mainPath, ContainerEq(param_.expectResult));
}


static const MainPathTestParam remove510Test = {
  TestUnit::TestGraph(
            " 6 7 6 8"
            " 7 9"
            " 8 14"
            " 9 10 9 5"
            " 10 11 10 12"
            " 11 9"
            " 12 13"
            " 14 5 14 15"
            " 15 16 15 17"
            " 16 14"
            " 17 18"),
  {
    {6,7,9,5},
    {6,7,9,10,11},
    {6,7,9,10,12,13},
    {6,8,14,5},
    {6,8,14,15,16},
    {6,8,14,15,17,18},
    {9,10,11,9},
    {10,11,9,5},
    {10,11,9,10},
    {11,9,10,11},
    {11,9,10,12,13},
    {14,15,16,14},
    {15,16,14,5},
    {15,16,14,15},
    {16,14,15,16},
    {16,14,15,17,18},
  },

};


INSTANTIATE_TEST_SUITE_P(remove510Test,
                MainPathTest,
                Values(remove510Test));