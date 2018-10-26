/*
*	オセロ	ヘッダーファイル	othello.h
*
*	平成28年6月19日	松江工業高等専門学校　J1318	高田亮介
*/

/* 【マクロ定義】 */
//表示デバッグ
//#define DEBUG
//パスデバッグ
//#define DEBUG_CPASS
//#define DEBUG_PPASS
//盤面情報
#define SIDE 8 // 一辺の長さ 
#define ASIDE (SIDE + 2) // 局面用配列の一辺 緩衝地帯分２を足す Side for array, add 2
#define BOARDSIZE (ASIDE * ASIDE) // ボードの大きさ　Size of board
#define UP (-ASIDE)
#define DOWN ASIDE
#define RIGHT 1
#define LEFT (-1)
// Piece number
#define B 1 // BLACK 1
#define W 2 // WHITE 2
#define N 3 // 番兵 Sentinel (out of board) 3
#define BLACK_TURN 0 
#define WHITE_TURN 1
#define PASSMOVE 99
#define MOVENUM 32
//座標からX座標またはY座標を計算するマクロ
#define X(pos) (pos % ASIDE)
#define Y(pos) (pos / ASIDE)
#define TURNCOLOR( turn ) (turn + 1)
#define OPPONENT(turn) !turn
//true, false
#define TRUE 1
#define FALSE 0
//AI
#define INFINITY 1000 //十分大きい数を無限大として扱う
#define SEARCH_LIMIT_DEPTH 128 //探索深さの上限
//MODE
#define MODE_NORMAL 0
#define MODE_HANDICAP 1
#define MODE_WEAK 2
#define MODE_REVOLUTION 3
//GUI
#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

typedef unsigned char Move;		//手だとわかりやすくするため型を宣言

/* 【関数プロトタイプ宣言】 */
int getposition( int, int );
int isLegalMove( Move );
int generateMoves( Move * );
int isTerminalNode();
int getTerminalValue();
int getEvaluationValue();
int Revolution( int, int );
void makeMove( Move, int );
void unmakeMove( int );
int search( int );
void comPlayer();
void randPlayer();
void init();

void InitGrp();
void InitSE();
void WaitDisp();
void Title();
int GetPosfromMouse();
void manPlayerGUI();
void ShowBoard();