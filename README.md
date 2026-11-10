# 嵌入式学习记录

> 27 届本科生 · 2026 年 7 月开始自学嵌入式 · 目标 2027 年春招嵌入式软件（MCU 方向）
>
> 这里记录我从零学 STM32 的全过程：每个外设的**可运行 demo**、踩过的**坑**、以及被整理成面试问答的**笔记**。

## 技术栈

<img alt="C" src="https://img.shields.io/badge/C-00599C?style=flat-square">
<img alt="STM32" src="https://img.shields.io/badge/STM32-F103-03234B?style=flat-square">
<img alt="StdPeriph" src="https://img.shields.io/badge/STM32-StdPeriph_Lib-03234B?style=flat-square">
<img alt="Keil" src="https://img.shields.io/badge/Keil-MDK5-1B9E48?style=flat-square">
<img alt="I2C" src="https://img.shields.io/badge/Protocol-I2C-4B8BBE?style=flat-square">
<img alt="SPI" src="https://img.shields.io/badge/Protocol-SPI-4B8BBE?style=flat-square">

## 这个仓库有什么

| 目录 | 内容 |
|---|---|
| [`demos/`](demos/) | 34 个可独立编译的 Keil 工程，一个外设一个 |
| [`notes/`](notes/) | 外设与协议学习笔记（重点在“面试可能问什么”） |
| [`pitfalls/`](pitfalls/) | **踩坑记录 —— 这个仓库最有价值的部分** |
| [`resources.md`](resources.md) | 手册、工具、调试命令、资料 |

## 学习进度

| 时间 | 内容 | 状态 |
|---|---|---|
| 2026.07 | C 语言基础 | ✅ |
| 2026.08 | STM32 工程结构 / Keil / 标准库模板 | ✅ |
| 2026.09 | GPIO / EXTI / 按键 / OLED | ✅ |
| 2026.10 | TIM / PWM / 输入捕获 / ADC / DMA | ✅ |
| 2026.11 | USART / 软件+硬件 I2C、SPI / 看门狗 / 内部 Flash | ✅ |
| 2026.11—12 | FreeRTOS | ⬜ |
| 2026.12—2027.01 | CAN / IAP / 工业测控终端项目 | ⬜ |

> 图例：✅ 已完成 · 🚧 进行中 · ⬜ 计划中

## demos 一览（34 个）

### 基础

| 目录 | 内容 |
|---|---|
| [00-project-template](demos/00-project-template/) | 标准库工程模板（后面所有 demo 的骨架） |
| [01-blink-led](demos/01-blink-led/) | GPIO 输出，LED 闪烁 |
| [02-led-marquee](demos/02-led-marquee/) | 流水灯 |
| [03-buzzer](demos/03-buzzer/) | 蜂鸣器 |
| [04-key-led](demos/04-key-led/) | 按键控制 LED（消抖） |
| [05-ldr-buzzer](demos/05-ldr-buzzer/) | 光敏传感器控制蜂鸣器 |
| [06-oled](demos/06-oled/) | OLED 驱动（I2C 软件模拟） |
| [07-ir-counter](demos/07-ir-counter/) | 对射式红外传感器计次 |
| [08-rotary-encoder](demos/08-rotary-encoder/) | 旋转编码器 |

### 定时器

| 目录 | 内容 |
|---|---|
| [09-tim-interrupt](demos/09-tim-interrupt/) | 定时器定时中断 |
| [10-tim-exti](demos/10-tim-exti/) | 定时器外部中断计数 |
| [11-pwm-breathing](demos/11-pwm-breathing/) | PWM 驱动 LED 呼吸灯 |
| [12-pwm-servo](demos/12-pwm-servo/) | PWM 驱动舵机 |
| [13-input-capture](demos/13-input-capture/) | 输入捕获测频率 |
| [14-pwm-input-duty](demos/14-pwm-input-duty/) | PWM 输入模式测频率 + 占空比 |

### 采集

| 目录 | 内容 |
|---|---|
| [15-adc-single](demos/15-adc-single/) | ADC 单通道 |
| [16-adc-multi](demos/16-adc-multi/) | ADC 多通道 |
| [17-ldr-pwm-adc](demos/17-ldr-pwm-adc/) | 光敏 + ADC + PWM 闭环调光 |
| [18-dma-transfer](demos/18-dma-transfer/) | DMA 数据转运 |
| [19-dma-adc](demos/19-dma-adc/) | DMA + ADC 多通道 |

### 通信

| 目录 | 内容 |
|---|---|
| [20-usart-tx](demos/20-usart-tx/) | 串口发送 + printf 重定向 |
| [21-usart-rx](demos/21-usart-rx/) | 串口发送 + 接收 |
| [22-usart-hex-packet](demos/22-usart-hex-packet/) | 收发 HEX 数据包（状态机解析） |
| [23-usart-text-packet](demos/23-usart-text-packet/) | 收发文本数据包 |
| [24-softi2c-mpu6050](demos/24-softi2c-mpu6050/) | 软件 I2C 读写 MPU6050 |
| [25-hwi2c-mpu6050](demos/25-hwi2c-mpu6050/) | **硬件 I2C** 读写 MPU6050 |
| [26-softspi-w25q64](demos/26-softspi-w25q64/) | 软件 SPI 读写 W25Q64 |
| [27-hwspi-w25q64](demos/27-hwspi-w25q64/) | **硬件 SPI** 读写 W25Q64 |

### 存储与可靠性

| 目录 | 内容 |
|---|---|
| [28-iwdg](demos/28-iwdg/) | 独立看门狗 IWDG |
| [29-wwdg](demos/29-wwdg/) | 窗口看门狗 WWDG |
| [30-internal-flash](demos/30-internal-flash/) | 读写 STM32 内部 Flash |

### 综合

| 目录 | 内容 |
|---|---|
| [31-smart-fire-system](demos/31-smart-fire-system/) | 智能消防检测喷水系统（火焰 + 温度 + 水泵联动） |

## 踩坑记录 Top 2

> 面试最常问“你遇到过什么困难”，这些就是我的答案。

| # | 问题 | 根因 | 耗时 |
|---|---|---|---|
| [001](pitfalls/001-i2c-data-65xxx.md) | I2C 读 MPU6050，六个数据全显示成 65xxx | 参数写成 `uint16_t`，0xFFE2 被零扩展成 65506 | 2h |
| [002](pitfalls/002-spi-write-read-mismatch.md) | SPI 写 W25Q64 “写正常、读不对” | `ReadID` 漏了 `MySPI_Stop()`，CS 没释放导致擦除被静默忽略 | 4h |

## 怎么编译

- **IDE**：Keil MDK 5.x
- **库**：STM32F10x 标准外设库（每个 demo 自带 `Library/` + `Start/`，可独立编译，不依赖外部模板）
- **操作**：打开 `demos/<name>/Project/*.uvprojx` → Build → 下载

> 仓库已排除全部编译产物（`Objects/`、`*.o`、`*.axf`、`*.hex` 等），克隆后**需要自己 Build 一次**。
> 如果开关灯类 demo 的 `Objects/` 目录不存在，Keil 会自动创建，属正常现象。

## 相关仓库

- 🚧 **stm32-industrial-terminal** —— 基于 STM32F103 + FreeRTOS 的工业测控终端（Modbus RTU / CAN 多节点 / IAP 升级），2026.11 起
