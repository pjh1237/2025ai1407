#include <stdio.h>

int main() {
    // ���� ��ǥ�� (1,1)�� ����                             
    printf("#");

    // ���簢�� ũ��
    int width = 6;
    int height = 5;

    // ������ ��ǥ�� '#' ���
    for (int y = 1; y <= height; y++) {
        for (int x = 1; x <= width; x++) {
            // (1,1)�� �̹� ��������Ƿ� �ǳʶ�
            if (x == 1 && y == 1) continue;

            // �׵θ� ����: ù��, ��������, ùĭ, ������ĭ
            if (y == 1 || y == height || x == 1 || x == width) {
                // ANSI escape �ڵ�� Ŀ�� �̵� �� '#' ���
                printf("\033[%d;%dH#", y, x);
            }
        }
    }

    // ��� �� Ŀ���� ������ �� �Ʒ��� �Ű� ����ϰ� ����
    printf("\033[%d;1H", height + 1);

    return 0;
}
