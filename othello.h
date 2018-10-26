/*
*	�I�Z��	�w�b�_�[�t�@�C��	othello.h
*
*	����28�N6��19��	���]�H�ƍ������w�Z�@J1318	���c����
*/

/* �y�}�N����`�z */
//�\���f�o�b�O
//#define DEBUG
//�p�X�f�o�b�O
//#define DEBUG_CPASS
//#define DEBUG_PPASS
//�Ֆʏ��
#define SIDE 8 // ��ӂ̒��� 
#define ASIDE (SIDE + 2) // �ǖʗp�z��̈�� �ɏՒn�ѕ��Q�𑫂� Side for array, add 2
#define BOARDSIZE (ASIDE * ASIDE) // �{�[�h�̑傫���@Size of board
#define UP (-ASIDE)
#define DOWN ASIDE
#define RIGHT 1
#define LEFT (-1)
// Piece number
#define B 1 // BLACK 1
#define W 2 // WHITE 2
#define N 3 // �ԕ� Sentinel (out of board) 3
#define BLACK_TURN 0 
#define WHITE_TURN 1
#define PASSMOVE 99
#define MOVENUM 32
//���W����X���W�܂���Y���W���v�Z����}�N��
#define X(pos) (pos % ASIDE)
#define Y(pos) (pos / ASIDE)
#define TURNCOLOR( turn ) (turn + 1)
#define OPPONENT(turn) !turn
//true, false
#define TRUE 1
#define FALSE 0
//AI
#define INFINITY 1000 //�\���傫�����𖳌���Ƃ��Ĉ���
#define SEARCH_LIMIT_DEPTH 128 //�T���[���̏��
//MODE
#define MODE_NORMAL 0
#define MODE_HANDICAP 1
#define MODE_WEAK 2
#define MODE_REVOLUTION 3
//GUI
#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

typedef unsigned char Move;		//�肾�Ƃ킩��₷�����邽�ߌ^��錾

/* �y�֐��v���g�^�C�v�錾�z */
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