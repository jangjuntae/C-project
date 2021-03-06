#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

// 테트리스 보드의 크기
#define Tetris_height 22
#define Tetris_width 14

// 키보드 값
#define LEFT 75 // 좌로 이동
#define RIGHT 77 // 우로 이동
#define UP 72 // 회전
#define DOWN 80 // 천천히 내려오기
#define SPACE 32 // 빠르게 내려오기
#define SPEED 600 // 속도

// 구조체
typedef struct _tetris {
    int level; // 게임의 레벨
    int gameover; // 게임오버의 유무
    int score; // 점수
    int rotation; // 테트리스의 도형의 현재 방향
    int absX, absY; //도형의 절대 좌표
    int curX, curY; //도형의 현재 좌표
    int curblock; //현재 블럭
    int nextblock; // 다음 블럭
}Tetris;

int gameboard[Tetris_height + 2][Tetris_width + 2];

// 함수 재정의
void removeCursor();
void setCursor(int x, int y);
void init(Tetris* t);
void showCurrentBlock(Tetris t);
void removeCurrentBlock(Tetris t);
void showTable(Tetris t);
void nextBlockTable(Tetris t);
void newBlock(Tetris* t);
int collisionCheck(Tetris t);
void mergeBlock(Tetris t);
int moveBlock(Tetris* t);
void gameoverCheck();
void lineCheck(Tetris t);

// 도형은 4차원 배열로 선언한다.
// 도형의 모양과 회전
// [도형의 개수(종류)][도형의 회전 개수][도형의 세로 길이][도형의 가로 길이]
int block[7][4][4][4] = {

    // ㄴ 반대
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

// 커서를 이동시키는 함수 
// SetConsoleCursorPosition = 콘솔 커서 위치 지정 함수
// 표준 출력 핸들을 가져와서 x와 y 값을 넣은 COORD형 변수(window.h에 내장) pos 위치에 커서 위치를 지정
void setCursor(int x, int y) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// setCursor 함수를 사용하면 화면이 업데이트 될 때 (도형이 움직이거나) 커서가 보이게 된다.
// 표준 출력 핸들을 가져오는데 CONSOLE_CURSOR_INFO 형식의 curinfo의 주소를 가져와서 커서를 안보이게 하겠다.
// window.h에 내장
void removeCursor() {
    CONSOLE_CURSOR_INFO curinfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curinfo);
    curinfo.bVisible = 0;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curinfo);
}

// Tetris_height = 22, Tetris_width = 15 즉 y == Tetris_height - 1, y = 21일 때 바닥이 된다.
// x == 0이거나 x == Tetris_width - 1, x가 0이거나 x가 13일 때 벽이 된다.
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
    for (int y = 0; y < Tetris_height; y++) {
        for (int x = 0; x < Tetris_width; x++) {
            if ((x == 0) || (x == Tetris_width - 1))
                gameboard[y][x] = 1;
            else if (y == Tetris_height - 1)
                gameboard[y][x] = 1;
            else
                gameboard[y][x] = 0;
        }
    }
}

// 현재 블럭이 생성돼서 위에서 아래로 떨어질 때
// 다음에 생성될 블럭을 보여주도록 도와주는 함수
void showCurrentBlock(Tetris t)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((j + t.curY) < 0)
                continue;
            if (block[t.curblock][t.rotation][j][i] == 1)
            {
                setCursor((i + t.curX) * 2 + t.absX, j + t.curY + t.absY);
                printf("■");
            }
        }
    }
    setCursor(77, 23);
}

// 그렸던 도형을 제거해주는 함수
// 테트리스의 도형이 아래로 한칸씩 떨어지면서 지나간 자리에 도형이 남겨지게 되는 것을 없애줌
// 도형이 있던 부분 curX absX, curY + absY를 기점으로 " "으로 채우는 기능을 하는 함수
// 도형이 아래로 떨어질때, 한 라인이 채워질 때, 도형이 회전할 때 호출
void removeCurrentBlock(Tetris t)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (block[t.curblock][t.rotation][j][i] == 1)
            {
                setCursor((i + t.curX) * 2 + t.absX, j + t.curY + t.absY);
                printf("  ");
            }
}

// 테트리스의 gameboard의 실체화
void showTable(Tetris t)
{
    for (int y = 0; y < Tetris_height; y++)
    {
        for (int x = 0; x < Tetris_width; x++)
        {
            setCursor((x * 2) + t.absX, y + t.absY);
            if (gameboard[y][x] == 1)
                printf("■");
            else
                printf("  ");
        }
    }
    setCursor(77, 23);
}

// 랜덤함수를 사용해서 도형을 무작위로 나오게 하기
// 랜덤 범위는 도형이 7개이니 0 ~ 6이 나와야해서 % 7
void newBlock(Tetris* t)
{
    int rot = rand() % 7;
    if (t->curblock == -1) {
        t->curblock = rot;
        rot = rand() % 7;
        t->nextblock = rot;
    }
    else
        t->nextblock = rot;
    nextBlockTable(*t);
}

// 다음 블록을 보여주는 테이블
void nextBlockTable(Tetris t)
{
    Tetris tmp = t;
    tmp.rotation = 0;
    tmp.curX = 14;
    tmp.curY = 1;
    tmp.curblock = tmp.nextblock;
    for (int i = 0; i < 6; i++)
    {
        setCursor(31, i + 1);
        for (int j = 0; j < 6; j++)
        {
            if (i == 0 || i == 5 || j == 0 || j == 5)
                printf("■");
            else
                printf("  ");
        }
    }
    showCurrentBlock(tmp);
}

// 충돌을 확인하는 함수
// 미리 판단
// 현재 블록의 부분이 1이고, 현재 좌표에 비례해서 이 블록의 다음 y좌표의 gameboard값이 1일 때
// 이 블록의 다음 y좌표의 gameboard의 값이 벽이거나 바닥일 때 1을 반환하고, 
// 벽이나 바닥에 부딛히지 않았다면 0을 반환합니다.
// 즉, 현재 도형의 다음 라인이 1일 경우 멈추게 하는 함수
int collisionCheck(Tetris t)
{
    int dat = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (((t.curX + j) == 0) || ((t.curX + j) == Tetris_width - 1))
                dat = 1;
            else
                dat = gameboard[t.curY + i][t.curX + j];
            if ((block[t.curblock][t.rotation][i][j] == 1) && (dat == 1))
                return 1;
        }
    }
    return 0;
}


// 도형이 바닥에 닿을 경우 벽이나 바닥으로 인식하는 함수
// 현재 t 구조체의 curY curX에 대하여 gameboard 좌표상에 block 추가 시킨다
// 이후부터는 합쳐진 블럭은 gameboard상에서 벽 또는 바닥으로 인식하게 됨
void mergeBlock(Tetris t)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gameboard[t.curY + i][t.curX + j] |= block[t.curblock][t.rotation][i][j];
        }
    }
    lineCheck(t);
    showTable(t);
}

// 인식된 키에 따라 반응하는 함수
// 블럭이 움직이게 해주는 함수를 만들고, 블럭의 그림을 제거해주는
// "removeCurrentBlock"을 호출해주고 curY의 값을 1 증가시켜준다.
// 또한 충돌했다면 새로 블럭을 생성하도록 초기화해주는 과정이 들어갔다.
// 만약에 충돌했는데 Y가 0보다 작다, 즉 블럭이 꼭대기에 닿았을 경우에 1을 반환하고
// 그렇지 않으면 2 또는 충돌을 안했다면 0을 반환한다.
// 즉, 1을 반환하면 게임 오버
int moveBlock(Tetris* t)
{
    removeCurrentBlock(*t);

    t->curY++;	//블럭을 한칸 아래로 내림
    if (collisionCheck(*t) == 1)
    {
        if (t->curY <= 0)
            return 1;
        t->curY--;
        mergeBlock(*t);
        t->curblock = t->nextblock;
        newBlock(t);
        t->curX = 5;
        t->curY = -3;
        t->rotation = 0;
        nextBlockTable(*t);
        return 2;
    }
    return 0;
}

// 줄이 꽉 찼을 때 인식하여 삭제하는 함수
// 내려오는 테트리스 조각과 이미 내려온 배열은 전부 하나의 배열이다.
// 항상 1줄만 사라지는 것이 아니라 2줄 이상을 제거할 수 있다. -> 따라서 특정 라인이 아닌 전체 라인을 확인
// 라인을 체크할 때 가로의 길이 - 2 부분을 체크해야함(테트리스의 테두리 제외)
// 한 라인의 모든 요소가 1이 됐을 때 0으로 바꾸어줘서 제거한다.

void lineCheck(Tetris t)
{
    // 앞에 j를 미리 선언하는 이유는 j의 경우에는 다른 부분에서 중복되게 체크해야 되기 때문
    // i와 k는 선언을 하는김에 미리 선언
    int i, j, k;
    for (i = 0; i < Tetris_height - 1; i++)
    {
        // j가 1부터 시작하는 이유는 라인의 첫번째는 테트리스의 테두리
        // j의 끝 부분이 Tetris_width - 1인 이유는 라인의 마지막이 테트리스의 테투리이기 때문
        for (j = 1; j < Tetris_width - 1; j++)
        {
            // 라인의 한 요소라도 0이라면 for문 전체를 탈출
            // 즉 라인이 채워지지 않아서 제거가 되지 않음
            if (gameboard[i][j] == 0)
                break;
        }

        // 라인의 한 요소라도 0이 아닐 경우 즉, 라인이 모두 채워졌을 경우 실행
        if (j == Tetris_width - 1)	//한줄이 다 채워졌음
        {
            showTable(t);

            // 라인을 0으로 만들고 공백으로 채운다
            setCursor(1 * 2 + t.absX, i + t.absY);
            for (j = 1; j < Tetris_width - 1; j++)
            {
                printf("  ");
                Sleep(10);
            }

            // 제거되지 않은 라인은 전부 한킨씩 내린다
            for (k = i; k > 0; k--)
            {
                for (j = 1; j < Tetris_width - 1; j++)
                    gameboard[k][j] = gameboard[k - 1][j];
            }

            // 가장 위 라인은 빈 부분으로 둔다
            // 도형이 생성되는 부분
            for (j = 1; j < Tetris_width - 1; j++)
                gameboard[0][j] = 0;
            t.score += 100 + (t.level * 10) + (rand() % 10);
        }
    }
}

int main() {
    Tetris t, tmpT;
    char key;
    init(&t);
    while (1) {
        removeCursor();
        showTable(t);
        newBlock(&t);
        nextBlockTable(t);
        while (1) {
            // kbhit() 함수는 키보드가 눌렸는지 판단하는 함수
            // 키보드가 눌렸으면 1, 아니면 0
            while (-_kbhit()) {
                removeCursor();
                key = _getch(); // scanf와 달리 입력 받는 동시에 반응을 하는 함수
                switch (key) {
                case UP: // 위 키를 눌렀을 때
                    tmpT = t;
                    tmpT.rotation = (tmpT.rotation + 1) % 4; // 도형의 회전 개수 4개(0 ~ 3)
                    if (collisionCheck(tmpT) == 0) {
                        removeCurrentBlock(t);
                        t.rotation = tmpT.rotation;
                        showCurrentBlock(t);
                    }
                    break;
                case DOWN: // 아래 키를 눌렀을 때
                    t.gameover = moveBlock(&t);
                    showCurrentBlock(t);
                    break;
                case LEFT: // 왼쪽 키를 눌렀을 때
                    if (t.curX > 0) {
                        removeCurrentBlock(t);
                        t.curX--;
                        if (collisionCheck(t) == 1)
                            t.curX++;
                        showCurrentBlock(t);
                    }
                    break;
                case RIGHT: // 오른쪽 키를 눌렀을 때
                    if (t.curX < Tetris_height) {
                        removeCurrentBlock(t);
                        t.curX++;
                        if (collisionCheck(t) == 1)
                            t.curX--;
                        showCurrentBlock(t);
                    }
                    break;
                }
                // 스페이스바는 swich문으로 안하는 이유는
                // swich문에 넣으면 방향키와 스페이스바를 동시에 사용이 불가능하기 때문에
                if (key == SPACE) // 아래 키를 눌렀을 때 
                {
                    while (t.gameover == 0)
                        t.gameover = moveBlock(&t);
                    showCurrentBlock(t);
                }

            }
            if (t.gameover == 1) {
                return 0;
            }
            while (!- _kbhit()) {
                showCurrentBlock(t);
                removeCursor();
                Sleep(SPEED - (t.level * 10));
                t.gameover = moveBlock(&t);
            }
        }
        init(&t);
    }
    system("pause");
    return 0;
}
