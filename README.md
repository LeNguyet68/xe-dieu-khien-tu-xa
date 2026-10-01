# 🏎️ ESP32 MQTT RC Car - Hệ Thống Xe Điều Khiển Từ Xa Qua Internet

Một dự án cá nhân (Personal Project) về xe mô hình điều khiển từ xa (RC Car) ứng dụng nền tảng Internet of Things (IoT). Thay vì sử dụng các module sóng RF (như nRF24L01) hay Bluetooth truyền thống có giới hạn về khoảng cách, dự án này cho phép điều khiển xe từ bất kỳ đâu thông qua mạng Internet nhờ sức mạnh của vi điều khiển ESP32 và giao thức MQTT.

---

## ✨ Tính Năng Nổi Bật

* **🌐 Điều khiển không giới hạn khoảng cách:** Miễn là xe và thiết bị điều khiển đều có kết nối Internet (Wi-Fi gia đình hoặc Mobile Hotspot).
* **📱 Giao diện Web tiện lợi:** Không cần tải app. Trình điều khiển là một file HTML/JS nhẹ gọn, có thể chạy thẳng trên trình duyệt của bất kỳ điện thoại hay máy tính nào.
* **⚡ Phản hồi thời gian thực (Real-time):** Sử dụng MQTT qua chuẩn WebSockets cho độ trễ cực thấp (< 50ms). Bắt sự kiện chạm/nhả vật lý cực nhạy (nhả tay là xe tự động phanh).
* **🚗 Hệ dẫn động 4 bánh (4WD) mạnh mẽ:** Tích hợp module PWM điều tốc giúp xe chạy đầm, vượt địa hình tốt mà không bị trượt bánh khi vào cua.
* **🔄 Tự động phục hồi kết nối:** Thuật toán tự động cấp lại Client ID và reconnect vào máy chủ MQTT nếu xe bị rớt mạng.

---

## 🛠️ Danh Sách Linh Kiện (Hardware Requirements)

1. **Bộ não trung tâm:** 1 x Vi điều khiển NodeMCU ESP32 (DOIT DevKit V1 hoặc tương đương).
2. **Mạch công suất:** 1 x Module điều khiển động cơ L298N.
3. **Động cơ:** 4 x Động cơ giảm tốc DC (thường đi kèm bộ khung Smart Car 4WD).
4. **Nguồn cấp:** 2 x Cell pin 18650 (Dung lượng cao, dòng xả lớn) + Đế đựng 2 pin.
5. **Khác:** Dây cắm test board (Đực-Cái, Đực-Đực), khung xe mica.

---

## 🔌 Sơ Đồ Đấu Nối (Pinout & Wiring)

### 1. Đấu nối Cụm Động Cơ (Hệ 4WD)
Do L298N chỉ có 2 kênh đầu ra độc lập, ta sẽ mắc song song các động cơ để tạo thành hệ dẫn động 2 bên:
* **Cụm Trái (2 động cơ bên trái):** Chập 2 dây cùng màu lại với nhau, nối vào terminal **OUT1** và **OUT2** trên L298N.
* **Cụm Phải (2 động cơ bên phải):** Chập 2 dây cùng màu lại với nhau, nối vào terminal **OUT3** và **OUT4** trên L298N.

### 2. Tín hiệu Điều Khiển (ESP32 ↔ L298N)

| Chân trên ESP32 | Chân trên L298N | Chức năng |
| :--- | :--- | :--- |
| **GPIO 23** | **ENA** | Băm xung (PWM) kiểm soát tốc độ cụm Trái |
| **GPIO 5** | **ENB** | Băm xung (PWM) kiểm soát tốc độ cụm Phải |
| **GPIO 19** | **IN1** | Điều hướng cụm Trái |
| **GPIO 18** | **IN2** | Điều hướng cụm Trái |
| **GPIO 22** | **IN3** | Điều hướng cụm Phải |
| **GPIO 21** | **IN4** | Điều hướng cụm Phải |

### 3. Sơ đồ Cấp Nguồn (Rất quan trọng)
* Cực Dương (+) của đế pin cắm vào terminal **12V** của L298N.
* Cực Âm (-) của đế pin cắm vào terminal **GND** của L298N.
* Lấy 1 sợi dây cắm từ terminal **5V** của L298N sang chân **VIN** (hoặc 5V) của ESP32 để nuôi mạch.
* **BẮT BUỘC:** Lấy 1 sợi dây nối chân **GND** của ESP32 với terminal **GND** của L298N để đồng bộ mức tín hiệu 0V.

---

## 🚀 Hướng Dẫn Cài Đặt (Software Setup)

### Bước 1: Nạp Code cho ESP32
1. Tải và cài đặt phần mềm [Arduino IDE](https://www.arduino.cc/en/software).
2. Cài đặt thư viện hỗ trợ board ESP32 vào Arduino IDE.
3. Vào `Sketch` > `Include Library` > `Manage Libraries`, tìm và cài đặt thư viện **PubSubClient** (tác giả Nick O'Leary).
4. Mở file mã nguồn `MQTT_RCCar.ino`.
5. Chỉnh sửa cấu hình Wi-Fi theo mạng nhà bạn:
   ```cpp
   const char* ssid = "TÊN_WIFI_CỦA_BẠN";
   const char* password = "MẬT_KHẨU_WIFI";
