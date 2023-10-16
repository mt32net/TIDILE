#pragma once

#include <vector>
#include <ArduinoJson.h>

JsonArray serializeVector(std::vector<String> &vector);

std::vector<String> deserializeVector(JsonArray &array);