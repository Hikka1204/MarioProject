/********************************************************************
** 
** 
********************************************************************/
#include "DxLib.h"
#define size 32	//32
#define WIDTH 640
#define HEIGHT 480
#define WLINE 20
#define HLINE 15
#define allmas 15
#define ESAMAX 100
#define ENEMYMAX 2
#define PAI 3.14159f
#define Font_Size_S 16
#define Font_Size_M 32
#define Font_Size_L 64
#define RED
/***********************************************
 * �񋓑̂̐錾
 ***********************************************/
typedef enum GAME_MODE{
	GAME_TITLE,
	GAME_INIT,
	GAME_HELP,
	GAME_END,
	GAME_MAIN,
	GAME_OVER,
	GAME_CLEAR,
	END = 99,
};

typedef enum BLOCK_STUTAS{
	e_KABE,
	e_MITI,
	e_ESA,
	e_PLAYER,
	e_POWER,
	e_WAPU1,
	e_WAPU2
};

typedef enum MUKI_STUATS{
	e_DOWN,
	e_LEFT,
	e_UP,
	e_RIGHT
};

/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey; // �O��̓��̓L�[
int g_NowKey; // ����̓��̓L�[
int g_KeyFlg; // ���̓L�[���

int g_GameState = GAME_TITLE; // �Q�[�����[�h

int g_TitleImage; // �^�C�g���摜

int g_PlayerImage[2];		//�v���C���[�̉摜
int g_MukiImage[4];		//�����̉摜
int g_EnemyImage[ENEMYMAX][2];		//�G�l�~�[�̉摜
int g_EnemyEyeImage[4];			//�G�l�~�[�̖�
int g_EsaImage;		//�N�b�L�[�̉摜
int g_suponImage;		//�X�|�[���̉摜
int g_EnemyPowerImage[2];	//�G�l�~�[���ア���̉摜
int g_PowerImage;			//�p���[�N�b�L�[�̉摜
int g_WapuImage;			//���[�v�̉摜


int g_PowerCount;		//�p���[�J�E���g

int g_GameScore;		//���Ŏ擾�����X�R�A�̐�
int g_Score;			//�X�R�A
int g_MaxScore;		//�����Ƃ����X�R�A�̐�
int g_TmpGameScore;	//�Q�[���N���A�ƃQ�[���I�[�o�[���̉��o
int g_TmpScore;		//��L�Ɠ���
int g_EatEnemy;		//�G�l�~�[��H�ׂ���
int g_PowerCookie;	//�p���[�N�b�L�[��H�ׂ���
int g_GamePlay;		//�v���C��
int g_GameOver;		//�Q�[���I�[�o�[�̐�
int g_GameClear;		//�Q�[���N���A�̐�

int g_cookie;		//�N�b�L�[

int g_animecount;		//�Q�[���̃J�E���g
int g_animescore;

int g_taikakuX;		//�Ίp�^�̂w
int g_taikakuY;		//�Ίp�^�̂x

int g_Stage;			//�X�e�[�W�̕ϐ�

int g_WaitTime;		//�G���h�̕ϐ�



/***********************************************
 * �\���̂̐錾
 ***********************************************/
struct Object {
 int flg; // �t���O(0:�\�����Ȃ� ,1:�\��)
 int x,y; // ���W x,y
 int w,h; // �� w ���� h
 int image; // �摜
 int backup; // �摜�o�b�N�A�b�v
};
struct Object g_Block[HLINE][WLINE]; 

int g_IntBlock[HLINE][WLINE] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

 
struct PLAYER{
	int flg;		//	0.�� 1.���C�� 2.������ 3.�p���[
	int x,y;		//	�����̍��W
	int mx,my;	//	���������̍��W
	int mx2,my2;	//	�G�̒ǐՏ����Ɏg�����W
	int muki;	//	�p�b�N�}���̌���
	int muki2;	//	�p�b�N�}������
	int img;		//	�摜�ϐ�
	int count;	//	���i�ގ���
	int zanki;	//	�c�@
};
struct PLAYER g_Player;

struct ENEMY{
	int flg;		//	0.�� 1.���C�� 2.�p���[ 3.�ޔ�
	int x,y;		//	�����̍��W
	int mx,my;	//	���������̍��W
	int muki;	//	�G�̌���
	int muki2;	//	�G�̐i�ނׂ��łȂ�����
	int img;		//	�摜�ϐ�
	int count;	//	���i�ގ���
	int idouflg;	//	�ړ����������ĂȂ����̊m�F
};				 
struct ENEMY g_Enemy[ENEMYMAX];
/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[������������
void GameMain(void); //�Q�[�����C������

void DrawGameTitle(void); //�Q�[���^�C�g���`�揈��
void DrawGameOver(void); //�Q�[���I�[�o�[�`�揈��
void DrawClear(void);
void DrawEnd(void); //�G���h�`�揈��

void DrawHelp(void); //�w���v�`�揈��

void DrawResult(void);		//���U���g�`�揈��

void EsaInit(void);
void PlayerInit(void);	//�v���C���[�̏�����
void EnemyInit(void);		//�G�l�~�[�̏�����

void PlayerMove(void);	//�v���C���[�̈ړ�����
void EnemyMove(void);		//�G�l�~�[�̈ړ�����
void Xhantei(int e);		//�c�����̈ړ�����
void Yhantei(int e);		//�������̈ړ�����
int XYhantei(int x, int y, int x2, int y2);	//�ŒZ���[�g�̔��� TRUE:x�������� FALSE:y��������
void eRandMove(int e);	//�����_���Ɉړ�����G�̏���

void HitBox(void);	//�����蔻��

void DrawStage(void);		//�X�e�[�W�̕`��
void DrawPlayer(void);	//�v���C���[�̕`��
void DrawEnemy(void);		//�G�l�~�[�̕`��
void RandMove(int e);		//�����_���Ɉړ�
void EndInit(void);		//�G���h�̏�����

int LoadImages(); //�摜�Ǎ���
int LoadMusic();	//���y�̓ǂݍ���

void taikaku(void);	//�Ίp�̎擾
/***********************************************
 * �v���O�����̊J�n
 ***********************************************/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
							LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText( "�X�[�p�[�}���I�u���U�[�Y" ); // �^�C�g����ݒ�

	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ŋN��

	if ( DxLib_Init() == -1 ) return -1; // DX ���C�u�����̏���������

	SetDrawScreen( DX_SCREEN_BACK ); // �`����ʂ𗠂ɂ���

	if ( LoadImages() == -1 ) return -1; // �摜�Ǎ��݊֐����Ăяo��

	// �Q�[�����[�v
	while ( ProcessMessage() == 0 && g_GameState != END && !(g_KeyFlg & PAD_INPUT_START)){
		
		// ���̓L�[�擾
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState( DX_INPUT_KEY_PAD1 );
		g_KeyFlg = g_NowKey & ~g_OldKey; 

		ClearDrawScreen(); // ��ʂ̏�����
		switch (g_GameState){
			case GAME_TITLE:
				DrawGameTitle(); //�^�C�g������
				break;
			case GAME_INIT:
				GameInit(); //�Q�[����������
				break;
			case GAME_HELP:
				DrawHelp(); //�����L���O�`�揈��
				break;
			case GAME_END:
				DrawEnd(); //�G���h�`�揈��
				break;
			case GAME_MAIN:
				GameMain(); //�Q�[�����C������
				break;
			case GAME_OVER:
				DrawGameOver(); //�Q�[���I�[�o�[�`�揈��
				break;
			case GAME_CLEAR:
				DrawClear(); //�Q�[���N���A�`�揈��
				break;
		}

		ScreenFlip(); // ����ʂ̓��e��\��ʂɔ��f
	}

	DxLib_End(); // DX ���C�u�����g�p�̏I������

	return 0; // �\�t�g�̏I��
} 

/***********************************************
 * �Q�[���^�C�g���`�揈���i���j���[��ʁj
 ***********************************************/
void DrawGameTitle(void)
{

	static int MenuNo = 0;
	if(g_GamePlay == 0) g_Player.zanki = 3;

	int PosY;
	//���j���[�J�[�\���ړ�����
	if ( g_KeyFlg & PAD_INPUT_DOWN ){
		if ( ++MenuNo > 2 ) MenuNo = 0;
	}

	if ( g_KeyFlg & PAD_INPUT_UP ){
		if ( --MenuNo < 0 ) MenuNo = 2;
	}

	int STARTs = 0;
	int HELPs = 0;
	int ENDs = 0;

	if(MenuNo == 0)
		STARTs = 10;
	else if(MenuNo == 1)
		HELPs = 10;
	else if(MenuNo == 2)
		ENDs = 10;

	// �y�L�[�Ń��j���[�I��
	if(g_KeyFlg & PAD_INPUT_M){
		g_GameState = MenuNo + 1;
	}

	// ���j���[�J�[�\���i�p�b�N�}���j�̕\��
	PosY = MenuNo * 45;

	SetFontSize(Font_Size_L*2);

	////���o��////

	//�薼
	SetFontThickness(9);	//����
	int x = WIDTH/2 - GetDrawStringWidth("PacMan", 6)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	DrawString(x, 60, "PacMan",GetColor(200,200,255));

	
	int y = 240;

	SetFontSize(Font_Size_M+STARTs);
	 x = WIDTH/2 - GetDrawStringWidth("START", 5)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	 DrawFormatString(x, y, GetColor(0,0,255),"START");
	//DrawString(x, y, "START",GetColor(0,0,255));
	y += 45;

	SetFontSize(Font_Size_M+HELPs);
	 x = WIDTH/2 - GetDrawStringWidth("HELP", 4)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	DrawString(x, y, "HELP",GetColor(0,255,0));
	y += 45;

	SetFontSize(Font_Size_M+ENDs);
	 x = WIDTH/2 - GetDrawStringWidth("END", 3)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	DrawString(x, y, "END",GetColor(255,0,0));
	y += 45;

	SetFontSize(Font_Size_S);

	x = WIDTH/2 - GetDrawStringWidth("���L�[�ŏ�ɁA���L�[�ŉ��ɁA�X�y�[�X�L�[�Ō���", 23)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	DrawString(x, HEIGHT-20, "���L�[�ŏ�ɁA���L�[�ŉ��ɁA�X�y�[�X�L�[�Ō���",GetColor(200,200,255));
	
}

/***********************************************
 * �Q�[����������
 ***********************************************/
void GameInit(void)
{
	// �Q�[�����C��������
	g_GameState = GAME_MAIN;

	SetFontSize(Font_Size_S);
	SetFontThickness(3);

 	

	// �u���b�N�̏�����
	for(int h = 0; h < allmas; h++){
		for(int w = 0; w < allmas; w++){
				g_Block[h][w].flg = 1;
				g_Block[h][w].x = w * size;
				g_Block[h][w].y = h * size;
				g_Block[h][w].image = g_IntBlock[h][w]; //GetRand(3) + 1;
		}
	} 

	PlayerInit();

	EnemyInit();



	
}

/***********************************************
 * �v���C���[�̏�����
 ***********************************************/
void PlayerInit(void)
{

}

/***********************************************
 * �G�l�~�[�̏�����
 ***********************************************/
void EnemyInit(void)
{

}

/***********************************************
 * �w���v�`�揈��
 ***********************************************/
void DrawHelp(void)
{


	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE;
	}

	SetFontSize(Font_Size_S);
	
	int x = WIDTH/2 - GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g����", 15)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT - 20, "�X�y�[�X�L�[�������ă^�C�g����",GetColor(200,200,255));
	}

}

/***********************************************
 * ���U���g�`�揈��
 ***********************************************/
void DrawResult(void)
{


	SetFontSize(Font_Size_S);

	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE; 
	}
	int x = WIDTH/2 - GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g����", 15)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT-20, "�X�y�[�X�L�[�������ă^�C�g����",GetColor(200,200,255));
	}

}



/***********************************************
 * �Q�[���G���h�`�揈��
 ***********************************************/
void DrawEnd(void)
{


	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE; 
	}

	SetFontSize(Font_Size_S);
	int x = WIDTH/2 - GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g����", 15)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT-20, "�X�y�[�X�L�[�������ă^�C�g����",GetColor(200,200,255));
	}
}


/***********************************************
 * �Q�[�����C��
 ***********************************************/
void GameMain(void)
{
	DrawStage();
	
	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE; 
	}
	int x = WIDTH/2 - GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g����", 15)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
		if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT-20, "�X�y�[�X�L�[�������ă^�C�g����",GetColor(200,200,255));
	}


}
/***********************************************
 * �Q�[���I�[�o�[�`�揈��
 ***********************************************/
void DrawGameOver(void)
{
	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE;
	}

	SetFontSize(Font_Size_S);
	SetFontThickness(6);

	int x = WIDTH/2 - GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g����", 15)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	if(g_animecount % 60 <= 30){
		DrawString(x, HEIGHT-20, "�X�y�[�X�L�[�������ă^�C�g����",GetColor(200,200,255));
	}


}

/***********************************************
 * �Q�[���N���A�`�揈��
 ***********************************************/
void DrawClear(void)
{
	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE;
	}



	SetFontSize(Font_Size_S);
	SetFontThickness(5);	//����
	int x = WIDTH/2 - GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g����", 15)/2;		//������̒�����}���Ă�@�����񂹂ɂł���
	if(g_animecount % 60 <= 30){
		DrawString(x, HEIGHT-20, "�X�y�[�X�L�[�������ă^�C�g����",GetColor(200,200,255));
	}

}

/***********************************************
 * �v���C���[�̈ړ�����
 ***********************************************/
void PlayerMove(void)
{
	
}
/***********************************************
 * �G�l�~�[�̈ړ�����
 ***********************************************/
void EnemyMove(void)
{

}

/***********************************************
 * �X�e�[�W�̕`�揈��
 ***********************************************/
void DrawStage(void)
{
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			DrawBox(w * size,h * size, (w + 1) * size,(h + 1) * size,0xffffff,FALSE);
		}
	}
	
}

/***********************************************
 * �v���C���[�̕`��
 ***********************************************/
void DrawPlayer(void)
{


}
/***********************************************
 * �G�l�~�[�̕`��
 ***********************************************/
void DrawEnemy(void)
{
	
}
/***********************************************
 * �摜�Ǎ���
 ***********************************************/
int LoadImages()
{

		//�v���C���[
	//if((g_PlayerImage[0] = LoadGraph("images/player.png")) == -1) return -1;
	//if((g_PlayerImage[1] = LoadGraph("images/player2.png")) == -1) return -1;


	return 0;
} 

