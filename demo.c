/*
 * demo.c - Chương trình minh họa toàn bộ các hàm của conio_ansi.h
 *
 * Biên dịch:
 *   Linux/macOS : gcc demo.c -o demo
 *   Windows     : gcc demo.c -o demo.exe   (hoặc cl demo.c)
 *
 * Các phần minh họa:
 *   1. Bảng 16 màu chữ / 16 màu nền   (textcolor, textbackground, reset_color)
 *   2. Vẽ khung bằng gotoxy           (gotoxy, clrscr)
 *   3. Hiệu ứng chữ chạy              (delay, hidecursor, kbhit)
 *   4. Trò chơi nhỏ: điều khiển "@"   (kbhit, getch, delay, hidecursor)
 *   5. Kết thúc                        (showcursor, close_conio_ansi)
 */

#define CONIO_ANSI_IMPLEMENTATION
#include "conio_ansi.h"
#include <stdio.h>
#include <string.h>

/* Tên màu để hiển thị trong bảng */
static const char *color_names[16] = {
    "BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGENTA", "CYAN", "WHITE",
    "LIGHTBLACK", "LIGHTRED", "LIGHTGREEN", "LIGHTYELLOW",
    "LIGHTBLUE", "LIGHTMAGENTA", "LIGHTCYAN", "LIGHTWHITE"
};

/* In một chuỗi tại (x, y) với màu chữ cho trước */
static void print_at(int x, int y, int color, const char *s) {
    gotoxy(x, y);
    textcolor(color);
    printf("%s", s);
    reset_color();
}

/* Vẽ khung chữ nhật từ (x1, y1) đến (x2, y2) */
static void draw_box(int x1, int y1, int x2, int y2, int color) {
    int i;
    textcolor(color);
    for (i = x1; i <= x2; i++) {
        gotoxy(i, y1); printf("-");
        gotoxy(i, y2); printf("-");
    }
    for (i = y1; i <= y2; i++) {
        gotoxy(x1, i); printf("|");
        gotoxy(x2, i); printf("|");
    }
    gotoxy(x1, y1); printf("+");
    gotoxy(x2, y1); printf("+");
    gotoxy(x1, y2); printf("+");
    gotoxy(x2, y2); printf("+");
    reset_color();
}

/* Tiêu đề + chờ phím để sang phần tiếp theo */
static void header(const char *title) {
    clrscr();
    draw_box(1, 1, 60, 3, C_LIGHTCYAN);
    print_at(3, 2, C_LIGHTYELLOW, title);
}

static void wait_key(int y) {
    print_at(3, y, C_LIGHTBLACK, "Nhan phim bat ky de tiep tuc...");
    getch();
}

/* ---------- 1. Bảng màu ---------- */
static void demo_colors(void) {
    int c;
    header("1. Bang mau: textcolor / textbackground");

    for (c = 0; c < 16; c++) {
        /* Cột trái: màu chữ trên nền mặc định */
        gotoxy(3, 5 + c);
        textcolor(c);
        printf("%2d %-14s", c, color_names[c]);
        reset_color();

        /* Cột phải: màu nền, chữ tương phản */
        gotoxy(30, 5 + c);
        textbackground(c);
        textcolor(c == C_BLACK || c == C_BLUE || c == C_LIGHTBLACK
                  ? C_LIGHTWHITE : C_BLACK);
        printf(" nen %-14s", color_names[c]);
        reset_color();
    }
    wait_key(22);
}

/* ---------- 2. gotoxy ---------- */
static void demo_gotoxy(void) {
    int i;
    header("2. gotoxy: dat chu tai bat ky vi tri nao");

    draw_box(5, 6, 55, 18, C_LIGHTGREEN);
    print_at(6, 7,  C_WHITE, "(6,7) goc tren trai");
    print_at(35, 17, C_WHITE, "(35,17) goc duoi phai");

    /* Đường chéo */
    for (i = 0; i < 9; i++)
        print_at(20 + i * 2, 8 + i, 9 + (i % 7), "*");

    wait_key(20);
}

/* ---------- 3. Chữ chạy ---------- */
static void demo_marquee(void) {
    const char *msg = "  CONIO_ANSI - gotoxy, textcolor, delay, kbhit...  ";
    int len = (int)strlen(msg);
    int width = 40, pos = 0, i;

    header("3. Chu chay: delay + hidecursor + kbhit");
    print_at(3, 9, C_LIGHTBLACK, "Nhan phim bat ky de dung.");
    draw_box(4, 5, 4 + width + 1, 7, C_LIGHTMAGENTA);

    hidecursor();
    while (!kbhit()) {
        gotoxy(5, 6);
        textcolor(C_LIGHTYELLOW);
        for (i = 0; i < width; i++)
            putchar(msg[(pos + i) % len]);
        reset_color();
        pos = (pos + 1) % len;
        delay(80);
    }
    getch();          /* bỏ phím vừa nhấn */
    showcursor();
}

/* ---------- 4. Trò chơi nhỏ ---------- */
static void demo_game(void) {
    const int L = 3, T = 5, R = 57, B = 20;   /* biên khung */
    int x = 30, y = 12, star_x = 10, star_y = 8;
    int score = 0, ticks = 0, running = 1;
    char buf[64];

    header("4. Tro choi: W/A/S/D di chuyen, Q thoat");
    draw_box(L, T, R, B, C_LIGHTBLUE);
    hidecursor();

    while (running) {
        /* Xử lý phím (không chặn) */
        if (kbhit()) {
            int k = getch();
            print_at(x, y, C_BLACK, " ");     /* xóa vị trí cũ */
            switch (k) {
                case 'w': case 'W': if (y > T + 1) y--; break;
                case 's': case 'S': if (y < B - 1) y++; break;
                case 'a': case 'A': if (x > L + 1) x--; break;
                case 'd': case 'D': if (x < R - 1) x++; break;
                case 'q': case 'Q': running = 0; break;
            }
        }

        /* Ăn ngôi sao */
        if (x == star_x && y == star_y) {
            score++;
            star_x = L + 1 + (star_x * 7 + score * 13) % (R - L - 1);
            star_y = T + 1 + (star_y * 5 + score * 3)  % (B - T - 1);
        }

        /* Vẽ: ngôi sao nhấp nháy đổi màu theo thời gian */
        print_at(star_x, star_y, 9 + (ticks / 5) % 7, "*");
        print_at(x, y, C_LIGHTGREEN, "@");

        snprintf(buf, sizeof buf, "Diem: %d   ", score);
        print_at(L, B + 1, C_LIGHTWHITE, buf);

        ticks++;
        delay(30);
    }
    showcursor();
}

/* ---------- 5. Kết thúc ---------- */
int main(void) {
    init_conio_ansi();

    demo_colors();
    demo_gotoxy();
    demo_marquee();
    demo_game();

    clrscr();
    textcolor(C_LIGHTGREEN);
    printf("Cam on ban da xem demo CONIO_ANSI!\n");
    reset_color();

    close_conio_ansi();   /* khôi phục terminal, màu và con trỏ */
    return 0;
}
