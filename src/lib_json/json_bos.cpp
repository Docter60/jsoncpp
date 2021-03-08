#include <json/bos.h>

namespace Json {
Bos::Bos() : length(4), capacity(256) {
  bytes = new unsigned char[capacity];
  setLengthBytes();
}

Bos::Bos(const Bos& other) {
  bytes = new unsigned char[other.capacity];
  memcpy(bytes, other.bytes, other.length);
  length = other.length;
  capacity = other.capacity;
}

Bos::~Bos() {
  delete[] bytes;
  bytes = nullptr;
  length = 0;
  capacity = 0;
}

Bos& Bos::operator=(const Bos& other) {
  if (this == &other)
    return *this;

  if (bytes)
    delete[] bytes;

  length = other.length;
  capacity = other.capacity;

  bytes = new unsigned char[other.capacity];
  memcpy(bytes, other.bytes, other.length);

  return *this;
}

const void* Bos::data() const { return bytes; }

uint32_t Bos::lengthInBytes() const { return length; }

void Bos::append(BosDataType type) {
  if (length == capacity)
    resize();
  bytes[length] = ((unsigned char*)&type)[0];
  ++length;
  setLengthBytes();
}

void Bos::append(unsigned char uc) {
  if (length == capacity)
    resize();
  bytes[length] = uc;
  ++length;
  setLengthBytes();
}

void Bos::append(const void* v, unsigned int size) {
  if ((length + size) > capacity)
    resizeToFit(size);
  length += size;
  memcpy(bytes, v, size);
  setLengthBytes();
}

void Bos::clear() {
  length = 4;
  setLengthBytes();
}

void Bos::setLengthBytes() { memcpy(bytes, &length, sizeof(uint32_t)); }

void Bos::resize() {
  uint32_t oldCapacity = capacity;
  capacity *= 2;
  unsigned char* newBytes = new unsigned char[capacity];
  memcpy(newBytes, bytes, oldCapacity);
  delete[] bytes;
  bytes = newBytes;
}
void Bos::resizeToFit(uint32_t additionalBytes) {
  uint32_t oldCapacity = capacity;
  uint32_t newLength = oldCapacity + additionalBytes;
  while (capacity < newLength) {
    capacity *= 2;
  }
  unsigned char* newBytes = new unsigned char[capacity];
  memcpy(newBytes, bytes, oldCapacity);
  delete[] bytes;
  bytes = newBytes;
}
} // namespace Json