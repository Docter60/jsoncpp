#include <iostream>

#include <json/bos.h>
#include <json/json.h>

static void debugPrintBos(const Json::Bos& b) {
  unsigned char* uc = (unsigned char*)b.data();
  for (int i = 0; i < b.lengthInBytes(); ++i) {
    printf(" 0x%02X", uc[i]);
  }
}

int main() {
  Json::Value value;
  value["id"] = 1;
  value["method"] = "mining.subscribe";
  value["params"].append("docter60/firominer");

  // Used for determining int sizes in BOS
  Json::BosTemplate bTemplate;
  bTemplate["id"] = (int)Json::BosDataType::UINT8_T;

  Json::Bos bos;
  debugPrintBos(bos);

  return 0;
}