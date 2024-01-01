#include "gmock/gmock.h"
#include "generate_graph.h"
#include<fstream>
#include<cstring>

#include "test_unit.h"

using namespace testing;

struct GenerateGraphTestParam{
  std::string fileName;
  std::string functionName;
  GraphType expectResult;
};

struct GenerateMultiGraphTestParam {
  std::string fileName;
  std::string functionName;
  std::vector<GraphType> expectResult;
};

class GenerateGraphTest: 
    public TestWithParam<GenerateGraphTestParam>{
  protected:
    void SetUp() override{
      param_ = GetParam();
    }
    void TearDown() override{

    }
    GenerateGraphTestParam param_;
};


class GenerateMultiGraphTest : 
    public TestWithParam<GenerateMultiGraphTestParam> 
{
  protected:
    void SetUp() override {
        param_ = GetParam();
    }
    void TearDown() override {
        
    }
    GenerateMultiGraphTestParam param_;
};


TEST_P(GenerateGraphTest,Normal){
  const std::string filePrefix = "../../testData/";
  std::ifstream iFile(filePrefix+param_.fileName);
  GraphType result;
  GenerateGraph(iFile,param_.functionName,result);
  const GraphType& expect = param_.expectResult;
  ASSERT_THAT(result.MaxNodeId() ,Eq(expect.MaxNodeId()));
  for(int i =0;i<result.MaxNodeId();i++){
    for(int j =0;j<result.MaxNodeId();j++){
      ASSERT_THAT(result.IsEdge(i,j),Eq(expect.IsEdge(i,j)))
        << "(" << i <<","<<j <<")" << " "
        <<"result "<< result.IsEdge(i,j) << "  "
        <<"expect "<< expect.IsEdge(i,j) << std::endl;
    }
  }
}

TEST_P(GenerateMultiGraphTest, Base) {
  const std::string filePrefix = "../../testData/";
  std::ifstream iFile(filePrefix+param_.fileName);
  std::vector<GraphType> resultVec;
  GenerateMultiGraph(iFile,param_.functionName,resultVec);
  const std::vector<GraphType>& expectVec= param_.expectResult;
  ASSERT_THAT(resultVec.size(), Eq(expectVec.size()));
  for (int i = 0; i < resultVec.size(); i++) {
    const GraphType& result = resultVec[i];
    const GraphType& expect = expectVec[i];
    ASSERT_THAT(result.MaxNodeId() ,Eq(expect.MaxNodeId()));
    for(int i =0;i<result.MaxNodeId();i++){
      for(int j =0;j<result.MaxNodeId();j++){
        ASSERT_THAT(result.IsEdge(i,j),Eq(expect.IsEdge(i,j)))
          << "(" << i <<","<<j <<")" << " "
          <<"result "<< result.IsEdge(i,j) << "  "
          <<"expect "<< expect.IsEdge(i,j) << std::endl;
      }
    }
  }
}


static const GenerateGraphTestParam ifTestParam = {
  "if.txt", 
  "ifTest",
  TestUnit::TestGraph("0 1 0 2 1 3 2 5 3 4 3 8 4 3 5 6 6 7 6 8 7 6"),
};


static const GenerateGraphTestParam remove486TestParam = {
  "remove486.txt",
  "remove",
  TestUnit::TestGraph("0 2 2 3 3 1 1 4 2 1"),
};

static const GenerateGraphTestParam remove510TestParam = {
  "remove510.txt",
  "remove",
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
};

static const GenerateGraphTestParam whileTestParam = {
  "whileTest.txt",
  "whileTest",
  TestUnit::TestGraph(
            " 0 1 0 4"
            " 1 3"
            " 2 0"
            " 3 2"),
};

static const GenerateGraphTestParam whileTestNoBodyParam = {
  "whileTestNoBody.txt",
  "whileTestNoBody",
  TestUnit::TestGraph(
            " 0 1"
            " 1 3"
            " 2 0"),
};

static const GenerateMultiGraphTestParam removeMultiTestParam = {
    "remove.txt",
    "remove",
    {
      remove486TestParam.expectResult, 
      remove510TestParam.expectResult
    },
};


INSTANTIATE_TEST_SUITE_P(ifTest,
                GenerateGraphTest,
                Values(ifTestParam));

INSTANTIATE_TEST_SUITE_P(remove486Test,
                GenerateGraphTest,
                Values(remove486TestParam));

INSTANTIATE_TEST_SUITE_P(remove510Test,
                GenerateGraphTest,
                Values(remove510TestParam));

INSTANTIATE_TEST_SUITE_P(whileTest,
                GenerateGraphTest,
                Values(whileTestParam));

INSTANTIATE_TEST_SUITE_P(whileTestNoBody,
                GenerateGraphTest,
                Values(whileTestNoBodyParam));

INSTANTIATE_TEST_SUITE_P(removeMultiTest,
                GenerateMultiGraphTest,
                Values(removeMultiTestParam));