#include "vectorSerialization.hpp"

#pragma deprecated()
JsonArray serializeVector(std::vector<String> &vector) {
    DynamicJsonDocument doc(1024);
    JsonArray arr = doc.to<JsonArray>();
    for (String &s : vector) {
        arr.add(s);
    }
    return arr;
}

std::vector<String> deserializeVector(JsonArray &array) {
    std::vector<String> vec = {};
    for (JsonVariant v : array) {
        vec.push_back(v.as<String>());
    }
    return vec;
}
