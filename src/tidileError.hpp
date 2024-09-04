//
// Created by Tom Arlt on 05.09.24.
//

#pragma once

void tidile_error(int errorCode, const char *message);

#define FILESYSTEM_INIT_ERROR tidile_error(1, "Error starting SPIFFS.")