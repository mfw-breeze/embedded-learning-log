# 001 I2C 读 MPU6050，六个数据全显示成 65xxx

**日期**：2026-10-19
**环境**：STM32F103C8T6 最小系统板 / OLED 0.96" / 标准库 / Keil MDK 5.38 / 硬件 I2C2

## 现象

OLED 显示：

```
ID:74
+65478  +65506
+65528  +00027
+02246  +65533
```

配置是 `ACCEL_CONFIG=0x18`（±16g）、`GYRO_CONFIG=0x18`（±2000dps）。
静止时 Z 轴应约 2048、其余轴接近 0，但屏上出现了四个 6 万多的数。

## 排查过程

1. **先算量纲，不动硬件**：±16g 下 1g ≈ 2048，实测 2246 ≈ 1.1g（合理，芯片零偏），27 也合理（陀螺静止值）→ **数据本身是真的，不是通信失败**。
2. **找规律**：65478 / 65506 / 65528 / 65533 全部满足 `65536 - x`，且 x 只有 58 / 30 / 8 / 3 → 这是**负数补码被当成无符号数打印**的特征分布。
3. **定位到类型转换**：`MPU6050_GetData` 的参数和 `main.c` 的变量都是 `uint16_t`，而 `OLED_ShowSignedNum` 的形参是 `int32_t`。
   `uint16_t → int32_t` 是**零扩展**（高位补 0），0xFFE2 变成 0x0000FFE2 = 65506，而不是 -30。
4. **对照官方例程**：教程原版就是 `int16_t`，是我自己改成了 `uint16_t`。

## 根因

**整型提升的方向决定了符号位是否被保留**：

| 转换 | 行为 | 0xFFE2 的结果 |
|---|---|---|
| `uint16_t → int32_t` | 零扩展 | 65506 |
| `int16_t → int32_t` | 符号扩展 | -30 |

## 解决

三处统一改回 `int16_t`：`MPU6050_GetData` 的六个指针参数、函数内 `DataH/DataL`、`main.c` 的六个全局变量。

## 学到的

- **数据成批出现在 65536 附近（65 开头），几乎一定是符号位问题**，不是硬件问题；真通信失败读到的是全 0xFF 或全 0x00，不会"恰好等于某个小负数"。
- 读传感器前先算一遍物理量纲（1g 对应多少 LSB），比拿示波器快得多。
- 顺带发现 `WHO_AM_I` 读到 0x74 而非官方 0x68，说明买到的是兼容芯片 —— **不要用 ID 做"器件是否存在"的硬判断**。
- 量程与分辨率成反比：±16g 是 2048 LSB/g，±2g 是 16384 LSB/g。

## 延伸（还没做）

现在的 `GetData` 逐字节读 12 次事务，六个轴不是同一时刻的采样。应该从 0x3B 起一次连读 14 字节：

```c
void MPU6050_ReadBytes(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count)
{
    uint8_t i;
    I2C_GenerateSTART(I2C2, ENABLE);
    /* ... 等 EV5 → 地址+W → 等 EV6 → 发 RegAddress → 等 EV8_2 */
    I2C_GenerateSTART(I2C2, ENABLE);              /* 重复起始 */
    /* ... 地址+R → 等 EV6 */
    for (i = 0; i < Count; i++) {
        if (i == Count - 1) {                      /* 最后一个字节 */
            I2C_AcknowledgeConfig(I2C2, DISABLE);
            I2C_GenerateSTOP(I2C2, ENABLE);
        }
        while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
        DataArray[i] = I2C_ReceiveData(I2C2);
    }
    I2C_AcknowledgeConfig(I2C2, ENABLE);
}
```

## 对应面试题

- 有符号数和无符号数在类型提升时有什么区别？`uint16_t` 转 `int32_t` 会发生什么？
- 传感器读出来的数明显不对，你怎么判断是通信问题还是数据处理问题？
- 为什么加速度计静止时 Z 轴不是 0？
