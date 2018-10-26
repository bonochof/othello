/*
*	�I�Z��	GUI�֌W�t�@�C��		othello_GUI.cpp
*	
*	����28�N6��20��	���]�H�ƍ������w�Z�@J1318	���c����
*/

/* �y�C���N���[�h�t�@�C���z */
#include "DxLib.h"
#include "othello.h"
#include "othello_functions.c"

/* �y�\���̌^�z�摜 */
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

/* �y�O���[�o���ϐ��z */
int d = 50;				//�}�X�̊Ԋu
int r = d / 2;			//�΂̔��a
int sx = 20, sy = 20;	//�Ս���̍��W
int ex = sx + d * SIDE;	//�ՉE����x���W
int ey = sy + d * SIDE;	//�ՉE����y���W
int rx = ex + 10;		//��ʉE�@���ʂȂǕ\�����邽�߂̏ꏊ�����W
int STRCOLOR = GetColor( 0, 0, 200 );	//�����F
int STRFONTSIZE = 32;					//�����t�H���g�T�C�Y
int exitFlag = 0;		//�I���t���O

/* �y�֐��z�摜�t�@�C���ǂݍ��� */
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

/* �y�֐��z���t�@�C���ǂݍ��� */
void InitSE()
{
	se.next = LoadSoundMem( "Othello_data/Sound/next.wav" );
	se.win = LoadSoundMem( "Othello_data/Sound/win.wav" );
	se.putCOM = LoadSoundMem( "Othello_data/Sound/putCOM.wav" );
	se.putMAN = LoadSoundMem( "Othello_data/Sound/putMAN.wav" );
}

/* �y�֐��z�҂���� */
void WaitDisp()
{
	while( !CheckHitKeyAll() )
	{
		//���b�Z�[�W���[�v�ɑ��鏈��
		if( ProcessMessage() == -1 )
		{
			break;	//�G���[���N�����烋�[�v�𔲂���
		}
	}
	if( CheckHitKey( KEY_INPUT_ESCAPE ) )
	{
		exitFlag = 1;
	}
	PlaySoundMem( se.next, DX_PLAYTYPE_NORMAL );
}

/* �y�֐��z�^�C�g���\�� */
void Title()
{
	int scene = 0;
	int MouseX, MouseY, MouseInput, MouseFlag = 0;

	//�w�i
	DrawGraph( 0, 0, grp.title, FALSE );
	DrawGraph( 0, WINDOW_SIZE_Y / 2, grp.scene0, TRUE );
	WaitDisp();
	scene++;

	while( scene <= 3 )
	{
		//�w�i
		DrawGraph( 0, 0, grp.title, FALSE );

		//�e�V�[������
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

		//�}�E�X����
		GetMousePoint( &MouseX, &MouseY );
		MouseInput = GetMouseInput();

		if( ( MouseInput & MOUSE_INPUT_LEFT ) != 0 )	//�N���b�N��
		{
			if( MouseFlag == 0 && MouseY > WINDOW_SIZE_Y / 2 )	//���ڂ�I�����Ă����ꍇ
			{
				switch( scene )
				{
				case 1:
					//���ځF�[��3
					if( MouseX < WINDOW_SIZE_X / 3 )
					{
						maxdepth = 3;
					}
					//���ځF�[��5
					else if( MouseX >= WINDOW_SIZE_X / 3 && MouseX < WINDOW_SIZE_X * 2 / 3 )
					{
						maxdepth = 5;
					}
					//���ځF�[��7
					else
					{
						maxdepth = 7;
					}
					scene++;
					break;
				case 2:
					//���ځF�m�[�}�����[�h
					if( MouseX < WINDOW_SIZE_X / 4 )
					{
						mode = MODE_NORMAL;
					}
					//���ځF�n���f���[�h
					else if( MouseX >= WINDOW_SIZE_X / 4 && MouseX < WINDOW_SIZE_X / 2 )
					{
						mode = MODE_HANDICAP;
					}
					//���ځF�Ŏヂ�[�h
					else if( MouseX >= WINDOW_SIZE_X / 2 && MouseX < WINDOW_SIZE_X * 3 / 4 )
					{
						mode = MODE_WEAK;
					}
					//���ځF�v�����[�h
					else
					{
						mode = MODE_REVOLUTION;
					}
					scene++;
					break;
				case 3:
					//���ځF�Q�[���X�^�[�g
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

/* �y�֐��z�}�E�X�̍��W���I�Z���̍��W�ɕϊ� */
int GetPosfromMouse(int MouseX, int MouseY)
{
	int x, y;

	x = ( MouseX - sx + d ) / d;
	y = ( MouseY - sy + d ) / d;

	return getposition( x, y );
}

/* �y�֐��zGUI�p��manPlayer */
void manPlayerGUI()
{
	int num, move;
	int MouseX, MouseY, MouseInput;
	
	//�p�X�̃`�F�b�N
	Move moves[MOVENUM];
	num = generateMoves( moves );
	//���@�肪�����ꍇ
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

/* �y�֐��z��ʕ`�� */
void ShowBoard()
{
	int color;
	int x, y, stone;
	int ry = 40, rdy = STRFONTSIZE + 5;
	DrawGraph( 0 , 0 , grp.back , FALSE );//�w�i��`��
	SetFontSize( STRFONTSIZE ) ;

	//�Ղ̕\��
	DrawBox( sx, sy, ex, ey, GetColor( 0, 255, 0 ), TRUE );

	//�g�̕\��
	for( x = 0; x < SIDE; x++ )
	{
		DrawLine( sx + d * x, sy, sx + d * x, sy + d * SIDE, GetColor( 255, 255, 255 ) );	//�c��
	}
	for( y = 0; y < SIDE; y++ )
	{
		DrawLine( sx, sy + d * y, sx + d * SIDE, sy + d * y, GetColor( 255, 255, 255 ) );	//����
	}

	//�΂̕\��
	for ( y = 1; y <= SIDE; y++ )
	{
		for ( x = 1; x <= SIDE; x++ )
		{
			stone = board[ getposition( x, y ) ];//�΂̐F�𒲂ׂ�
			if( stone == B )
			{
				color = GetColor( 0, 0, 0 );//��
			}
			else if( stone == W )
			{
				color = GetColor( 255, 255, 255 );//��
			}
			else
			{
				continue;
			}
			DrawCircle( sx + d * (x-1) + r, sy + d * (y-1) + r, r, color, TRUE );//�~�Ő΂�`��
		}
	}
	DrawFormatString( rx, ry, GetColor( 0, 0, 0 ),"��: %d", stonenum[BLACK_TURN] );
	ry += rdy;
	DrawFormatString( rx, ry, GetColor( 255, 255, 255 ),"��: %d", stonenum[WHITE_TURN] );
	ry += rdy * 2;
	DrawFormatString( rx, ry, STRCOLOR,"%d ��", ply );
}

/* �yWinMain�֐��z */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow )
{
	int manturn = BLACK_TURN;

	//�^�C�g���ύX
	SetMainWindowText( "Othello" );
	//�E�B���h�E���[�h�ݒ�
	ChangeWindowMode( TRUE );
	//�c�w���C�u��������������
	if( DxLib_Init() == -1 )
	{
		return -1;	//�G���[���N�����璼���ɏI��
	}
	//�}�E�X��\����Ԃɂ���
	SetMouseDispFlag( TRUE );
	
	//�摜�t�@�C���ǂݍ���
	InitGrp();
	//���t�@�C���ǂݍ���
	InitSE();

	do
	{
	//�^�C�g�����
	Title();

	//������
	init();

	//�����\��
	ShowBoard();
	SetFontSize( 32 );
	DrawString( rx , ey / 2 - 20, "GAME START!!", STRCOLOR );
	WaitTimer(1000);

	ShowBoard();

	//�΋ǃ��C�����[�v
	while ( !CheckHitKey( KEY_INPUT_ESCAPE ) )	//ESC�ŃQ�[���I��
	{
		int num, result;
		Move moves[MOVENUM];

		//���b�Z�[�W���[�v�ɑ��鏈��
		if( ProcessMessage() == -1 )
		{
			break;		//�G���[���N�����烋�[�v�𔲂���
		}

		num = generateMoves( moves );
		if ( num == 0 && isTerminalNode() )//�I�ǂ��`�F�b�N
		{
			SetFontSize( 32 );
			//�΂̐��ŏ����������肵�\��
			if( mode == MODE_WEAK )
			{
				result = stonenum[WHITE_TURN] - stonenum[BLACK_TURN];
			}
			else
			{
				result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];
			}
			//result �ɂ��\����ς���
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

		//�\��
		ShowBoard();

		//���ʉ�
		if( turn == manturn )
		{
			PlaySoundMem( se.putCOM, DX_PLAYTYPE_NORMAL );
		}
		else
		{
			PlaySoundMem( se.putMAN, DX_PLAYTYPE_NORMAL );
		}

		//�҂�����
		WaitTimer(100);
	}

	//�ҋ@���
	DrawString( rx , ey / 2 + 50, "Push Key!!", STRCOLOR );
	DrawString( rx , ey / 2 + 80, "[ANY]title", STRCOLOR );
	DrawString( rx , ey / 2 + 110, "[ESC]end", STRCOLOR );
	WaitDisp();

	}while( exitFlag == 0 );

	DxLib_End();	//�c�w���C�u�����g�p�̏I������
	return 0;		//�\�t�g�̏I�� 
}