#include <gtest/gtest.h>

#include "../src/filter_graph.h"

namespace filter_graph {

// Allows checking how many times OnProcess() has been called
// Allows adding ports externally
class TestableFilter : public Filter {
 public:
  void AddInput(Port* port) { Filter::AddInput(port); }
  void AddOutput(Port* port) { Filter::AddOutput(port); }
  TestableFilter() { calls_ = 0; }
  int Calls() { return calls_; }

 protected:
  void OnProcess() { calls_++; }

 private:
  int calls_;
};

class FilterTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
};

void test_OnProcessIsCalled(TestableFilter& filter) {
  filter.Process();
  ASSERT_EQ(1, filter.Calls()) << "OnProcess() wasn't called.";
  filter.Process();
  ASSERT_EQ(2, filter.Calls()) << "OnProcess() wasn't called the second time.";
  filter.Process();
  ASSERT_EQ(3, filter.Calls()) << "OnProcess() wasn't called the third time.";
}

TEST_F(FilterTest, OnProcessIsCalled) {
  TestableFilter filter;
  test_OnProcessIsCalled(filter);
}

TEST_F(FilterTest, OnProcessIsCalled_FilterHasOutputPorts) {
  TestableFilter filter;
  Port output("output");
  filter.AddOutput(&output);
  test_OnProcessIsCalled(filter);
}

// test if Filter will process if its input port has data.
// and will not otherwise.
TEST_F(FilterTest, OnProcessIsCalled_FilterHasInputPorts) {
  TestableFilter filter;
  Port input("input");
  filter.AddInput(&input);
  input.Write(NULL, 0);
  filter.Process();
  ASSERT_EQ(1, filter.Calls()) << "OnProcess() wasn't called.";
  filter.Process();
  ASSERT_EQ(1, filter.Calls()) 
    << "OnProcess() should not be called if input port has no data.";
  input.Write(NULL, 0);
  filter.Process();
  ASSERT_EQ(2, filter.Calls()) << "OnProcess() wasn't called the second time.";
}

TEST_F(FilterTest, AddGetPorts) {
  TestableFilter filter;
  Port input("input"), output("output");
  filter.AddInput(&input);
  filter.AddOutput(&output);
  EXPECT_EQ(&input, filter.GetPort("input")) << "Input port wasn't retrieved correctly.";
  EXPECT_EQ(&output, filter.GetPort("output")) << "Output port wasn't retrieved correctly.";
  EXPECT_EQ(&output, filter["output"]) << "Output port wasn't retrieved correctly.";
}

TEST_F(FilterTest, AddGetPorts_UsingBracketOperator) {
  TestableFilter filter;
  Port input("input"), output("output");
  filter.AddInput(&input);
  filter.AddOutput(&output);
  EXPECT_EQ(&input, filter["input"]) << "Input port wasn't retrieved correctly.";
  EXPECT_EQ(&output, filter["output"]) << "Output port wasn't retrieved correctly.";
}

}  // namespace
