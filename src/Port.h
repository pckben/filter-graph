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
#include <string>
#include "./utils.h"

namespace filter_graph {

// Ports are mean of communication between Filters. Ports can be connected
// with each others, such that data written to an output port will be
// delivered to all input ports connected to them.
class Port {
 public:
  Port(std::string name, size_t capacity);
  virtual ~Port();

  // Gets the name of the port.
  std::string Name() const { return name_; }
  // Gets the capacity of the port.
  size_t Capacity() const { return capacity_; }

  bool Readable() const { return readable_; }
  bool Writable() const { return writable_; }
  void Reset() { readable_ = false; writable_ = true; }

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

  // Connects this port to the given port.
  void Connect(Port* port);

  // Returns the number of input ports that connect to this output port.
  int FanOut() const { return connected_ports_.size(); }

 private:
  std::string name_;
  bool readable_, writable_;
  void* data_;
  size_t capacity_;
  size_t size_;
  std::vector<Port*> connected_ports_;
  DISALLOW_COPY_AND_ASSIGN(Port);
};

};  // namespace

#endif  // FILTERGRAPH_PORT_H_
