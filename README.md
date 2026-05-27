# FlexibleDelay

一个轻量级、跨平台、支持 Tick 溢出的 C 语言延时模块。

适用于：

- STM32
- Linux
- Windows
- Bare Metal
- RTOS
- MCU 工程
- 嵌入式状态机
- 周期任务调度

该模块只依赖一个时间获取函数，不依赖任何操作系统。

---

# 特性

## 支持阻塞延时

```c
delay_ms(1000);
```

---

## 支持非阻塞延时

```c
delay_start(&tick, 3000);

if(delay_expired(&tick))
{
    printf("timeout\n");
}
```

---

## 支持周期定时器

```c
if(delay_period(&tick, 1000))
{
    printf("1 second\n");
}
```

---

## 自动处理 Tick 溢出

内部使用无符号减法进行时间比较：

```c
(now - start)
```

即使系统 Tick 溢出也不会影响定时结果。

适用于：

- 32bit Tick
- 64bit Tick
- FreeRTOS Tick
- HAL_GetTick()
- GetTickCount64()

---

# API 说明

---

# 1. delay_install

安装时间函数。

## 原型

```c
void delay_install(get_time_func_t func);
```

## 参数

| 参数 | 说明                 |
| ---- | -------------------- |
| func | 获取系统 Tick 的函数 |

## 示例

```c
size_t get_ms_ticks(void)
{
    return HAL_GetTick();
}

delay_install(get_ms_ticks);
```

---

# 2. delay_ms

阻塞延时。

## 原型

```c
void delay_ms(size_t ms);
```

## 参数

| 参数 | 说明             |
| ---- | ---------------- |
| ms   | 延时时间（毫秒） |

## 示例

```c
delay_ms(1000);
```

---

# 3. delay_start

启动一次性定时器。

## 原型

```c
void delay_start(size_t *tick, size_t timeout);
```

## 参数

| 参数    | 说明       |
| ------- | ---------- |
| tick    | 定时器变量 |
| timeout | 超时时间   |

## 示例

```c
size_t tick = 0;

delay_start(&tick, 5000);
```

---

# 4. delay_expired

检测一次性定时器是否超时。

超时后自动关闭定时器。

## 原型

```c
uint8_t delay_expired(size_t *tick);
```

## 返回值

| 返回值 | 说明   |
| ------ | ------ |
| 1      | 已超时 |
| 0      | 未超时 |

## 示例

```c
if(delay_expired(&tick))
{
    printf("timeout\n");
}
```

---

# 5. delay_period

周期定时器。

## 原型

```c
uint8_t delay_period(size_t *tick, size_t period);
```

## 参数

| 参数   | 说明       |
| ------ | ---------- |
| tick   | 定时器变量 |
| period | 周期       |

## 返回值

| 返回值 | 说明     |
| ------ | -------- |
| 1      | 周期到达 |
| 0      | 未到达   |

## 示例

```c
size_t tick = 0;

if(delay_period(&tick, 1000))
{
    printf("1 second\n");
}
```

---

# 6. delay_stop

关闭定时器。

## 原型

```c
void delay_stop(size_t *tick);
```

## 示例

```c
delay_stop(&tick);
```

---

# 7. delay_is_active

检测定时器是否启用。

## 原型

```c
uint8_t delay_is_active(size_t *tick);
```

## 返回值

| 返回值 | 说明   |
| ------ | ------ |
| 1      | 已启动 |
| 0      | 未启动 |

---

# 工作原理

---

# Tick 溢出安全

核心思想：

```c
(now - start) < timeout
```

利用无符号整数溢出特性实现安全比较。

例如：

```text
0xFFFFFFF0 + 100
```

溢出后仍可正确计算时间差。

---

# 周期定时器防累计误差

错误做法：

```c
tick = now + period;
```

正确做法：

```c
tick += period;
```

这样即使任务执行时间不稳定，也不会累计漂移。

---

# Windows 示例

```c
#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "flexible_delay.h"

size_t get_ms_ticks(void)
{
    return (size_t)GetTickCount64();
}

int main(void)
{
    size_t tick = 0;

    delay_install(get_ms_ticks);

    while (1)
    {
        if (delay_period(&tick, 1000))
        {
            printf("1 second\n");
        }

        delay_ms(10);
    }
}
```

---

# Windows 编译

## 环境

- GCC
- MinGW
- CMake

---

## 编译步骤

进入测试目录：

```bash
cd example/test
```

创建 build：

```bash
mkdir build
cd build
```

生成 Makefile：

```bash
cmake .. -G "MinGW Makefiles"
```

编译：

```bash
make
```

运行：

```bash
./test.exe
```

---

# STM32 使用示例

```c
#include "flexible_delay.h"

size_t get_ms_ticks(void)
{
    return HAL_GetTick();
}

int main(void)
{
    HAL_Init();

    delay_install(get_ms_ticks);

    size_t led_tick = 0;

    while (1)
    {
        if(delay_period(&led_tick, 500))
        {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }
    }
}
```

---

# 适用场景

- MCU 裸机开发
- 状态机超时控制
- 周期任务
- 软件定时器
- 按键消抖
- 通信超时
- LED 闪烁
- RTOS 辅助定时

---

# 设计目标

本模块追求：

- 简洁
- 零依赖
- 可移植
- Tick 溢出安全
- 小体积
- 易集成