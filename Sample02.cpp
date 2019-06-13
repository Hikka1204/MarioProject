/********************************************************************
** 
** 
********************************************************************/
#include "DxLib.h"
/***********************************************
 * 列挙体の宣言
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
 * 定数の宣言
 ***********************************************/

const int size = 32;			//ブロックサイズ
const int WIDTH = 640;			//画面の横
const int HEIGHT = 480;			//画面の縦
const int WLINE = 40;			//横のライン
const int HLINE = 15;			//縦のライン
const int ENEMYMAX = 2;			//敵の最大表示数
const int MarioX = 7;			//マリオ初期位置
const int MarioY = 0;			
const int Font_Size_S = 16;		//フォントサイズ
const int Font_Size_M = 32;
const int Font_Size_L = 64;
const int BlockHitX = 5;		//当たり判定用


/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey; // 前回の入力キー
int g_NowKey; // 今回の入力キー
int g_KeyFlg; // 入力キー情報
int g_PushFlg;	//TRUE:押してる　FALSE:押されてない

int g_GameState = GAME_TITLE; // ゲームモード

int g_TitleImage; // タイトル画像

int g_PlayerImage[2];		//プレイヤーの画像
int g_MukiImage[4];		//方向の画像
int g_EnemyImage[ENEMYMAX][2];		//エネミーの画像


int g_PowerCount;		//パワーカウント

int g_GameScore;		//一回で取得したスコアの数
int g_Score;			//スコア
int g_MaxScore;		//多くとったスコアの数
int g_TmpGameScore;	//ゲームクリアとゲームオーバー時の演出
int g_TmpScore;		//上記と同じ
int g_EatEnemy;		//エネミーを食べた数
int g_PowerCookie;	//パワークッキーを食べた数
int g_GamePlay;		//プレイ回数
int g_GameOver;		//ゲームオーバーの数
int g_GameClear;		//ゲームクリアの数


int g_animecount;		//ゲームのカウント

int g_BlockMx;		//ブロックの横の移動量

int g_Stage;			//ステージの変数

int g_WaitTime;		//エンドの変数





/***********************************************
 * 構造体の宣言
 ***********************************************/
struct s_BLOCK {
 int flg; // フラグ(0:表示しない ,1:表示)
 double x,y; // 座標 x,y
 int w,h; // 幅 w 高さ h
 int image; // 画像
 int backup; // 画像バックアップ
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
	int flg;		//	0.無 1.メイン 2.当たり 3.パワー
	double x,y;		//	初期の座標
	double mx,my;	//	動く処理
	int muki;	//	マリオの向き
	int type;	//	動きのタイプ
	int img;		//	画像変数
	int Jump;	//	繰り返しに使う 
	int JumpCount;	//	ジャンプ時間
	int JumpFlag;	//ジャンプしているかしてないか
	int zanki;	//	残機
};
struct s_PLAYER g_Player;

struct s_ENEMY{
	int flg;		//	0.無 1.メイン 2.パワー 3.退避
	double x,y;		//	初期の座標
	double mx,my;	//	動く処理の座標
	int muki;	//	敵の向き
	int img;		//	画像変数
	int count;	//	一回進む時間
	int idouflg;	//	移動したかしてないかの確認
};				 
struct s_ENEMY g_Enemy[ENEMYMAX];
/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期化処理
void GameMain(void); //ゲームメイン処理

void DrawGameTitle(void); //ゲームタイトル描画処理
void DrawGameOver(void); //ゲームオーバー描画処理
void DrawClear(void);


void PlayerInit(void);	//プレイヤーの初期化
void EnemyInit(void);		//エネミーの初期化

void PlayerMove(void);	//プレイヤーの移動処理
void Gravity(void);		//重力処理
void Jump(void);			//ジャンプ処理
void BlockMove(void);		//ブロックの移動処理


void HitBox(void);	//体の当たり判定
void HitHead(void);	//頭の当たり判定

void DrawStage(void);		//ステージの描画
void DrawPlayer(void);	//プレイヤーの描画
void DrawEnemy(void);		//エネミーの描画

int LoadImages(); //画像読込み
/***********************************************
 * プログラムの開始
 ***********************************************/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
							LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText( "スーパーマリオブラザーズ" ); // タイトルを設定

	ChangeWindowMode(TRUE); // ウィンドウモードで起動

	if ( DxLib_Init() == -1 ) return -1; // DX ライブラリの初期化処理

	SetDrawScreen( DX_SCREEN_BACK ); // 描画先画面を裏にする

	if ( LoadImages() == -1 ) return -1; // 画像読込み関数を呼び出し

	// ゲームループ
	while ( ProcessMessage() == 0 && g_GameState != END && !(g_KeyFlg & PAD_INPUT_START)){
		
		// 入力キー取得
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState( DX_INPUT_KEY_PAD1 );
		g_KeyFlg = g_NowKey & ~g_OldKey; 

		g_PushFlg = FALSE;

		ClearDrawScreen(); // 画面の初期化
		switch (g_GameState){
			case GAME_TITLE:
				DrawGameTitle(); //タイトル処理
				break;
			case GAME_INIT:
				GameInit(); //ゲーム初期処理
				break;
			case GAME_MAIN:
				GameMain(); //ゲームメイン処理
				break;
			case GAME_OVER:
				DrawGameOver(); //ゲームオーバー描画処理
				break;
			case GAME_CLEAR:
				DrawClear(); //ゲームクリア描画処理
				break;
		}

		ScreenFlip(); // 裏画面の内容を表画面に反映
	}

	DxLib_End(); // DX ライブラリ使用の終了処理

	return 0; // ソフトの終了
} 

/***********************************************
 * ゲームタイトル描画処理（メニュー画面）
 ***********************************************/
void DrawGameTitle(void)
{

	static int MenuNo = 0;
	if(g_GamePlay == 0) g_Player.zanki = 3;

	int PosY;

	int STARTs = 10;

	// Ｚキーでメニュー選択
	if(g_KeyFlg & PAD_INPUT_M){
		g_GameState = MenuNo + 1;
	}

	// メニューカーソル（パックマン）の表示
	PosY = MenuNo * 45;

	SetFontSize(Font_Size_L*2);

	////見出し////

	//題名
	SetFontThickness(9);	//太さ
	int x = WIDTH/2 - GetDrawStringWidth("PacMan", 6)/2;		//文字列の長さを図ってる　中央寄せにできる
	DrawString(x, 60, "PacMan",GetColor(200,200,255));

	
	int y = 240;

	SetFontSize(Font_Size_M+STARTs);
	 x = WIDTH/2 - GetDrawStringWidth("START", 5)/2;		//文字列の長さを図ってる　中央寄せにできる
	 DrawFormatString(x, y, GetColor(0,0,255),"START");
	//DrawString(x, y, "START",GetColor(0,0,255));
	y += 45;

	SetFontSize(Font_Size_S);

	x = WIDTH/2 - GetDrawStringWidth("↑キーで上に、↓キーで下に、スペースキーで決定", 23)/2;		//文字列の長さを図ってる　中央寄せにできる
	DrawString(x, HEIGHT-20, "↑キーで上に、↓キーで下に、スペースキーで決定",GetColor(200,200,255));
	
}

/***********************************************
 * ゲーム初期処理
 ***********************************************/
void GameInit(void)
{
	// ゲームメイン処理へ
	g_GameState = GAME_MAIN;

	SetFontSize(Font_Size_S);
	SetFontThickness(3);

 	

	// ブロックの初期化
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
 * プレイヤーの初期化
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
 * エネミーの初期化
 ***********************************************/
void EnemyInit(void)
{

}

/***********************************************
 * ヘルプ描画処理
 ***********************************************/
void DrawHelp(void)
{


	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE;
	}

	SetFontSize(Font_Size_S);
	
	int x = WIDTH/2 - GetDrawStringWidth("スペースキーを押してタイトルへ", 15)/2;		//文字列の長さを図ってる　中央寄せにできる
	if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT - 20, "スペースキーを押してタイトルへ",GetColor(200,200,255));
	}

}




/***********************************************
 * ゲームエンド描画処理
 ***********************************************/
void DrawEnd(void)
{


	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE; 
	}

	SetFontSize(Font_Size_S);
	int x = WIDTH/2 - GetDrawStringWidth("スペースキーを押してタイトルへ", 15)/2;		//文字列の長さを図ってる　中央寄せにできる
	if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT-20, "スペースキーを押してタイトルへ",GetColor(200,200,255));
	}
}


/***********************************************
 * ゲームメイン
 ***********************************************/
void GameMain(void)
{
	PlayerMove();

	DrawStage();

	DrawPlayer();
	
	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE; 
	}
	int x = WIDTH/2 - GetDrawStringWidth("スペースキーを押してタイトルへ", 15)/2;		//文字列の長さを図ってる　中央寄せにできる
		if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT-20, "スペースキーを押してタイトルへ",GetColor(200,200,255));
	}


}
/***********************************************
 * ゲームオーバー描画処理
 ***********************************************/
void DrawGameOver(void)
{
	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE;
	}

	SetFontSize(Font_Size_S);
	SetFontThickness(6);

	int x = WIDTH/2 - GetDrawStringWidth("スペースキーを押してタイトルへ", 15)/2;		//文字列の長さを図ってる　中央寄せにできる
	if(g_animecount % 60 <= 30){
		DrawString(x, HEIGHT-20, "スペースキーを押してタイトルへ",GetColor(200,200,255));
	}


}

/***********************************************
 * ゲームクリア描画処理
 ***********************************************/
void DrawClear(void)
{
	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE;
	}



	SetFontSize(Font_Size_S);
	SetFontThickness(5);	//太さ
	int x = WIDTH/2 - GetDrawStringWidth("スペースキーを押してタイトルへ", 15)/2;		//文字列の長さを図ってる　中央寄せにできる
	if(g_animecount % 60 <= 30){
		DrawString(x, HEIGHT-20, "スペースキーを押してタイトルへ",GetColor(200,200,255));
	}

}

/***********************************************
 * プレイヤーの移動処理
 ***********************************************/
void PlayerMove(void)
{
	if(g_KeyFlg & PAD_INPUT_RIGHT) g_Player.muki = e_RIGHT;
	if(g_KeyFlg & PAD_INPUT_LEFT) g_Player.muki = e_LEFT;

	Jump();	//ジャンプ処理

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
 * ジャンプ処理
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
 * 重力処理
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
 * エネミーの移動処理
 ***********************************************/
void EnemyMove(void)
{

}

/***********************************************
 * ブロックの移動処理
 ***********************************************/
void BlockMove(void)
{
	
}


/***********************************************
 * 体の当たり判定
 ***********************************************/
void HitBox(void)
{
	
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].flg == TRUE && g_Block[h][w].image != MITI){
				//ブロックの上部（ピンク色）
				if(g_Player.x < g_Block[h][w].x+size - 5 - g_BlockMx && g_Block[h][w].x + 5 - g_BlockMx < g_Player.x + size && 
						g_Player.y < g_Block[h][w].y + size - 30 && g_Block[h][w].y < g_Player.y + size){
					g_Player.y = g_Block[h][w].y-size;
					g_Player.my = 0;
					if(g_NowKey & PAD_INPUT_UP) g_Player.JumpFlag = TRUE;
					else g_Player.JumpFlag = FALSE;
					g_Player.JumpCount = 0;
					return ;
				}

				//ブロックの下部（ピンク色）
				if(g_Player.x < g_Block[h][w].x + size - 5 - g_BlockMx && g_Block[h][w].x + 5 - g_BlockMx < g_Player.x+size 
					&& g_Player.y < g_Block[h][w].y + size && g_Block[h][w].y+30 < g_Player.y+size){
					g_Player.y = g_Block[h][w].y+size;
					g_Player.my = 0;
					g_Player.JumpFlag = FALSE;
					g_Player.JumpCount = 16;
					return ;
				}
				//ブロックの右部(青）
				if(g_Player.x < g_Block[h][w].x + size-26 - g_BlockMx && g_Block[h][w].x + 4 - g_BlockMx < g_Player.x+size 
					&& g_Player.y < g_Block[h][w].y+size-1 && g_Block[h][w].y+1 < g_Player.y+size){
					g_Player.x = g_Block[h][w].x-size - g_BlockMx;
					return ;
				}
				//ブロックの左部(青）
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
 * ステージの描画処理
 ***********************************************/
void DrawStage(void)
{
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			switch(g_Block[h][w].image){

				case KABE:
					DrawBox(w * size-g_BlockMx,h * size, (w + 1) * size-g_BlockMx,(h + 1) * size,0xff7f50,TRUE);
					DrawBox(g_Block[h][w].x+5 - g_BlockMx ,g_Block[h][w].y,   g_Block[h][w].x+size-5 - g_BlockMx, g_Block[h][w].y+size-30,0xff00ff,TRUE); //上当たり判定描画
					DrawBox(g_Block[h][w].x+5 - g_BlockMx ,g_Block[h][w].y+30,g_Block[h][w].x+size-5 - g_BlockMx, g_Block[h][w].y+size,   0xff00ff,TRUE); //下当たり判定描画
					DrawBox(g_Block[h][w].x+4 - g_BlockMx, g_Block[h][w].y+1, g_Block[h][w].x+size-26- g_BlockMx, g_Block[h][w].y+size-1, 0x00FFFF,TRUE);//右
					DrawBox(g_Block[h][w].x+26- g_BlockMx, g_Block[h][w].y+1, g_Block[h][w].x+size-4 - g_BlockMx, g_Block[h][w].y+size-1, 0x00FFFF,TRUE);//左
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
 * プレイヤーの描画
 ***********************************************/
void DrawPlayer(void)
{
	DrawGraph(g_Player.x,g_Player.y,g_PlayerImage[0],TRUE);

}
/***********************************************
 * エネミーの描画
 ***********************************************/
void DrawEnemy(void)
{
	
}
/***********************************************
 * 画像読込み
 ***********************************************/
int LoadImages()
{
		//プレイヤー
	if((g_PlayerImage[0] = LoadGraph("images/player.png")) == -1) return -1;
	//if((g_PlayerImage[1] = LoadGraph("images/player2.png")) == -1) return -1;



	return 0;
} 

