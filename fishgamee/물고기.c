#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <errno.h>

struct Fish
{
    char* name;
    char* talk;     // 물고기가 퀴즈를 냄
    int answer;     // 정답 번호
    char* talk_O;   // 맞았을 때 대사
    char* talk_X;   // 틀렸을 때 대사
};

// log.txt에서 기존 통계 읽기
void getGameStats(int* correctCount, int* totalGames)
{
    FILE* fp = fopen("log.txt", "r");
    if (fp == NULL) {
        *correctCount = 0;
        *totalGames = 0;
        return;
    }

    char line[256];
    int totalLines = 0;
    int correct = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == 'O') correct++;
        totalLines++;
    }

    fclose(fp);

    *totalGames = totalLines / 2;
    *correctCount = correct;
}

int main()
{
    struct Fish fishes[5] = {
        { "새우", "Q.새우요리중 제일 맛있는 것은?\n1) 생새우  2) 새우구이  3) 새우튀김\n", 3,
          "정답! 새우튀김은 goat야...", " 땡! 새우튀김이 제일 맛있어 물론 내 취향" },
        { "도미", "Q. 도미는 어떤 동물일까요?\n1) 포유류  2) 조류  3) 어류\n", 3,
          "정답! 도미는 어류야!", "땡! 도미는 어류야! 넌 상식을 모르는구나?" },
        { "광어", "Q. 우유가 넘어지면?\n1) 아야  2)터진다  3)상처를 입는다\n", 1,
          "정답! 답은 아야입니다!", "땡! 답은 아야입니다!" },
        { "연어", "Q. 왕이 작별인사를 하면?\n1) 사요나라  2) 바이킹  3) 잘가..\n", 2,
          "정답! 답은 바이킹! 센스있네!", "땡! 답은 바이킹! 센스없다!" },
        { "참치", "Q. 멀티미디어 공학과의 새로운 이름은?\n1) 멀티테스킹어쩌구  2) 미디어커뮤니케이션어쩌구  3) AI어쩌구\n", 3,
          "정답! 놀랍게도 정답은 AI 어쩌구! 이름 끝까지 모르긴해!", "땡! 정답은 AI어쩌구! 나도 사실 끝까지는 몰라ㅋ" }
    };

    int t, a = 0;
    int prevCorrect = 0, prevTotal = 0;
    getGameStats(&prevCorrect, &prevTotal);

    printf("%d번째 게임입니다!! (현재 정답률: %d / %d)\n",
        prevTotal + 1, prevCorrect, prevTotal);

    while (a < 1) {
        printf("찌를 던진다 (Y/N) > ");
        char yn = _getch();
        printf("%c\n", yn);

        if (yn == 'Y' || yn == 'y') {
            for (t = 0; t < 5; t++) {
                printf(". ");
                Sleep(500);
            }
            printf("\n\033[32m입질이 왔다!!!!\033[0m\n");

            srand((unsigned int)time(NULL));
            int r = rand() % 5;
            printf("%s(이)가 말을 건다!\n", fishes[r].name);
            printf("%s", fishes[r].talk);

            int user_answer;
            printf("답 입력 > ");
            scanf("%d", &user_answer);

            int wasCorrect = 0;
            if (user_answer == fishes[r].answer) {
                printf("%s\n", fishes[r].talk_O);
                wasCorrect = 1;
            }
            else {
                printf("%s\n", fishes[r].talk_X);
                wasCorrect = 0;
            }

            FILE* fp = fopen("log.txt", "a");
            if (fp == NULL) {
                perror("log.txt 파일을 열 수 없습니다");
            }
            else {
                fprintf(fp, "%s\n", fishes[r].name);
                fprintf(fp, "%s\n", (wasCorrect ? "O" : "X"));
                fclose(fp);
            }

            a++;
        }
        else if (yn == 'n' || yn == 'N') {
            printf("다시 생각해봐.....물고기를 낚아야지\n");
            
        }
        else {
            printf("Y키나 N키를 눌러야지.....\n");
           
        }
    }

    return 0;
}
