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

#include "./filter_graph.h"

#include <assert.h>

using std::vector;
using std::string;

namespace filter_graph {

Port::Port(string name, size_t capacity)
    : name_(name),
      readable_(false),
      writable_(true),
      data_(NULL),
      capacity_(capacity),
      size_(0) {
  data_ = malloc(capacity);
  assert(data_ != NULL);
}

Port::~Port() {
  if (data_)
    free(data_);
}

size_t Port::Read(void** data) const {
  if (!Readable()) return 0;
  *data = data_;
  return size_;
}

size_t Port::Write(const void* data, size_t size) {
  if (!Writable()) return 0;
  writable_ = false;
  assert(size <= capacity_);
  memcpy(data_, data, size);
  size_ = size;
  // Send to remote ports
  vector<Port*>::iterator it = connected_ports_.begin();
  for (; it != connected_ports_.end(); ++it) {
    (*it)->Write(data, size);
  }
  readable_ = true;
  // notify listeners
  vector<PortListener*>::iterator it1 = listeners_.begin();
  for (; it1 != listeners_.end(); ++it1) {
    (*it1)->OnData();
  }
  return size;
}

void Port::Connect(Port* port) {
  connected_ports_.push_back(port);
}

}  // namespace
