#include "mutex.hpp"

void Mutex::lock() {
    while(mut == 0);
    mut--;
}

void Mutex::unlock() {
    mut++;
}