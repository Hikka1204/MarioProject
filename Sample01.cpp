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
 * 列挙体の宣言
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
 * 変数の宣言
 ***********************************************/
int g_OldKey; // 前回の入力キー
int g_NowKey; // 今回の入力キー
int g_KeyFlg; // 入力キー情報

int g_GameState = GAME_TITLE; // ゲームモード

int g_TitleImage; // タイトル画像

int g_PlayerImage[2];		//プレイヤーの画像
int g_MukiImage[4];		//方向の画像
int g_EnemyImage[ENEMYMAX][2];		//エネミーの画像
int g_EnemyEyeImage[4];			//エネミーの目
int g_EsaImage;		//クッキーの画像
int g_suponImage;		//スポーンの画像
int g_EnemyPowerImage[2];	//エネミーが弱い時の画像
int g_PowerImage;			//パワークッキーの画像
int g_WapuImage;			//ワープの画像


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

int g_cookie;		//クッキー

int g_animecount;		//ゲームのカウント
int g_animescore;

int g_taikakuX;		//対角型のＸ
int g_taikakuY;		//対角型のＹ

int g_Stage;			//ステージの変数

int g_WaitTime;		//エンドの変数



/***********************************************
 * 構造体の宣言
 ***********************************************/
struct Object {
 int flg; // フラグ(0:表示しない ,1:表示)
 int x,y; // 座標 x,y
 int w,h; // 幅 w 高さ h
 int image; // 画像
 int backup; // 画像バックアップ
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
	int flg;		//	0.無 1.メイン 2.当たり 3.パワー
	int x,y;		//	初期の座標
	int mx,my;	//	動く処理の座標
	int mx2,my2;	//	敵の追跡処理に使う座標
	int muki;	//	パックマンの向き
	int muki2;	//	パックマン向き
	int img;		//	画像変数
	int count;	//	一回進む時間
	int zanki;	//	残機
};
struct PLAYER g_Player;

struct ENEMY{
	int flg;		//	0.無 1.メイン 2.パワー 3.退避
	int x,y;		//	初期の座標
	int mx,my;	//	動く処理の座標
	int muki;	//	敵の向き
	int muki2;	//	敵の進むべきでない向き
	int img;		//	画像変数
	int count;	//	一回進む時間
	int idouflg;	//	移動したかしてないかの確認
};				 
struct ENEMY g_Enemy[ENEMYMAX];
/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期化処理
void GameMain(void); //ゲームメイン処理

void DrawGameTitle(void); //ゲームタイトル描画処理
void DrawGameOver(void); //ゲームオーバー描画処理
void DrawClear(void);
void DrawEnd(void); //エンド描画処理

void DrawHelp(void); //ヘルプ描画処理

void DrawResult(void);		//リザルト描画処理

void EsaInit(void);
void PlayerInit(void);	//プレイヤーの初期化
void EnemyInit(void);		//エネミーの初期化

void PlayerMove(void);	//プレイヤーの移動処理
void EnemyMove(void);		//エネミーの移動処理
void Xhantei(int e);		//縦方向の移動判定
void Yhantei(int e);		//横方向の移動判定
int XYhantei(int x, int y, int x2, int y2);	//最短ルートの判定 TRUE:xが小さい FALSE:yが小さい
void eRandMove(int e);	//ランダムに移動する敵の処理

void HitBox(void);	//当たり判定

void DrawStage(void);		//ステージの描画
void DrawPlayer(void);	//プレイヤーの描画
void DrawEnemy(void);		//エネミーの描画
void RandMove(int e);		//ランダムに移動
void EndInit(void);		//エンドの初期化

int LoadImages(); //画像読込み
int LoadMusic();	//音楽の読み込み

void taikaku(void);	//対角の取得
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

		ClearDrawScreen(); // 画面の初期化
		switch (g_GameState){
			case GAME_TITLE:
				DrawGameTitle(); //タイトル処理
				break;
			case GAME_INIT:
				GameInit(); //ゲーム初期処理
				break;
			case GAME_HELP:
				DrawHelp(); //ランキング描画処理
				break;
			case GAME_END:
				DrawEnd(); //エンド描画処理
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
	//メニューカーソル移動処理
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

	SetFontSize(Font_Size_M+HELPs);
	 x = WIDTH/2 - GetDrawStringWidth("HELP", 4)/2;		//文字列の長さを図ってる　中央寄せにできる
	DrawString(x, y, "HELP",GetColor(0,255,0));
	y += 45;

	SetFontSize(Font_Size_M+ENDs);
	 x = WIDTH/2 - GetDrawStringWidth("END", 3)/2;		//文字列の長さを図ってる　中央寄せにできる
	DrawString(x, y, "END",GetColor(255,0,0));
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
 * プレイヤーの初期化
 ***********************************************/
void PlayerInit(void)
{

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
 * リザルト描画処理
 ***********************************************/
void DrawResult(void)
{


	SetFontSize(Font_Size_S);

	if(g_KeyFlg == PAD_INPUT_M){
		g_GameState = GAME_TITLE; 
	}
	int x = WIDTH/2 - GetDrawStringWidth("スペースキーを押してタイトルへ", 15)/2;		//文字列の長さを図ってる　中央寄せにできる
	if(g_animecount++ % 60 <= 30){
		DrawString(x, HEIGHT-20, "スペースキーを押してタイトルへ",GetColor(200,200,255));
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
	DrawStage();
	
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
	
}
/***********************************************
 * エネミーの移動処理
 ***********************************************/
void EnemyMove(void)
{

}

/***********************************************
 * ステージの描画処理
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
 * プレイヤーの描画
 ***********************************************/
void DrawPlayer(void)
{


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
	//if((g_PlayerImage[0] = LoadGraph("images/player.png")) == -1) return -1;
	//if((g_PlayerImage[1] = LoadGraph("images/player2.png")) == -1) return -1;


	return 0;
} 

