/*
*	オセロ	GUI関係ファイル		othello_GUI.cpp
*	
*	平成28年6月20日	松江工業高等専門学校　J1318	高田亮介
*/

/* 【インクルードファイル】 */
#include "DxLib.h"
#include "othello.h"
#include "othello_functions.c"

/* 【構造体型】画像 */
struct GrpData
{
	int title, back;
	int scene0, scene1, scene2, scene3;
}grp;
struct SEData
{
	int next, win;
	int putCOM, putMAN;
}se;

/* 【グローバル変数】 */
int d = 50;				//マスの間隔
int r = d / 2;			//石の半径
int sx = 20, sy = 20;	//盤左上の座標
int ex = sx + d * SIDE;	//盤右下のx座標
int ey = sy + d * SIDE;	//盤右下のy座標
int rx = ex + 10;		//画面右　結果など表示するための場所ｘ座標
int STRCOLOR = GetColor( 0, 0, 200 );	//文字色
int STRFONTSIZE = 32;					//文字フォントサイズ
int exitFlag = 0;		//終了フラグ

/* 【関数】画像ファイル読み込み */
void InitGrp()
{
	SetTransColor( 255, 255, 255 );
	grp.title = LoadGraph( "Othello_data/Graphic/title.jpg" );
	grp.back = LoadGraph( "Othello_data/Graphic/back.jpg" );
	grp.scene0 = LoadGraph( "Othello_data/Graphic/scene0.png" );
	grp.scene1 = LoadGraph( "Othello_data/Graphic/scene1.png" );
	grp.scene2 = LoadGraph( "Othello_data/Graphic/scene2.png" );
	grp.scene3 = LoadGraph( "Othello_data/Graphic/scene3.png" );
}

/* 【関数】音ファイル読み込み */
void InitSE()
{
	se.next = LoadSoundMem( "Othello_data/Sound/next.wav" );
	se.win = LoadSoundMem( "Othello_data/Sound/win.wav" );
	se.putCOM = LoadSoundMem( "Othello_data/Sound/putCOM.wav" );
	se.putMAN = LoadSoundMem( "Othello_data/Sound/putMAN.wav" );
}

/* 【関数】待ち画面 */
void WaitDisp()
{
	while( !CheckHitKeyAll() )
	{
		//メッセージループに代わる処理
		if( ProcessMessage() == -1 )
		{
			break;	//エラーが起きたらループを抜ける
		}
	}
	if( CheckHitKey( KEY_INPUT_ESCAPE ) )
	{
		exitFlag = 1;
	}
	PlaySoundMem( se.next, DX_PLAYTYPE_NORMAL );
}

/* 【関数】タイトル表示 */
void Title()
{
	int scene = 0;
	int MouseX, MouseY, MouseInput, MouseFlag = 0;

	//背景
	DrawGraph( 0, 0, grp.title, FALSE );
	DrawGraph( 0, WINDOW_SIZE_Y / 2, grp.scene0, TRUE );
	WaitDisp();
	scene++;

	while( scene <= 3 )
	{
		//背景
		DrawGraph( 0, 0, grp.title, FALSE );

		//各シーン項目
		switch( scene )
		{
		case 1:
			DrawGraph( 0, WINDOW_SIZE_Y / 2, grp.scene1, TRUE );
			break;
		case 2:
			DrawGraph( 0, WINDOW_SIZE_Y / 2, grp.scene2, TRUE );
			break;
		case 3:
			DrawGraph( 0, WINDOW_SIZE_Y / 2, grp.scene3, TRUE );
			break;
		}

		//マウス入力
		GetMousePoint( &MouseX, &MouseY );
		MouseInput = GetMouseInput();

		if( ( MouseInput & MOUSE_INPUT_LEFT ) != 0 )	//クリック時
		{
			if( MouseFlag == 0 && MouseY > WINDOW_SIZE_Y / 2 )	//項目を選択していた場合
			{
				switch( scene )
				{
				case 1:
					//項目：深さ3
					if( MouseX < WINDOW_SIZE_X / 3 )
					{
						maxdepth = 3;
					}
					//項目：深さ5
					else if( MouseX >= WINDOW_SIZE_X / 3 && MouseX < WINDOW_SIZE_X * 2 / 3 )
					{
						maxdepth = 5;
					}
					//項目：深さ7
					else
					{
						maxdepth = 7;
					}
					scene++;
					break;
				case 2:
					//項目：ノーマルモード
					if( MouseX < WINDOW_SIZE_X / 4 )
					{
						mode = MODE_NORMAL;
					}
					//項目：ハンデモード
					else if( MouseX >= WINDOW_SIZE_X / 4 && MouseX < WINDOW_SIZE_X / 2 )
					{
						mode = MODE_HANDICAP;
					}
					//項目：最弱モード
					else if( MouseX >= WINDOW_SIZE_X / 2 && MouseX < WINDOW_SIZE_X * 3 / 4 )
					{
						mode = MODE_WEAK;
					}
					//項目：革命モード
					else
					{
						mode = MODE_REVOLUTION;
					}
					scene++;
					break;
				case 3:
					//項目：ゲームスタート
					if( MouseX > WINDOW_SIZE_X / 4 && MouseX < WINDOW_SIZE_X * 3 / 4 )
					{
						scene++;
					}
					break;
				}
				MouseFlag = 1;

				PlaySoundMem( se.next, DX_PLAYTYPE_NORMAL );
			}
		}
		else
		{
			MouseFlag = 0;
		}
	}
}

/* 【関数】マウスの座標をオセロの座標に変換 */
int GetPosfromMouse(int MouseX, int MouseY)
{
	int x, y;

	x = ( MouseX - sx + d ) / d;
	y = ( MouseY - sy + d ) / d;

	return getposition( x, y );
}

/* 【関数】GUI用のmanPlayer */
void manPlayerGUI()
{
	int num, move;
	int MouseX, MouseY, MouseInput;
	
	//パスのチェック
	Move moves[MOVENUM];
	num = generateMoves( moves );
	//合法手が無い場合
	if( num == 0 )
	{
		nextmove = PASSMOVE;
		return;
	}

	while(1)
	{
		GetMousePoint( &MouseX, &MouseY );
		MouseInput = GetMouseInput();

		if( ( MouseInput & MOUSE_INPUT_LEFT ) != 0 )
		{
			move = GetPosfromMouse( MouseX, MouseY );
			if( isLegalMove( move ) )
			{
				nextmove = move;
				return;
			}
		}
	}
}

/* 【関数】画面描画 */
void ShowBoard()
{
	int color;
	int x, y, stone;
	int ry = 40, rdy = STRFONTSIZE + 5;
	DrawGraph( 0 , 0 , grp.back , FALSE );//背景を描く
	SetFontSize( STRFONTSIZE ) ;

	//盤の表示
	DrawBox( sx, sy, ex, ey, GetColor( 0, 255, 0 ), TRUE );

	//枠の表示
	for( x = 0; x < SIDE; x++ )
	{
		DrawLine( sx + d * x, sy, sx + d * x, sy + d * SIDE, GetColor( 255, 255, 255 ) );	//縦線
	}
	for( y = 0; y < SIDE; y++ )
	{
		DrawLine( sx, sy + d * y, sx + d * SIDE, sy + d * y, GetColor( 255, 255, 255 ) );	//横線
	}

	//石の表示
	for ( y = 1; y <= SIDE; y++ )
	{
		for ( x = 1; x <= SIDE; x++ )
		{
			stone = board[ getposition( x, y ) ];//石の色を調べる
			if( stone == B )
			{
				color = GetColor( 0, 0, 0 );//黒
			}
			else if( stone == W )
			{
				color = GetColor( 255, 255, 255 );//白
			}
			else
			{
				continue;
			}
			DrawCircle( sx + d * (x-1) + r, sy + d * (y-1) + r, r, color, TRUE );//円で石を描画
		}
	}
	DrawFormatString( rx, ry, GetColor( 0, 0, 0 ),"●: %d", stonenum[BLACK_TURN] );
	ry += rdy;
	DrawFormatString( rx, ry, GetColor( 255, 255, 255 ),"●: %d", stonenum[WHITE_TURN] );
	ry += rdy * 2;
	DrawFormatString( rx, ry, STRCOLOR,"%d 手", ply );
}

/* 【WinMain関数】 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow )
{
	int manturn = BLACK_TURN;

	//タイトル変更
	SetMainWindowText( "Othello" );
	//ウィンドウモード設定
	ChangeWindowMode( TRUE );
	//ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )
	{
		return -1;	//エラーが起きたら直ちに終了
	}
	//マウスを表示状態にする
	SetMouseDispFlag( TRUE );
	
	//画像ファイル読み込み
	InitGrp();
	//音ファイル読み込み
	InitSE();

	do
	{
	//タイトル画面
	Title();

	//初期化
	init();

	//初期表示
	ShowBoard();
	SetFontSize( 32 );
	DrawString( rx , ey / 2 - 20, "GAME START!!", STRCOLOR );
	WaitTimer(1000);

	ShowBoard();

	//対局メインループ
	while ( !CheckHitKey( KEY_INPUT_ESCAPE ) )	//ESCでゲーム終了
	{
		int num, result;
		Move moves[MOVENUM];

		//メッセージループに代わる処理
		if( ProcessMessage() == -1 )
		{
			break;		//エラーが起きたらループを抜ける
		}

		num = generateMoves( moves );
		if ( num == 0 && isTerminalNode() )//終局かチェック
		{
			SetFontSize( 32 );
			//石の数で勝ち負け判定し表示
			if( mode == MODE_WEAK )
			{
				result = stonenum[WHITE_TURN] - stonenum[BLACK_TURN];
			}
			else
			{
				result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];
			}
			//result により表示を変える
			if ( result == 0 )
			{
				DrawString( rx , ey / 2 - 20, "DRAW!!", STRCOLOR );
			}
			else
			{
				DrawFormatString( rx , ey / 2 - 20, STRCOLOR, "%s WIN!!", ( result > 0 ? "BLACK": "WHITE") );
			}
			PlaySoundMem( se.win, DX_PLAYTYPE_BACK );
			break;
		}
		if ( turn == manturn )
		{
			manPlayerGUI();
		}
		else
		{
			comPlayer();
		}
		makeMove( nextmove, 0 );
		if( nextmove == PASSMOVE)
		{
			DrawString( rx , ey / 2 - 20, "PASS!!", STRCOLOR );
			WaitDisp();
		}
		else
		{
			ply++;
		}

		//表示
		ShowBoard();

		//効果音
		if( turn == manturn )
		{
			PlaySoundMem( se.putCOM, DX_PLAYTYPE_NORMAL );
		}
		else
		{
			PlaySoundMem( se.putMAN, DX_PLAYTYPE_NORMAL );
		}

		//待ち時間
		WaitTimer(100);
	}

	//待機画面
	DrawString( rx , ey / 2 + 50, "Push Key!!", STRCOLOR );
	DrawString( rx , ey / 2 + 80, "[ANY]title", STRCOLOR );
	DrawString( rx , ey / 2 + 110, "[ESC]end", STRCOLOR );
	WaitDisp();

	}while( exitFlag == 0 );

	DxLib_End();	//ＤＸライブラリ使用の終了処理
	return 0;		//ソフトの終了 
}