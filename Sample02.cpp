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
 * 定数の宣言
 ***********************************************/

const int size = 32;		//ブロックサイズ
const int WIDTH = 640;
const int HEIGHT = 480;
const int WLINE = 230; //40;
const int HLINE = 15;
const int ENEMYMAX = 5;
const int MarioX = 9;
const int Font_Size_S = 16;
const int Font_Size_M = 32;
const int Font_Size_L = 64;
const int BlockHitXH = 5;		//ブロックの幅の微調整
const int BlockHitYH = 30;	
const int BlockHitXW = 26;
const int BlockHitYW = 1;
const int MARIOHITXH = 7;	//マリオの横幅の調整（当たり判定）
const int JAMPEND = 24;	//ジャンプの飛距離
const int COINMAX = 5;	//コインの最大数
const int ItemMAX = 2;	//アイテムの最大数

/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey; // 前回の入力キー
int g_NowKey; // 今回の入力キー
int g_KeyFlg; // 入力キー情報
int g_PushFlg;	//TRUE:押してる　FALSE:押されてない

int g_GameState = GAME_TITLE; // ゲームモード

int g_TitleImage; // タイトル画像
int g_BlockImage[84]; //ブロックの画像
int g_PlayerImage[3][15];		//プレイヤーの画像
int g_MukiImage[4];		//方向の画像
int g_EnemyImage[ENEMYMAX][2];		//エネミーの画像


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

int g_Blockcount;		//ブロック用アニメカウント

int g_Mariocount;		//マリオ用アニメカウント

int g_animecount;


double g_BlockMx;		//ブロックの横の移動量
double g_TmpBlockMx;	//上のセーブ用

int g_Stage;			//ステージの変数
int g_StageType;		//ステージのタイプ 0:ブロックを移動させる　1:させない;

int g_WaitTime;		//エンドの変数

/***********************************************
 * 構造体の宣言
 ***********************************************/
struct s_BLOCK {
 int flg; // フラグ(0:表示しない ,1:表示)
 int animeflg;	//0:アニメしない 1:上がる 2:下がる
 double x,y; // 座標 x,y
 double mx,my;	//アニメ用の動かす変数
 int w,h; // 幅 w 高さ h
 int image; // 画像
 int backup; // 画像バックアップ
 int Item;	//アイテムの数
 int type;	//0:無 1:壊せる 2:コイン入り 3:タイマー付きコイン 4:アイテム入り 5:スター 6:1pキノコ
 int count;	//カウント
};
struct s_BLOCK g_Block[HLINE][WLINE]; 

char g_IntBlock[HLINE*WLINE];

int g_NoHitImage[23] = {14,15,16,17,18,19,20,21,22,23,30,31,32,33,34,35,42,43,44,45,46,47,59};
 
struct s_PLAYER{
	int flg;		//	0.無 1.メイン 2.当たり 3.パワー
	int animeflg;	//アニメフラグ
	double x,y;		//	座標
	double mx,my;	//	動く処理
	int size;		//0:子供 1:大人
	int muki;	//	マリオの向き
	int type;	//	動きのタイプ
	int img;		//	画像変数
	int JumpCount;	//	ジャンプ時間
	int JumpFlag;	//0:空中にいない 1:いる
	int zanki;	//	残機
	int animecount;	//アニメ用カウント
	double Scroll;
	int Gravitycount;
};
struct s_PLAYER g_Player;
struct s_PLAYER g_Coin[5];
struct s_PLAYER g_Enemy[ENEMYMAX];
struct s_PLAYER g_Item[ItemMAX];

struct s_ENEMY{
	int flg;		//	0.無 1.描画
	int animeflg;	//アニメのフラグ
	double x,y;		//	座標
	double mx,my;	//	動く処理の座標
	int muki;	//	敵の向き
	int img;		//	画像変数
	int count;	//	カウント
};				 
/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期化処理
void GameMain(void); //ゲームメイン処理
void BlockOption(void);

void DrawGameTitle(void); //ゲームタイトル描画処理
void DrawGameOver(void); //ゲームオーバー描画処理
void DrawClear(void);		//ゲームクリア描画処理

int BlockInit();		//ブロックの初期化
void PlayerInit(void);	//プレイヤーの初期化
void EnemyInit(void);		//エネミーの初期化
void CoinInit(void);		//コインの初期化
void ItemInit(void);		//アイテムの初期化

void PlayerMove(void);	//プレイヤーの移動処理
void CoinMove(void);		//コインの移動処理
void ItemMove(void);		//アイテムの移動処理
void EnemyMove(void);		//エネミーの移動処理

void Gravity(s_PLAYER *p );		//重力処理
void Jump(void);			//ジャンプ処理
void BlockMove(void);		//ブロックの移動処理


void HitBox(void);	//体の当たり判定
int BlockHitUp(s_PLAYER *p,s_BLOCK *b);	//ブロックの上
int BlockHitDown(s_PLAYER *p,s_BLOCK *b);	//ブロックの下
int BlockHitRight(s_PLAYER *p,s_BLOCK *b);	//ブロックの右
int BlockHitLeft(s_PLAYER *p,s_BLOCK *b);	//ブロックの左

int Hit_P_Odown(s_PLAYER *p ,s_PLAYER *o);	//プレイヤーと他のオブジェクトの下部の当たり判定
int Hit_P_Oup(s_PLAYER *p ,s_PLAYER *o);	//プレイヤーと他のオブジェクトの上部の当たり判定


void CreateCoin(s_BLOCK *b);	//コインの生成
void CreateItem(s_BLOCK *b);	//アイテムの生成
void CreateEnemy(s_BLOCK *b);	//エネミーの生成

void MarioAnime(void);	//マリオのアニメーション

void DrawStage(void);		//ステージの描画
void DrawPlayer(void);	//プレイヤーの描画
void DrawEnemy(void);		//エネミーの描画
void DrawCoin(void);		//コインの描画
void DrawAnimeItem(void);	//アニメーションのアイテム描画
void DrawItem(void);		//アイテムの描画

int LoadImages(); //画像読込み
/***********************************************
 * プログラムの開始
 ***********************************************/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
							LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText( "スーパーマリオブラザーズ" ); // タイトルを設定

	ChangeWindowMode(TRUE); // ウィンドウモードで起動
	//SetGraphMode( 800 , 600 , 16 ) ;
	//SetDrawArea( 0 , 0 , 320 , 480 ) ;

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
		SetBackgroundColor( 95, 151, 255 ) ;

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

	BlockInit();		//ブロックの初期化
	PlayerInit();	//プレイヤーの初期化
	EnemyInit();		//エネミーの初期化
	CoinInit();
	ItemInit();
}

/***********************************************
 * ブロックの初期化
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
			g_Block[i][j].image = strtol(p, &end, 10);         // 10進数を読み取る
			p = end + 1;    // ',' をスキップする
		}
	}
	fclose(fp);
	
	

	// ブロックの初期化
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
 * ブロックのタイプ設定 (0:無 1:壊せる 2:コイン入り 3:アイテム入り 4:スター 5:1pキノコ)
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
 * プレイヤーの初期化
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
 * エネミーの初期化
 ***********************************************/
void EnemyInit(void)
{
	for(int i = 0; i < ENEMYMAX; i++)
	{
		g_Enemy[i].flg = FALSE;
	}
}
/***********************************************
 * コインの初期化
 ***********************************************/
void CoinInit(void)
{
	for(int i = 0; i < COINMAX; i++)
	{
		g_Coin[i].flg = FALSE;
	}
}

/***********************************************
 * アイテムの初期化
 ***********************************************/
void ItemInit(void)
{
	for(int i = 0; i < ItemMAX; i++)
	{
		g_Item[i].flg = FALSE;
	}
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
	
	//スクロールさせる処理
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
 * ジャンプ処理
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
 * 重力処理
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
 * 体の当たり判定
 ***********************************************/
void HitBox(void)
{
	int flg = 0;

	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].flg == TRUE && g_Block[h][w].image != -1){
				flg = 0;

				//当たり判定が必要のないimageの判定　flg 0:なし 1:ある
				for(int i = 0; i < 23; i++){
					if(g_NoHitImage[i] == g_Block[h][w].image){
						flg = 1;
						break;
					}
				}
				
				//判定の中でflgが0なら判定する
				if(flg == 0){
					//ブロックの上部（ピンク色）
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

					//ブロックの下部（ピンク色）
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
				
					//ブロックの右部(青）
					if(BlockHitRight(&g_Player, &g_Block[h][w]) == TRUE)
					{
						if(g_Block[h][w].image != 10 && g_Block[h][w].image != -1){
							g_Player.x = g_Block[h][w].x + size - g_BlockMx - MARIOHITXH;
						}
						else g_Block[h][w].image = -1;
					}
				
					//ブロックの左部(青）
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
 * ブロックの上下左右の当たり判定
 ***********************************************/
//ブロックの上
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

//ブロックの下
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

//ブロックの左
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

//ブロックの右
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
 * pプレイヤーとo他のオブジェクトの当たり判定
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
 * コインの生成
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
 * コインの移動処理
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
 * アイテムの生成
 ***********************************************/
void CreateItem(s_BLOCK *b){
	for(int i = 0; i < ItemMAX; i++){
		if(g_Item[ i ].flg == FALSE ){
			g_Item[i].flg = TRUE;
			g_Item[i].animeflg = TRUE;
			g_Item[i].muki = e_RIGHT;
			//12:キノコ 13:1Pキノコ 24:フラワー 25:スター
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
 * アイテムの移動処理
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
 * ステージの描画処理
 ***********************************************/
void DrawStage(void)
{
	int flg = 0;

	
	for(int h = 0; h < HLINE; h++){
		for(int w = 0; w < WLINE; w++){
			if(g_Block[h][w].flg == TRUE && g_Block[h][w].image != -1){

				//ブロックの上下移動処理　//上移動
				if(g_Block[h][w].animeflg == 1){
					g_Block[h][w].my--;
					if(g_Block[h][w].my == -5) g_Block[h][w].animeflg = 2;
				}
				//下移動
				else if(g_Block[h][w].animeflg == 2){
					g_Block[h][w].my++;
					if(g_Block[h][w].my == 0) {
						g_Block[h][w].animeflg = 0;
						g_Block[h][w].my = 0;
						g_Block[h][w].Item--;
						//ブロックがコインまたはアイテムがあり、アイテムの数が０であったら
						if(g_Block[h][w].type >= 2 && g_Block[h][w].Item == 0){
							g_Block[h][w].image = 0;
							g_Block[h][w].type = e_typefalse;
						}
					}
				}

				//タイマーコインの処理
				if(g_Block[h][w].type == e_timecoin && g_Block[h][w].Item != 50)
				{
					g_Block[h][w].count--;
					if(g_Block[h][w].count <= 0) g_Block[h][w].Item = 1;
				}

				//当たり判定が必要のないimageの判定　flg 0:なし 1:ある
				for(int i = 0; i < 23; i++){
					if(g_NoHitImage[i] == g_Block[h][w].image){
						flg = 1;
						break;
					}
				}

				DrawGraph(g_Block[h][w].x - g_BlockMx ,g_Block[h][w].y + g_Block[h][w].my,g_BlockImage[g_Block[h][w].image],TRUE);
				if(flg == 0){
				DrawBox(g_Block[h][w].x + BlockHitXH - g_BlockMx ,g_Block[h][w].y,   g_Block[h][w].x+size- BlockHitXH - g_BlockMx, g_Block[h][w].y + size- BlockHitYH,0xff00ff,TRUE); //上当たり判定描画
				DrawBox(g_Block[h][w].x + BlockHitXH - g_BlockMx ,g_Block[h][w].y + BlockHitYH,g_Block[h][w].x+size- BlockHitXH - g_BlockMx, g_Block[h][w].y+size,   0xff00ff,TRUE); //下当たり判定描画
				DrawBox(g_Block[h][w].x - g_BlockMx, g_Block[h][w].y + BlockHitYW, g_Block[h][w].x+size - BlockHitXW - g_BlockMx, g_Block[h][w].y+size - BlockHitYW, 0x00FFFF,TRUE);//右
				DrawBox(g_Block[h][w].x + BlockHitXW - g_BlockMx, g_Block[h][w].y + BlockHitYW, g_Block[h][w].x + size - g_BlockMx, g_Block[h][w].y+size - BlockHitYW, 0x00FFFF,TRUE);//左
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
 * プレイヤーの描画
 ***********************************************/
void DrawPlayer(void)
{
		//右移動
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

		//左移動
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
 * エネミーの描画
 ***********************************************/
void DrawEnemy(void)
{
	
}

/***********************************************
 * コインの描画処理
 ***********************************************/
void DrawCoin(void){
	for(int i = 0; i < COINMAX; i++){
		if(g_Coin[i].flg == TRUE){
			DrawGraph(g_Coin[i].x - g_BlockMx,g_Coin[i].y,g_BlockImage[26+(g_Coin[i].animecount/8%4)],TRUE);
		}
	}
}

/***********************************************
 * アイテムの描画処理
 ***********************************************/
void DrawItem(void){
	for(int i = 0; i < ItemMAX; i++){
		if(g_Item[i].flg == TRUE && g_Item[i].animeflg == 2){

			DrawGraph(g_Item[i].x - g_Item[i].Scroll,g_Item[i].y,g_BlockImage[g_Item[i].img],TRUE);

		}
	}
}

/***********************************************
 * アイテムの描画処理
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
 * 画像読込み
 ***********************************************/
int LoadImages()
{
	//プレイヤー
	//if((g_PlayerImage[0] = LoadGraph("images/player.png")) == -1) return -1;
	if(LoadDivGraph("images/mario_chara.png",15,5,3,32,32,g_PlayerImage[0]) == -1) return -1;
	if(LoadDivGraph("images/super_mario_chara.png",15,5,3,32,64,g_PlayerImage[1]) == -1) return -1;
	if(LoadDivGraph("images/fire_mario_chara .png",15,5,3,32,64,g_PlayerImage[2]) == -1) return -1;
	if(LoadDivGraph("images/backimage.png",84,12,7,32,32,g_BlockImage) == -1) return -1;
	
	return 0;
} 

