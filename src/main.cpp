#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BleMouse.h>
#include <ESPmDNS.h>
#include "secrets.h"

// Web 服务器
WebServer server(80);

// BLE 鼠标
BleMouse bleMouse("ESP32 Scroll Mouse", "MyCompany", 100);

// 改进的 HTML 页面
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 智能滚动控制器</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            color: white;
        }
        .container {
            background: rgba(255, 255, 255, 0.15);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
            border: 1px solid rgba(255, 255, 255, 0.2);
            text-align: center;
            max-width: 400px;
            width: 90%;
        }
        h1 {
            margin-bottom: 20px;
            font-size: 24px;
            font-weight: 300;
        }
        .status {
            background: rgba(255, 255, 255, 0.2);
            border-radius: 15px;
            padding: 15px;
            margin-bottom: 25px;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }
        .status-item {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin: 8px 0;
        }
        .status-label {
            font-weight: 500;
        }
        .status-value {
            padding: 4px 12px;
            border-radius: 20px;
            font-size: 12px;
            font-weight: 600;
        }
        .connected {
            background: #00c851;
            color: white;
        }
        .disconnected {
            background: #ff4444;
            color: white;
        }
        .controls {
            margin: 25px 0;
        }
        .control-row {
            display: flex;
            justify-content: center;
            margin: 10px 0;
            gap: 10px;
        }
        .btn {
            width: 80px;
            height: 80px;
            border: none;
            border-radius: 15px;
            font-size: 24px;
            cursor: pointer;
            transition: all 0.3s ease;
            background: rgba(255, 255, 255, 0.2);
            color: white;
            border: 2px solid rgba(255, 255, 255, 0.3);
            display: flex;
            align-items: center;
            justify-content: center;
            user-select: none;
        }
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.3);
            background: rgba(255, 255, 255, 0.3);
        }
        .btn:active {
            transform: translateY(0);
        }
        .btn.up { border-color: #4CAF50; }
        .btn.down { border-color: #f44336; }
        .btn.left { border-color: #2196F3; }
        .btn.right { border-color: #FF9800; }
        
        .auto-mode {
            margin-top: 25px;
            padding: 20px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        .auto-controls {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 15px;
            margin: 15px 0;
        }
        .toggle-btn {
            padding: 10px 20px;
            border: none;
            border-radius: 25px;
            font-size: 14px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
        }
        .toggle-btn.start {
            background: #00c851;
            color: white;
        }
        .toggle-btn.stop {
            background: #ff4444;
            color: white;
        }
        .speed-control {
            display: flex;
            align-items: center;
            gap: 10px;
            margin: 15px 0;
        }
        .speed-slider {
            flex: 1;
            height: 6px;
            border-radius: 3px;
            background: rgba(255, 255, 255, 0.3);
            outline: none;
            -webkit-appearance: none;
        }
        .speed-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: white;
            cursor: pointer;
        }
        .auto-status {
            font-size: 14px;
            margin-top: 10px;
            padding: 8px;
            border-radius: 10px;
            background: rgba(255, 255, 255, 0.1);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🖱️ ESP32 智能滚动控制器</h1>
        
        <div class="status">
            <div class="status-item">
                <span class="status-label">BLE 状态:</span>
                <span class="status-value" id="bleStatus">检查中...</span>
            </div>
            <div class="status-item">
                <span class="status-label">WiFi 状态:</span>
                <span class="status-value connected">已连接</span>
            </div>
        </div>

        <div class="controls">
            <div class="control-row">
                <button class="btn up" onmousedown="startScroll('up')" onmouseup="stopScroll()">⬆️</button>
            </div>
            <div class="control-row">
                <button class="btn left" onmousedown="startScroll('left')" onmouseup="stopScroll()">⬅️</button>
                <button class="btn right" onmousedown="startScroll('right')" onmouseup="stopScroll()">➡️</button>
            </div>
            <div class="control-row">
                <button class="btn down" onmousedown="startScroll('down')" onmouseup="stopScroll()">⬇️</button>
            </div>
        </div>

        <div class="auto-mode">
            <h3>🤖 自动滚动模式</h3>
            <div class="auto-controls">
                <button class="toggle-btn start" id="autoBtn" onclick="toggleAuto()">启动自动滚动</button>
            </div>
            <div class="speed-control">
                <span>速度:</span>
                <input type="range" class="speed-slider" id="speedSlider" min="100" max="10000" value="500">
                <span id="speedValue">500ms</span>
            </div>
            <div class="auto-status" id="autoStatus">自动模式: 关闭</div>
        </div>
    </div>

    <script>
        let scrollInterval = null;
        let autoInterval = null;
        let isAutoMode = false;
        let currentAutoDirection = 'down';

        function startScroll(dir) {
            if (isAutoMode) return;
            scrollInterval = setInterval(() => {
                fetch('/scroll?dir=' + dir);
            }, 100);
        }

        function stopScroll() {
            if (scrollInterval) {
                clearInterval(scrollInterval);
                scrollInterval = null;
            }
        }

        function toggleAuto() {
            const btn = document.getElementById('autoBtn');
            const status = document.getElementById('autoStatus');
            
            if (!isAutoMode) {
                // 启动自动模式
                isAutoMode = true;
                btn.textContent = '停止自动滚动';
                btn.className = 'toggle-btn stop';
                startAutoScroll();
                status.textContent = '自动模式: 运行中 - ' + currentAutoDirection;
            } else {
                // 停止自动模式
                isAutoMode = false;
                btn.textContent = '启动自动滚动';
                btn.className = 'toggle-btn start';
                stopAutoScroll();
                status.textContent = '自动模式: 关闭';
            }
        }

        function startAutoScroll() {
            const speed = document.getElementById('speedSlider').value;
            autoInterval = setInterval(() => {
                fetch('/scroll?dir=' + currentAutoDirection);
                // 每10次滚动后随机改变方向
                if (Math.random() < 0.1) {
                    const directions = ['up', 'down', 'left', 'right'];
                    currentAutoDirection = directions[Math.floor(Math.random() * directions.length)];
                    document.getElementById('autoStatus').textContent = '自动模式: 运行中 - ' + currentAutoDirection;
                }
            }, parseInt(speed));
        }

        function stopAutoScroll() {
            if (autoInterval) {
                clearInterval(autoInterval);
                autoInterval = null;
            }
        }

        function checkStatus() {
            fetch('/status').then(r => r.text()).then(data => {
                const statusElement = document.getElementById('bleStatus');
                statusElement.textContent = data;
                statusElement.className = 'status-value ' + (data.includes('已连接') ? 'connected' : 'disconnected');
            });
        }

        // 速度滑块更新
        document.getElementById('speedSlider').addEventListener('input', function() {
            document.getElementById('speedValue').textContent = this.value + 'ms';
            if (isAutoMode) {
                stopAutoScroll();
                startAutoScroll();
            }
        });

        // 防止触摸设备上的长按菜单
        document.addEventListener('contextmenu', e => e.preventDefault());
        
        // 定期检查状态
        setInterval(checkStatus, 3000);
        checkStatus();
    </script>
</body>
</html>
)rawliteral";

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 WiFi + BLE Mouse 启动中...");

    // 启动 BLE 鼠标
    Serial.println("初始化 BLE 鼠标...");
    bleMouse.begin();
    Serial.println("BLE 鼠标已启动");

    // 延迟一下，避免冲突
    delay(1000);

    // 连接到 WiFi 网络
    Serial.println("连接到 WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    // 等待连接
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi 连接成功");
        Serial.print("IP 地址: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println();
        Serial.println("WiFi 连接失败");
        return; // 如果WiFi连接失败，停止setup
    }

    // 启动 mDNS 服务
    Serial.println("启动 mDNS...");
    if (MDNS.begin("esp32mouse")) {
        Serial.println("mDNS 启动成功");
        Serial.println("可通过 http://esp32mouse.local 访问");
        // 添加服务
        MDNS.addService("http", "tcp", 80);
    } else {
        Serial.println("mDNS 启动失败");
    }

    // 设置网页路由
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", htmlPage);
    });

    // 滚动控制接口
    server.on("/scroll", HTTP_GET, []() {
        String direction = server.arg("dir");
        
        if (bleMouse.isConnected()) {
            if (direction == "up") {
                bleMouse.move(0, 0, 1);
                Serial.println("向上滚动");
            }
            else if (direction == "down") {
                bleMouse.move(0, 0, -1);
                Serial.println("向下滚动");
            }
            else if (direction == "left") {
                bleMouse.move(0, 0, 0, -1);
                Serial.println("向左滚动");
            }
            else if (direction == "right") {
                bleMouse.move(0, 0, 0, 1);
                Serial.println("向右滚动");
            }
            server.send(200, "text/plain", "OK");
        } else {
            server.send(200, "text/plain", "BLE 未连接");
        }
    });

    // BLE 状态查询接口
    server.on("/status", HTTP_GET, []() {
        if (bleMouse.isConnected()) {
            server.send(200, "text/plain", "已连接");
        } else {
            server.send(200, "text/plain", "未连接");
        }
    });

    // WiFi 状态查询接口
    server.on("/wifi-status", HTTP_GET, []() {
        if (WiFi.status() == WL_CONNECTED) {
            server.send(200, "text/plain", "已连接");
        } else {
            server.send(200, "text/plain", "断开");
        }
    });

    // 启动 Web 服务器
    server.begin();
    Serial.println("Web 服务器已启动");
    Serial.println("请确保设备和ESP32在同一WiFi网络中");
    Serial.println("然后访问: http://" + WiFi.localIP().toString());
    Serial.println("或访问: http://esp32mouse.local");
    Serial.println("初始化完成！");
}

void loop() {
    server.handleClient();
    
    // 每10秒输出一次状态
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 10000) {
        lastPrint = millis();
        Serial.println("运行状态 - BLE: " + String(bleMouse.isConnected() ? "已连接" : "未连接") + 
                      ", WiFi: " + String(WiFi.status() == WL_CONNECTED ? "已连接" : "断开"));
    }
}
