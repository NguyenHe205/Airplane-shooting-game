# <img src="Assets/player.png" width="30"> **Airplane Shooting** <img src="Assets/player.png" width="30">

<div align="center">
  <img src="Assets/startscreen.png" width="500">
</div>

---

## 🎮 **Giới thiệu**

- **Người phát triển:** Nguyễn Thế Hệ - K69I_IS4 - Trường Đại học Công nghệ-Đại học Quốc gia Hà Nội
- **Mã số sinh viên:** 24022649
- **Game:** Airplane Shooting

## 🌟 **Mô tả**

### **Tổng quan**

Airplane Shooting là một game bắn súng arcade 2D được phát triển bằng C++ sử dụng thư viện SDL2, được tạo ra như một dự án cho lớp lập trình. Game theo dõi một chiếc máy bay do người chơi điều khiển trong nhiệm vụ tiêu diệt kẻ thù và trùm, di chuyển qua một cấp độ cuộn dọc vô tận đầy thách thức.

Lấy cảm hứng từ các game bắn súng arcade kinh điển, Airplane Shooting tập trung vào lối chơi nhanh, điều khiển chính xác và độ khó tăng dần để giữ người chơi hứng thú.

### **Các yếu tố của trò chơi**

- **Màn hình bắt đầu** với menu tương tác để bắt đầu game
- **Lối chơi vô tận** với kẻ thù và trùm xuất hiện ngẫu nhiên
- **Hệ thống máu** hiển thị bằng các biểu tượng trái tim ở góc trên bên phải
- **Gặp gỡ kẻ thù** bao gồm kẻ thù thường và trùm
- **Vật phẩm hỗ trợ (Buffs)** để nâng cấp khả năng bắn
- **Hệ thống điểm số và điểm cao nhất** để theo dõi thành tích người chơi
- **Điều kiện chiến thắng và thất bại** dựa trên số mạng và tiến trình

## 🎲 **Lối chơi**

### **Điều khiển**

- **Di chuyển:** Phím W (lên)/A(phải)/S(xuống)/D(trái)
- **Bắn:** Phím Space
- **Điều hướng menu:** Chuột (cho màn hình bắt đầu và kết thúc)

### **Khả năng nhân vật**

- **Bay:** Di chuyển ngang và dọc trên màn hình
- **Bắn:** Bắn đạn để tiêu diệt kẻ thù và trùm
- **Vật phẩm hỗ trợ:** Thu thập buff để nâng cấp bắn (Bắn đôi, Bắn ba)
- **Phát hiện va chạm:** Tránh kẻ thù và đạn trùm để sống sót

### **Tiến trình**

Game có một cấp độ vô tận với độ khó tăng dần:

- **Kẻ thù thường:** Xuất hiện ngẫu nhiên, di chuyển xuống và phải bị tiêu diệt
- **Gặp trùm:** Xuất hiện định kỳ với lượng máu cao hơn và các mẫu bắn đạn
- **Vật phẩm hỗ trợ:** Buff xuất hiện khi đạt ngưỡng điểm số hoặc tiêu diệt trùm
- **Hệ thống điểm số:** Kiếm điểm bằng cách tiêu diệt kẻ thù và trùm 

### **Hệ thống máu**

- Người chơi bắt đầu với 3 mạng (được biểu thị bằng trái tim)
- Mạng giảm khi va chạm với kẻ thù hoặc đạn trùm
- Khi hết mạng, GAME OVER!
- Mạng được hiển thị bằng các biểu tượng trái tim ở góc trên bên phải

<div align="center">
  <img src="Assets/heart.png" height="50">
</div>

## 🔧 **Kiến trúc kỹ thuật**

Dự án theo kiến trúc module được tổ chức theo chức năng:

### **Thành phần cốt lõi**

- **Lớp Game:** Bộ điều khiển trung tâm quản lý vòng lặp game, chuyển đổi trạng thái và các hệ thống cốt lõi

  ```cpp
  void Game::run() {
      Background bg(renderer);
      Player player(renderer);
      std::vector<Bullet> bullets;
      std::vector<Enemy> enemies;
      bullets.reserve(100);

      const float targetFrameTime = 1000.0f / 60.0f;
      while (running) {
          Uint32 currentTime = SDL_GetTicks();
          if (inStartScreen) {
              handleStartScreenEvents();
              render(player, bullets, enemies, bg);
          } else if (inGameOver) {
              handleGameOverEvents(player, bullets, enemies);
              render(player, bullets, enemies, bg);
          } else {
              handleEvents(player, bullets);
              bg.update();
              update(player, bullets, enemies);
              render(player, bullets, enemies, bg);
          }
          Uint32 frameTime = SDL_GetTicks() - currentTime;
          if (frameTime < targetFrameTime) {
              SDL_Delay(static_cast<Uint32>(targetFrameTime - frameTime));
          }
      }
  }
  ```

- **Hệ thống đồ họa:** Xử lý render, quản lý texture và hoạt động cửa sổ sử dụng SDL2

### **Hệ thống thực thể**

- **Người chơi:** Thực hiện di chuyển, bắn, phát hiện va chạm và quản lý buff

  ```cpp
  // Hệ thống bắn của người chơi
  void Player::shoot(std::vector<Bullet>& bullets) {
      if (buffActive) {
          if (buffType == 1) { // Bắn ba
              bullets.emplace_back(renderer, rect.x + rect.w / 2 - 15, rect.y);
              bullets.emplace_back(renderer, rect.x + rect.w / 2, rect.y);
              bullets.emplace_back(renderer, rect.x + rect.w / 2 + 15, rect.y);
          } else if (buffType == 2) { // Bắn đôi
              bullets.emplace_back(renderer, rect.x + rect.w / 2 - 10, rect.y);
              bullets.emplace_back(renderer, rect.x + rect.w / 2 + 10, rect.y);
          }
      } else {
          bullets.emplace_back(renderer, rect.x + rect.w / 2 - 2, rect.y);
      }
  }
  ```

- **Kẻ thù và trùm:** Có các mẫu di chuyển, cơ chế bắn và phát hiện va chạm

### **Quản lý trạng thái**

- **Hệ thống trạng thái game:** Điều khiển chuyển đổi giữa màn hình bắt đầu, gameplay và màn hình kết thúc
- **Logic cấp độ:** Cấp độ vô tận với kẻ thù và trùm xuất hiện ngẫu nhiên, độ khó tăng theo thời gian

### **Hệ thống hỗ trợ**

- **Hoạt ảnh:** Hiệu ứng nổ khi tiêu diệt kẻ thù và trùm sử dụng sprite sheet
- **Âm thanh:** Nhạc nền và hiệu ứng âm thanh cho bắn, nổ và va chạm
- **Đầu vào:** Xử lý đầu vào bàn phím và chuột
- **Vật lý:** Cơ chế di chuyển đơn giản và va chạm cho đạn, kẻ thù và buff

## ✨ **Tính năng game**

### **Hệ thống Menu**

<div align="center">
  <table>
    <tr>
      <td><img src="Assets/startscreen.png" width="200"></td>
      <td><img src="Assets/gameover.png" width="200"></td>
    </tr>
    <tr>
      <td>Màn hình bắt đầu</td>
      <td>Màn hình kết thúc</td>
    </tr>
  </table>
</div>

- **Màn hình bắt đầu:** Bắt đầu game bằng cách nhấp vào nút bắt đầu
- **Màn hình kết thúc:** Tùy chọn khởi động lại hoặc thoát khi hết mạng
- **Hiển thị điểm số:** Hiển thị điểm hiện tại và điểm cao nhất trên cả hai màn hình

### **Trạng thái game**

<div align="center">
  <table>
    <tr>
      <td><img src="Assets/explosion.png" width="200"></td>
      <td><img src="Assets/gameover.png" width="200"></td>
    </tr>
    <tr>
      <td>Hiệu ứng nổ</td>
      <td>Màn hình kết thúc</td>
    </tr>
  </table>
</div>

- **Hoạt ảnh nổ:** Phản hồi trực quan khi tiêu diệt kẻ thù hoặc trùm
- **Màn hình kết thúc:** Hiển thị khi hết mạng, với tùy chọn khởi động lại hoặc thoát
- **Theo dõi điểm số:** Điểm cao nhất được lưu vào file (`highscore.txt`) để duy trì

### **Nhân vật**

<div align="center">
  <table>
    <tr>
      <td><img src="Assets/player.png" width="100"></td>
      <td><img src="Assets/enemy.png" width="100"></td>
      <td><img src="Assets/boss.png" width="100"></td>
    </tr>
    <tr>
      <td>Người chơi (Máy bay)</td>
      <td>Kẻ thù</td>
      <td>Trùm</td>
    </tr>
  </table>
</div>

- **Người chơi:** Máy bay có thể điều khiển, bắn và thu thập buff
- **Kẻ thù:** Kẻ thù thường di chuyển xuống và có thể bị tiêu diệt
- **Trùm:** Kẻ thù lớn với lượng máu cao và các mẫu bắn đạn

### **Đồ họa & Âm thanh**

- **Hoạt ảnh sprite:** Hiệu ứng nổ sử dụng sprite sheet
- **Nền:** Nền cuộn đơn giản để tăng cảm giác thực tế
- **Âm thanh động:** Nhạc nền và hiệu ứng âm thanh cho bắn, nổ và va chạm
- **Phản hồi trực quan:** Trái tim cho mạng, hiển thị điểm số và hiệu ứng nổ

## 🛠️ **Chi tiết phát triển**

### **Công nghệ sử dụng**

- **Ngôn ngữ:** C++
- **Thư viện đồ họa:** SDL2 (SDL2_image, SDL2_ttf, SDL2_mixer)
- **Hệ thống build:** Code::Blocks
- **Môi trường phát triển:** Windows

### **Triển khai cơ chế**

Game triển khai các cơ chế sau:

- Cơ chế di chuyển và bắn của người chơi
- Hệ thống xuất hiện kẻ thù và trùm với độ khó tăng dần
- Phát hiện va chạm sử dụng Axis-Aligned Bounding Box (AABB)
- Hệ thống buff cho vật phẩm hỗ trợ (Bắn đôi, Bắn ba)

### **Hệ thống hoạt ảnh**

Hiệu ứng sử dụng hoạt ảnh dựa trên khung hình:

- Hoạt ảnh nổ khi tiêu diệt kẻ thù và trùm
- Chuyển đổi mượt mà giữa các khung hình sử dụng sprite sheet
- Render nhận biết hướng cho người chơi và kẻ thù

### **Tổ chức tài nguyên**

Tài nguyên được tổ chức theo loại và mục đích:

- Sprite nhân vật (người chơi, kẻ thù, trùm)
- Phần tử UI (màn hình bắt đầu, màn hình kết thúc, trái tim, hiển thị điểm số)
- Hiệu ứng (sprite sheet nổ)
- Hiệu ứng âm thanh và nhạc (bắn, nổ, va chạm, nhạc nền)

## 🏆 **Ghi công**

- **Khái niệm trò chơi & Lập trình:** Nguyễn Thế Hệ
- **Tham khảo thư viện SDL2:**
  - [Hướng dẫn SDL của Lazyfoo](https://lazyfoo.net/tutorials/SDL/index.php)
  - [Kênh YouTube Tianchinchiko](https://www.youtube.com/@tianchinchiko)
- **Đồ họa:**
  - [OpenGameArt](https://opengameart.org) (nếu áp dụng)
  - Tự thiết kế hoặc tải về
- **Âm thanh:**
  - Nhạc nền: Tự tạo
  - Hiệu ứng âm thanh: Bộ sưu tập SFX retro (ví dụ: `shoot.wav`, `explosion.wav`, `hit.wav`)
- **Nguồn cảm hứng:**
  - Game bắn súng arcade kinh điển
  - Tài liệu lớp từ Lập trình nâng cao