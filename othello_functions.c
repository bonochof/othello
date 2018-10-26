/*
*	�I�Z��	�֐��t�@�C��		othello_functions.c
*	
*	����28�N6��20��	���]�H�ƍ������w�Z�@J1318	���c����
*/

/* �y�C���N���[�h�t�@�C���z */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "othello.h"

/* �y�O���[�o���ϐ��z */
int mode;
//�\���Ɏg�������萔
const char* piece[4] = { "  ", "��", "��"};
const char* abc[8] = { "a","b","c","d","e","f","g","h"};
//��Ɋւ���ϐ�
int turn;		//���
int ply;		//�萔
Move nextmove;	//���̎� 
unsigned char stonenum[2];//�΂̐����L������ϐ�
//<AI>
int maxdepth;	//�T�����s���ő�[��
struct Position //�ߋ��̋ǖʂȂǂ��L�����邽�߂̍\����
{
	unsigned char board[BOARDSIZE];
	unsigned char stonenum[2];
};
struct Position history[SEARCH_LIMIT_DEPTH];
//�]���z��
//����
int evalboard0[BOARDSIZE] =
{
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  99, -20,  -5, -10, -10,  -5, -20,  99,   0,
   0, -20, -25,  -5,  -3,  -3,  -5, -25, -20,   0,
   0,  -5,  -5,  -1,  -1,  -1,  -1,  -5,  -5,   0,
   0, -10,  -3,  -1,  -1,  -1,  -1,  -3, -10,   0,
   0, -10,  -3,  -1,  -1,  -1,  -1,  -3, -10,   0,
   0,  -5,  -5,  -1,  -1,  -1,  -1,  -5,  -5,   0,
   0, -20, -25,  -6,  -3,  -3,  -5, -25, -20,   0,
   0,  99, -20,  -5, -10, -10,  -5, -20,  99,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
//�I��
int evalboard1[BOARDSIZE] =
{
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  99, -15,  20,   4,   4,  20, -15,  99,   0,
   0, -15, -15,  10,   4,   4,  10, -15, -15,   0,
   0,  20,  10,  15,   5,   5,  15,  10,  20,   0,
   0,   4,   4,   5,   5,   5,   5,   4,   4,   0,
   0,   4,   4,   5,   5,   5,   5,   4,   4,   0,
   0,  20,  10,  15,   5,   5,  15,  10,  20,   0,
   0, -15, -15,  10,   4,   4,  10, -15, -15,   0,
   0,  99, -15,  20,   4,   4,  20, -15,  99,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
//</AI>
//�Ֆ�
#if defined( DEBUG_CPASS )	//COM�p�X���ؗp
unsigned char tmp[BOARDSIZE] =
{
	N, N, N, N, N, N, N, N, N, N, 
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, W, W, W, W, W, W, 0, N,
	N, 0, W, W, W, W, W, W, 0, N,
	N, 0, W, 0, W, W, B, W, 0, N,
	N, 0, W, W, W, W, W, W, 0, N,
	N, 0, W, W, W, W, W, W, 0, N,
	N, 0, W, W, W, W, W, W, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, N, N, N, N, N, N, N, N, N 
};
#elif defined( DEBUG_PPASS )	//PLAYER�p�X���ؗp
unsigned char tmp[BOARDSIZE] =
{
	N, N, N, N, N, N, N, N, N, N, 
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, B, B, B, B, B, B, 0, N,
	N, 0, B, W, W, W, W, B, 0, N,
	N, 0, B, W, W, W, W, B, 0, N,
	N, 0, B, W, W, W, W, B, 0, N,
	N, 0, B, W, W, W, W, B, 0, N,
	N, 0, B, B, B, B, B, B, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, N, N, N, N, N, N, N, N, N 
};
#else
unsigned char tmp[BOARDSIZE] =
{
	N, N, N, N, N, N, N, N, N, N, 
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, B, W, 0, 0, 0, N,
	N, 0, 0, 0, W, B, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, N, N, N, N, N, N, N, N, N 
};
#endif
unsigned char board[BOARDSIZE];

/* �y�֐��z2�����̍��W���ꎟ���ɕϊ� */
int getposition( int x, int y )
{
	return y * ASIDE + x;
}

/* �y�֐��z���@�蔻�� */
int isLegalMove( Move pos )
{
	int dirx, diry, dir;
	int pos1;
	//�����̐F, ����̐F
	int color = TURNCOLOR( turn );
	int opponentcolor = TURNCOLOR( OPPONENT(turn) );

	//�󂫃}�X�`�F�b�N
	if( board[pos] > 0 )
	{
		return FALSE;
	}

	//pos�̎���8�}�X�𒲂ׂ�
	for( dirx = -1; dirx <= 1; dirx++ )
	{
		for( diry = -ASIDE; diry <= ASIDE; diry += ASIDE )
		{
			//���f�}�X�����v�Z
			dir = dirx + diry;

			//����0�͈Ӗ��Ȃ�
			if( dir == 0 )
			{
				continue;
			}

			//���f�}�X�ړ�
			pos1 = pos + dir;

			//�܂�����̐΂����邩���ׂ�
			if( board[pos1] != opponentcolor )
			{
				continue;
			}

			//����̐΂�����Ԃ͎��𒲂ׂ�
			do
			{
				pos1 += dir;
			}while( board[pos1] == opponentcolor );

			//�Ђ�����Ԃ����߂ɂ͍Ō�Ɏ����̐΂��Ȃ��Ƃ����Ȃ�
			if( board[pos1] != color )
			{
				continue;
			}

			//�Ō�܂ŗ����琬��
			return TRUE;
		}
	}

	return FALSE;	
}

/* �y�֐��z��̐��� */
int generateMoves( Move moves[] )	
{
	int num = 0;//�������鍇�@��̐�
	int pos;

	// ���ォ�珇�ɐ΂�u���A���@�肩���ׂ�
	for ( pos = 0; pos < BOARDSIZE; pos++ )
	{
		if ( isLegalMove( pos ) )
		{
			moves[num++] = pos;//num�Ԗڂ̔z���pos������
		}
	}
	return num;
}

/* �y�֐��z�Q�[���I���ǖ� */
int isTerminalNode()
{
	int num;
	Move moves[MOVENUM];

	//��Ԍ��
	turn = OPPONENT(turn);
	//��𐶐����Đ�������
	num = generateMoves( moves );
	//��Ԗ߂�
	turn = OPPONENT(turn);

	//���@�肪�Ȃ��ꍇ
	if ( num == 0 )
	{
		return TRUE;
	}
	return FALSE;
}

/* �y�֐��z�I���ǖʂ̕]���l */
int getTerminalValue()
{
	//�ΐ��̍����v�Z�@�����̐΁[����̐�
	int diff = stonenum[turn] - stonenum[OPPONENT(turn)];

	if( diff > 0 )
	{
		return INFINITY;	//����
	}
	else if( diff < 0 )
	{
		return -INFINITY;	//����
	}
	else
	{
		return 0;			//��������
	}
}

/* �y�֐��z�]���֐��̌v�Z */
int getEvaluationValue()
{
	int pos, value, c;
	Move moves[MOVENUM];
	// ���@�萔�̍���]���֐��Ƃ���(���R�x)
	//�����̍��@�萔�𑫂�
	value = generateMoves( moves );
	//��Ԉꎞ���
	turn = OPPONENT(turn);
	//����̍��@�萔������
	value -= generateMoves( moves );
	//��Ԗ߂�
	turn = OPPONENT(turn);

	value *= 20;//���R�x�P��20�_�Ƃ��Ă���
	for ( pos = 11; pos <= 88; pos++ )
	{
		c = board[pos];
		if ( c == 0 )
		{
			continue;
		}
		else if ( c == TURNCOLOR(turn) )
		{
			value += ( ply < 30 ? evalboard0[pos] : evalboard1[pos] );
		}
		else
		{
			value -= ( ply < 30 ? evalboard0[pos] : evalboard1[pos] );
		}
	}
	return value;
}

/* �y�֐��z�v�����[�h */
int Revolution( int num, int color )
{
	int count = 0;

	if( board[11] > 0 && num != 11 )
	{
		switch( board[11] )
		{
		case B:
			board[11] = W;
			break;
		case W:
			board[11] = B;
			break;
		}
		
		if( board[11] == color )
		{
			count++;
		}
		else
		{
			count--;
		}
	}
	if( board[18] > 0 && num != 18 )
	{
		switch( board[18] )
		{
		case B:
			board[18] = W;
			break;
		case W:
			board[18] = B;
			break;
		}
		
		if( board[18] == color )
		{
			count++;
		}
		else
		{
			count--;
		}
	}
	if( board[81] > 0 && num != 81 )
	{
		switch( board[81] )
		{
		case B:
			board[81] = W;
			break;
		case W:
			board[81] = B;
			break;
		}
		
		if( board[81] == color )
		{
			count++;
		}
		else
		{
			count--;
		}
	}
	if( board[88] > 0 && num != 88 )
	{
		switch( board[88] )
		{
		case B:
			board[88] = W;
			break;
		case W:
			board[88] = B;
			break;
		}
		
		if( board[88] == color )
		{
			count++;
		}
		else
		{
			count--;
		}
	}

	return count;
}

/* �y�֐��z���ۂɎ��i�߂� */
void makeMove( Move pos, int depth )
{
	int dirx, diry, dir;
	int pos1;
	//�����̐F, ����̐F
	int color = TURNCOLOR( turn );
	int opponentcolor = TURNCOLOR( OPPONENT(turn) );
	int count = 0;
	int rev_count = 0;

	//�ǖʂ�z��history�ɕۑ�
	memcpy( history[depth].board, board, sizeof( board ));
	memcpy( history[depth].stonenum, stonenum, sizeof( stonenum ));

	//�w�肳�ꂽ�ꏊ�ɐ΂�u��
	board[pos] = color;

	//pos�̎���8�}�X�𒲂ׂ�
	for( dirx = -1; dirx <= 1; dirx++ )
	{
		for( diry = -ASIDE; diry <= ASIDE; diry += ASIDE )
		{
			//���f�}�X�����v�Z
			dir = dirx + diry;

			//����0�͈Ӗ��Ȃ�
			if( dir == 0 )
			{
				continue;
			}

			//���f�}�X�ړ�
			pos1 = pos + dir;

			//�܂�����̐΂����邩���ׂ�
			if( board[pos1] != opponentcolor )
			{
				continue;
			}

			//����̐΂�����Ԃ͎��𒲂ׂ�
			do
			{
				pos1 += dir;
			}while( board[pos1] == opponentcolor );

			//�Ђ�����Ԃ����߂ɂ͍Ō�Ɏ����̐΂��Ȃ��Ƃ����Ȃ�
			if( board[pos1] != color )
			{
				continue;
			}

			//�Ђ�����Ԃ�
			do
			{
				pos1 -= dir;
				board[pos1] = color;
				count++;	//�J�E���g
			}while( pos1 != pos + dir );
			//�v�����[�h����
			if( mode == MODE_REVOLUTION && ( pos == 11 || pos == 18 || pos == 81 || pos == 88 ) )
			{
				rev_count = Revolution( pos, color );
			}
		}
	}

	//�΂̐��̃J�E���g
	stonenum[turn] += ( (count+1) + rev_count );
	stonenum[OPPONENT(turn)] -= ( count + rev_count );

	//��Ԍ��
	turn = OPPONENT(turn);
}

/* �y�֐��z���߂� */
void unmakeMove( int depth )
{
	memcpy( board, history[depth].board, sizeof( board ));
	memcpy( stonenum, history[depth].stonenum, sizeof( stonenum ));
	turn = OPPONENT(turn);
}

/* �y�֐��z�T�� */
int search( int depth )
{
	int i;
	int movenum;			//��̐�
	Move moves[MOVENUM];	//�������z�� an array of moves
	int value;
	int bestvalue = -INFINITY-1;	//�܂��ŏ��l������

	//�ċA�E�o
	if( depth >= maxdepth )
	{
		return getEvaluationValue();
	}

	//��𐶐�
	movenum = generateMoves( moves );
	if ( movenum == 0 )
	{
		if ( isTerminalNode() )// Game Over
		{
			return getTerminalValue();
		}
		else // �p�X
		{
			moves[movenum++] = PASSMOVE;
		}
	}
	for ( i = 0; i < movenum; i++ )
	{
		makeMove( moves[i], depth );//���i��
		//value = getEvaluationValue();
		value = -search( depth + 1 );	//NEGAMAX�ċA
		unmakeMove( depth );//���߂�
		//�ő傩�ǂ������ׁA�ő�Ȃ�nextmove�ɑ������
		if( value > bestvalue )
		{
			bestvalue = value;
			if( depth == 0 )
			{
				nextmove = moves[i];
			}
		}
	}
	return bestvalue;
}

/* �y�֐��zCOM�̎�𐶐� */
void comPlayer()
{
	int value;
	value = search(0);
}

/* �y�֐��z�����_���v���C���[ */
void randPlayer()
{
	int num;
	Move moves[MOVENUM];
	num = generateMoves( moves );

	//�p�X
	if( num == 0 )
	{
		nextmove = PASSMOVE;
	}
	//�p�X�ȊO
	else
	{
		nextmove = moves[ rand() % num ];
	}
}

/* �y�֐��z������ */
void init()
{
	int i, j;

	turn = BLACK_TURN;
	ply = 0;
#if defined( DEBUG_CPASS )
	stonenum[BLACK_TURN] = 1;
	stonenum[WHITE_TURN] = 34;
#elif defined( DEBUG_PPASS )
	stonenum[BLACK_TURN] = 20;
	stonenum[WHITE_TURN] = 16;
#else
	stonenum[BLACK_TURN] = 2;
	stonenum[WHITE_TURN] = 2;
#endif

	//�Ֆ�
	for( i = 0; i < BOARDSIZE; i++ )
	{
		board[i] = tmp[i];
	}

	if( mode == MODE_HANDICAP )
	{
		board[11] = W;
		board[18] = W;
		board[81] = W;
		board[88] = W;
	}
}