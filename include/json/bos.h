#pragma once

#include "forwards.h"

namespace Json {
enum class JSON_API BosDataType {
  NULL_T = 0x00,
  BOOL_T = 0x01,
  INT8_T = 0x02,
  INT16_T = 0x03,
  INT32_T = 0x04,
  INT64_T = 0x05,
  UINT8_T = 0x06,
  UINT16_T = 0x07,
  UINT32_T = 0x08,
  UINT64_T = 0x09,
  FLOAT_T = 0x0A,
  DOUBLE_T = 0x0B,
  STRING_T = 0x0C,
  BYTES_T = 0x0D,
  ARRAY_T = 0x0E,
  OBJ_T = 0x0F
};

class JSON_API Bos {
  friend class Value;

public:
  Bos();
  Bos(const void* data);
  Bos(const Bos& other);
  virtual ~Bos();

  Bos& operator=(const Bos& other);

  const void* data() const;
  uint32_t lengthInBytes() const;

  void append(BosDataType type);
  void append(unsigned char uc);
  void append(const void* v, unsigned int size);

  void clear();

  void deserialize(Value& root);

private:
  void deserializeArray(Value& root, unsigned int& i);
  void deserializeObject(Value& root, unsigned int& i);
  size_t deserializeUVarInt(unsigned int& i);
  std::string deserializeRawString(unsigned int& i);
  Value deserializeElement(unsigned int& i);
  void setLengthBytes();
  void resize();
  void resizeToFit(uint32_t additionalBytes);

  unsigned char* bytes;
  uint32_t length; // maybe remove this field because of redundancy
  uint32_t capacity;
};
} // namespace Json