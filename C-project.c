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

typedef struct _tetris {
    int level; // 게임의 레벨
    int gameover; // 게임오버의 유무
    int score; // 점수
    int rotation; // 테트리스의 도형의 현재 방향
    int absX, absY; //도형의 절대 좌표
    int curX, curY; //도형의 현재 좌표
    int curblock; //현재 블럭
    int nextblockl; //다음 불럭
}Tetris;

/
//키를 입력받는 함수 
//getch 함수는 scanf 함수와 달리 입력하는 동시에 반응을 하는 함수이다.
char key;
key = getch(); cs


/*
//kbhit 함수
//kbhit 함수는 키보드가 눌렸는지 판단하는 함수이다. 눌렸으면 1 아니면 0 반환
while(kbhit){
    key = getch();
    swich(key) {
        case(up);
        .......
        .......
    if(key == SPACEBAR){
        //스페이스바는 if를 쓰는 이유는 스위치 문에 넣으면 방향키와 스페이스바를 동시에
        //사용이 불가능.
    }
    }
}
*/


// 커서 좌표 구조체
// COORD 자료형은 Windows.h 에 내장
// 정말 간단하게 x좌표와 y좌표를 저장할 수 있게 정의 되어 있음.
typedef struct _COORD{
    short x;
    short y;
}COORD;


// 커서를 이동시키는 함수 
void setCursor(int x, int y){
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// SetConsoleCursorPosition = 콘솔 커서 위치 지정 함수
// 표준 출력 핸들을 가져와서 x와 y 값을 넣은 COORD형 변수 pos 위치에 커서 위치를 지정


// 위의 함수를 사용하면 화면이 업데이트 될 때 (도형이 움직이거나) 커서가 보이게 된다.
void removeCursor() {
    CONSOLE_CURSOR_INFO curinfo;
    curinfo.bVisible = 0;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curinfo);
}
// 표준 출력 핸들을 가져오는데 CONSOLE_CURSOR_INFO 형식의 curinfo의 주소를 가져와서 커서를 안보이게 하겠다.


// 도형의 모양

int block[7][4][4][4] = {

    // ㄴ 모양
        0,0,0,0,
        0,1,0,0,
        0,1,1,1,
        0,0,0,0,

        0,0,0,0,
        0,1,1,0,
        0,1,0,0,
        0,1,0,0,

        0,0,0,0,
        1,1,1,0,
        0,0,1,0,
        0,0,0,0,

        0,0,1,0,
        0,0,1,0,
        0,1,1,0,
        0,0,0,0,

        // ㅡ 모양
        0,0,0,0,
        0,0,0,0,
        1,1,1,1,
        0,0,0,0,

        0,1,0,0,
        0,1,0,0,
        0,1,0,0,
        0,1,0,0,

        0,0,0,0,
        1,1,1,1,
        0,0,0,0,
        0,0,0,0,

        0,0,1,0,
        0,0,1,0,
        0,0,1,0,
        0,0,1,0,


    // ㅁ 모양

        0,0,0,0
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,


    // . 모양

        0,0,0,0,
        0,0,0,0,
        0,0,1,0,
        0,0,0,0,


    //  ㅗ 모양

        0,0,0,0,
        0,0,1,0,
        0,1,1,1,
        0,0,0,0,

        0,0,0,0,
        0,0,1,0,
        0,0,1,1,
        0,0,1,0,

        0,0,0,0,
        0,0,0,0,
        0,1,1,1,
        0,0,1,0,

        0,0,0,0,
        0,0,1,0,
        0,1,1,0,
        0,0,1,0,


    // -
    //  ㅣ
    //   - 모양

        0,0,0,0,
        0,1,1,0,
        0,0,1,1,
        0,0,0,0,

        0,0,0,0,
        0,0,0,1,
        0,0,1,1,
        0,0,1,0,

};










/*
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



int main(void) {

    Tetris_wall();

    return 0;
}
*/
