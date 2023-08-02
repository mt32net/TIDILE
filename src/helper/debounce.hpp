#pragma once


class Debouncer {
public:
    Debouncer(int pin, void (*func)());

    void setDebounceDelay(long delay);

    int getPin();

    void handle(int state);

private:
    void prepare();
    void (*func)();
    long delay = 100;
    int pin;

    long lastMillis = 0;
};