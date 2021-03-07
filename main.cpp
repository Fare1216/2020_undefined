#include "DxLib.h"
#include "mouse.h"
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MOVE_BUTTON_WIDTH 128
#define MOVE_BUTTON_HEIGHT 720
#define TITLE_BUTTON_WIDTH 200
#define TITLE_BUTTON_HEIGHT 70
#define MENU_BUTTON_WIDTH 300
#define MENU_BUTTON_HEIGHT 120
#define ROOM_WIDTH 710
#define ROOM_HEIGHT 400

#define PI 3.1415926f

/*===== 列挙体 =====*/
/*----- メイン用 -----*/
typedef enum {
	TitleLeady,
	Title,
	MenuLeady,
	Menu,
	GameLeady,
	GameBgm,
	Game,
	CutinLeady,
	Cutin,
	EndLeady,
	End,
}eScene;
/*----- ゲーム用 -----*/
typedef enum {
	Wall1,
	Wall2,
	Wall3,
	Wall4,
}eGScene;

/*===== 関数 =====*/
void fade(int fade_mode);
void Graph();
void Sound();
void TextBox(const char *name, const char *sentence1, const char* sentence2, const char* sentence3, const char* sentence4, int size);

/*===== 変数 =====*/
/*----- マウス関連 -----*/
int mx; //マウスx座標取得
int my; //マウスy座標取得
int Mouse;
int cflag = 0; //クリックフラグ

/*----- シーン変換用 -----*/
int Scene = TitleLeady;
int GScene = Wall1;

/*----- 色 -----*/
int White = GetColor( 255, 255, 255);
int Black = GetColor( 0, 0, 0);
int Red = GetColor( 255, 0, 0);
int Green = GetColor( 0, 255, 0);
int Blue = GetColor( 0, 0, 255);

/*----- 画像用 -----*/
int pic_head;
int pic_wall[10];
int pic_all[20];

/*----- 音声用 -----*/
int se;
int bgm;
int clear;

/*----- セーブデータ用 -----*/
FILE* rsave;
FILE* wsave;


/*===== メイン =====*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetBackgroundColor( 255, 255, 255);
	SetDXArchiveKeyString("Fumika_Sagisawa");
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	/*----- 動作処理用変数 -----*/
	int eflag = 0; //エンドフラグ
	unsigned int event = 0b100000000000000000; //イベントフラグ
	int esflag = 0; //イベントシーンフラグ
	int sflag = 0; //シーンフラグ
	int title_alpha = 0;
	int fadeflag = 0;
	int menu_alpha = 0;

	/*----- フォントハンドル -----*/
	int Size16 = CreateFontToHandle("游明朝", 16, 2, DX_FONTTYPE_ANTIALIASING);
	int Size48 = CreateFontToHandle("游明朝", 48, 3, DX_FONTTYPE_ANTIALIASING);
	int Size64 = CreateFontToHandle("游明朝", 64, 3, DX_FONTTYPE_ANTIALIASING);
	int Size96 = CreateFontToHandle("游明朝", 96, 4, DX_FONTTYPE_ANTIALIASING);
	int Size128 = CreateFontToHandle("游明朝", 128, 5, DX_FONTTYPE_ANTIALIASING);
	int text = CreateFontToHandle("游ゴシック", 30, 2, DX_FONTTYPE_ANTIALIASING);

	/*----- 画像読み込み -----*/
	Graph();
	Sound();


	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !eflag) {
		/*----- マウス座標取得 -----*/
		Mouse = GetMouseInput();
		GetMousePoint(&mx, &my);


		/*----- シーン遷移 -----*/
		switch (Scene) {
		case TitleLeady: {
			StopSoundMem(bgm);
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);

			errno_t error = fopen_s(&rsave, "save.dat", "rb");
			if (!(error != 0)) {
				fread(&event, sizeof(unsigned int), 1, rsave);
				fclose(rsave);
			}

			title_alpha = 0;
			fadeflag = 0;
			Scene = Title;
			break;
		}

		case Title: {
			/*----- 背景色変化 -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((PI / 2 / 90 * title_alpha) + 1) * 32);
			DrawBox( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			title_alpha = (title_alpha + 1) % 360;

			/*----- タイトル文字表示 -----*/
			DrawStringToHandle(225, 200, "undefined", Black, Size128);

			/*----- ボタン表示 -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox( 960, 500, 960 + TITLE_BUTTON_WIDTH, 500 + TITLE_BUTTON_HEIGHT, Black, TRUE);
			DrawBox( 960, 600, 960 + TITLE_BUTTON_WIDTH, 600 + TITLE_BUTTON_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawStringToHandle( 980, 515, "Start", White, Size48);
			DrawStringToHandle( 980, 615, "End", White, Size48);

			/*----- マウスクリック -----*/
			if (event) {
				//イベントのフラグが立っていたら続きから
				MouseXY(mx, my, 960, 500, 960 + TITLE_BUTTON_WIDTH, 500 + TITLE_BUTTON_HEIGHT, &Scene, GameBgm, Mouse, &cflag); //スタートボタンクリック
			}
			else {
				//イベントのフラグが立っていなかったら最初から
				MouseXY(mx, my, 960, 500, 960 + TITLE_BUTTON_WIDTH, 500 + TITLE_BUTTON_HEIGHT, &Scene, GameLeady, Mouse, &cflag); //スタートボタンクリック
			}
			MouseXY(mx, my, 960, 600, 960+TITLE_BUTTON_WIDTH, 600+TITLE_BUTTON_HEIGHT, &Scene, EndLeady, Mouse, &cflag); //エンドボタンクリック
			MouseEND(Mouse, &cflag);

			/*----- フェードイン -----*/
			if (fadeflag != 1) {
				fade(1);
				fadeflag = 1;
			}

			/*----- フェードアウト -----*/
			if (Scene != Title) {
				fade(0);
			}
			
			break;
		}

		case MenuLeady: {
			Scene = Menu;
			break;
		}

		case Menu: {
			/*----- 背景色変化 -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((PI / 2 / 90 * menu_alpha) + 1) * 16);
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			menu_alpha = (menu_alpha + 1) % 360;

			/*----- 文字表示 -----*/
			DrawStringToHandle(225, 200, "Save & exit?", Black, Size64);

			/*----- ボタン表示 -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox( 800, 400, 800 + MENU_BUTTON_WIDTH, 400 + MENU_BUTTON_HEIGHT, Black, TRUE);
			DrawBox( 800, 550, 800 + MENU_BUTTON_WIDTH, 550 + MENU_BUTTON_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawStringToHandle( 830, 415, "Yes", White, Size96);
			DrawStringToHandle( 830, 565, "No", White, Size96);

			/*----- マウスクリック -----*/
			MouseXY(mx, my, 800, 400, 800 + MENU_BUTTON_WIDTH, 400 + MENU_BUTTON_HEIGHT, &Scene, TitleLeady, Mouse, &cflag); //イエスボタンクリック
			MouseXY(mx, my, 800, 550, 800 + MENU_BUTTON_WIDTH, 550 + MENU_BUTTON_HEIGHT, &Scene, GameBgm, Mouse, &cflag); //ノーボタンクリック
			MouseEND(Mouse, &cflag);

			/*----- フェードアウト -----*/
			if (Scene != Menu && Scene == TitleLeady) {
				fade(0);
				errno_t error = fopen_s(&wsave, "save.dat", "wb+");
				if (error != 0) {
					Scene = EndLeady;
				}
				else {
					fwrite(&event, sizeof(unsigned int), 1, wsave);
					fclose(wsave);
				}
			}

			break;
		}

		case GameLeady: {
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			event = 0;
			GScene = Wall1;
			Scene = GameBgm;
			break;
		}

		case GameBgm: {
			PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, TRUE);
			ChangeVolumeSoundMem( 128, bgm);
			Scene = Game;
			break;
		}

		case Game: {
			/*----- Escでメニュー遷移 -----*/
			if (CheckHitKey(KEY_INPUT_ESCAPE)) {
				Scene = MenuLeady;
			}

			/*----- 背景描画 -----*/
			static int room_x = SCREEN_WIDTH / 2 - ROOM_WIDTH / 2;
			static int room_y = SCREEN_HEIGHT / 2 - ROOM_HEIGHT / 2;
			DrawBox( room_x, room_y, room_x + ROOM_WIDTH, room_y + ROOM_HEIGHT, Black, FALSE); //壁
			DrawLine( 0, 0, room_x, room_y, Black); //左上
			DrawLine( SCREEN_WIDTH - room_x, room_y, SCREEN_WIDTH, 0, Black); //右上
			DrawLine( 0, SCREEN_HEIGHT, room_x, SCREEN_HEIGHT - room_y, Black); //左下
			DrawLine( SCREEN_WIDTH - room_x, SCREEN_HEIGHT - room_y, SCREEN_WIDTH, SCREEN_HEIGHT, Black); //右下

			/*----- ゲームシーン遷移 -----*/
			switch (GScene) {
			case Wall1: {
				/*----- 看板の表示 -----*/
				if ((event&0b1000000) != 0b1000000) {
					DrawGraph(room_x, room_y, pic_wall[0], FALSE);
					if ((room_x + 205 <= mx && mx < room_x + 505) && (room_y + 115 <= my && my < room_y + 395)) {
						DrawBox(room_x + 205, room_y + 115, room_x + 505, room_y + 395, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 205, room_y + 115, room_x + 505, room_y + 395, &Scene, CutinLeady, Mouse, &cflag); //看板クリック
					
					/*----- 看板調査かつのこ有かつのこ刃有 -----*/
					if ((Scene == CutinLeady) && ((event & 0b1011) == 0b0011)) {
						sflag = 6; //のこ刃削る
					}
					/*----- 看板調査かつロケラン有 -----*/
					else if ((Scene == CutinLeady) && ((event & 0b100000) == 0b100000)) {
						sflag = 9; //ロケランぶっぱ
					}
					/*----- それ以外 -----*/
					else if (Scene == CutinLeady) {
						sflag = 0; //看板見る
					}
				}
				/*----- インク等の表示 -----*/
				else if ((event & 0b1000000) == 0b1000000) {
					DrawGraph(room_x, room_y, pic_wall[1], FALSE);
					if ((room_x + 160 <= mx && mx < room_x + 540) && (room_y + 355 <= my && my < room_y + 455)) {
						DrawBox(room_x + 160, room_y + 355, room_x + 540, room_y + 455, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 160, room_y + 355, room_x + 540, room_y + 455, &Scene, CutinLeady, Mouse, &cflag); //インク等クリック

					/*----- コード取得済み -----*/
					if ((Scene == CutinLeady) && ((event & 0b1101000000000) == 0b0101000000000)) {
						sflag = 15; //コード入力
					}
					/*----- それ以外 -----*/
					else if(Scene == CutinLeady) {
						sflag = 12;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall4, Mouse, &cflag); //左移動用ボタンクリック
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall2, Mouse, &cflag); //右移動用ボタンクリック
				MouseEND(Mouse, &cflag);
				break;
			}

			case Wall2: {
				/*----- のこぎりの表示 -----*/
				if ((event & 0b10) != 0b10) {
					DrawGraph(room_x, room_y, pic_wall[2], FALSE);
					if ((room_x + 65 <= mx && mx < room_x + 640) && (room_y + 100 <= my && my < room_y + 270)) {
						DrawBox(room_x + 65, room_y + 100, room_x + 640, room_y + 270, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 65, room_y + 100, room_x + 640, room_y + 270, &Scene, CutinLeady, Mouse, &cflag); //のこクリック

					/*----- のこ壁有 -----*/
					if (Scene == CutinLeady) {
						sflag = 1;
					}
				}
				/*----- のこぎりが置いてあるやつの表示 -----*/
				else if ((event & 0b10) == 0b10) {
					DrawGraph(room_x, room_y, pic_wall[3], FALSE);
					if ((room_x + 120 <= mx && mx < room_x + 590) && (room_y + 200 <= my && my < room_y + 265)) {
						DrawBox(room_x + 120, room_y + 200, room_x + 590, room_y + 265, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 120, room_y + 200, room_x + 590, room_y + 265, &Scene, CutinLeady, Mouse, &cflag); //くいクリック

					/*----- のこ壁無 -----*/
					if (Scene == CutinLeady) {
						sflag = 2;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall1, Mouse, &cflag); //左移動用ボタンクリック
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall3, Mouse, &cflag); //右移動用ボタンクリック
				MouseEND(Mouse, &cflag);
				break;
			}

			case Wall3: {
				/*----- 隙間の表示 -----*/
				if ((event & 0b10000) != 0b10000) {
					DrawGraph(room_x, room_y, pic_wall[4], FALSE);
					if ((room_x + 35 <= mx && mx < room_x + 675) && (room_y + 35 <= my && my < room_y + 365)) {
						DrawBox(room_x + 35, room_y + 35, room_x + 675, room_y + 365, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 35, room_y + 35, room_x + 675, room_y + 365, &Scene, CutinLeady, Mouse, &cflag); //すきまクリック

					/*----- のこ刃有かつ隙間調査有 -----*/
					if ((Scene == CutinLeady) && ((event & 0b1110) == 0b0110)) {
						sflag = 5;
					}
					/*----- のこ刃無かつ隙間調査有 -----*/
					else if ((Scene == CutinLeady) && ((event & 0b1100) == 0b1100)) {
						sflag = 7;
					}
					/*----- それ以外 -----*/
					else if (Scene == CutinLeady) {
						sflag = 3;
					}
				}
				/*----- ロケランの表示 -----*/
				else if ((event & 0b110000) == 0b010000) {
					DrawGraph(room_x, room_y, pic_wall[5], FALSE);
					if ((room_x + 100 <= mx && mx < room_x + 590) && (room_y + 170 <= my && my < room_y + 290)) {
						DrawBox(room_x + 100, room_y + 170, room_x + 590, room_y + 290, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 100, room_y + 170, room_x + 590, room_y + 290, &Scene, CutinLeady, Mouse, &cflag); //ロケランクリック

					/*----- ロケラン有 -----*/
					if (Scene == CutinLeady) {
						sflag = 8;
					}
				}
				/*----- ボタンの表示 -----*/
				else if ((event & 0b100000) == 0b100000) {
					DrawGraph(room_x, room_y, pic_wall[6], FALSE);
					if ((room_x + 275 <= mx && mx < room_x + 405) && (room_y + 245 <= my && my < room_y + 345)) {
						DrawBox(room_x + 275, room_y + 245, room_x + 405, room_y + 345, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 275, room_y + 245, room_x + 405, room_y + 345, &Scene, CutinLeady, Mouse, &cflag); //ボタンクリック

					/*----- コード入力済 -----*/
					if ((Scene == CutinLeady) && ((event & 0b1000000000000) == 0b1000000000000)) {
						sflag = 16;
					}
					/*----- それ以外 -----*/
					else if (Scene == CutinLeady) {
						sflag = 10;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall2, Mouse, &cflag); //左移動用ボタンクリック
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall4, Mouse, &cflag); //右移動用ボタンクリック
				MouseEND(Mouse, &cflag);
				break;
			}

			case Wall4: {
				/*----- 階段の表示 -----*/
				if ((event & 0b100000) != 0b100000) {
					DrawGraph(room_x, room_y, pic_wall[7], FALSE);
					if ((room_x + 160 <= mx && mx < room_x + 550) && (room_y + 40 <= my && my < room_y + 405)) {
						DrawBox(room_x + 160, room_y + 40, room_x + 550, room_y + 410, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 160, room_y + 40, room_x + 550, room_y + 410, &Scene, CutinLeady, Mouse, &cflag); //階段クリック

					/*----- クリック -----*/
					if (Scene == CutinLeady) {
						sflag = 4;
					}
				}
				/*----- シャッターの表示 -----*/
				else if ((event & 0b10000000000) != 0b10000000000) {
					DrawGraph(room_x, room_y, pic_wall[8], FALSE);
					if ((room_x + 140 <= mx && mx < room_x + 570) && (room_y + 40 <= my && my < room_y + 405)) {
						DrawBox(room_x + 140, room_y + 40, room_x + 570, room_y + 410, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 140, room_y + 40, room_x + 570, room_y + 410, &Scene, CutinLeady, Mouse, &cflag); //シャッタークリック

					/*----- シャッター調査済かつインク所持 -----*/
					if ((Scene == CutinLeady) && ((event & 0b1100000000) == 0b1100000000)) {
						sflag = 13;
					}
					/*----- それ以外 -----*/
					else if (Scene == CutinLeady) {
						sflag = 11;
					}
				}
				/*----- シャッター(インク)の表示 -----*/
				else if ((event & 0b10000000000) == 0b10000000000) {
					DrawGraph(room_x, room_y, pic_wall[9], FALSE);
					if ((room_x + 140 <= mx && mx < room_x + 570) && (room_y + 40 <= my && my < room_y + 405)) {
						DrawBox(room_x + 140, room_y + 40, room_x + 570, room_y + 410, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 140, room_y + 40, room_x + 570, room_y + 410, &Scene, CutinLeady, Mouse, &cflag); //シャッター(インク)クリック

					/*----- クリック -----*/
					if (Scene == CutinLeady) {
						sflag = 14;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall3, Mouse, &cflag); //左移動用ボタンクリック
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall1, Mouse, &cflag); //右移動用ボタンクリック
				MouseEND(Mouse, &cflag);
				break;
			}
			}

			/*----- 幽霊後ろ姿描画 -----*/
			DrawGraph(0, 0, pic_head, TRUE);

			/*----- 移動用ボタン描画 -----*/
			/*----- 左ボタン -----*/
			if ((0 <= mx && mx < MOVE_BUTTON_WIDTH) && (0 <= my && my < MOVE_BUTTON_HEIGHT)) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
				DrawBox(0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, Black, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawTriangle((MOVE_BUTTON_WIDTH / 2) - 40, (MOVE_BUTTON_HEIGHT / 2), (MOVE_BUTTON_WIDTH / 2) + 40, (MOVE_BUTTON_HEIGHT / 2) - 80, (MOVE_BUTTON_WIDTH / 2) + 40, (MOVE_BUTTON_HEIGHT / 2) + 80, White, TRUE);
			}
			/*----- 右ボタン -----*/
			if ((SCREEN_WIDTH - MOVE_BUTTON_WIDTH <= mx && mx < SCREEN_WIDTH) && (0 <= my && my < MOVE_BUTTON_HEIGHT)) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
				DrawBox(SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, Black, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawTriangle(SCREEN_WIDTH - (MOVE_BUTTON_WIDTH / 2) + 40, (MOVE_BUTTON_HEIGHT / 2), SCREEN_WIDTH - (MOVE_BUTTON_WIDTH / 2) - 40, (MOVE_BUTTON_HEIGHT / 2) - 80, SCREEN_WIDTH - (MOVE_BUTTON_WIDTH / 2) - 40, (MOVE_BUTTON_HEIGHT / 2) + 80, White, TRUE);
			}

			break;
		}

		case CutinLeady: {
			esflag = 0;
			Scene = Cutin;
			break;
		}

		case Cutin: {
			

			switch (sflag) {

			case 0: {
				DrawGraph(0, 0, pic_all[0], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "なにこの「看板」？", "何か書いてあるよ？", "", "",  text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "『白い部屋だよ！出口はないよ！』って書いてあるみたいだね。", "どうやら私たちは閉じ込められたようだよ。", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "なんてこったい！", "何とかして脱出してやる！", "", "", text);
					break;
				}
				case 3: {
					event |= 0b1;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 1: {
				DrawGraph(0, 0, pic_all[1], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "ねえねえ、壁にかかってるこれはなあに？", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "これは「のこぎり」っていうんだよ。", "いろいろなものを切ることができるんだ。", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "はえー、こんなに薄いのにすっごい！", "これでさっきの「看板」も切られるかな？", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "それは...やってみないと分からないね。", "あとで試しに行こうね。", "", "", text);
					break;
				}
				case 4: {
					event |= 0b10;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 2: {
				DrawGraph(0, 0, pic_all[2], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("ユウコ", "ここには...ほかに何もないね。", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("レイコ", "ないねー。", "", "", "", text);
					break;
				}
				case 2: {
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 3: {
				DrawGraph(0, 0, pic_all[3], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "わぁ、ここの壁ちょっと「すきま」があるよ！", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "本当だ、よく見つけたね。", "えらいぞー。", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "エヘヘ。", "あっ、ここに何か差し込めないかな？", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "薄いものなら差し込めるかもしれないね。", "あとで探してみようね。", "", "", text);
					break;
				}
				case 4: {
					event |= 0b100;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 4: {
				DrawGraph(0, 0, pic_all[4], TRUE);

				switch (esflag) {
				case 0: {
					TextBox("レイコ", "うわぁ...なんか体がふわふわする～。", "あはははは。", "", "", text);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "昇天しかけてるー！", "行っちゃダメ―！", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "はっ、わたしは今何を！？", "", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "ここは危険だわ...。", "近づかないようにしましょう。", "", "", text);
					break;
				}
				case 4: {
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 5: {
				DrawGraph(0, 0, pic_all[5], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "うーんうーん。", "「のこぎり」がしっかり刺さらないよぉ。", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "そうか！", "縦引きの「のこぎり」だから刃が引っかかってるんだわ！", "どうにかして刃を削らないといけないわね...。", "近くにそんなものあったかしら。", text);
					break;
				}
				case 2: {
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 6: {
				DrawGraph(0, 0, pic_all[6], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "よーし、「看板」を切るぞー！", "おりゃりゃりゃりゃりゃー！", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "あっ！", "「のこぎり」の刃がどんどん削れてる！", "というか、どれだけ固いのよこの「看板」！", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "おりゃりゃりゃりゃりゃー！", "", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "もうやめて―！", "「のこぎり」がなくなっちゃうー！", "", "", text);
					break;
				}
				case 4: {
					event |= 0b1000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 7: {
				DrawGraph(0, 0, pic_all[7], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "あっ、「のこぎり」刺さった！", "これを...ふんっ！", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "壁が外れた！", "お手柄だよ！", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "奥にも何かあるみたいだね。", "覗いてみよう。", "", "", text);
					break;
				}
				case 3: {
					event |= 0b10000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 8: {
				DrawGraph(0, 0, pic_all[8], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("ユウコ", "これは...「ロケットランチャー」ってやつかな？", "なんでこんなところに？", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("何かの音", "ガシャーン！！！！！！！！！！！！", "", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "えっ、なになに！？", "すごく大きい音！", "びっくり！", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "下の「スイッチ」が押されなくなったから、何かが動いたのね。", "ちょっとまわりを見てみましょうか。", "", "", text);
					break;
				}
				case 4: {
					event |= 0b100000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 9: {
				DrawGraph(0, 0, pic_all[9], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("ユウコ", "「のこぎり」で切れなくても、これなら！", "ファイア！！！！！！！！！！！", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("レイコ", "おー！！！！！！！", "いっつそーくーる！！！！！！！", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("ユウコ", "これで跡形もなく消しt...ん？", "待って！", "何かが残ってる！", "", text);
					break;
				}
				case 3: {
					event |= 0b1000000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 10: {
				DrawGraph(0, 0, pic_all[10], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "ねーねー、この「スイッチ」ってやつ押していい？", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "ダーメ。", "さっき軽く押してみたけど、びくともしなかったわ。", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "ちぇー。", "つまんないのー。", "", "", text);
					break;
				}
				case 3: {
					event |= 0b10000000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 11: {
				DrawGraph(0, 0, pic_all[11], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "さっきふわふわしたところ、何かでふさがれてるよ！", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "これは「シャッター」ってやつね。", "「スイッチ」が押されなくなったからしまったんだわ。", "にしてもすごい丈夫ね、これ。", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "たたいてもびくともしなーい！", "おもしろーい！", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "ちょっと、たたきすぎ！", "うるさい！", "", "", text);
					break;
				}
				case 4: {
					event |= 0b100000000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 12: {
				switch (esflag) {
				case 0: {
					DrawGraph(0, 0, pic_all[12], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "なんだろうこれ？", "何かが入力できそう！", "", "", text);
					break;
				}
				case 1: {
					DrawGraph(0, 0, pic_all[12], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "たぶん「入力板」ってものじゃないかな。", "あっ、こっちにも何かあるよ？", "", "", text);
					break;
				}
				case 2: {
					DrawGraph(0, 0, pic_all[13], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "これは...「インク」だー！", "...なんでだろう？", "", "", text);
					break;
				}
					  DrawGraph(0, 0, pic_all[13], TRUE);
				case 3: {
					DrawGraph(0, 0, pic_all[13], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "使用用途が全く分からない...。", "謎だ...。", "", "", text);
					break;
				}
				case 4: {
					event |= 0b1000000000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 13: {
				DrawGraph(0, 0, pic_all[14], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "へぶっっっっっっ！！！！！！", "いてててて...。", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "大丈夫！？", "あ...「インク」が「シャッター」にかかっちゃったわね。", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "あれ？", "ちょっと待って？", "何かが浮かび上がってくる！", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("二人", "こっ...これは！！！！！！！！！！！！", "", "", "", text);
					break;
				}
				case 4: {
					event |= 0b10000000000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 14: {
				DrawGraph(0, 0, pic_all[15], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "「入力板」に打ち込む用の「コード」だ！", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "こんなところに...。", "だから「インク」があったのね！", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "これを「入力板」に打ち込んだら脱出できるかも！", "早くやってみよう！", "", "", text);
					break;
				}
				case 3: {
					event |= 0b100000000000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 15: {
				DrawGraph(0, 0, pic_all[16], TRUE);

				switch (esflag) {
				case 0: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "...これでよしっと。", "開けゴマ！", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "...何も起きないね。", "何かほかにトリガーになるものがいるのかな？", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("レイコ", "あっ！さっきの「スイッチ」！", "今ので押せるようになったんじゃない？", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("ユウコ", "物は試しね。", "押してみましょうか。", "", "", text);
					break;
				}
				case 4: {
					event |= 0b1000000000000;
					Scene = Game;
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			case 16: {
				switch (esflag) {
				case 0: {
					DrawGraph(0, 0, pic_all[17], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					TextBox("レイコ", "ぽちっとな。", "", "", "", text);
					break;
				}
				case 1: {
					DrawGraph(0, 0, pic_all[17], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("ユウコ", "なにもおこらn...", "", "", "", text);
					break;
				}
				case 2: {
					DrawGraph(0, 0, pic_all[18], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("！？！？！？！？", "ユウコ", "", "", "", text);
					break;
				}
				case 3: {
					StopSoundMem(bgm);
					PlaySoundMem(clear, DX_PLAYTYPE_BACK, TRUE);
					DrawGraph(0, 0, pic_all[19], TRUE);
					esflag = 4;
					break;
				}
				case 4: {
					DrawGraph(0, 0, pic_all[19], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 5, Mouse, &cflag);
					break;
				}
				case 5: {
					DrawGraph(0, 0, pic_all[19], TRUE);
					Scene = EndLeady;
					/*----- フェードアウト -----*/
					if (Scene != Cutin) {
						fade(0);
						int reset = 0b100000000000000000;
						fopen_s(&wsave, "save.dat", "wb+");
						fwrite(&reset, sizeof(unsigned int), 1, wsave);
						fclose(wsave);
					}
					break;
				}
				}
				MouseEND(Mouse, &cflag);
				break;
			}

			}

			break;
		}

		case EndLeady: {
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			Scene = End;
			break;
		}

		case End: {
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			eflag = 1;
			break;
		}
		}
	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;

}


void fade(int fade_mode) {
	/*----- フェードアウト -----*/
	if (fade_mode == 0) {
		PlaySoundMem(se, DX_PLAYTYPE_BACK, TRUE);
		for (int i = 0; i < 256/3; i++) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 12);
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			ScreenFlip();
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		StopSoundMem(se);
	}
	/*----- フェードイン -----*/
	if (fade_mode == 1) {
		for (int j = 0; j < 255/4; j++) {
			ClearDrawScreen();
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-(j*4));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			ScreenFlip();
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Graph() {
	pic_head = LoadGraph("./Graph/head.png");

	pic_wall[0] = LoadGraph("./Graph/wall1-1.png");
	pic_wall[1] = LoadGraph("./Graph/wall1-2.png");
	pic_wall[2] = LoadGraph("./Graph/wall2-1.png");
	pic_wall[3] = LoadGraph("./Graph/wall2-2.png");
	pic_wall[4] = LoadGraph("./Graph/wall3-1.png");
	pic_wall[5] = LoadGraph("./Graph/wall3-2.png");
	pic_wall[6] = LoadGraph("./Graph/wall3-3.png");
	pic_wall[7] = LoadGraph("./Graph/wall4-1.png");
	pic_wall[8] = LoadGraph("./Graph/wall4-2.png");
	pic_wall[9] = LoadGraph("./Graph/wall4-3.png");

	pic_all[0] = LoadGraph("./Graph/all0.png");
	pic_all[1] = LoadGraph("./Graph/all1.png");
	pic_all[2] = LoadGraph("./Graph/all2.png");
	pic_all[3] = LoadGraph("./Graph/all3.png");
	pic_all[4] = LoadGraph("./Graph/all4.png");
	pic_all[5] = LoadGraph("./Graph/all5.png");
	pic_all[6] = LoadGraph("./Graph/all6.png");
	pic_all[7] = LoadGraph("./Graph/all7.png");
	pic_all[8] = LoadGraph("./Graph/all8.png");
	pic_all[9] = LoadGraph("./Graph/all9.png");
	pic_all[10] = LoadGraph("./Graph/all10.png");
	pic_all[11] = LoadGraph("./Graph/all11.png");
	pic_all[12] = LoadGraph("./Graph/all12-1.png");
	pic_all[13] = LoadGraph("./Graph/all12-2.png");
	pic_all[14] = LoadGraph("./Graph/all13.png");
	pic_all[15] = LoadGraph("./Graph/all14.png");
	pic_all[16] = LoadGraph("./Graph/all15.png");
	pic_all[17] = LoadGraph("./Graph/all16-1.png");
	pic_all[18] = LoadGraph("./Graph/all16-2.png");
	pic_all[19] = LoadGraph("./Graph/all16-3.png");
}

void Sound() {
	bgm = LoadSoundMem("./Sound/bgm.mp3");
	se = LoadSoundMem("./Sound/se1.wav");
	clear = LoadSoundMem("./Sound/clear.mp3");
}

void TextBox(const char* name, const char* sentence1, const char* sentence2, const char* sentence3, const char* sentence4, int size) {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(128 * 0.9));
	DrawBox(5, 475, 405, 525, Black, TRUE);
	DrawBox(10, 480, 400, 525, White, TRUE);
	DrawBox(5, 525, 1275, 715, Black, TRUE);
	DrawBox(10, 530, 1270, 710, White, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(15, 490, name, Black, size);
	DrawStringToHandle(15, 540, sentence1, Black, size);
	DrawStringToHandle(15, 585, sentence2, Black, size);
	DrawStringToHandle(15, 630, sentence3, Black, size);
	DrawStringToHandle(15, 675, sentence4, Black, size);
}