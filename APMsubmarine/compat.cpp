#include "Submarine.h"

void Submarine::delay(uint32_t ms)
{
    hal.scheduler->delay(ms);
}

void Submarine::mavlink_delay(uint32_t ms)
{
    hal.scheduler->delay(ms);
}

uint32_t Submarine::millis()
{
    return hal.scheduler->millis();
}

uint32_t Submarine::micros()
{
    return hal.scheduler->micros();
}
