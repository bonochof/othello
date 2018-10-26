/*
*	オセロ	関数ファイル		othello_functions.c
*	
*	平成28年6月20日	松江工業高等専門学校　J1318	高田亮介
*/

/* 【インクルードファイル】 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "othello.h"

/* 【グローバル変数】 */
int mode;
//表示に使う文字定数
const char* piece[4] = { "  ", "●", "○"};
const char* abc[8] = { "a","b","c","d","e","f","g","h"};
//手に関する変数
int turn;		//手番
int ply;		//手数
Move nextmove;	//次の手 
unsigned char stonenum[2];//石の数を記憶する変数
//<AI>
int maxdepth;	//探索を行う最大深さ
struct Position //過去の局面などを記憶するための構造体
{
	unsigned char board[BOARDSIZE];
	unsigned char stonenum[2];
};
struct Position history[SEARCH_LIMIT_DEPTH];
//評価配列
//序盤
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
//終盤
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
//盤面
#if defined( DEBUG_CPASS )	//COMパス検証用
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
#elif defined( DEBUG_PPASS )	//PLAYERパス検証用
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

/* 【関数】2次元の座標を一次元に変換 */
int getposition( int x, int y )
{
	return y * ASIDE + x;
}

/* 【関数】合法手判定 */
int isLegalMove( Move pos )
{
	int dirx, diry, dir;
	int pos1;
	//自分の色, 相手の色
	int color = TURNCOLOR( turn );
	int opponentcolor = TURNCOLOR( OPPONENT(turn) );

	//空きマスチェック
	if( board[pos] > 0 )
	{
		return FALSE;
	}

	//posの周囲8マスを調べる
	for( dirx = -1; dirx <= 1; dirx++ )
	{
		for( diry = -ASIDE; diry <= ASIDE; diry += ASIDE )
		{
			//判断マス方向計算
			dir = dirx + diry;

			//方向0は意味なし
			if( dir == 0 )
			{
				continue;
			}

			//判断マス移動
			pos1 = pos + dir;

			//まず相手の石があるか調べる
			if( board[pos1] != opponentcolor )
			{
				continue;
			}

			//相手の石がある間は次を調べる
			do
			{
				pos1 += dir;
			}while( board[pos1] == opponentcolor );

			//ひっくり返すためには最後に自分の石がないといけない
			if( board[pos1] != color )
			{
				continue;
			}

			//最後まで来たら成功
			return TRUE;
		}
	}

	return FALSE;	
}

/* 【関数】手の生成 */
int generateMoves( Move moves[] )	
{
	int num = 0;//生成する合法手の数
	int pos;

	// 左上から順に石を置き、合法手か調べる
	for ( pos = 0; pos < BOARDSIZE; pos++ )
	{
		if ( isLegalMove( pos ) )
		{
			moves[num++] = pos;//num番目の配列にposを入れる
		}
	}
	return num;
}

/* 【関数】ゲーム終了局面 */
int isTerminalNode()
{
	int num;
	Move moves[MOVENUM];

	//手番交代
	turn = OPPONENT(turn);
	//手を生成して数を見る
	num = generateMoves( moves );
	//手番戻す
	turn = OPPONENT(turn);

	//合法手がない場合
	if ( num == 0 )
	{
		return TRUE;
	}
	return FALSE;
}

/* 【関数】終了局面の評価値 */
int getTerminalValue()
{
	//石数の差を計算　自分の石ー相手の石
	int diff = stonenum[turn] - stonenum[OPPONENT(turn)];

	if( diff > 0 )
	{
		return INFINITY;	//勝ち
	}
	else if( diff < 0 )
	{
		return -INFINITY;	//負け
	}
	else
	{
		return 0;			//引き分け
	}
}

/* 【関数】評価関数の計算 */
int getEvaluationValue()
{
	int pos, value, c;
	Move moves[MOVENUM];
	// 合法手数の差を評価関数とする(自由度)
	//自分の合法手数を足す
	value = generateMoves( moves );
	//手番一時交代
	turn = OPPONENT(turn);
	//相手の合法手数を引く
	value -= generateMoves( moves );
	//手番戻す
	turn = OPPONENT(turn);

	value *= 20;//自由度１を20点としておく
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

/* 【関数】革命モード */
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

/* 【関数】実際に手を進める */
void makeMove( Move pos, int depth )
{
	int dirx, diry, dir;
	int pos1;
	//自分の色, 相手の色
	int color = TURNCOLOR( turn );
	int opponentcolor = TURNCOLOR( OPPONENT(turn) );
	int count = 0;
	int rev_count = 0;

	//局面を配列historyに保存
	memcpy( history[depth].board, board, sizeof( board ));
	memcpy( history[depth].stonenum, stonenum, sizeof( stonenum ));

	//指定された場所に石を置く
	board[pos] = color;

	//posの周囲8マスを調べる
	for( dirx = -1; dirx <= 1; dirx++ )
	{
		for( diry = -ASIDE; diry <= ASIDE; diry += ASIDE )
		{
			//判断マス方向計算
			dir = dirx + diry;

			//方向0は意味なし
			if( dir == 0 )
			{
				continue;
			}

			//判断マス移動
			pos1 = pos + dir;

			//まず相手の石があるか調べる
			if( board[pos1] != opponentcolor )
			{
				continue;
			}

			//相手の石がある間は次を調べる
			do
			{
				pos1 += dir;
			}while( board[pos1] == opponentcolor );

			//ひっくり返すためには最後に自分の石がないといけない
			if( board[pos1] != color )
			{
				continue;
			}

			//ひっくり返す
			do
			{
				pos1 -= dir;
				board[pos1] = color;
				count++;	//カウント
			}while( pos1 != pos + dir );
			//革命モード処理
			if( mode == MODE_REVOLUTION && ( pos == 11 || pos == 18 || pos == 81 || pos == 88 ) )
			{
				rev_count = Revolution( pos, color );
			}
		}
	}

	//石の数のカウント
	stonenum[turn] += ( (count+1) + rev_count );
	stonenum[OPPONENT(turn)] -= ( count + rev_count );

	//手番交代
	turn = OPPONENT(turn);
}

/* 【関数】手を戻す */
void unmakeMove( int depth )
{
	memcpy( board, history[depth].board, sizeof( board ));
	memcpy( stonenum, history[depth].stonenum, sizeof( stonenum ));
	turn = OPPONENT(turn);
}

/* 【関数】探索 */
int search( int depth )
{
	int i;
	int movenum;			//手の数
	Move moves[MOVENUM];	//手を入れる配列 an array of moves
	int value;
	int bestvalue = -INFINITY-1;	//まず最小値を入れる

	//再帰脱出
	if( depth >= maxdepth )
	{
		return getEvaluationValue();
	}

	//手を生成
	movenum = generateMoves( moves );
	if ( movenum == 0 )
	{
		if ( isTerminalNode() )// Game Over
		{
			return getTerminalValue();
		}
		else // パス
		{
			moves[movenum++] = PASSMOVE;
		}
	}
	for ( i = 0; i < movenum; i++ )
	{
		makeMove( moves[i], depth );//一手進め
		//value = getEvaluationValue();
		value = -search( depth + 1 );	//NEGAMAX再帰
		unmakeMove( depth );//一手戻る
		//最大かどうか調べ、最大ならnextmoveに代入する
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

/* 【関数】COMの手を生成 */
void comPlayer()
{
	int value;
	value = search(0);
}

/* 【関数】ランダムプレイヤー */
void randPlayer()
{
	int num;
	Move moves[MOVENUM];
	num = generateMoves( moves );

	//パス
	if( num == 0 )
	{
		nextmove = PASSMOVE;
	}
	//パス以外
	else
	{
		nextmove = moves[ rand() % num ];
	}
}

/* 【関数】初期化 */
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

	//盤面
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