/**
 * @brief Flexible delay module
 * 
 * @copyright Copyright (c) 2026 LiuJiashun. All rights reserved.
 * 
 * @attention :
 * Date           Author         Notes
 * 2026-05-27     Liu Jiashun    the first version
 */
#ifndef __FLEXIBLE_DELAY_H
#define __FLEXIBLE_DELAY_H

#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef size_t (*get_time_func_t)(void);

/* Initialize the delay module */
void delay_install(get_time_func_t func);

/* Blocking delay */
void delay_ms(size_t ms);

/* Non-blocking delay */
void delay_start(size_t *tick, size_t timeout);
void delay_stop(size_t *tick);
uint8_t delay_is_active(size_t *tick);
uint8_t delay_expired(size_t *tick);
uint8_t delay_period(size_t *tick, size_t period);

#if defined(__cplusplus)
}
#endif

#endif /* __FLEXIBLE_DELAY_H */