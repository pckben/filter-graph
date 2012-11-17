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

#include "./Port.h"

#include <assert.h>

using std::vector;

namespace filter_graph {

Port::Port(size_t capacity)
    : ready_(true),
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
  if (!ready_) return 0;
  *data = data_;
  return size_;
}

size_t Port::Write(const void* data, size_t size) {
  assert(size <= capacity_);
  if (!ready_) return 0;
  memcpy(data_, data, size);
  size_ = size;
  return size;
}

void Port::Send() {
  vector<Port*>::iterator it = connected_ports_.begin();
  for (; it != connected_ports_.end(); ++it) {
    (*it)->Write(data_, size_);
  }
}

void Port::Connect(Port* port) {
  connected_ports_.push_back(port);
}

}  // namespace
