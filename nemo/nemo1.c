#include <stdio.h>

int main() {
    // 시작 좌표를 (1,1)로 잡음                             
    printf("#");

    // 직사각형 크기
    int width = 6;
    int height = 5;

    // 나머지 좌표에 '#' 찍기
    for (int y = 1; y <= height; y++) {
        for (int x = 1; x <= width; x++) {
            // (1,1)은 이미 출력했으므로 건너뜀
            if (x == 1 && y == 1) continue;

            // 테두리 조건: 첫줄, 마지막줄, 첫칸, 마지막칸
            if (y == 1 || y == height || x == 1 || x == width) {
                // ANSI escape 코드로 커서 이동 후 '#' 출력
                printf("\033[%d;%dH#", y, x);
            }
        }
    }

    // 출력 후 커서를 마지막 줄 아래로 옮겨 깔끔하게 정리
    printf("\033[%d;1H", height + 1);

    return 0;
}
