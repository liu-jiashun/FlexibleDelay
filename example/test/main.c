#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "flexible_delay.h"

enum
{
    STATE_WAIT,
    STATE_NEXT,
    STATE_MAX
};

uint8_t state = STATE_WAIT;
uint8_t count = 0;

size_t wait_tick = 0;
size_t test_tick = 0;

size_t get_ms_ticks(void)
{
    return (size_t)GetTickCount64();
}

int main(void)
{
    delay_install(get_ms_ticks);

    while (1)
    {
        switch (state)
        {
        case STATE_WAIT:
            if (!delay_is_active(&wait_tick))
            {
                delay_start(&wait_tick, 3000);
                printf("delay is active at %u\n", get_ms_ticks());
            }

            if (delay_expired(&wait_tick))
            {
                printf("delay expired at %u\n", get_ms_ticks());
                state = STATE_NEXT;
            }
            break;

        case STATE_NEXT:
            if (delay_period(&test_tick, 1000))
            {
                count++;
                if(count > 10)
                    return 0;
                printf("delay period at %u\n", get_ms_ticks());
            }
            break;
        }

        delay_ms(10);
    }
    return 0;
}