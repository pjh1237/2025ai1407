#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <math.h>

#define PI 3.14159226535897
#define MAXPTS 800

static HANDLE hOut = NULL;

// (x,y) 위치에 문자열 출력 (1-based 좌표)
void putStrAt(int x, int y, const char* s)
{
    COORD pos = { (SHORT)(x - 1), (SHORT)(y - 1) };
    SetConsoleCursorPosition(hOut, pos);
    DWORD written = 0;
    WriteConsoleA(hOut, s, (DWORD)strlen(s), &written, NULL);
}

// (x,y) 위치에 문자 하나 출력 (1-based)
void putCharAt(int x, int y, char ch)
{
    COORD pos = { (SHORT)(x - 1), (SHORT)(y - 1) };
    SetConsoleCursorPosition(hOut, pos);
    DWORD written = 0;
    WriteConsoleA(hOut, &ch, 1, &written, NULL);
}

// 폭탄 출력
// isExploded == 0 : "(  B  )"
// isExploded == 1 : 폭발 3줄 출력
void printBomb(int isExploded, int x, int y)
{
    if (!isExploded)
    {
        putStrAt(x, y, "(  B  )");
    }
    else
    {
        // 폭발 모양: 위/중/아래 3줄
        putStrAt(x, y - 1, "^^^^^^^");
        putStrAt(x, y, "!!BAM!!");
        putStrAt(x, y + 1, "^^^^^^^");
    }
}

// 콘솔 전체를 공백으로 채움
void clearConsole()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    COORD origin = { 0,0 };
    DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written = 0;
    FillConsoleOutputCharacterA(hOut, ' ', cells, origin, &written);
    SetConsoleCursorPosition(hOut, origin);
}

int main(void)
{
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return 1;

    // 콘솔 크기 확인 (폭발 출력 범위 체크에 필요)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return 1;
    int screenW = csbi.dwSize.X;
    int screenH = csbi.dwSize.Y;

    clearConsole();

    // 경로 저장 배열
    int xs[MAXPTS];
    int ys[MAXPTS];
    int idx = 0;

    // 시작 좌표 (요청대로)
    int startX = 14, startY = 9;
    int curX = startX, curY = startY;

    // 나선 규칙: down len, right len, up len+2, left len+2, cycles = 3
    int len = 2;
    const int cycles = 3;

    for (int cyc = 0; cyc < cycles && idx < MAXPTS; ++cyc)
    {
        // 아래로 len
        for (int s = 0; s < len && idx < MAXPTS; ++s) { curY += 1; xs[idx] = curX; ys[idx] = curY; idx++; }
        // 오른쪽 len
        for (int s = 0; s < len && idx < MAXPTS; ++s) { curX += 1; xs[idx] = curX; ys[idx] = curY; idx++; }
        // 위로 len+2
        for (int s = 0; s < len + 2 && idx < MAXPTS; ++s) { curY -= 1; xs[idx] = curX; ys[idx] = curY; idx++; }
        // 왼쪽 len+2
        for (int s = 0; s < len + 2 && idx < MAXPTS; ++s) { curX -= 1; xs[idx] = curX; ys[idx] = curY; idx++; }

        len += 4;
    }

    if (idx <= 0) {
        // 경로 생성 실패
        SetConsoleCursorPosition(hOut, (COORD) { 0, (SHORT)(screenH - 2) });
        printf("나선 경로 생성 실패\n");
        return 1;
    }

    int nHash = idx;

    // 나선의 마지막 좌표(여기에 '*'가 마지막으로 도달함)
    int lastHashX = xs[nHash - 1];
    int lastHashY = ys[nHash - 1];

    // 폭탄 배치: 폭탄의 오른쪽 괄호 ')' 가 lastHash 좌표와 같도록 조정
    // 폭탄은 7문자 폭이므로 bombX + 6 == lastHashX  => bombX = lastHashX - 6
    int bombX = lastHashX - 6;
    int bombY = lastHashY;

    // 화면 경계 체크 (폭발 표현을 위해 위/아래 공간도 확보)
    if (bombX < 1 || bombX + 6 > screenW || bombY - 1 < 1 || bombY + 1 > screenH) {
        SetConsoleCursorPosition(hOut, (COORD) { 0, (SHORT)(screenH - 2) });
        printf("콘솔 창이 너무 작거나 폭발이 화면 밖으로 나갑니다. 창을 넓힌 뒤 다시 실행하세요.\n");
        return 1;
    }

    // --- 초기 그리기 ---
    // 1) 나선 '#' 그리기 (모두)
    for (int i = 0; i < nHash; ++i)
    {
        putCharAt(xs[i], ys[i], '#');
    }

    // 2) 폭탄을 그린다. (나선 그린 뒤에 그리므로 폭탄 문자가 #을 덮어 보인다)
    printBomb(0, bombX, bombY);

    // 3) 시작 '*' 찍기
    putCharAt(startX, startY, '*');

    // --- 애니메이션: '*'가 경로를 따라 이동 ---
    int starX = startX, starY = startY;
    for (int i = 0; i < nHash; ++i)
    {
        int nextX = xs[i], nextY = ys[i];

        // 이전 별 위치 지움
        putCharAt(starX, starY, ' ');

        // 다음 위치에 '*' 출력 (이때 마지막 위치는 폭탄의 ')'자리에 해당)
        putCharAt(nextX, nextY, '*');

        // 화면 반영
        fflush(stdout);

        // 업데이트
        starX = nextX;
        starY = nextY;

        // 만약 지금 '*'이 폭탄의 ')' 위치라면 즉시 폭발 연출
        if (starX == bombX + 6 && starY == bombY)
        {
            // 짧게 딜레이 뒤 폭발
            Sleep(150); // 사용자가 볼 수 있게 아주 약간만 대기
            printBomb(1, bombX, bombY);
            fflush(stdout);
            break; // 루프 종료 (폭발 후 더 이동하지 않음)
        }

        Sleep(200);
    }   

  

    return 0;
}
