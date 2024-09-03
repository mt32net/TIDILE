#pragma once
#include <functional>

class Debouncer {
public:
    Debouncer(int pin, const std::function<void()> &func);

    void setDebounceDelay(long delay);

    int getPin();

    void handle(int state);

private:
    void prepare();
    std::function<void()> func;
    long delay = 100;
    int pin;

    unsigned long lastMillis = 0;
};