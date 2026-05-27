/**
 * @brief Flexible delay module
 * 
 * @copyright Copyright (c) 2026 LiuJiashun. All rights reserved.
 * 
 * @attention :
 * Date           Author         Notes
 * 2026-05-27     Liu Jiashun    the first version
 */
#include "flexible_delay.h"

static get_time_func_t get_time_func = NULL;

/**
 * @brief Initialize the delay module with a time function
 * 
 * @param func 
 */
void delay_install(get_time_func_t func)
{
    get_time_func = func;
}

/**
 * @brief Get the now tick
 * 
 * @return size_t 
 */
static inline size_t get_now(void)
{
    return get_time_func ? get_time_func() : 0;
}

/**
 * @brief Blocking delay for a specified number of milliseconds
 * 
 * @param ms 
 */
void delay_ms(size_t ms)
{
    if (!get_time_func || ms == 0)
        return;

    size_t start_time = get_now();

    /**
     * By using unsigned subtraction, 
     * the difference can be accurately calculated even if start_time overflows.
     */
    while ((size_t)(get_now() - start_time) < ms)
    { 
        /* Busy wait */
    }
}

/**
 * @brief Start one-time/cycle timing
 * 
 * @param tick 
 * @param timeout 
 */
void delay_start(size_t *tick, size_t timeout)
{
    if (!tick)
        return;
    *tick = get_now() + timeout;
}

/**
 * @brief Stop the timer (treat setting the tick to 0 as turning it off)
 * 
 * @param tick 
 */
void delay_stop(size_t *tick)
{
    if (!tick)
        return;
    *tick = 0;
}

/**
 * @brief Check if a timer is active
 * 
 * @param tick 
 * @return true 
 * @return false 
 */
uint8_t delay_is_active(size_t *tick)
{
    if (!tick)
        return 0;
    return *tick == 0 ? 0 : 1;
}

/**
 * @brief Return true upon expiration and automatically shut down the timer
 * 
 * @param tick 
 * @return true 
 * @return false 
 */
uint8_t delay_expired(size_t *tick)
{
    if (!tick || *tick == 0)
        return 0;

    /**
     * Use the positive or negative sign of (*tick - now) to perform the wrap-around determination
     * When now reaches or exceeds the *tick, 
     * the result of (*tick - get_now()) will become a large negative number at the high end.
     */
    if ((size_t)(*tick - get_now()) > (size_t)(~0UL >> 1))
    {
        /**
         * Automatically shuts down after a single trigger.
         */
        *tick = 0;
        return 1;
    }

    return 0;
}

/**
 * @brief Check if a periodic timer has expired
 * 
 * @param tick 
 * @param period 
 * @return true 
 * @return false 
 */
uint8_t delay_period(size_t *tick, size_t period)
{
    if (!tick || period == 0)
        return 0;

    /**
     * If the timer is not initialized or has been stopped, 
     * it will be activated for the first time based on the current time.
     */
    if (*tick == 0)
    {
        *tick = get_now() + period;
        return 0;
    }

    if ((size_t)(*tick - get_now()) > (size_t)(~0UL >> 1))
    {
        /**
         * Prevent the accumulation of errors multiple times
         */
        *tick += period;
        return 1;
    }

    return 0;
}



