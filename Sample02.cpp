/********************************************************************
** 
** 
********************************************************************/
#include "DxLib.h"
/***********************************************
 * �񋓑̂̐錾
 ***********************************************/
typedef enum GAME_MODE{
	GAME_TITLE,
	GAME_INIT,
	GAME_MAIN,
	GAME_OVER,
	GAME_CLEAR,
	END = 99,
};

typedef enum GAME_BLOCK{
	MITI,
	KABE,
	BLOCK,
	COINBLOCK,
	COINHATENA,
	ITEMHATENA,
};


typedef enum MUKI_STUATS{
	e_DOWN,
	e_LEFT,
	e_UP,
	e_RIGHT
};

/***********************************************
 * �萔�̐錾
 ***********************************************/

const int size = 32;
const int WIDTH = 640;
const int HEIGHT = 480;
const int WLINE = 40;
const int HLINE = 15;
const int ENEMYMAX = 2;
const int MarioX = 7;
const int Font_Size_S = 16;
const int Font_Size_M = 32;
const int Font_Size_L = 64;
const int BlockHitX = 5;


/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey; // �O��̓��̓L�[
int g_NowKey; // ����̓��̓L�[
int g_KeyFlg; // ���̓L�[���
int g_PushFlg;	//TRUE:�����Ă�@FALSE:������ĂȂ�

int g_GameState = GAME_TITLE; // �Q�[�����[�h

int g_TitleImage; // �^�C�g���摜

int g_PlayerImage[2];		//�v���C���[�̉摜
int g_MukiImage[4];		//�����̉摜
int g_EnemyImage[ENEMYMAX][2];		//�G�l�~�[�̉摜


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


int g_animecount;		//�Q�[���̃J�E���g

int g_BlockMx;		//�u���b�N�̉��̈ړ���

int g_Stage;			//�X�e�[�W�̕ϐ�

int g_WaitTime;		//�G���h�̕ϐ�





/***********************************************
 * �\���̂̐錾
 ***********************************************/
struct s_BLOCK {
 int flg; // �t���O(0:�\�����Ȃ� ,1:�\��)
 double x,y; // ���W x,y
 int w,h; // �� w ���� h
 int image; // �摜
 int backup; // �摜�o�b�N�A�b�v
};
struct s_BLOCK g_Block[HLINE][WLINE]; 

int g_IntBlock[HLINE][WLINE] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

 
struct s_PLAYER{
	int flg;		//	0.�� 1.���C�� 2.������ 3.�p���[
	double x,y;		//	�����̍��W
	double mx,my;	//	��������
	int muki;	//	�}���I�̌���
	int type;	//	�����̃^�C�v
	int img;		//	�摜�ϐ�
	int Jump;	//	�J��Ԃ��Ɏg�� 
	int JumpCount;	//	�W�����v����
	int JumpFlag;	//�W�����v���Ă��邩���ĂȂ���
	int zanki;	//	�c�@
};
struct s_PLAYER g_Player;

struct s_ENEMY{
	int flg;		//	0.�� 1.���C�� 2.�p���[ 3.�ޔ�
	double x,y;		//	�����̍��W
	double mx,my;	//	���������̍��W
	int muki;	//	�G�̌���
	int img;		//	�摜�ϐ�
	int count;	//	���i�ގ���
	int idouflg;	//	�ړ����������ĂȂ����̊m�F
};				 
struct s_ENEMY g_Enemy[ENEMYMAX];
/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[������������
void GameMain(void); //�Q�[�����C������

void DrawGameTitle(void); //�Q�[���^�C�g���`�揈��
void DrawGameOver(void); //�Q�[���I�[�o�[�`�揈��
void DrawClear(void);


void PlayerInit(void);	//�v���C���[�̏�����
void EnemyInit(void);		//�G�l�~�[�̏�����

void PlayerMove(void);	//�v���C���[�̈ړ�����
void Gravity(void);		//�d�͏���
void Jump(void);			//�W�����v����
void BlockMove(void);		//�u���b�N�̈ړ�����


void HitBox(void);	//�̂̓����蔻��
void HitHead(void);	//���̓����蔻��

void DrawStage(void);		//�X�e�[�W�̕`��
void DrawPlayer(void);	//�v���C���[�̕`��
void DrawEnemy(void);		//�G�l�~�[�̕`��

int LoadImages(); //�摜�Ǎ���
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

		g_PushFlg = FALSE;

		ClearDrawScreen(); // ��ʂ̏�����
		switch (g_GameState){
			case GAME_TITLE:
				DrawGameTitle(); //�^�C�g������
				break;
			case GAME_INIT:
				GameInit(); //�Q�[����������
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

	int STARTs = 10;

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
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
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
	g_Player.x = size * 2;
	g_Player.y = size * 12;
	g_Player.mx = 0;
	g_Player.my = 0;
	g_Player.img = 0;
	g_Player.zanki = 3;

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
	PlayerMove();

	DrawStage();

	DrawPlayer();
	
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
	if(g_KeyFlg & PAD_INPUT_RIGHT) g_Player.muki = e_RIGHT;
	if(g_KeyFlg & PAD_INPUT_LEFT) g_Player.muki = e_LEFT;

	Jump();	//�W�����v����

	//if(g_NowKey & PAD_INPUT_DOWN ) {
	//	//g_Player.y++;
	//}

	if(g_NowKey & PAD_INPUT_RIGHT && g_Player.muki == e_RIGHT) {
		g_Player.mx += 0.1;
		if(g_Player.mx < 0) g_Player.mx += 0.4;
		if(g_Player.mx > 5.0) g_Player.mx = 5.0;
		g_PushFlg = TRUE;
	}

	if(g_NowKey & PAD_INPUT_LEFT && g_Player.muki == e_LEFT) {
		g_Player.mx += -0.1;
		if(g_Player.mx > 0) g_Player.mx += -0.4;
		if(g_Player.mx < -5.0) g_Player.mx = -5.0;
		g_PushFlg = TRUE;
	}

	if(g_Player.x < 0){
		g_Player.x = 0;
		g_Player.mx = 0;
	}
	if(g_Player.x+32 > WIDTH){
		g_Player.x = WIDTH-32;
		g_Player.mx= 0;
	}


	g_Player.x += g_Player.mx;

	if(g_Player.x > size * MarioX){
		g_BlockMx += g_Player.x - size * MarioX;
		g_Player.x = size * MarioX;
	}
	

	Gravity();

	if(g_PushFlg == FALSE){
		if(g_Player.mx > 0.0) {
			g_Player.mx -= 0.2;
			if(g_Player.mx < 0.0) g_Player.mx = 0.0; 
		}

		if(g_Player.mx < 0.0) {
			g_Player.mx += 0.2;
			if(g_Player.mx > 0.0) g_Player.mx = 0.0;
		}
	}

	HitBox();
}

/***********************************************
 * �W�����v����
 ***********************************************/
void Jump(void)
{
	if(g_NowKey & PAD_INPUT_UP && g_Player.JumpFlag == 0 && g_Player.JumpCount <= 15 ) {
		g_Player.y -= 10;
		g_Player.JumpCount++;
	}
	else{
		g_Player.JumpFlag = 1;
	}

	DrawFormatString(0,0,0xffff00,"%d",g_Player.JumpCount);
}

/***********************************************
 * �d�͏���
 ***********************************************/
void Gravity(void)
{
	if(g_NowKey & PAD_INPUT_UP && g_Player.JumpFlag == 0){
	}
	else{
		g_Player.my += 0.5;
		g_Player.y += g_Player.my ;	
	}
	
}


/***********************************************
 * �G�l�~�[�̈ړ�����
 ***********************************************/
void EnemyMove(void)
{

}

/***********************************************
 * �u���b�N�̈ړ�����
 ***********************************************/
void BlockMove(void)
{
	
}


/***********************************************
 * �̂̓����蔻��
 ***********************************************/
void HitBox(void)
{
	
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].flg == TRUE && g_Block[h][w].image != MITI){
				//�u���b�N�̏㕔�i�s���N�F�j
				if(g_Player.x < g_Block[h][w].x+size - 5 - g_BlockMx && g_Block[h][w].x + 5 - g_BlockMx < g_Player.x + size && 
						g_Player.y < g_Block[h][w].y + size - 30 && g_Block[h][w].y < g_Player.y + size){
					g_Player.y = g_Block[h][w].y-size;
					g_Player.my = 0;
					if(g_NowKey & PAD_INPUT_UP) g_Player.JumpFlag = TRUE;
					else g_Player.JumpFlag = FALSE;
					g_Player.JumpCount = 0;
					return ;
				}

				//�u���b�N�̉����i�s���N�F�j
				if(g_Player.x < g_Block[h][w].x + size - 5 - g_BlockMx && g_Block[h][w].x + 5 - g_BlockMx < g_Player.x+size 
					&& g_Player.y < g_Block[h][w].y + size && g_Block[h][w].y+30 < g_Player.y+size){
					g_Player.y = g_Block[h][w].y+size;
					g_Player.my = 0;
					g_Player.JumpFlag = FALSE;
					g_Player.JumpCount = 16;
					return ;
				}
				//�u���b�N�̉E��(�j
				if(g_Player.x < g_Block[h][w].x + size-26 - g_BlockMx && g_Block[h][w].x + 4 - g_BlockMx < g_Player.x+size 
					&& g_Player.y < g_Block[h][w].y+size-1 && g_Block[h][w].y+1 < g_Player.y+size){
					g_Player.x = g_Block[h][w].x-size - g_BlockMx;
					return ;
				}
				//�u���b�N�̍���(�j
				if(g_Player.x < g_Block[h][w].x+size - 4 - g_BlockMx&& g_Block[h][w].x + 26 - g_BlockMx< g_Player.x+size 
					&& g_Player.y < g_Block[h][w].y+size-1 && g_Block[h][w].y+1 < g_Player.y+size){
					g_Player.x = g_Block[h][w].x+size - g_BlockMx;
					return ;
				}
				
				
			}
			DrawBox(g_Player.x , g_Player.y,g_Player.x+size,g_Player.y + size,0xff00ff,TRUE);
			//DrawBox(g_Block[h][w].x+3 , g_Block[h][w].y+16,g_Block[h][w].x+size-3,g_Block[h][w].y + size,0xff00ff,TRUE);
		}
	}


}

/***********************************************
 * �X�e�[�W�̕`�揈��
 ***********************************************/
void DrawStage(void)
{
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			switch(g_Block[h][w].image){

				case KABE:
					DrawBox(w * size-g_BlockMx,h * size, (w + 1) * size-g_BlockMx,(h + 1) * size,0xff7f50,TRUE);
					DrawBox(g_Block[h][w].x+5 - g_BlockMx ,g_Block[h][w].y,   g_Block[h][w].x+size-5 - g_BlockMx, g_Block[h][w].y+size-30,0xff00ff,TRUE); //�㓖���蔻��`��
					DrawBox(g_Block[h][w].x+5 - g_BlockMx ,g_Block[h][w].y+30,g_Block[h][w].x+size-5 - g_BlockMx, g_Block[h][w].y+size,   0xff00ff,TRUE); //�������蔻��`��
					DrawBox(g_Block[h][w].x+4 - g_BlockMx, g_Block[h][w].y+1, g_Block[h][w].x+size-26- g_BlockMx, g_Block[h][w].y+size-1, 0x00FFFF,TRUE);//�E
					DrawBox(g_Block[h][w].x+26- g_BlockMx, g_Block[h][w].y+1, g_Block[h][w].x+size-4 - g_BlockMx, g_Block[h][w].y+size-1, 0x00FFFF,TRUE);//��
					break;
				case BLOCK:
					DrawBox(w * size,h * size, (w + 1) * size,(h + 1) * size,0xffa500,TRUE);
					break;
				default:
					DrawBox(w * size,h * size, (w + 1) * size,(h + 1) * size,0xffffff,FALSE);
					break;
			}
		}
	}

	DrawFormatString(0,20,0xff88ff,"Px:%f",g_Player.x);
	
}

/***********************************************
 * �v���C���[�̕`��
 ***********************************************/
void DrawPlayer(void)
{
	DrawGraph(g_Player.x,g_Player.y,g_PlayerImage[0],TRUE);

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
	if((g_PlayerImage[0] = LoadGraph("images/player.png")) == -1) return -1;
	//if((g_PlayerImage[1] = LoadGraph("images/player2.png")) == -1) return -1;



	return 0;
} 
