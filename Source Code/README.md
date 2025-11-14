# Hệ Thống Điều Khiển IoT - Gas Detection System

Hệ thống điều khiển thiết bị IoT phát hiện khí gas và lửa sử dụng MQTT, WebSocket và hỗ trợ OTA update.

## 🚀 Tính năng

- ✅ Kết nối MQTT
- ✅ Giao diện web điều khiển real-time qua WebSocket
- ✅ Cập nhật firmware từ xa (OTA)
- ✅ Điều khiển relay, servo, cảm biến
- ✅ Hiển thị dữ liệu cảm biến real-time
- ✅ Cảnh báo khi phát hiện khí gas hoặc lửa
- ✅ Cấu hình WiFi và MQTT qua web interface

## 📋 Yêu cầu hệ thống

- Node.js (v14 trở lên)
- Arduino IDE
- ESP32 Development Board
- WiFi Router (2.4GHz)

## 📁 Cấu trúc dự án

```
Source Code/
├── backend/              # Node.js backend server
│   ├── server.js        # MQTT broker + WebSocket + REST API
│   ├── package.json     # Dependencies
│   └── firmware/        # Thư mục lưu firmware (tự động tạo)
├── frontend/            # Giao diện web
│   ├── index.html       # Trang chủ
│   ├── style.css        # CSS styling
│   └── app.js           # JavaScript logic
├── main_mqtt/           # Code ESP32
│   ├── main_mqtt.ino    # Code chính
│   ├── config.h         # Cấu hình
│   ├── def.h            # Định nghĩa
│   └── mybutton.h       # Thư viện button
├── INSTALL.md           # Hướng dẫn cài đặt chi tiết
└── README.md            # File này
```

## 🔧 Cài đặt

### Bước 1: Cài đặt Backend

1. **Cài đặt Node.js:**
   - Tải từ: https://nodejs.org/
   - Kiểm tra: `node --version` và `npm --version`

2. **Cài đặt dependencies:**
   ```bash
   cd backend
   npm install
   ```

3. **Chạy backend server:**
   ```bash
   npm start
   ```
   
   Backend sẽ chạy trên:
   - **HTTP API**: http://localhost:3000
   - **MQTT Broker**: mqtt://localhost:1883
   - **WebSocket**: ws://localhost:8888

4. **Kiểm tra IP của máy tính:**
   ```bash
   # Windows
   ipconfig
   
   # Linux/Mac
   ifconfig
   ```
   Ghi nhớ IP address (ví dụ: 192.168.1.5) để cấu hình cho ESP32.

### Bước 2: Cài đặt Frontend

**Cách 1: Sử dụng Python (đơn giản nhất)**
```bash
cd frontend
python -m http.server 8000
```

**Cách 2: Sử dụng Node.js http-server**
```bash
npx http-server frontend -p 8000
```

Truy cập: **http://localhost:8000**

### Bước 3: Cài đặt Code cho ESP32

1. **Cài đặt Arduino IDE:**
   - Tải từ: https://www.arduino.cc/en/software

2. **Cài đặt ESP32 Board Support:**
   - File → Preferences
   - Thêm URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools → Board → Boards Manager → Tìm "esp32" → Install

3. **Cài đặt các thư viện:**
   - **PubSubClient** (bởi Nick O'Leary)
   - **ArduinoJson** v6.x (bởi Benoit Blanchon)
   - **ESP32Servo** (bởi Kevin Harrington)
   - **LiquidCrystal** (có sẵn)

4. **Cấu hình và Upload code:**
   - Mở `main_mqtt/main_mqtt.ino` trong Arduino IDE
   - Tools → Board → ESP32 Dev Module
   - Tools → Port → Chọn COM port
   - Upload code

### Bước 4: Cấu hình ESP32 lần đầu

1. **Kết nối WiFi:**
   - ESP32 sẽ tạo Access Point "ESP32" (không cần password)
   - Kết nối vào WiFi "ESP32"
   - Mở trình duyệt: **http://192.168.4.1**

2. **Nhập thông tin:**
   - **SSID**: Tên WiFi của bạn
   - **Password**: Mật khẩu WiFi
   - **MQTT Server IP**: IP của máy chạy backend (ví dụ: 192.168.1.5)
   - Nhấn **Save**

3. **Đợi ESP32 khởi động lại:**
   - ESP32 sẽ tự động kết nối WiFi và MQTT
   - Kiểm tra Serial Monitor để xác nhận kết nối

## ✅ Kiểm tra hoạt động

1. **Backend:** Terminal sẽ hiển thị:
   ```
   [MQTT] Client connected: ESP32_abc123
   [MQTT] Client ESP32_abc123 subscribed to: device/ESP32_abc123/control
   ```

2. **Frontend:**
   - Mở http://localhost:8000
   - Trạng thái kết nối hiển thị "Đã kết nối"
   - Chọn thiết bị từ dropdown
   - Xem dữ liệu cảm biến real-time

## 🔄 Cập nhật Firmware (OTA)

### Qua giao diện web (Khuyến nghị):

1. **Export file .bin:**
   - Arduino IDE: Sketch → Export compiled Binary
   - File `.bin` sẽ được tạo trong thư mục sketch

2. **Upload firmware:**
   - Mở giao diện web → Phần "🔄 Cập Nhật Firmware (OTA)"
   - Nhấn "Chọn file .bin" → Chọn file vừa export
   - (Tùy chọn) Nhập phiên bản (ví dụ: 1.0.1)
   - Nhấn "Upload Firmware"

3. **Cập nhật thiết bị:**
   - Chọn thiết bị từ dropdown
   - Chọn firmware vừa upload
   - Nhấn "Bắt đầu cập nhật"
   - ESP32 sẽ tự động tải và cài đặt firmware mới

### Qua API:

```bash
# Upload firmware
curl -X POST http://localhost:3000/api/firmware/upload \
  -F "firmware=@main_mqtt.ino.bin" \
  -F "version=1.0.1"

# Gửi lệnh OTA
curl -X POST http://localhost:3000/api/devices/ESP32_abc123/ota \
  -H "Content-Type: application/json" \
  -d '{"version": "1.0.1"}'
```

## 📡 Giao thức MQTT

### Topics

**Device → Server:**
- `device/{deviceId}/data` - Dữ liệu cảm biến (mỗi 2 giây)
- `device/{deviceId}/ota/status` - Trạng thái OTA update
- `device/{deviceId}/alert` - Cảnh báo

**Server → Device:**
- `device/{deviceId}/control` - Lệnh điều khiển
- `device/{deviceId}/ota` - Lệnh cập nhật firmware

### Format dữ liệu

**Data (JSON):**
```json
{
  "deviceId": "ESP32_abc123",
  "gasValue": 2500,
  "fireValue": 0,
  "relay1State": 0,
  "relay2State": 0,
  "windowState": 0,
  "autoManual": 1,
  "threshold": 4000,
  "ipAddress": "192.168.1.50"
}
```

**Control (JSON):**
```json
{
  "relay1": 1,
  "relay2": 0,
  "window": 1,
  "autoManual": 0,
  "threshold": 4000
}
```

## 🎮 Sử dụng giao diện web

1. **Kết nối:** WebSocket tự động kết nối khi mở trang
2. **Chọn thiết bị:** Chọn từ dropdown
3. **Xem dữ liệu:** Dữ liệu cảm biến cập nhật real-time
4. **Điều khiển:**
   - Bật/tắt Relay 1, Relay 2
   - Mở/đóng cửa sổ (servo)
   - Chuyển chế độ AUTO/MANUAL
   - Đặt ngưỡng cảnh báo khí gas
5. **OTA Update:** Upload và cập nhật firmware từ xa

## 🔌 API Endpoints

### Devices
- `GET /api/devices` - Lấy danh sách thiết bị
- `GET /api/devices/:deviceId` - Lấy trạng thái thiết bị
- `POST /api/devices/:deviceId/control` - Điều khiển thiết bị
- `POST /api/devices/:deviceId/ota` - Gửi lệnh OTA

### Firmware
- `GET /api/firmware` - Lấy danh sách firmware
- `POST /api/firmware/upload` - Upload firmware
- `GET /api/firmware/:version` - Download firmware

## 🐛 Xử lý lỗi thường gặp

### ESP32 không kết nối WiFi
- Kiểm tra SSID và password
- Đảm bảo WiFi 2.4GHz (ESP32 không hỗ trợ 5GHz)
- Reset ESP32 và cấu hình lại

### ESP32 không kết nối MQTT
- Kiểm tra backend đang chạy: `[MQTT] Broker running on 0.0.0.0:1883`
- Kiểm tra IP MQTT broker đúng với IP máy chạy backend
- Đảm bảo ESP32 và backend cùng mạng WiFi
- Kiểm tra firewall không chặn port 1883

### WebSocket không kết nối
- Kiểm tra backend đang chạy
- Kiểm tra port 8888 không bị chặn
- Mở Console (F12) để xem lỗi

### OTA update thất bại
- Kiểm tra kích thước file < 5MB
- Đảm bảo ESP32 có đủ bộ nhớ
- Kiểm tra URL firmware có thể truy cập từ ESP32
- Kiểm tra kết nối WiFi ổn định

## 📝 Lưu ý

- Đảm bảo ESP32 và máy chạy backend cùng mạng WiFi
- IP của MQTT broker phải là IP local (không phải localhost)
- Khi upload firmware mới, đảm bảo file .bin hợp lệ cho ESP32
- Backup code trước khi cập nhật OTA

## 👥 Tác giả

Nhóm 10 - BTL IoT


- Serial Monitor của ESP32 để debug
- Console của trình duyệt (F12) để xem lỗi frontend
- Log của backend server để xem lỗi backend
