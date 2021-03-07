#pragma once

#include <json/value.h>
#include <vector>

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

class JSON_API BosTemplate : public Value {};

using BosBuffer = std::vector<unsigned char>;

class JSON_API Bos {
  friend class Value;

public:
  Bos();

  const void* data() const;
  size_t lengthInBytes() const;

private:
  BosBuffer buf;
};
} // namespace Json