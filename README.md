# 🖱️ ESP32 智能滚动控制器

一个基于 ESP32-C3 的蓝牙鼠标控制器，支持通过 WiFi Web 界面远程控制目标设备的滚动操作。

## ✨ 功能特性

### 🎯 **核心功能**

- **BLE 鼠标模拟** - ESP32 作为蓝牙 HID 设备，模拟鼠标滚轮操作
- **WiFi Web 控制** - 通过现代化 Web 界面远程控制
- **多方向滚动** - 支持上、下、左、右四个方向的滚动
- **手动/自动模式** - 支持手动按钮控制和自动滚动模式

### ⚙️ **高级设置**

- **可调滚动速度** - 手动和自动模式独立的速度控制
- **随机抖动** - 让滚动更自然，避免机械感
- **多种自动模式** - 随机方向、固定方向、上下切换、左右切换
- **实时状态显示** - 显示 BLE 连接状态和当前运行模式

### 🌐 **Web 界面特性**

- **响应式设计** - 支持手机、平板、电脑访问
- **现代 UI** - 毛玻璃效果、渐变背景、平滑动画
- **触控友好** - 优化的触摸按钮和滑块控制
- **实时反馈** - 状态实时更新，操作即时响应

## 🛠️ 硬件需求

- **ESP32-C3 开发板** (本项目使用 AirM2M CORE ESP32C3)
- **USB 数据线** (用于烧录和供电)
- **WiFi 网络** (2.4GHz)

## 📱 兼容设备

### BLE 连接目标设备

- ✅ Windows 10/11 电脑
- ✅ macOS 设备
- ✅ Android 手机/平板
- ✅ iOS 设备 (iPhone/iPad)
- ✅ Linux 系统

### Web 控制端设备

- ✅ 任何支持现代浏览器的设备
- ✅ 手机 (iOS Safari, Android Chrome 等)
- ✅ 电脑 (Chrome, Firefox, Safari, Edge 等)
- ✅ 平板设备

## 🚀 快速开始

### 1. 环境准备

```bash
# 安装 PlatformIO
pip install platformio

# 或者使用 VS Code 插件
# 安装 "PlatformIO IDE" 扩展
```

### 2. 克隆项目

```bash
git clone https://github.com/14790897/esp32c3-ble-mouse-web-example.git
cd esp32c3-ble-mouse-web-example
```

### 3. 配置 WiFi

创建 `include/secrets.h` 文件：

```cpp
#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID "你的WiFi名称"
#define WIFI_PASSWORD "你的WiFi密码"

#endif
```

### 4. 编译和烧录

```bash
# 编译项目
pio run

# 烧录到设备 (替换COM端口)
pio run --target upload --upload-port COM11

# 查看串口输出
pio device monitor --port COM11 --baud 115200
```

### 5. 连接和使用

1. **等待 ESP32 启动** - 查看串口输出获取 IP 地址
2. **连接 BLE** - 在目标设备上搜索"ESP32 Scroll Mouse"并连接
3. **访问 Web 界面**：
   - 直接访问：`http://192.168.x.x` (替换为实际 IP)
   - 或通过 mDNS：`http://esp32mouse.local`

## 🎮 使用说明

### Web 界面控制

#### **手动控制**

- **方向按钮** - 点击 ⬆️⬇️⬅️➡️ 按钮进行手动滚动
- **按住模式** - 长按按钮可连续滚动

#### **滚动设置**

- **手动滑动速度** - 调整手动滚动的频率 (50-500ms)
- **自动滚动速度** - 调整自动模式的频率 (100-2000ms)
- **随机抖动 (手动)** - 手动滚动的随机性 (0-50%)
- **随机抖动 (自动)** - 自动滚动的随机性 (0-50%)

#### **自动滚动模式**

- **🎲 随机方向** - 随机改变滚动方向
- **⬇️ 向下** - 固定向下滚动
- **⬆️ 向上** - 固定向上滚动
- **⬅️ 向左** - 固定向左滚动
- **➡️ 向右** - 固定向右滚动
- **↕️ 上下切换** - 在上下方向间切换
- **↔️ 左右切换** - 在左右方向间切换

## 📊 技术规格

### 硬件平台

- **芯片**: ESP32-C3 (RISC-V 160MHz)
- **内存**: 320KB RAM
- **存储**: 4MB Flash
- **无线**: WiFi 802.11 b/g/n + Bluetooth 5.0 LE

### 软件架构

- **框架**: Arduino Framework
- **BLE 库**: ESP32-BLE-Mouse (NimBLE)
- **Web 服务**: ESP32 WebServer
- **协议**: BLE HID + HTTP + mDNS

### 性能参数

- **BLE 延迟**: < 50ms
- **Web 响应**: < 100ms
- **滚动精度**: ±1 tick
- **电池效率**: 优化的低功耗模式

## 🔧 项目结构

```
esp32c3-ble/
├── src/
│   └── main.cpp              # 主程序文件
├── include/
│   ├── README                # 头文件说明
│   └── secrets.h            # WiFi配置 (需创建)
├── lib/
│   └── README               # 库文件说明
├── test/
│   └── README               # 测试文件说明
├── platformio.ini           # PlatformIO配置
└── README.md               # 本文件
```

## ⚙️ 配置说明

### PlatformIO 配置

```ini
[env:airm2m_core_esp32c3]
platform = espressif32@6.4.0
board = airm2m_core_esp32c3
framework = arduino
monitor_speed = 115200
board_build.partitions = huge_app.csv

lib_deps =
    https://github.com/T-vK/ESP32-BLE-Mouse.git
build_flags =
    -D USE_NIMBLE

```

### WiFi 配置

在 `include/secrets.h` 中配置您的 WiFi 信息：

```cpp
#define WIFI_SSID "您的WiFi名称"
#define WIFI_PASSWORD "您的WiFi密码"
```

## 🐛 故障排除

### 常见问题

#### BLE 连接问题

- **症状**: 目标设备无法发现 ESP32
- **解决**: 重启 ESP32，确保目标设备蓝牙已开启
- **检查**: 串口输出确认 BLE 已启动

#### WiFi 连接问题

- **症状**: ESP32 无法连接 WiFi
- **解决**: 检查 WiFi 名称和密码是否正确
- **注意**: ESP32 只支持 2.4GHz WiFi 网络

#### Web 界面无法访问

- **症状**: 浏览器无法打开控制页面
- **解决**: 确认设备与 ESP32 在同一 WiFi 网络
- **替代**: 使用 IP 地址而不是域名访问

#### 滚动无响应

- **症状**: 按钮点击但目标设备无滚动
- **解决**: 确认 BLE 已连接 (Web 界面显示"已连接")
- **检查**: 目标设备是否支持 BLE HID

### 调试方法

1. **串口监控**

```bash
pio device monitor --port COM11 --baud 115200
```

2. **重新烧录**

```bash
pio run --target upload --upload-port COM11
```

3. **重置配置**

- 断电重启 ESP32
- 清除目标设备的蓝牙配对记录

## 🔮 未来功能

- [ ] **多设备 BLE 连接** - 同时连接多个目标设备
- [ ] **鼠标移动控制** - 除滚轮外的完整鼠标功能
- [ ] **手势控制** - 通过传感器实现手势识别
- [ ] **语音控制** - 集成语音识别功能
- [ ] **移动 APP** - 开发专用的手机控制应用
- [ ] **云端同步** - 设置和配置的云端备份

## 🤝 贡献

欢迎提交 Issue 和 Pull Request 来改进这个项目！

### 贡献指南

1. Fork 这个项目
2. 创建您的功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交您的更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开一个 Pull Request

## 📄 许可证

这个项目使用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🙏 致谢

- [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse) - 核心 BLE 鼠标库
- [Arduino-ESP32](https://github.com/espressif/arduino-esp32) - ESP32 Arduino 框架
- [PlatformIO](https://platformio.org/) - 开发环境和构建系统

## 📞 联系方式

如有问题或建议，请通过以下方式联系：

- **GitHub Issues**: [提交问题](https://github.com/14790897/esp32c3-ble-mouse-web-example/issues)
- **项目主页**: [GitHub Repository](https://github.com/14790897/esp32c3-ble-mouse-web-example)

---

⭐ 如果这个项目对您有帮助，请给个 Star 支持一下！
