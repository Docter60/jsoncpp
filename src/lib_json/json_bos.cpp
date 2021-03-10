#include <json/bos.h>
#include <json/value.h>

namespace Json {
Bos::Bos() : length(4), capacity(256) {
  bytes = new unsigned char[capacity];
  setLengthBytes();
}

Bos::Bos(const void* data) : length(4), capacity(256) {
  memcpy(&length, data, sizeof(uint32_t));
  while (capacity < length) {
    capacity *= 2;
  }
  bytes = new unsigned char[capacity];
  memcpy(bytes, data, length);
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
  memcpy(&bytes[length], v, size);
  length += size;
  setLengthBytes();
}

void Bos::clear() {
  length = 4;
  setLengthBytes();
}

void Bos::deserialize(Value& root) {
  if (bytes[4] != (unsigned char)BosDataType::OBJ_T)
    return;
  root.clear();
  unsigned int i = 5;
  deserializeObject(root, i);
}

void Bos::deserializeArray(Value& root, unsigned int& i) {
  unsigned int count = deserializeUVarInt(i);
  for (unsigned int j = 0; j < count; ++j) {
    root.append(deserializeElement(i));
  }
}

void Bos::deserializeObject(Value& root, unsigned int& i) {
  unsigned int count = deserializeUVarInt(i);
  for (unsigned int j = 0; j < count; ++j) {
    std::string keyName = deserializeRawString(i);
    printf("Object key: %s\n", keyName.data());
    root[keyName] = deserializeElement(i);
  }
}

unsigned int Bos::deserializeUVarInt(unsigned int& i) {
  const char& b = bytes[i];
  ++i;
  if (b < 0xFD) {
    return b;
  } else {
    const char& b2 = bytes[i];
    if (b == 0xFD) {
      i += 2;
      return atoi(&b2) & 0x0000FFFF;
    } else if (b == 0xFE) {
      i += 4;
      return atoi(&b2);
    } else {
      i += 8;
      return atoll(&b2);
    }
  }
}

std::string Bos::deserializeRawString(unsigned int& i) {
  unsigned int count = deserializeUVarInt(i);
  unsigned int temp = i;
  i += count;
  return std::string((const char*)&bytes[temp], count);
}

Value Bos::deserializeElement(unsigned int& i) {
  BosDataType t = (BosDataType)bytes[i];
  printf("Position: %u    ", i);
  printf("0x%02x\n", bytes[i]);
  ++i;
  int temp = i;
  switch (t) {
  case BosDataType::NULL_T:
    return Value();
  case BosDataType::BOOL_T:
    ++i;
    return Value((bool)bytes[temp]);
  case BosDataType::INT8_T:
    ++i;
    return Value((int8_t)bytes[temp]);
  case BosDataType::INT16_T:
    i += 2;
    return Value(*((int16_t*)&bytes[temp]));
  case BosDataType::INT32_T:
    i += 4;
    return Value(*((int32_t*)&bytes[temp]));
  case BosDataType::INT64_T:
    i += 8;
    return Value(*((int64_t*)&bytes[temp]));
  case BosDataType::UINT8_T:
    ++i;
    return Value((uint8_t)bytes[temp]);
  case BosDataType::UINT16_T:
    i += 2;
    return Value(*((uint16_t*)&bytes[temp]));
  case BosDataType::UINT32_T:
    i += 4;
    return Value(*((uint32_t*)&bytes[temp]));
  case BosDataType::UINT64_T:
    i += 8;
    return Value(*((uint64_t*)&bytes[temp]));
  case BosDataType::FLOAT_T:
    i += 4;
    return Value(*((float*)&bytes[temp]));
  case BosDataType::DOUBLE_T:
    i += 8;
    return Value(*((double*)&bytes[temp]));
  case BosDataType::STRING_T:
    return Value(deserializeRawString(i));
  case BosDataType::BYTES_T:
    return Value(deserializeRawString(i));
  case BosDataType::ARRAY_T: {
    Value v;
    deserializeArray(v, i);
    return v;
  }
  case BosDataType::OBJ_T: {
    Value v;
    deserializeObject(v, i);
    return v;
  }
  default:
    return Value();
  }
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