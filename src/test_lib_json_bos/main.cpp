#include <iostream>

#include <json/bos.h>
#include <json/json.h>

static void debugPrintBos(const Json::Bos& b) {
  unsigned char* uc = (unsigned char*)b.data();
  uint32_t size = b.lengthInBytes();
  printf("BOS size in bytes: %u", size);
  for (uint32_t i = 0; i < size; ++i) {
    printf(" 0x%02X", uc[i]);
  }
  printf("\n");
}

int main() {
  Json::Value value;
  value["id"] = 1u;
  value["method"] = "method.parameter";
  value["params"].append("docter60/userAgent");

  // Used for determining int sizes in BOS
  Json::Value bTemplate;
  bTemplate["id"] = (int)Json::BosDataType::UINT8_T;
  bTemplate["method"] = (int)Json::BosDataType::STRING_T;
  bTemplate["params"].append((int)Json::BosDataType::STRING_T);
  bTemplate["params"].append((int)Json::BosDataType::BYTES_T);

  Json::Bos bos;
  debugPrintBos(bos);
  value.serialize(bos, bTemplate);
  debugPrintBos(bos);

  return 0;
}