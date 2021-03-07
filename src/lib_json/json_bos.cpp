#include <json/bos.h>

namespace Json {
Bos::Bos() : bytes(4, 0) { bytes[0] = u8'\x04'; }

const void* Bos::data() const { return bytes.data(); }

size_t Bos::lengthInBytes() const { return bytes.size(); }
} // namespace Json