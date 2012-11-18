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

#include "./Filter.h"

#include "./Port.h"

using std::vector;
using std::string;

namespace filter_graph {

void Filter::Process() {
  // check if all input data are ready, return and do nothing if one is not
  vector<Port*>::iterator it = input_ports_.begin();
  for (; it != input_ports_.end(); ++it)
    if (!(*it)->Readable())
      return;
  // check if all output ports are ready, return and do nothing if one is not
  it = output_ports_.begin();
  for (; it != output_ports_.end(); ++it)
    if (!(*it)->Writable())
      return;
  // do the concrete processing task
  OnProcess();
  // reset all input ports so they can receive new data
  it = input_ports_.begin();
  for (; it != input_ports_.end(); ++it)
    (*it)->Reset();
  // reset all output ports so they can write new data
  it = output_ports_.begin();
  for (; it != output_ports_.end(); ++it)
    (*it)->Reset();
}

Port* Filter::GetPort(string name) {
  vector<Port*>::iterator it = input_ports_.begin();
  for (; it != input_ports_.end(); ++it)
    if ((*it)->Name() == name)
      return *it;

  it = output_ports_.begin();
  for (; it != output_ports_.end(); ++it)
    if ((*it)->Name() == name)
      return *it;

  return NULL;
}

void Filter::AddInput(Port* port) {
  input_ports_.push_back(port);
}

void Filter::AddOutput(Port* port) {
  output_ports_.push_back(port);
}

}  // namespace filter_graph
