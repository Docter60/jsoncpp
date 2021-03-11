#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include <json/bos.h>
#include <json/json.h>

static void debugPrintBos(const Json::Bos& b) {
  unsigned char* uc = (unsigned char*)b.data();
  uint32_t size = b.lengthInBytes();
  printf("BOS size in bytes: %u\nBOS data: ", size);
  for (uint32_t i = 0; i < size; ++i) {
    printf(" 0x%02X", uc[i]);
  }
  printf("\n");
}

int main() {
  Json::Value value;
  value["null"] = Json::Value();
  value["bool"] = true;
  value["int8"] = 8;
  value["int16"] = 16;
  value["int32"] = 32;
  value["int64"] = 64ll;
  value["uint8"] = 9u;
  value["uint16"] = 17u;
  value["uint32"] = 33u;
  value["uint64"] = 65ull;
  value["float"] = M_PI;
  value["double"] = 2.0 * M_PI;
  value["string"] = "Hello BOS!";
  value["bytes"] = "abcdefg";
  value["array"].append("This is a string in an array");
  value["array"].append(17);
  value["array"].append(Json::Value());
  value["object"] = Json::Value(Json::objectValue);
  value["object"]["test"] = 42;

  // Used for determining int sizes in BOS
  Json::Value bTemplate;
  bTemplate["null"] = (int)Json::BosDataType::NULL_T;
  bTemplate["bool"] = (int)Json::BosDataType::BOOL_T;
  bTemplate["int8"] = (int)Json::BosDataType::INT8_T;
  bTemplate["int16"] = (int)Json::BosDataType::INT16_T;
  bTemplate["int32"] = (int)Json::BosDataType::INT32_T;
  bTemplate["int64"] = (int)Json::BosDataType::INT64_T;
  bTemplate["uint8"] = (int)Json::BosDataType::UINT8_T;
  bTemplate["uint16"] = (int)Json::BosDataType::UINT16_T;
  bTemplate["uint32"] = (int)Json::BosDataType::UINT32_T;
  bTemplate["uint64"] = (int)Json::BosDataType::UINT64_T;
  bTemplate["float"] = (int)Json::BosDataType::FLOAT_T;
  bTemplate["double"] = (int)Json::BosDataType::DOUBLE_T;
  bTemplate["string"] = (int)Json::BosDataType::STRING_T;
  bTemplate["bytes"] = (int)Json::BosDataType::BYTES_T;
  bTemplate["array"].append((int)Json::BosDataType::STRING_T);
  bTemplate["array"].append((int)Json::BosDataType::INT32_T);
  bTemplate["array"].append((int)Json::BosDataType::NULL_T);
  bTemplate["object"] = Json::Value(Json::objectValue);
  bTemplate["object"]["test"] = (int)Json::BosDataType::INT32_T;

  Json::Bos bos;
  debugPrintBos(bos);
  value.serialize(bos, bTemplate);
  debugPrintBos(bos);
  
  Json::Value v;
  bos.deserialize(v);

  Json::StreamWriterBuilder swb;
  swb["indentation"] = "    ";
  std::string document = Json::writeString(swb, v);
  std::cout << "Deserialized BOS: " << std::endl << document << std::endl;
  
  return 0;
}