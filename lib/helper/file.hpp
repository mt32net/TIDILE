//
// Created by Tom Arlt on 31.08.24.
//

#pragma once
#include <LittleFS.h>

const char* readFile(fs::FS &fs, const char * path);

void writeFile(fs::FS &fs, const char * path, const char * message);
