#include <gtest/gtest.h>

#include "../src/Port.h"

namespace filter_graph {

class PortTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
};

TEST_F(PortTest, BasicSetGet) {
  Port port("myname", 100);
  ASSERT_EQ("myname", port.Name()) 
    << "Port name wasn't set correctly.";
  ASSERT_EQ((size_t)100, port.Capacity()) 
    << "Port capacity wasn't set correctly.";
}

TEST_F(PortTest, ReadableWritableReset) {
  Port port("testport", sizeof(int));
  ASSERT_TRUE(port.Writable()) 
    << "Port should be writable after created.";
  ASSERT_FALSE(port.Readable()) 
    << "Port should not be readable after created.";
  int* read_data;
  ASSERT_EQ((size_t)0, port.Read((void**)&read_data))
    << "No bytes should have been read when port is not readable.";
  int data = 10;
  port.Write(&data, sizeof(data));
  ASSERT_FALSE(port.Writable())
    << "Port should not be writable after data is written.";
  ASSERT_TRUE(port.Readable())
    << "Port should be readable after data is written.";
  ASSERT_TRUE(port.Read((void**)&read_data) > 0)
    << "Port must read something when being readable.";
  ASSERT_TRUE(port.Read((void**)&read_data) > 0)
    << "Port must be readable multiple time.";
  port.Reset();
  ASSERT_TRUE(port.Writable())
    << "Port must be writable after reset.";
  ASSERT_FALSE(port.Readable())
    << "Port must not be readable after reset.";
}

TEST_F(PortTest, ConnectFanOut) {
  Port port1("port1", 0);
  Port port2("port2", 0);
  EXPECT_EQ(0, port1.FanOut());
  port1.Connect(&port2);
  EXPECT_EQ(1, port1.FanOut());
}

// Write to a port and then read back
TEST_F(PortTest, WriteThenReadSamePort) {
  int data = 10;
  Port port("testport", sizeof(data));
  ASSERT_EQ((size_t)0, port.Size()) 
    << "Port's data size must be zero after created.";
  size_t nbytes = port.Write(&data, sizeof(data));
  ASSERT_EQ(sizeof(data), nbytes) 
    << "Data written must have the same size as data given.";
  ASSERT_EQ(sizeof(data), port.Size()) 
    << "Port size must equal the size of data written.";
  // now read back on the same port
  int *data_read = NULL;
  nbytes = port.Read((void**)&data_read);
  ASSERT_EQ(sizeof(data), nbytes) 
    << "Data read must have the same size as data written.";
  ASSERT_EQ(data, *data_read) 
    << "Data read must be the same as data written.";
}

// Write to a port, and then read back from the remote port
TEST_F(PortTest, WriteThenReadRemote) {
  int data = 1234;
  Port input("input", sizeof(int));
  Port output("output", sizeof(int));
  output.Connect(&input);
  output.Write(&data, sizeof(data));
  // now read back on the remote port
  int *received_data = NULL;
  EXPECT_EQ(sizeof(data), input.Size()) 
    << "Size of input port must be the same as size written at output port.";
  input.Read((void**)&received_data);
  ASSERT_EQ(data, *received_data) 
    << "Data wasn't received correctly from input port.";
}

}  // namespace
