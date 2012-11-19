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

#ifndef FILTERGRAPH_H_
#define FILTERGRAPH_H_

#include <vector>
#include <string>

#include "./utils.h"

namespace filter_graph {

class PortListener {
 public:
  PortListener() {}
  virtual ~PortListener() {}
  virtual void OnData() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(PortListener);
};

// Ports are mean of communication between Filters. Ports can be connected
// with each others, such that data written to an output port will be
// delivered to all input ports connected to them.
class Port {
 public:
  explicit Port(std::string name);
  virtual ~Port();

  // Gets the name of the port.
  std::string Name() const { return name_; }
  // Gets the capacity of the port.
  size_t Capacity() const { return capacity_; }

  bool Readable() const { return readable_; }
  bool Writable() const { return writable_; }
  void Reset();

  // Gets the size of data the port is holding.
  size_t Size() { return size_; }

  // Force resize the port to the requested size.
  void Resize(size_t size);

  // Gets the generic data (void*) and put the given generic pointer to it.
  // Returns the number of bytes read.
  //
  // Sample usage:
  //  int* data;
  //  size_t size = port.Read((void**)&data);
  //  // data now points to the port's data.
  size_t Read(void** data) const;

  // Writes the given data. If data doesn't fit in the Port buffer, the buffer
  // is resized accordingly.
  // On success, return size; 0 otherwise.
  size_t Write(const void* data, size_t size);

  // Connects this port to the given port.
  void Connect(Port* port);

  // Returns the number of input ports that connect to this output port.
  int FanOut() const { return connected_ports_.size(); }

  // Regiters an object that listens for port related events.
  void AddListener(PortListener* listener) { listeners_.push_back(listener); }

 private:
  std::string name_;
  bool readable_, writable_;
  void* data_;
  size_t capacity_;
  size_t size_;
  std::vector<Port*> connected_ports_;
  std::vector<PortListener*> listeners_;
  DISALLOW_COPY_AND_ASSIGN(Port);
};

// A Filter is a unit block of processing. Filters can be connected with each
// other for sending and receiving data.
//
// Sample usage:
//    Port* out_port;
//    Port* in_port;
//    Filter* source;   // has output port out_port
//    Filter* sink;     // has input port in_port
//    // connects the ports
//    out_port->Connect(in_port);
//    // does work once, sink will do its work when source sends its output data
//    source->Process();
class Filter : public PortListener {
 public:
  Filter() { }
  virtual ~Filter() { }

  // Performs the filter processing task on the input ports' data and then
  // writes to output ports.
  void Process();

  // Gets the port with the given name.
  Port* GetPort(std::string name);

  // implementation of PortListener methods
  void OnData();

 protected:
  // Add ports
  void AddInput(Port* port);
  void AddOutput(Port* port);

  // The abstract processing function to be written by derived concrete Filter
  // classes. In this function, it can be assumed that all input data from input
  // ports are presented.
  virtual void OnProcess() = 0;

 private:
  std::vector<Port*> input_ports_;
  std::vector<Port*> output_ports_;
  DISALLOW_COPY_AND_ASSIGN(Filter);
};

}  // namespace filter_graph

#endif  // FILTERGRAPH_H_
