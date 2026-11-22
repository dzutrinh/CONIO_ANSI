/*
 * CONIO_ANSI.H (Thư viện mô phỏng conio.h đa nền tảng)
 *
 * Tác giả: Trinh D.D. Nguyen
 * Ngày: 29/10/2025
 *
 * Sử dụng ANSI Escape Sequences để cung cấp các hàm như gotoxy, clrscr,
 * textcolor, textbackground, getch, và kbhit.
 *
 * CÁCH SỬ DỤNG:
 * 1. Đặt tệp này trong dự án của bạn (ví dụ: "conio_ansi.h").
 * 2. Trong MỘT tệp .c (ví dụ: main.c), hãy định nghĩa:
 * #define CONIO_ANSI_IMPLEMENTATION
 * #include "conio_ansi.h"
 * 3. Trong các tệp .c khác, chỉ cần:
 * #include "conio_ansi.h"
 *
 * LƯU Ý:
 * Luôn gọi init_conio_ansi() ở đầu chương trình và close_conio_ansi()
 * ở cuối chương trình của bạn.
 *
 * CẬP NHẬT 12/11/2026:
 * - Thêm delay(ms): tạm dừng chương trình (dùng cho game thời gian thực).
 * - Thêm hidecursor()/showcursor(): ẩn/hiện con trỏ nhấp nháy khi vẽ.
 * - close_conio_ansi() khôi phục màu và con trỏ.
 * - Linux/macOS: getch() đọc trực tiếp bằng read() để không bị trễ/mất phím
 *   khi dùng chung với kbhit(); đặt rõ VMIN = 1, VTIME = 0.
 * - Phím mũi tên trả về mã khác nhau trên Windows và Linux/macOS:
 *   nên dùng các phím W, A, S, D để chương trình chạy giống nhau mọi nơi.
 */

#ifndef CONIO_ANSI_H
#define CONIO_ANSI_H

// --- 1. ĐỊNH NGHĨA MÀU SẮC (API công khai) ---

/**
 * @brief Enum các hằng số màu sắc tương tự conio.h
 */
enum CONIO_COLORS {
    C_BLACK = 0,        ///< 0
    C_RED,              ///< 1
    C_GREEN,            ///< 2
    C_YELLOW,           ///< 3
    C_BLUE,             ///< 4
    C_MAGENTA,          ///< 5
    C_CYAN,             ///< 6
    C_WHITE,            ///< 7
    C_LIGHTBLACK = 8,   ///< 8 (thường hiển thị là Gray)
    C_LIGHTRED,         ///< 9
    C_LIGHTGREEN,       ///< 10
    C_LIGHTYELLOW,      ///< 11
    C_LIGHTBLUE,        ///< 12
    C_LIGHTMAGENTA,     ///< 13
    C_LIGHTCYAN,        ///< 14
    C_LIGHTWHITE        ///< 15
};

// --- 2. GIAO DIỆN API (Prototypes) ---

#ifdef __cplusplus
extern "C" {   // liên kết kiểu C: dùng được cả trong tệp .c và .cpp (g++)
#endif

/**
 * @brief Khởi tạo môi trường. Bật chế độ VT100 (ANSI) trên Windows
 * và đặt terminal vào chế độ raw/non-canonical (không echo, không đệm)
 * trên Linux/macOS. Phải được gọi ở đầu chương trình.
 */
void init_conio_ansi();

/**
 * @brief Đóng và khôi phục cài đặt terminal về trạng thái ban đầu.
 * Phải được gọi ở cuối chương trình.
 */
void close_conio_ansi();

/**
 * @brief Xóa toàn bộ màn hình và di chuyển con trỏ về (1, 1).
 */
void clrscr();

/**
 * @brief Di chuyển con trỏ đến tọa độ (x, y).
 * (1, 1) là góc trên cùng bên trái.
 * @param x Tọa độ cột (hoành độ).
 * @param y Tọa độ hàng (tung độ).
 */
void gotoxy(int x, int y);

/**
 * @brief Đặt màu chữ (foreground).
 * @param color Một giá trị từ enum CONIO_COLORS.
 */
void textcolor(int color);

/**
 * @brief Đặt màu nền (background).
 * @param color Một giá trị từ enum CONIO_COLORS.
 */
void textbackground(int color);

/**
 * @brief Đặt lại tất cả các thuộc tính màu sắc về mặc định.
 */
void reset_color();

/**
 * @brief Kiểm tra xem có phím nào được nhấn hay không (không chặn).
 * @return 1 nếu có phím nhấn, 0 nếu không.
 */
int kbhit();

/**
 * @brief Lấy một ký tự từ bàn phím (chặn). Không yêu cầu nhấn Enter.
 * @return Mã ASCII của phím được nhấn.
 */
int getch();

/**
 * @brief Tạm dừng chương trình trong ms mili-giây (đẩy hết dữ liệu ra màn hình trước).
 * @param ms Số mili-giây cần dừng (ví dụ 100 = 0,1 giây).
 */
void delay(int ms);

/**
 * @brief Ẩn con trỏ nhấp nháy (nên gọi khi vẽ game).
 */
void hidecursor();

/**
 * @brief Hiện lại con trỏ.
 */
void showcursor();

#ifdef __cplusplus
}
#endif

#endif // CONIO_ANSI_H

// --- 3. PHẦN TRIỂN KHAI THỰC TẾ (Chỉ biên dịch nếu được định nghĩa) ---

#ifdef CONIO_ANSI_IMPLEMENTATION

#include <stdio.h>

// --- Triển khai đa nền tảng ---
#ifdef _WIN32
// --- NỀN TẢNG WINDOWS ---
// Không #include <conio.h>: tệp này đã khai báo getch()/kbhit() theo kiểu cũ
// (bị đánh dấu "deprecated") và xung đột khi biên dịch bằng g++.
// Chỉ cần khai báo hai hàm _kbhit/_getch của thư viện C trên Windows:
#ifdef __cplusplus
extern "C" {
#endif
int _kbhit(void);
int _getch(void);
#ifdef __cplusplus
}
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // chỉ lấy phần cần thiết của windows.h
#endif
#ifndef NOMINMAX
#define NOMINMAX            // không định nghĩa macro min/max (tránh trùng tên biến)
#endif
#include <windows.h> // Cho SetConsoleMode, Sleep

// Lưu trạng thái console cũ để khôi phục
static HANDLE hStdOut_ansi;
static DWORD dwOldMode_ansi = 0;

void init_conio_ansi() {
    hStdOut_ansi = GetStdHandle(STD_OUTPUT_HANDLE);
    // Lấy chế độ console hiện tại
    GetConsoleMode(hStdOut_ansi, &dwOldMode_ansi);
    // Bật chế độ xử lý VIRTUAL_TERMINAL (để kích hoạt ANSI)
    // Đây là bước quan trọng nhất trên Windows
    SetConsoleMode(hStdOut_ansi, dwOldMode_ansi | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    // Trên Windows, _getch và _kbhit đã xử lý chế độ raw
    // nên không cần thay đổi chế độ input.
}

void close_conio_ansi() {
    printf("\x1B[0m\x1B[?25h"); // Khôi phục màu và hiện con trỏ
    fflush(stdout);
    // Khôi phục chế độ console cũ
    SetConsoleMode(hStdOut_ansi, dwOldMode_ansi);
}

// Windows có sẵn các hàm này
int kbhit() {
    return _kbhit();
}

int getch() {
    return _getch();
}

void delay(int ms) {
    fflush(stdout);
    if (ms > 0) Sleep((DWORD)ms);
}

#elif defined(__linux__) || defined(__APPLE__)
// --- NỀN TẢNG LINUX & MACOS ---
#include <termios.h> // Để thay đổi chế độ terminal
#include <unistd.h>  // STDIN_FILENO, read
#include <sys/select.h> // Cho kbhit
#include <time.h>    // Cho nanosleep

// Lưu trạng thái terminal cũ
static struct termios oldt_ansi;

void init_conio_ansi() {
    struct termios newt;
    // Lấy cài đặt terminal hiện tại
    tcgetattr(STDIN_FILENO, &oldt_ansi);
    newt = oldt_ansi;

    // Tắt chế độ CANONICAL (đệm dòng) và ECHO (hiển thị phím nhấn)
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;  // read() chờ ít nhất 1 ký tự
    newt.c_cc[VTIME] = 0; // không giới hạn thời gian chờ

    // Áp dụng cài đặt mới ngay lập tức
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void close_conio_ansi() {
    printf("\x1B[0m\x1B[?25h"); // Khôi phục màu và hiện con trỏ
    fflush(stdout);
    // Khôi phục cài đặt terminal cũ
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt_ansi);
}

int kbhit() {
    struct timeval tv = { 0L, 0L }; // Timeout ngay lập tức
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); // Theo dõi STDIN
    // Dùng select để kiểm tra xem có dữ liệu để đọc hay không
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

int getch() {
    // Đọc trực tiếp 1 byte bằng read() (không qua bộ đệm của stdio),
    // để kbhit() và getch() luôn nhìn thấy cùng một dữ liệu.
    fflush(stdout);
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return -1;
    return c;
}

void delay(int ms) {
    fflush(stdout);
    if (ms <= 0) return;
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

#else
// --- NỀN TẢNG KHÔNG HỖ TRỢ ---
#warning "Nen tang khong ho tro conio_ansi.h. Cac ham getch/kbhit se khong hoat dong."
void init_conio_ansi() {}
void close_conio_ansi() {}
int kbhit() { return 0; }
int getch() { return 0; }
void delay(int ms) { (void)ms; fflush(stdout); }

#endif

// --- Triển khai các hàm ANSI (Độc lập nền tảng) ---

void clrscr() {
    printf("\x1B[2J\x1B[H"); // 2J = Xóa màn hình, H = Về (1,1)
    fflush(stdout); // Đẩy buffer ra ngay
}

void gotoxy(int x, int y) {
    printf("\x1B[%d;%dH", y, x); // [Hàng; Cột]
    fflush(stdout);
}

void textcolor(int color) {
    if (color >= 0 && color <= 7) {
        printf("\x1B[%dm", 30 + color); // Mã 30-37
    } else if (color >= 8 && color <= 15) {
        printf("\x1B[%dm", 90 + (color - 8)); // Mã 90-97 (sáng)
    }
    fflush(stdout);
}

void textbackground(int color) {
    if (color >= 0 && color <= 7) {
        printf("\x1B[%dm", 40 + color); // Mã 40-47
    } else if (color >= 8 && color <= 15) {
        printf("\x1B[%dm", 100 + (color - 8)); // Mã 100-107 (sáng)
    }
    fflush(stdout);
}

void reset_color() {
    printf("\x1B[0m"); // Reset tất cả
    fflush(stdout);
}

void hidecursor() {
    printf("\x1B[?25l");
    fflush(stdout);
}

void showcursor() {
    printf("\x1B[?25h");
    fflush(stdout);
}

#endif // CONIO_ANSI_IMPLEMENTATION