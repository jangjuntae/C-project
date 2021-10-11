#include <stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <stdbool.h>
#include <limits.h>

// 테트리스 보드의 크기
#define Tetris_height 20
#define Tetris_width 10

// 키보드 값
#define LEFT 75 // 좌로 이동
#define RIGHT 77 // 우로 이동
#define UP 72 // 회전
#define DOWN 80 // 천천히 내려오기
#define SPACE 32 // 빠르게 내려오기
#define p 112 //일시정지 
#define P 80 //일시정지
#define ESC 27 //게임종료 


char board[Tetris_height][Tetris_width]; // 테트리스 보드의 크기

int score; // 줄을 없앨 때마다 점수

//벽 모양 만들기
void Tetris_wall() {
    //위,아래 가로벽
    for (int i = 0; i < Tetris_height; i++) {
        board[0][i] = board[Tetris_width][i] = '▩';
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


int main(void) {

    Tetris_wall();
    display();
    while (0) {
        display();
    }
    return 0;
}
