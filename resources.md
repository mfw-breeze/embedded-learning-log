# 资源与工具

## 手册（必存本地）

| 文档 | 用途 |
|---|---|
| RM0008（STM32F10x 参考手册） | 外设寄存器，中文版可搜"STM32F10x 参考手册 中文" |
| STM32F103x8/B 数据手册 | 引脚、电气特性、Flash/RAM 大小 |
| MPU6050 寄存器手册（RM-MPU-6000A） | 传感器寄存器与量程 |
| W25Q64FV 数据手册 | SPI Flash 命令集与擦写时序 |
| Cortex-M3 权威指南 | 内核、异常、中断 |

## 工具链

| 工具 | 用途 |
|---|---|
| Keil MDK 5.x | 编译 + 下载 |
| ST-Link V2 + STM32CubeProgrammer | 下载、读保护、批量擦除 |
| 逻辑分析仪（8 通道 24MHz 起） | 抓 I2C / SPI 波形，排查通信问题的第一神器 |
| 万用表 | 量供电、共地、SWD 电平 |
| SerialPlot / VOFA+ | 串口数据可视化（调 PID、看波形） |

## 常用调试命令 / 技巧

```
# 确认开发板是否被识别（ST-Link）
STM32CubeProgrammer → Connect under Reset

# 串口抓包（Windows，需先装驱动）
putty / MobaXterm → 115200 8N1
```

- **connect under reset**：程序里进了 Stop/Standby 模式导致 SWD 掉线时用
- **逻辑分析仪触发**：触发条件设在 **CS 下降沿**（SPI）或 **START 条件**（I2C），一次就能看到整条命令

## GitHub 搜索配方（找参考实现用）

| 查询 | 用途 |
|---|---|
| `topic:stm32 language:C stars:>50` | 高质量 STM32 仓库 |
| `freemodbus language:C stars:>50` | Modbus 协议栈参考（BSD 许可，可放心参考） |
| `IAP bootloader stm32 language:C stars:>20 pushed:>2025-01-01` | Bootloader 实现 |
| `"MODBUS_RTU" path:*.c language:C` | 具体收发实现 |
| `is:issue "HardFault"` | 搜报错原文找同类坑 |

**注意事项**
- GitHub 代码搜索**必须登录**，且**不支持完全匹配/正则**
- 抄之前先看 `LICENSE`：**GPL 有传染性，不能抄进公开项目仓**；无 LICENSE 视为保留所有权利
- 搜索引擎名比自然语言有效（搜 `HAL_I2C_Mem_Read` 而不是"如何读 I2C"）

## 学习资料

- 江协科技 STM32 入门教程（外设入门，本仓库 demos 与之对应）
- 《嵌入式 C 语言自我修养》—— 讲清 `volatile`、位域、链接脚本
- FreeRTOS 官方文档 + Mastering the FreeRTOS Real Time Kernel
