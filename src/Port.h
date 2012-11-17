// Copyright 2012 Pham Chau Khoa (pckben@gmail.com)

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
// WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache 2 License for the specific language governing permissions and
// limitations under the License.

#ifndef FILTERGRAPH_PORT_H_
#define FILTERGRAPH_PORT_H_

#include <vector>
#include "./utils.h"

namespace filter_graph {

// Ports are mean of communication between Filters. Ports can be connected
// with each others, such that data written to an output port will be
// delivered to all input ports connected to them.
class Port {
 public:
  explicit Port(size_t capacity);
  virtual ~Port();

  // Returns true if the Port is ready for read/write data. An input port might
  // return false if data is not ready to be read, and an output port might
  // return false if old data has not been sent yet.
  bool Ready() { return ready_; }

  // Resets the state of the port. This is used to indicate that the data has
  // been consumed, and the port is ready to send/receive new data.
  void Reset() { ready_ = true; }

  // Gets the size of data the port is holding.
  size_t Size() { return size_; }

  // Gets the generic data (void*) and put the given generic pointer to it.
  // Returns the number of bytes read.
  //
  // Sample usage:
  //  int* data;
  //  size_t size = port.Read(&data);  // data now points to the port's data.
  size_t Read(void** data) const;

  // Writes the given data.
  // On success, return size; 0 otherwise.
  size_t Write(const void* data, size_t size);

  // Send the data written in this port to all other connected ports.
  void Send();

  // Connects this port to the given port.
  void Connect(Port* port);

 private:
  bool ready_;
  void* data_;
  size_t capacity_;
  size_t size_;
  std::vector<Port*> connected_ports_;
  DISALLOW_COPY_AND_ASSIGN(Port);
};

};  // namespace

#endif  // FILTERGRAPH_PORT_H_
