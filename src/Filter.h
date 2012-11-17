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

#ifndef FILTERGRAPH_FILTER_H_
#define FILTERGRAPH_FILTER_H_

#include <vector>
#include "./utils.h"

namespace filter_graph {

class Port;

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
class Filter {
 public:
  Filter() { }
  virtual ~Filter() { }

  // Performs the filter processing task on the input ports' data and then
  // writes to output ports.
  void Process();

 protected:
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

#endif  // FILTERGRAPH_FILTER_H_