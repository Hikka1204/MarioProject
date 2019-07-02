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
	e_typefalse,
	e_brokenblock,
	e_coinblock,
	e_timecoin,
	e_kinoko,
	e_star,
	e_1pkinoko
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

const int size = 32;		//�u���b�N�T�C�Y
const int WIDTH = 640;
const int HEIGHT = 480;
const int WLINE = 230; //40;
const int HLINE = 15;
const int ENEMYMAX = 5;
const int MarioX = 9;
const int Font_Size_S = 16;
const int Font_Size_M = 32;
const int Font_Size_L = 64;
const int BlockHitXH = 5;		//�u���b�N�̕��̔�����
const int BlockHitYH = 30;	
const int BlockHitXW = 26;
const int BlockHitYW = 1;
const int MARIOHITXH = 7;	//�}���I�̉����̒����i�����蔻��j
const int JAMPEND = 24;	//�W�����v�̔򋗗�
const int COINMAX = 5;	//�R�C���̍ő吔
const int ItemMAX = 2;	//�A�C�e���̍ő吔

/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey; // �O��̓��̓L�[
int g_NowKey; // ����̓��̓L�[
int g_KeyFlg; // ���̓L�[���
int g_PushFlg;	//TRUE:�����Ă�@FALSE:������ĂȂ�

int g_GameState = GAME_TITLE; // �Q�[�����[�h

int g_TitleImage; // �^�C�g���摜
int g_BlockImage[84]; //�u���b�N�̉摜
int g_PlayerImage[3][15];		//�v���C���[�̉摜
int g_MukiImage[4];		//�����̉摜
int g_EnemyImage[ENEMYMAX][2];		//�G�l�~�[�̉摜


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

int g_Blockcount;		//�u���b�N�p�A�j���J�E���g

int g_Mariocount;		//�}���I�p�A�j���J�E���g

int g_animecount;


double g_BlockMx;		//�u���b�N�̉��̈ړ���
double g_TmpBlockMx;	//��̃Z�[�u�p

int g_Stage;			//�X�e�[�W�̕ϐ�
int g_StageType;		//�X�e�[�W�̃^�C�v 0:�u���b�N���ړ�������@1:�����Ȃ�;

int g_WaitTime;		//�G���h�̕ϐ�

/***********************************************
 * �\���̂̐錾
 ***********************************************/
struct s_BLOCK {
 int flg; // �t���O(0:�\�����Ȃ� ,1:�\��)
 int animeflg;	//0:�A�j�����Ȃ� 1:�オ�� 2:������
 double x,y; // ���W x,y
 double mx,my;	//�A�j���p�̓������ϐ�
 int w,h; // �� w ���� h
 int image; // �摜
 int backup; // �摜�o�b�N�A�b�v
 int Item;	//�A�C�e���̐�
 int type;	//0:�� 1:�󂹂� 2:�R�C������ 3:�^�C�}�[�t���R�C�� 4:�A�C�e������ 5:�X�^�[ 6:1p�L�m�R
 int count;	//�J�E���g
};
struct s_BLOCK g_Block[HLINE][WLINE]; 

char g_IntBlock[HLINE*WLINE];

int g_NoHitImage[23] = {14,15,16,17,18,19,20,21,22,23,30,31,32,33,34,35,42,43,44,45,46,47,59};
 
struct s_PLAYER{
	int flg;		//	0.�� 1.���C�� 2.������ 3.�p���[
	int animeflg;	//�A�j���t���O
	double x,y;		//	���W
	double mx,my;	//	��������
	int size;		//0:�q�� 1:��l
	int muki;	//	�}���I�̌���
	int type;	//	�����̃^�C�v
	int img;		//	�摜�ϐ�
	int JumpCount;	//	�W�����v����
	int JumpFlag;	//0:�󒆂ɂ��Ȃ� 1:����
	int zanki;	//	�c�@
	int animecount;	//�A�j���p�J�E���g
	double Scroll;
	int Gravitycount;
};
struct s_PLAYER g_Player;
struct s_PLAYER g_Coin[5];
struct s_PLAYER g_Enemy[ENEMYMAX];
struct s_PLAYER g_Item[ItemMAX];

struct s_ENEMY{
	int flg;		//	0.�� 1.�`��
	int animeflg;	//�A�j���̃t���O
	double x,y;		//	���W
	double mx,my;	//	���������̍��W
	int muki;	//	�G�̌���
	int img;		//	�摜�ϐ�
	int count;	//	�J�E���g
};				 
/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[������������
void GameMain(void); //�Q�[�����C������
void BlockOption(void);

void DrawGameTitle(void); //�Q�[���^�C�g���`�揈��
void DrawGameOver(void); //�Q�[���I�[�o�[�`�揈��
void DrawClear(void);		//�Q�[���N���A�`�揈��

int BlockInit();		//�u���b�N�̏�����
void PlayerInit(void);	//�v���C���[�̏�����
void EnemyInit(void);		//�G�l�~�[�̏�����
void CoinInit(void);		//�R�C���̏�����
void ItemInit(void);		//�A�C�e���̏�����

void PlayerMove(void);	//�v���C���[�̈ړ�����
void CoinMove(void);		//�R�C���̈ړ�����
void ItemMove(void);		//�A�C�e���̈ړ�����
void EnemyMove(void);		//�G�l�~�[�̈ړ�����

void Gravity(s_PLAYER *p );		//�d�͏���
void Jump(void);			//�W�����v����
void BlockMove(void);		//�u���b�N�̈ړ�����


void HitBox(void);	//�̂̓����蔻��
int BlockHitUp(s_PLAYER *p,s_BLOCK *b);	//�u���b�N�̏�
int BlockHitDown(s_PLAYER *p,s_BLOCK *b);	//�u���b�N�̉�
int BlockHitRight(s_PLAYER *p,s_BLOCK *b);	//�u���b�N�̉E
int BlockHitLeft(s_PLAYER *p,s_BLOCK *b);	//�u���b�N�̍�

int Hit_P_Odown(s_PLAYER *p ,s_PLAYER *o);	//�v���C���[�Ƒ��̃I�u�W�F�N�g�̉����̓����蔻��
int Hit_P_Oup(s_PLAYER *p ,s_PLAYER *o);	//�v���C���[�Ƒ��̃I�u�W�F�N�g�̏㕔�̓����蔻��


void CreateCoin(s_BLOCK *b);	//�R�C���̐���
void CreateItem(s_BLOCK *b);	//�A�C�e���̐���
void CreateEnemy(s_BLOCK *b);	//�G�l�~�[�̐���

void MarioAnime(void);	//�}���I�̃A�j���[�V����

void DrawStage(void);		//�X�e�[�W�̕`��
void DrawPlayer(void);	//�v���C���[�̕`��
void DrawEnemy(void);		//�G�l�~�[�̕`��
void DrawCoin(void);		//�R�C���̕`��
void DrawAnimeItem(void);	//�A�j���[�V�����̃A�C�e���`��
void DrawItem(void);		//�A�C�e���̕`��

int LoadImages(); //�摜�Ǎ���
/***********************************************
 * �v���O�����̊J�n
 ***********************************************/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
							LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText( "�X�[�p�[�}���I�u���U�[�Y" ); // �^�C�g����ݒ�

	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ŋN��
	//SetGraphMode( 800 , 600 , 16 ) ;
	//SetDrawArea( 0 , 0 , 320 , 480 ) ;

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
		SetBackgroundColor( 95, 151, 255 ) ;

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

	BlockInit();		//�u���b�N�̏�����
	PlayerInit();	//�v���C���[�̏�����
	EnemyInit();		//�G�l�~�[�̏�����
	CoinInit();
	ItemInit();
}

/***********************************************
 * �u���b�N�̏�����
 ***********************************************/
int BlockInit(void)
{
	FILE *fp;
	char *p,*end;

	
	switch (g_Stage)
	{
		case 0:
			fp = fopen("maps/1-1.csv", "r");
			break;
		case 11:
			fp = fopen("maps/1-1-1.csv", "r");
			break;
	}

	for (int i = 0; i < HLINE; i++) {
		fgets(g_IntBlock, sizeof(g_IntBlock), fp);
		p = g_IntBlock;
		for (int j = 0; j < WLINE; j++) {
			g_Block[i][j].image = strtol(p, &end, 10);         // 10�i����ǂݎ��
			p = end + 1;    // ',' ���X�L�b�v����
		}
	}
	fclose(fp);
	
	

	// �u���b�N�̏�����
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
				g_Block[h][w].flg = FALSE;
				g_Block[h][w].animeflg = FALSE;
				g_Block[h][w].x = w * size;
				g_Block[h][w].y = h * size;
				g_Block[h][w].mx = 0;
				g_Block[h][w].my = 0;
				g_Block[h][w].type = 0;
				g_Block[h][w].Item = 0;
				if(g_Block[h][w].image == 1) {g_Block[h][w].type = 2; g_Block[h][w].Item = 1;}
				if(g_Block[h][w].image == 5) g_Block[h][w].type = 1;
		}
	} 
	BlockOption();

	return 0;
}

/***********************************************
 * �u���b�N�̃^�C�v�ݒ� (0:�� 1:�󂹂� 2:�R�C������ 3:�A�C�e������ 4:�X�^�[ 5:1p�L�m�R)
 (0:e_typefalse 1:e_brokenblock 2:e_coinblock 3:e_kinoko 4:e_star 5:e_1pkinoko
 ***********************************************/
void BlockOption(void)
{
	switch (g_Stage)
	{
	case 0:
		g_Block[9][24].type = e_kinoko;
		g_Block[9][24].Item = 1;
		g_Block[7][71].type = e_1pkinoko;
		g_Block[7][71].Item = 1;
		g_Block[9][85].type = e_kinoko;
		g_Block[9][85].Item = 1;
		g_Block[9][101].type = e_timecoin;
		g_Block[9][101].Item = 50;
		g_Block[9][108].type = e_star;
		g_Block[9][108].Item = 1;
		g_Block[5][116].type = e_kinoko;
		g_Block[5][116].Item = 1;
		break;
	}
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
	g_Player.muki = e_RIGHT;
	g_Player.zanki = 3;
	g_Player.Scroll = 0;
}

/***********************************************
 * �G�l�~�[�̏�����
 ***********************************************/
void EnemyInit(void)
{
	for(int i = 0; i < ENEMYMAX; i++)
	{
		g_Enemy[i].flg = FALSE;
	}
}
/***********************************************
 * �R�C���̏�����
 ***********************************************/
void CoinInit(void)
{
	for(int i = 0; i < COINMAX; i++)
	{
		g_Coin[i].flg = FALSE;
	}
}

/***********************************************
 * �A�C�e���̏�����
 ***********************************************/
void ItemInit(void)
{
	for(int i = 0; i < ItemMAX; i++)
	{
		g_Item[i].flg = FALSE;
	}
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

	BlockMove();

	CoinMove();

	ItemMove();

	HitBox();

	DrawAnimeItem();

	DrawStage();

	DrawCoin();

	DrawItem();

	DrawPlayer();
	
	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE;
		if(g_Stage == 0){
			g_Stage = 11;
			g_TmpBlockMx = g_BlockMx;
			g_BlockMx = 0;
			g_StageType = 1;
			return;
		}
		if(g_Stage == 11){
			g_Stage = 0;
			g_BlockMx = g_TmpBlockMx;
			g_StageType = 0;
			return;
		}
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


	if(g_NowKey & PAD_INPUT_RIGHT && g_Player.muki == e_RIGHT) {
		g_Player.mx += 0.1;
		if(g_Player.mx < 0) g_Player.mx += 0.2;
		if(g_Player.mx > 5.0) g_Player.mx = 5.0;
		g_PushFlg = TRUE;
	}

	if(g_NowKey & PAD_INPUT_LEFT && g_Player.muki == e_LEFT) {
		g_Player.mx += -0.1;
		if(g_Player.mx > 0) g_Player.mx += -0.2;
		if(g_Player.mx < -5.0) g_Player.mx = -5.0;
		g_PushFlg = TRUE;
	}

	if(g_Player.x < 0){
		g_Player.x = 0;
		g_Player.mx = 0;
	}
	if(g_Player.x+size > WIDTH){
		g_Player.x = WIDTH-size;
		g_Player.mx= 0;
	}


	g_Player.x += g_Player.mx;
	
	//�X�N���[�������鏈��
	if(g_Player.x > size * MarioX && g_StageType == 0){
		g_BlockMx += g_Player.x - size * MarioX;
		g_Player.x = size * MarioX;
	}
	

	Gravity(&g_Player);

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

	
}

/***********************************************
 * �W�����v����
 ***********************************************/
void Jump(void)
{

	if(g_NowKey & PAD_INPUT_UP && g_Player.JumpFlag == 0 && g_Player.JumpCount < JAMPEND ) {

		if(g_Player.JumpCount < JAMPEND-18) g_Player.y -= 10;
		else if(g_Player.JumpCount < JAMPEND-12) g_Player.y -= 8;
		else if(g_Player.JumpCount < JAMPEND-6) g_Player.y -= 5;
		else if(g_Player.JumpCount < JAMPEND) g_Player.y -= 2;

		//g_Player.y -= 12;
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
void Gravity(s_PLAYER *p)
{
	if(p->JumpFlag == 1 && p->JumpCount != 0){
		/*if(g_Player.Gravitycount < 4) g_Player.y += 2;
		else if(g_Player.Gravitycount < 8) g_Player.y += 4;
		else if(g_Player.Gravitycount < 12) g_Player.y += 6;
		else if(g_Player.Gravitycount >= 12) g_Player.y += 8;*/

		if(p->Gravitycount < 4) p->my += 0.2;
		else if(p->Gravitycount < 8) p->my += 0.4;
		else if(p->Gravitycount < 12) p->my += 0.6;
		else if(p->Gravitycount >= 12) p->my += 0.8;

		p->y += p->my;

		p->Gravitycount++;
	}
	
	else if( p->JumpCount == 0){
		p->my += 1.0f;
		p->y += p->my;
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
	static int Trigger = 30;
	int anime = 1;

	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].x - g_BlockMx > -size && g_Block[h][w].x - g_BlockMx < WIDTH + size && g_Block[h][w].flg == FALSE){
				g_Block[h][w].flg = TRUE;
			}

			if(g_Block[h][w].x - g_BlockMx < -size && g_Block[h][w].flg == TRUE){
				g_Block[h][w].flg = FALSE;
			}
		}
	}


	if(Trigger-- > 0){
		anime = 1;
	}
	else{
		anime = g_Blockcount++ / 12 % 4 + 1;
		if(g_Blockcount > 12*4){
			g_Blockcount = 0;
			Trigger = 30;
		}
			
	}
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].flg == TRUE && g_Block[h][w].image >= 1 && g_Block[h][w].image <= 4){
				g_Block[h][w].image = anime;
			}
		}
	}

}


/***********************************************
 * �̂̓����蔻��
 ***********************************************/
void HitBox(void)
{
	int flg = 0;

	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].flg == TRUE && g_Block[h][w].image != -1){
				flg = 0;

				//�����蔻�肪�K�v�̂Ȃ�image�̔���@flg 0:�Ȃ� 1:����
				for(int i = 0; i < 23; i++){
					if(g_NoHitImage[i] == g_Block[h][w].image){
						flg = 1;
						break;
					}
				}
				
				//����̒���flg��0�Ȃ画�肷��
				if(flg == 0){
					//�u���b�N�̏㕔�i�s���N�F�j
					//BlockHitUp(h , w);
					if(BlockHitUp(&g_Player, &g_Block[h][w]) == TRUE){
						if(g_Block[h][w].image != 10 && g_Block[h][w].image != -1){
							g_Player.y = g_Block[h][w].y - size * (g_Player.size + 1);
							g_Player.my = 0;
							g_Player.JumpCount = 0;
							g_Player.Gravitycount = 0;
							if(g_NowKey & PAD_INPUT_UP) g_Player.JumpFlag = 1;
							else g_Player.JumpFlag = FALSE;
						}
						else g_Block[h][w].image = -1;
					}

					//�u���b�N�̉����i�s���N�F�j
					if(BlockHitDown(&g_Player, &g_Block[h][w]) == TRUE)
					{
						if(g_Block[h][w].image != 10 && g_Block[h][w].image != -1){
							g_Player.y = g_Block[h][w].y + size;
							g_Player.my = 0;
							g_Player.JumpFlag = FALSE;
							g_Player.JumpCount = JAMPEND;

							if(g_Block[h][w].animeflg != 1 && g_Block[h][w].image != 0) g_Block[h][w].animeflg++;

							if(g_Block[h][w].type == e_coinblock) CreateCoin( &g_Block[h][w] );
		
							else if(g_Block[h][w].type == e_timecoin){
								CreateCoin( &g_Block[h][w] );
								if(g_Block[h][w].Item == 50) g_Block[h][w].count = 300;
							}
							else if(g_Block[h][w].type >= e_kinoko) CreateItem( &g_Block[h][w] );

							else if(g_Block[h][w].type == e_brokenblock && g_Player.size != 0) g_Block[h][w].image = -1;
						}
						else g_Block[h][w].image = -1;
					}
				
					//�u���b�N�̉E��(�j
					if(BlockHitRight(&g_Player, &g_Block[h][w]) == TRUE)
					{
						if(g_Block[h][w].image != 10 && g_Block[h][w].image != -1){
							g_Player.x = g_Block[h][w].x + size - g_BlockMx - MARIOHITXH;
						}
						else g_Block[h][w].image = -1;
					}
				
					//�u���b�N�̍���(�j
					if(BlockHitLeft(&g_Player, &g_Block[h][w]) == TRUE)
					{
						if(g_Block[h][w].image != 10 && g_Block[h][w].image != -1){
							g_Player.x = g_Block[h][w].x - size - g_BlockMx + MARIOHITXH;
						}
						else g_Block[h][w].image = -1;
					}
				}

				
				
			}
			//DrawBox(g_Player.x + MARIOHITXH , g_Player.y,g_Player.x+size-MARIOHITXH,g_Player.y + size,0xff00ff,TRUE);
			//DrawBox(g_Block[h][w].x+3 , g_Block[h][w].y+16,g_Block[h][w].x+size-3,g_Block[h][w].y + size,0xff00ff,TRUE);
		}
	}


}
/***********************************************
 * �u���b�N�̏㉺���E�̓����蔻��
 ***********************************************/
//�u���b�N�̏�
int BlockHitUp(s_PLAYER *p, s_BLOCK *b)
{
	double px = p->x - p->Scroll;
	double px2 = px + size;
	double py = p->y;
	double py2 = py + size * (p->size + 1);
	double bx = b->x - g_BlockMx;
	double bx2 = bx + size;
	double by = b->y;
	double by2 = by + size;
	//DrawBox(px,py,px2,py2,0x000000,TRUE);

	if(px + MARIOHITXH < bx2 - BlockHitXH && bx + BlockHitXH < px2 - MARIOHITXH && py < by2 - BlockHitYH && by < py2){
		return TRUE;
	}
	return FALSE;

	
	
}

//�u���b�N�̉�
int BlockHitDown(s_PLAYER *p, s_BLOCK *b)
{
	double px = p->x - p->Scroll;
	double px2 = px + size;
	double py = p->y;
	double py2 = py + size * (p->size + 1);
	double bx = b->x - g_BlockMx;
	double bx2 = bx + size;
	double by = b->y;
	double by2 = by + size;

	if(px + MARIOHITXH < bx2 - BlockHitXH && bx + BlockHitXH < px2 - MARIOHITXH && py < by2 && by + BlockHitYH < py2){
		return TRUE;
	}
	return FALSE;
}

//�u���b�N�̍�
int BlockHitLeft(s_PLAYER *p, s_BLOCK *b)
{
	double px = p->x - p->Scroll;
	double px2 = px + size;
	double py = p->y;
	double py2 = py + size * (p->size + 1);
	double bx = b->x - g_BlockMx;
	double bx2 = bx + size;
	double by = b->y;
	double by2 = by + size;

	if(px + MARIOHITXH < bx2 - BlockHitXW && bx < px2 - MARIOHITXH && py < by2 - BlockHitYW && by + BlockHitYW < py2){
		return TRUE;
	}
	return FALSE;
}

//�u���b�N�̉E
int BlockHitRight(s_PLAYER *p,s_BLOCK *b)
{
	double px = p->x - p->Scroll;
	double px2 = px + size;
	double py = p->y;
	double py2 = py + size * (p->size + 1);
	double bx = b->x - g_BlockMx;
	double bx2 = bx + size;
	double by = b->y;
	double by2 = by + size;

	if(px + MARIOHITXH < bx2 && bx + BlockHitXW < px2 - MARIOHITXH && py < by2 - BlockHitYW && by + BlockHitYW < py2){
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 * p�v���C���[��o���̃I�u�W�F�N�g�̓����蔻��
 ***********************************************/

int Hit_P_Oup(s_PLAYER *p,s_PLAYER *o)
{
	double px = p->x - p->Scroll;
	double px2 = px + size;
	double py = p->y;
	double py2 = py + size * (p->size + 1);
	double ox = o->x - o->Scroll;
	double ox2 = ox + size;
	double oy = o->y;
	double oy2 = oy + size/2;

	if(px + MARIOHITXH < ox2 && ox < px2 - MARIOHITXH && py < oy2 && oy < py2){
		return TRUE;
	}
	return FALSE;

}

int Hit_P_Odown(s_PLAYER *p,s_PLAYER *o)
{
	double px = p->x - p->Scroll;
	double px2 = px + size;
	double py = p->y;
	double py2 = py + size * (p->size + 1);
	double ox = o->x - o->Scroll;
	double ox2 = ox + size;
	double oy = o->y + size/2;
	double oy2 = oy + size/2;

	if(px + MARIOHITXH < ox2 && ox < px2 - MARIOHITXH && py < oy2 && oy < py2){
		return TRUE;
	}
	return FALSE;

}


/***********************************************
 * �R�C���̐���
 ***********************************************/
void CreateCoin(s_BLOCK *b){
	for(int i = 0; i < COINMAX; i++){
		if(g_Coin[ i ].flg == FALSE ){
			g_Coin[i].flg = TRUE;
			g_Coin[i].img = 0;
			g_Coin[i].x = b->x;
			g_Coin[i].y = b->y-size;
			g_Coin[i].mx = 0;
			g_Coin[i].my = 10;
			g_Coin[i].animecount = 0;
			//PlaySoundMem(g_BakuhaSE,DX_PLAYTYPE_BACK);
			return ;
		}
	}

}
/***********************************************
 * �R�C���̈ړ�����
 ***********************************************/
void CoinMove(void){
	
	for(int i = 0; i < COINMAX; i++){
		if(g_Coin[i].flg == TRUE){

			if(g_Coin[i].animecount++ < 15){
				g_Coin[i].y -= g_Coin[i].my;
			}
			else if(g_Coin[i].animecount < 30){
				g_Coin[i].y += g_Coin[i].my;
			}
			else if(g_Coin[i].animecount <= 30){
				g_Coin[i].flg = 0;
				continue;
			}


		}
	}
	DrawFormatString(0,100,0x000000,"%d",g_Coin[0].animecount);
}

/***********************************************
 * �A�C�e���̐���
 ***********************************************/
void CreateItem(s_BLOCK *b){
	for(int i = 0; i < ItemMAX; i++){
		if(g_Item[ i ].flg == FALSE ){
			g_Item[i].flg = TRUE;
			g_Item[i].animeflg = TRUE;
			g_Item[i].muki = e_RIGHT;
			//12:�L�m�R 13:1P�L�m�R 24:�t�����[ 25:�X�^�[
			if(b->type == e_kinoko && g_Player.type == 0) g_Item[i].img = 12; 
			else if(b->type == e_kinoko && g_Player.type != 0) g_Item[i].img = 24;
			else if(b->type == e_star) g_Item[i].img = 25;
			else if(b->type == e_1pkinoko) g_Item[i].img = 13;
			g_Item[i].x = b->x;
			g_Item[i].y = b->y-size;
			g_Item[i].JumpCount = 0;
			g_Item[i].JumpFlag = 0;
			if(g_Item[i].img == 12 || g_Item[i].img == 13 || g_Item[i].img == 25) g_Item[i].mx = 2;
			else if(g_Item[i].img == 24) g_Item[i].mx = 0;
			g_Item[i].my = -size;
			g_Item[i].animecount = 0;
			g_Item[i].Gravitycount = 0;
			g_Item[i].Scroll = g_BlockMx;
			return ;
		}
	}

}

/***********************************************
 * �A�C�e���̈ړ�����
 ***********************************************/
void ItemMove(void){

	int flg = 0;

	for(int i = 0; i < ItemMAX; i++){
		if(g_Item[i].flg == TRUE && g_Item[i].animeflg == 2){
			if(g_Item[i].muki == e_RIGHT)
				g_Item[i].x += g_Item[i].mx;

			else if(g_Item[i].muki == e_LEFT)
				g_Item[i].x -= g_Item[i].mx;
			
			g_Item[i].Scroll = g_BlockMx;

			//g_Item[i].y+=5;
			Gravity(&g_Item[i]);

			for(int h = 0;h < HLINE; h++){
				for(int w = 0; w < WLINE; w++){
					if(g_Block[h][w].flg == TRUE && g_Block[h][w].image != -1){

						flg = 0;

						for(int j = 0; j < 23; j++){
							if(g_NoHitImage[j] == g_Block[h][w].image){
								flg = 1;
								break;
							}
						}

						
						
						if(flg == 0){
							if(BlockHitUp(&g_Item[i],&g_Block[h][w]) == TRUE){
								g_Item[i].JumpFlag = 0;
								g_Item[i].y = g_Block[h][w].y - size;
								g_Item[i].Gravitycount = 0;
								g_Item[i].my = 0;
							}
							if(BlockHitDown(&g_Item[i],&g_Block[h][w]) == TRUE){
							}
							if(BlockHitRight(&g_Item[i],&g_Block[h][w]) == TRUE){
								g_Item[i].muki = e_RIGHT;
							}
							if(BlockHitLeft(&g_Item[i],&g_Block[h][w]) == TRUE){
								g_Item[i].muki = e_LEFT;
								
							}
						}
					}
				}
			}
			if(Hit_P_Oup(&g_Player,&g_Item[i]) == TRUE){
				if(g_Item[i].img == 12) g_Player.type = 1;
				else if(g_Item[i].img == 24) g_Player.type = 2;
				else if(g_Item[i].img == 13) g_Player.zanki++;
				if(g_Item[i].img == 12 || g_Item[i].img == 24) g_Player.size = 1;
				g_Item[i].flg = FALSE;
				//if(g_Item[i].size == 0) g_Player.y = g_Player.y - size;
			}
			else if(Hit_P_Odown(&g_Player,&g_Item[i]) == TRUE){
				if(g_Item[i].img == 12) g_Player.type = 1;
				else if(g_Item[i].img == 24) g_Player.type = 2;
				else if(g_Item[i].img == 13) g_Player.zanki++;
				if(g_Item[i].img == 12 || g_Item[i].img == 24) g_Player.size = 1;
				g_Item[i].flg = FALSE;
				//if(g_Item[i].size == 0) g_Player.y = g_Player.y - size;

			}

			if(g_Item[i].x - g_BlockMx< -size || g_Item[i].x - g_BlockMx > WIDTH + size || g_Item[i].y > 480){
				g_Item[i].flg = FALSE;
			}
			
		}

	}

}
/***********************************************
 * �X�e�[�W�̕`�揈��
 ***********************************************/
void DrawStage(void)
{
	int flg = 0;

	
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].flg == TRUE && g_Block[h][w].image != -1){

				//�u���b�N�̏㉺�ړ������@//��ړ�
				if(g_Block[h][w].animeflg == 1){
					g_Block[h][w].my--;
					if(g_Block[h][w].my == -5) g_Block[h][w].animeflg = 2;
				}
				//���ړ�
				else if(g_Block[h][w].animeflg == 2){
					g_Block[h][w].my++;
					if(g_Block[h][w].my == 0) {
						g_Block[h][w].animeflg = 0;
						g_Block[h][w].my = 0;
						g_Block[h][w].Item--;
						//�u���b�N���R�C���܂��̓A�C�e��������A�A�C�e���̐����O�ł�������
						if(g_Block[h][w].type >= 2 && g_Block[h][w].Item == 0){
							g_Block[h][w].image = 0;
							g_Block[h][w].type = e_typefalse;
						}
					}
				}

				//�^�C�}�[�R�C���̏���
				if(g_Block[h][w].type == e_timecoin && g_Block[h][w].Item != 50)
				{
					g_Block[h][w].count--;
					if(g_Block[h][w].count <= 0) g_Block[h][w].Item = 1;
				}

				//�����蔻�肪�K�v�̂Ȃ�image�̔���@flg 0:�Ȃ� 1:����
				for(int i = 0; i < 23; i++){
					if(g_NoHitImage[i] == g_Block[h][w].image){
						flg = 1;
						break;
					}
				}

				DrawGraph(g_Block[h][w].x - g_BlockMx ,g_Block[h][w].y + g_Block[h][w].my,g_BlockImage[g_Block[h][w].image],TRUE);
				if(flg == 0){
				DrawBox(g_Block[h][w].x + BlockHitXH - g_BlockMx ,g_Block[h][w].y,   g_Block[h][w].x+size- BlockHitXH - g_BlockMx, g_Block[h][w].y + size- BlockHitYH,0xff00ff,TRUE); //�㓖���蔻��`��
				DrawBox(g_Block[h][w].x + BlockHitXH - g_BlockMx ,g_Block[h][w].y + BlockHitYH,g_Block[h][w].x+size- BlockHitXH - g_BlockMx, g_Block[h][w].y+size,   0xff00ff,TRUE); //�������蔻��`��
				DrawBox(g_Block[h][w].x - g_BlockMx, g_Block[h][w].y + BlockHitYW, g_Block[h][w].x+size - BlockHitXW - g_BlockMx, g_Block[h][w].y+size - BlockHitYW, 0x00FFFF,TRUE);//�E
				DrawBox(g_Block[h][w].x + BlockHitXW - g_BlockMx, g_Block[h][w].y + BlockHitYW, g_Block[h][w].x + size - g_BlockMx, g_Block[h][w].y+size - BlockHitYW, 0x00FFFF,TRUE);//��
				}
				DrawFormatString(g_Block[h][w].x - g_BlockMx ,g_Block[h][w].y,0x000000,"%d",g_Block[h][w].image);

				flg = 0;
				//if(g_Block[h][w].image == 1 || g_Block[h][w].image == 5 || g_Block[h][w].image == 80)
					//DrawFormatString(g_Block[h][w].x-g_BlockMx,g_Block[h][w].y,0x000000,"%1.1f,%1.1f\nT:%d",g_Block[h][w].y/32,g_Block[h][w].x/32,g_Block[h][w].type);
			}
		}
	}

	DrawFormatString(0,20,0xff88ff,"Px:%1.1f BMx%1.1f",g_Player.x + g_BlockMx , g_BlockMx);
	
}

/***********************************************
 * �v���C���[�̕`��
 ***********************************************/
void DrawPlayer(void)
{
		//�E�ړ�
	if(g_Player.muki == e_RIGHT && g_Player.JumpCount != 0) DrawGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][4],TRUE);

	else if(g_Player.muki == e_RIGHT && g_Player.mx > 0){
		DrawGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][g_Mariocount++/10%3 + 1],TRUE);
	}

	else if(g_Player.muki == e_RIGHT && g_Player.mx < 0){
		if(g_Player.size == 0) DrawGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][6],TRUE);
		else DrawGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][7],TRUE);
	}

	else if(g_Player.muki == e_RIGHT){
		DrawGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][0],TRUE);
	}

		//���ړ�
	if(g_Player.muki == e_LEFT && g_Player.JumpCount != 0) DrawTurnGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][4],TRUE);

	else if(g_Player.muki == e_LEFT && g_Player.mx < 0){
		DrawTurnGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][g_Mariocount++/10%3 + 1],TRUE);
	}

	else if(g_Player.muki == e_LEFT && g_Player.mx > 0){
		if(g_Player.size == 0) DrawTurnGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][6],TRUE);
		else DrawTurnGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][7],TRUE);
	}

	else if(g_Player.muki == e_LEFT){
		DrawTurnGraph(g_Player.x,g_Player.y,g_PlayerImage[g_Player.type][0],TRUE);
	}
	
	if(g_Mariocount > 10000)
		g_Mariocount = 0;

}
/***********************************************
 * �G�l�~�[�̕`��
 ***********************************************/
void DrawEnemy(void)
{
	
}

/***********************************************
 * �R�C���̕`�揈��
 ***********************************************/
void DrawCoin(void){
	for(int i = 0; i < COINMAX; i++){
		if(g_Coin[i].flg == TRUE){
			DrawGraph(g_Coin[i].x - g_BlockMx,g_Coin[i].y,g_BlockImage[26+(g_Coin[i].animecount/8%4)],TRUE);
		}
	}
}

/***********************************************
 * �A�C�e���̕`�揈��
 ***********************************************/
void DrawItem(void){
	for(int i = 0; i < ItemMAX; i++){
		if(g_Item[i].flg == TRUE && g_Item[i].animeflg == 2){

			DrawGraph(g_Item[i].x - g_Item[i].Scroll,g_Item[i].y,g_BlockImage[g_Item[i].img],TRUE);

		}
	}
}

/***********************************************
 * �A�C�e���̕`�揈��
 ***********************************************/
void DrawAnimeItem(void){
	for(int i = 0; i < ItemMAX; i++){
		if(g_Item[i].flg == TRUE){
			g_Item[i].Scroll = g_BlockMx;
			if(g_Item[i].animeflg == 1){
				g_Item[i].my++;
				if(g_Item[i].my >= 0){
					g_Item[i].my = 0;
					g_Item[i].animeflg = 2;
				}
				DrawGraph(g_Item[i].x - g_Item[i].Scroll,g_Item[i].y - g_Item[i].my,g_BlockImage[g_Item[i].img],TRUE);
				
			}
			

		}
	}
}


/***********************************************
 * �摜�Ǎ���
 ***********************************************/
int LoadImages()
{
	//�v���C���[
	//if((g_PlayerImage[0] = LoadGraph("images/player.png")) == -1) return -1;
	if(LoadDivGraph("images/mario_chara.png",15,5,3,32,32,g_PlayerImage[0]) == -1) return -1;
	if(LoadDivGraph("images/super_mario_chara.png",15,5,3,32,64,g_PlayerImage[1]) == -1) return -1;
	if(LoadDivGraph("images/fire_mario_chara .png",15,5,3,32,64,g_PlayerImage[2]) == -1) return -1;
	if(LoadDivGraph("images/backimage.png",84,12,7,32,32,g_BlockImage) == -1) return -1;
	
	return 0;
} 

