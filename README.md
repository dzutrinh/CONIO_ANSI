# CONIO_ANSI

Thư viện C/C++ một tệp (header-only) mô phỏng `conio.h` thời DOS/Turbo C, chạy trên **Windows, Linux và macOS** nhờ ANSI Escape Sequences.

## Cài đặt

Chép `conio_ansi.h` vào dự án. Trong **đúng một** tệp `.c`/`.cpp`:

```c
#define CONIO_ANSI_IMPLEMENTATION
#include "conio_ansi.h"
```

Các tệp khác chỉ cần `#include "conio_ansi.h"`.

> Luôn gọi `init_conio_ansi()` ở đầu chương trình và `close_conio_ansi()` ở cuối.

## Danh sách hàm

| Hàm | Ý nghĩa |
|---|---|
| `void init_conio_ansi()` | Khởi tạo: bật ANSI trên Windows; tắt đệm dòng và echo trên Linux/macOS. |
| `void close_conio_ansi()` | Khôi phục terminal, màu sắc và con trỏ như ban đầu. |
| `void clrscr()` | Xóa màn hình, đưa con trỏ về (1, 1). |
| `void gotoxy(int x, int y)` | Di chuyển con trỏ đến cột `x`, hàng `y` (góc trên trái là (1, 1)). |
| `void textcolor(int color)` | Đặt màu chữ (0–15). |
| `void textbackground(int color)` | Đặt màu nền (0–15). |
| `void reset_color()` | Trả màu chữ và màu nền về mặc định. |
| `int kbhit()` | Trả về 1 nếu có phím đang chờ, 0 nếu không (không chặn). |
| `int getch()` | Đọc một phím, không cần Enter, không hiện ra màn hình (chặn). |
| `void delay(int ms)` | Tạm dừng `ms` mili-giây. |
| `void hidecursor()` | Ẩn con trỏ nhấp nháy. |
| `void showcursor()` | Hiện lại con trỏ. |

### Hằng số màu (`enum CONIO_COLORS`)

| Thường (0–7) | Sáng (8–15) |
|---|---|
| `C_BLACK` | `C_LIGHTBLACK` (xám) |
| `C_RED` | `C_LIGHTRED` |
| `C_GREEN` | `C_LIGHTGREEN` |
| `C_YELLOW` | `C_LIGHTYELLOW` |
| `C_BLUE` | `C_LIGHTBLUE` |
| `C_MAGENTA` | `C_LIGHTMAGENTA` |
| `C_CYAN` | `C_LIGHTCYAN` |
| `C_WHITE` | `C_LIGHTWHITE` |

## Ví dụ

### 1. In chữ màu tại một vị trí

```c
#define CONIO_ANSI_IMPLEMENTATION
#include "conio_ansi.h"
#include <stdio.h>

int main() {
    init_conio_ansi();
    clrscr();

    gotoxy(10, 5);
    textcolor(C_LIGHTYELLOW);
    textbackground(C_BLUE);
    printf(" Xin chao! ");
    reset_color();

    gotoxy(10, 7);
    printf("Nhan phim bat ky de thoat...");
    getch();

    close_conio_ansi();
    return 0;
}
```

### 2. Di chuyển nhân vật bằng W/A/S/D

```c
#define CONIO_ANSI_IMPLEMENTATION
#include "conio_ansi.h"
#include <stdio.h>

int main() {
    int x = 20, y = 10, running = 1;

    init_conio_ansi();
    hidecursor();
    clrscr();

    while (running) {
        if (kbhit()) {
            gotoxy(x, y); printf(" ");          // xóa vị trí cũ
            switch (getch()) {
                case 'w': y--; break;
                case 's': y++; break;
                case 'a': x--; break;
                case 'd': x++; break;
                case 'q': running = 0; break;
            }
        }
        gotoxy(x, y);
        textcolor(C_LIGHTGREEN);
        printf("@");
        delay(30);                              // khoảng 30 khung hình/giây
    }

    close_conio_ansi();                         // tự hiện lại con trỏ, khôi phục màu
    return 0;
}
```

## Chương trình demo

[demo.c](demo.c) minh họa toàn bộ 12 hàm của thư viện. Nhấn phím bất kỳ để chuyển sang phần tiếp theo.

| Phần | Nội dung | Hàm minh họa |
|---|---|---|
| 1. Bảng màu | Hiển thị 16 màu chữ và 16 màu nền | `textcolor`, `textbackground`, `reset_color` |
| 2. gotoxy | Vẽ khung, in chữ ở các góc, vẽ đường chéo | `clrscr`, `gotoxy` |
| 3. Chữ chạy | Dòng chữ cuộn liên tục cho đến khi nhấn phím | `delay`, `hidecursor`, `kbhit`, `showcursor` |
| 4. Trò chơi | Điều khiển `@` ăn ngôi sao `*` để ghi điểm | `kbhit`, `getch`, `delay` |

`init_conio_ansi()` được gọi ở đầu và `close_conio_ansi()` ở cuối chương trình.

**Điều khiển trò chơi:** `W` lên, `S` xuống, `A` trái, `D` phải, `Q` thoát.

**Biên dịch và chạy:**

Linux / macOS:

```bash
gcc demo.c -o demo && ./demo
```

Windows (MinGW):

```bash
gcc demo.c -o demo.exe && demo.exe
```

Demo cũng biên dịch được bằng `g++` hoặc MSVC (`cl demo.c`).

## Lưu ý

- Phím mũi tên trả về mã khác nhau giữa Windows và Linux/macOS; nên dùng W/A/S/D để chương trình chạy giống nhau mọi nơi.
- Trên Windows cần Windows 10 trở lên (console hỗ trợ ANSI/VT100).
- Dùng được với cả trình biên dịch C (`gcc`) lẫn C++ (`g++`).

## Giấy phép

Xem tệp [LICENSE](LICENSE).
