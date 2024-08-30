//
// Created by Tom Arlt on 31.08.24.
//

#include "file.hpp"
#include <Arduino.h>


const char* readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return nullptr;
    }

    size_t fileSize = file.size();
    char* buf = (char*) malloc(fileSize + 1);
    char* writer = buf;

    Serial.println("- read from file:");
    while(file.available()){
        char c = file.read();
        *writer = c;
        writer++;
    }
    file.close();
    *writer = '\0';
    return buf;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}
