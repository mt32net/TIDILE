#include "debounce.hpp"

#include <rom/gpio.h>

#include "Arduino.h"
#include "vector"

std::vector<Debouncer *> debouncers = std::vector<Debouncer *>();
QueueHandle_t interruptQueue;
bool queueCreated = false;

void debouncer_handler_isr(void *args)
{
    int pin = (int) args;
    xQueueSendFromISR(interruptQueue, &pin, NULL);
}

void debounce_handler(void *args)
{
    int pin = 0;
    while (true)
    {
        if (xQueueReceive(interruptQueue, &pin, portMAX_DELAY))
        {
            for (auto deb : debouncers)
            {
                if (deb == nullptr)
                    continue;
                if (deb->getPin() == pin)
                {
                    deb->handle(0);
                    break;
                }
            }
        }
    }
}


Debouncer::Debouncer(int pin, const std::function<void()> &func){
    this->pin = pin;
    this->func = func;

    prepare();
}

void Debouncer::prepare(){
    if (!queueCreated)
    {
        interruptQueue = xQueueCreate(20, sizeof(int));
        xTaskCreate(debounce_handler, "DebounceHandler", 20248, nullptr, 1, nullptr);
        queueCreated = true;
    }

    debouncers.push_back(this);

    gpio_pad_select_gpio(pin);
    gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
    gpio_pulldown_en((gpio_num_t)pin);
    gpio_pullup_dis((gpio_num_t)pin);
    gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_POSEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add((gpio_num_t)pin, debouncer_handler_isr, (void *)pin);
}

void Debouncer::setDebounceDelay(long delay)
{
    this->delay = delay;
}

int Debouncer::getPin()
{
    return pin;
}

void Debouncer::handle(int state)
{
    unsigned long now = millis();
    if(lastMillis + delay < now){
        this->func();
    }
    lastMillis = now;
}