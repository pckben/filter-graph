#include <gtest/gtest.h>

#include "../src/filter_graph.h"

namespace filter_graph {

// Has one output port to write an increasing counter.
class CounterFilter : public Filter {
 public:
  CounterFilter() : data_(0), counterPort_("counter_out") {
    AddOutput(&counterPort_);
  }

 protected:
  void OnProcess() { 
    data_++; 
    counterPort_.Write(&data_, sizeof(data_));
  }
 
 private:
  int data_;
  Port counterPort_;
};

// Has one input port to receive an integer.
class IntegerConsumer : public Filter {
 public:
  IntegerConsumer() : data_(0), input_("integer_in") {
    AddInput(&input_);
  }
 
  int Data() const { return data_; }

 protected:
  void OnProcess() { 
    int* received_data = NULL;
    input_.Read((void**)&received_data); 
    data_ = *received_data;
  }

 private:
  int data_;
  Port input_;
};


class CounterFilterGraphTest : public ::testing::Test {
};

// Test if a source filter escalate the processing chain to the sink filter so
// the sink filter can work automatically.
TEST_F(CounterFilterGraphTest, OneConsumer) {
  CounterFilter counter;
  IntegerConsumer consumer;
  counter.GetPort("counter_out")->Connect(consumer.GetPort("integer_in"));
  counter.Process();
  ASSERT_EQ(1, consumer.Data()) 
    << "Consumer's Process() wasn't called, or data was not sent correctly.";
  counter.Process();
  ASSERT_EQ(2, consumer.Data())
    << "Consumer's Process() wasn't called the second time, or data was not sent correctly.";
  counter.Process();
  ASSERT_EQ(3, consumer.Data())
    << "Consumer's Process() wasn't called the third time, or data was not sent correctly.";
}

TEST_F(CounterFilterGraphTest, TwoConsumer) {
  CounterFilter counter;
  IntegerConsumer consumer1, consumer2;
  counter.GetPort("counter_out")->Connect(consumer1.GetPort("integer_in"));
  counter.GetPort("counter_out")->Connect(consumer2.GetPort("integer_in"));
  counter.Process();
  counter.Process();
  counter.Process();
  ASSERT_EQ(3, consumer1.Data())
    << "Consumer1's Process() wasn't called the third time, or data was not sent correctly.";
  ASSERT_EQ(3, consumer2.Data())
    << "Consumer2's Process() wasn't called the third time, or data was not sent correctly.";
}

}  // namespace
