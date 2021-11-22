#include <stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <stdbool.h>
#include <limits.h>

// 테트리스 보드의 크기
#define Tetris_height 22
#define Tetris_width 14

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

int gameboard[Tetris_height + 2][Tetris_width + 2];


/*
//kbhit 함수
//kbhit 함수는 키보드가 눌렸는지 판단하는 함수이다. 눌렸으면 1 아니면 0 반환
//키를 입력받는 함수
//getch 함수는 scanf 함수와 달리 입력하는 동시에 반응을 하는 함수이다.
    char key;
    key = getch();
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

/*
// 커서 좌표 구조체
// COORD 자료형은 Windows.h 에 내장
// 정말 간단하게 x좌표와 y좌표를 저장할 수 있게 정의 되어 있음.
typedef struct _COORD {
    int x;
    int y;
}COORD;
*/

// 커서를 이동시키는 함수 
void setCursor(int x, int y) {
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


// 도형의 모양과 회전
// [도형의 종류][도형의 회전 개수][도형의 세로 최대 길이][도형의 가로 최대 길이]
int block[6][4][4][4] = {

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

        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,

        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,

        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,

        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,

        // . 모양

        0,0,0,0,
        0,0,0,0,
        0,0,1,0,
        0,0,0,0,

        0,0,0,0,
        0,0,0,0,
        0,1,0,0,
        0,0,0,0,

        0,0,0,0,
        0,1,0,0,
        0,0,0,0,
        0,0,0,0,

        0,0,0,0,
        0,0,1,0,
        0,0,0,0,
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

        0,0,0,0,
        0,0,0,0,
        0,1,1,0,
        0,0,1,1,

        0,0,0,0,
        0,0,1,0,
        0,1,1,0,
        0,1,0,0,
};


// 버튼을 눌렀을 때 배열을 회전 시켜 도형을 돌아가게 하는 함수

// 그렸던 도형을 제거해주는 함수
// 테트리스의 도형이 아래로 한칸씩 떨어지면서 지나간 자리에 도형이 남겨지게 되는 것을 없애줌
// 도형이 있던 부분 curX absX, curY + absY를 기점으로 " "으로 채우는 기능을 하는 함수
// 도형이 아래로 떨어질때, 한 라인이 채워질 때, 도형이 회전할 때 호출
void removeCurrentBlock(Tetris t)
{
    for (int i = 0; i < 4; i++);
        for (int j = 0; j < 4; j++);
            if (block[t.curblock][t.rotation][j][i] == 1)
            {
                setCursor(i + t.curX) * 2 + t.absX, j + t.curY + t.absY);
                print(" ");
            }
}

// gameboard 벽
// FRAME_HEIGHT = 22, FRAME_WIDTH = 15 즉 y == FRAME_HEIGHT - 1, y = 21일 때 바닥이 된다
// x == 0이거나 x == FRAME_WIDTH - 1, x가 0이거나 x가 13일 때 벽이 된다
void init(Tetris* t) {
    t->absX = 5; //16
    t->absY = 1; //3
    t->gameover = 0;
    t->level = 1;
    t->score = 0;
    t->curblock = -1;
    t->nextblock = -1;
    t->curX = 5;
    t->curY = -3;
    t->rotation = 0;
    srand((unsigned)time(NULL));
    for (int y = 0; y < FRAME_HEIGHT; y++) {
        for (int x = 0; x < FRAME_WIDTH; x++) {
            if ((x == 0) || (x == FRAME_WIDTH - 1))
                gameboard[y][x] = 1;
            else if (y == FRAME_HEIGHT - 1)
                gameboard[y][x] = 1;
            else
                gameboard[y][x] = 0;
        }
    }
}

// 도형과 충돌할 때 도형이 바닥에 닿을경우"벽이나 바닥으로 인식"
// 현재 t 구조체의 curY와 curX에 대하여 gameboard 좌표 상에 blocks을 추가 시키는 것
// 이후부터는 합쳐진 블럭은 gameboard상에서 벽 또는 바닥으로 인식하게 된다.

void mergeBlock(Tetris t)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gameboard[t.curY + i][t.curX + j] |= blocks[t.curblock][t.rotation][i][j];
        }
    }
    lineCheck(t);
    showTable(t);
}


int main() {

}
