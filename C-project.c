#include <stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <stdbool.h>
#include <limits.h>

// 테트리스 보드의 크기
#define Tetris_height 20
#define Tetris_width 20

// 키보드 값
#define LEFT 75 // 좌로 이동
#define RIGHT 77 // 우로 이동
#define UP 72 // 회전
#define DOWN 80 // 천천히 내려오기
#define SPACE 32 // 빠르게 내려오기
#define p 112 //일시정지 
#define P 80 //일시정지
#define ESC 27 //게임종료 

//보드의 크기
//세로 + 1, 가로 + 2 = 경계선을 위해서
long board[Tetris_height + 1][Tetris_width + 2] = { 0 };
int block[][4][4];

//벽
void Tetris_wall() {
    // 위, 아래 가로벽
    for (int i = 0; i < Tetris_width + 2; i++) {
        board[0][i] = board[Tetris_height][i] = '▩';
    }
    // 안쪽의 공백
    for (int i = 1; i < Tetris_height + 1; i++) {
        for (int j = 1; j < Tetris_width; j++) {
            board[i][j] = ' ';
        }
    }
    // 좌,우 세로벽
    for (int i = 1; i < Tetris_height + 1; i++) {
        board[i][0] = board[i][Tetris_width - 1] = '▩';
    }
}

// 키보드를 눌렀는지 안눌렀는지 확인해주는 함수
void kbhit() {

}

// 이 함수는 어디서부터 출력을 할지 결정하는 함수
// gotoxy 함수는 계속 화면을 초기화하면서 1초마다 깜빡거리는 현상을 없애준다.
void gotoxy(int x, int y)  //이 함수도 외우기 힘드실 테니 당분간 복붙으로 쓰세요!!
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 커서를 숨길 수 있는 함수, 함수의 인자로 1이 전달 되면 커서가 나타나고, 함수의 인자로 0이 나타나면 커서가 사라진다
// 블럭을 지우고 다시 그리고 하는 형식
void CursorView(char show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}


/*
char board[Tetris_height][Tetris_width] = {0}; // 테트리스 보드의 크기

int score; // 줄을 없앨 때마다 점수


//벽 모양 만들기

void Tetris_wall() {
    //위,아래 가로벽
    for (int i = 0; i < Tetris_width; i++) {
        board[0][i] = board[Tetris_height - 1][i] = '▩';
    }

    // 안쪽 공백
    for (int i = 1; i < Tetris_height - 1; i++) {
        for (int j = 1; j < Tetris_width - 1; j++) {
            board[i][j] = ' ';
        }
    }
        // 좌,우 세로벽
        for (int i = 0; i < Tetris_height; i++) {
            board[i][0] = board[i][Tetris_width - 1] = '▩';
        }

}

// 게임판 화면 출력
void display() {
    for (int i = 0; i < Tetris_height; i++) {
        for (int j = 0; j < Tetris_width; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}
*/


int main(void) {

    Tetris_wall();

    return 0;
}
