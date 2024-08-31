#pragma once

class Mutex {
private:
    int mut = 1;
public:
    void lock();
    void unlock();
};