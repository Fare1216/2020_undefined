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

/*===== �񋓑� =====*/
/*----- ���C���p -----*/
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
/*----- �Q�[���p -----*/
typedef enum {
	Wall1,
	Wall2,
	Wall3,
	Wall4,
}eGScene;

/*===== �֐� =====*/
void fade(int fade_mode);
void Graph();
void Sound();
void TextBox(const char *name, const char *sentence1, const char* sentence2, const char* sentence3, const char* sentence4, int size);

/*===== �ϐ� =====*/
/*----- �}�E�X�֘A -----*/
int mx; //�}�E�Xx���W�擾
int my; //�}�E�Xy���W�擾
int Mouse;
int cflag = 0; //�N���b�N�t���O

/*----- �V�[���ϊ��p -----*/
int Scene = TitleLeady;
int GScene = Wall1;

/*----- �F -----*/
int White = GetColor( 255, 255, 255);
int Black = GetColor( 0, 0, 0);
int Red = GetColor( 255, 0, 0);
int Green = GetColor( 0, 255, 0);
int Blue = GetColor( 0, 0, 255);

/*----- �摜�p -----*/
int pic_head;
int pic_wall[10];
int pic_all[20];

/*----- �����p -----*/
int se;
int bgm;
int clear;

/*----- �Z�[�u�f�[�^�p -----*/
FILE* rsave;
FILE* wsave;


/*===== ���C�� =====*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetBackgroundColor( 255, 255, 255);
	SetDXArchiveKeyString("Fumika_Sagisawa");
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	/*----- ���쏈���p�ϐ� -----*/
	int eflag = 0; //�G���h�t���O
	unsigned int event = 0b100000000000000000; //�C�x���g�t���O
	int esflag = 0; //�C�x���g�V�[���t���O
	int sflag = 0; //�V�[���t���O
	int title_alpha = 0;
	int fadeflag = 0;
	int menu_alpha = 0;

	/*----- �t�H���g�n���h�� -----*/
	int Size16 = CreateFontToHandle("������", 16, 2, DX_FONTTYPE_ANTIALIASING);
	int Size48 = CreateFontToHandle("������", 48, 3, DX_FONTTYPE_ANTIALIASING);
	int Size64 = CreateFontToHandle("������", 64, 3, DX_FONTTYPE_ANTIALIASING);
	int Size96 = CreateFontToHandle("������", 96, 4, DX_FONTTYPE_ANTIALIASING);
	int Size128 = CreateFontToHandle("������", 128, 5, DX_FONTTYPE_ANTIALIASING);
	int text = CreateFontToHandle("���S�V�b�N", 30, 2, DX_FONTTYPE_ANTIALIASING);

	/*----- �摜�ǂݍ��� -----*/
	Graph();
	Sound();


	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !eflag) {
		/*----- �}�E�X���W�擾 -----*/
		Mouse = GetMouseInput();
		GetMousePoint(&mx, &my);


		/*----- �V�[���J�� -----*/
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
			/*----- �w�i�F�ω� -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((PI / 2 / 90 * title_alpha) + 1) * 32);
			DrawBox( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			title_alpha = (title_alpha + 1) % 360;

			/*----- �^�C�g�������\�� -----*/
			DrawStringToHandle(225, 200, "undefined", Black, Size128);

			/*----- �{�^���\�� -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox( 960, 500, 960 + TITLE_BUTTON_WIDTH, 500 + TITLE_BUTTON_HEIGHT, Black, TRUE);
			DrawBox( 960, 600, 960 + TITLE_BUTTON_WIDTH, 600 + TITLE_BUTTON_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawStringToHandle( 980, 515, "Start", White, Size48);
			DrawStringToHandle( 980, 615, "End", White, Size48);

			/*----- �}�E�X�N���b�N -----*/
			if (event) {
				//�C�x���g�̃t���O�������Ă����瑱������
				MouseXY(mx, my, 960, 500, 960 + TITLE_BUTTON_WIDTH, 500 + TITLE_BUTTON_HEIGHT, &Scene, GameBgm, Mouse, &cflag); //�X�^�[�g�{�^���N���b�N
			}
			else {
				//�C�x���g�̃t���O�������Ă��Ȃ�������ŏ�����
				MouseXY(mx, my, 960, 500, 960 + TITLE_BUTTON_WIDTH, 500 + TITLE_BUTTON_HEIGHT, &Scene, GameLeady, Mouse, &cflag); //�X�^�[�g�{�^���N���b�N
			}
			MouseXY(mx, my, 960, 600, 960+TITLE_BUTTON_WIDTH, 600+TITLE_BUTTON_HEIGHT, &Scene, EndLeady, Mouse, &cflag); //�G���h�{�^���N���b�N
			MouseEND(Mouse, &cflag);

			/*----- �t�F�[�h�C�� -----*/
			if (fadeflag != 1) {
				fade(1);
				fadeflag = 1;
			}

			/*----- �t�F�[�h�A�E�g -----*/
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
			/*----- �w�i�F�ω� -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((PI / 2 / 90 * menu_alpha) + 1) * 16);
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			menu_alpha = (menu_alpha + 1) % 360;

			/*----- �����\�� -----*/
			DrawStringToHandle(225, 200, "Save & exit?", Black, Size64);

			/*----- �{�^���\�� -----*/
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox( 800, 400, 800 + MENU_BUTTON_WIDTH, 400 + MENU_BUTTON_HEIGHT, Black, TRUE);
			DrawBox( 800, 550, 800 + MENU_BUTTON_WIDTH, 550 + MENU_BUTTON_HEIGHT, Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawStringToHandle( 830, 415, "Yes", White, Size96);
			DrawStringToHandle( 830, 565, "No", White, Size96);

			/*----- �}�E�X�N���b�N -----*/
			MouseXY(mx, my, 800, 400, 800 + MENU_BUTTON_WIDTH, 400 + MENU_BUTTON_HEIGHT, &Scene, TitleLeady, Mouse, &cflag); //�C�G�X�{�^���N���b�N
			MouseXY(mx, my, 800, 550, 800 + MENU_BUTTON_WIDTH, 550 + MENU_BUTTON_HEIGHT, &Scene, GameBgm, Mouse, &cflag); //�m�[�{�^���N���b�N
			MouseEND(Mouse, &cflag);

			/*----- �t�F�[�h�A�E�g -----*/
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
			/*----- Esc�Ń��j���[�J�� -----*/
			if (CheckHitKey(KEY_INPUT_ESCAPE)) {
				Scene = MenuLeady;
			}

			/*----- �w�i�`�� -----*/
			static int room_x = SCREEN_WIDTH / 2 - ROOM_WIDTH / 2;
			static int room_y = SCREEN_HEIGHT / 2 - ROOM_HEIGHT / 2;
			DrawBox( room_x, room_y, room_x + ROOM_WIDTH, room_y + ROOM_HEIGHT, Black, FALSE); //��
			DrawLine( 0, 0, room_x, room_y, Black); //����
			DrawLine( SCREEN_WIDTH - room_x, room_y, SCREEN_WIDTH, 0, Black); //�E��
			DrawLine( 0, SCREEN_HEIGHT, room_x, SCREEN_HEIGHT - room_y, Black); //����
			DrawLine( SCREEN_WIDTH - room_x, SCREEN_HEIGHT - room_y, SCREEN_WIDTH, SCREEN_HEIGHT, Black); //�E��

			/*----- �Q�[���V�[���J�� -----*/
			switch (GScene) {
			case Wall1: {
				/*----- �Ŕ̕\�� -----*/
				if ((event&0b1000000) != 0b1000000) {
					DrawGraph(room_x, room_y, pic_wall[0], FALSE);
					if ((room_x + 205 <= mx && mx < room_x + 505) && (room_y + 115 <= my && my < room_y + 395)) {
						DrawBox(room_x + 205, room_y + 115, room_x + 505, room_y + 395, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 205, room_y + 115, room_x + 505, room_y + 395, &Scene, CutinLeady, Mouse, &cflag); //�ŔN���b�N
					
					/*----- �Ŕ������̂��L���̂��n�L -----*/
					if ((Scene == CutinLeady) && ((event & 0b1011) == 0b0011)) {
						sflag = 6; //�̂��n���
					}
					/*----- �Ŕ��������P�����L -----*/
					else if ((Scene == CutinLeady) && ((event & 0b100000) == 0b100000)) {
						sflag = 9; //���P�����Ԃ���
					}
					/*----- ����ȊO -----*/
					else if (Scene == CutinLeady) {
						sflag = 0; //�Ŕ���
					}
				}
				/*----- �C���N���̕\�� -----*/
				else if ((event & 0b1000000) == 0b1000000) {
					DrawGraph(room_x, room_y, pic_wall[1], FALSE);
					if ((room_x + 160 <= mx && mx < room_x + 540) && (room_y + 355 <= my && my < room_y + 455)) {
						DrawBox(room_x + 160, room_y + 355, room_x + 540, room_y + 455, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 160, room_y + 355, room_x + 540, room_y + 455, &Scene, CutinLeady, Mouse, &cflag); //�C���N���N���b�N

					/*----- �R�[�h�擾�ς� -----*/
					if ((Scene == CutinLeady) && ((event & 0b1101000000000) == 0b0101000000000)) {
						sflag = 15; //�R�[�h����
					}
					/*----- ����ȊO -----*/
					else if(Scene == CutinLeady) {
						sflag = 12;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall4, Mouse, &cflag); //���ړ��p�{�^���N���b�N
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall2, Mouse, &cflag); //�E�ړ��p�{�^���N���b�N
				MouseEND(Mouse, &cflag);
				break;
			}

			case Wall2: {
				/*----- �̂�����̕\�� -----*/
				if ((event & 0b10) != 0b10) {
					DrawGraph(room_x, room_y, pic_wall[2], FALSE);
					if ((room_x + 65 <= mx && mx < room_x + 640) && (room_y + 100 <= my && my < room_y + 270)) {
						DrawBox(room_x + 65, room_y + 100, room_x + 640, room_y + 270, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 65, room_y + 100, room_x + 640, room_y + 270, &Scene, CutinLeady, Mouse, &cflag); //�̂��N���b�N

					/*----- �̂��ǗL -----*/
					if (Scene == CutinLeady) {
						sflag = 1;
					}
				}
				/*----- �̂����肪�u���Ă����̕\�� -----*/
				else if ((event & 0b10) == 0b10) {
					DrawGraph(room_x, room_y, pic_wall[3], FALSE);
					if ((room_x + 120 <= mx && mx < room_x + 590) && (room_y + 200 <= my && my < room_y + 265)) {
						DrawBox(room_x + 120, room_y + 200, room_x + 590, room_y + 265, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 120, room_y + 200, room_x + 590, room_y + 265, &Scene, CutinLeady, Mouse, &cflag); //�����N���b�N

					/*----- �̂��ǖ� -----*/
					if (Scene == CutinLeady) {
						sflag = 2;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall1, Mouse, &cflag); //���ړ��p�{�^���N���b�N
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall3, Mouse, &cflag); //�E�ړ��p�{�^���N���b�N
				MouseEND(Mouse, &cflag);
				break;
			}

			case Wall3: {
				/*----- ���Ԃ̕\�� -----*/
				if ((event & 0b10000) != 0b10000) {
					DrawGraph(room_x, room_y, pic_wall[4], FALSE);
					if ((room_x + 35 <= mx && mx < room_x + 675) && (room_y + 35 <= my && my < room_y + 365)) {
						DrawBox(room_x + 35, room_y + 35, room_x + 675, room_y + 365, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 35, room_y + 35, room_x + 675, room_y + 365, &Scene, CutinLeady, Mouse, &cflag); //�����܃N���b�N

					/*----- �̂��n�L�����Ԓ����L -----*/
					if ((Scene == CutinLeady) && ((event & 0b1110) == 0b0110)) {
						sflag = 5;
					}
					/*----- �̂��n�������Ԓ����L -----*/
					else if ((Scene == CutinLeady) && ((event & 0b1100) == 0b1100)) {
						sflag = 7;
					}
					/*----- ����ȊO -----*/
					else if (Scene == CutinLeady) {
						sflag = 3;
					}
				}
				/*----- ���P�����̕\�� -----*/
				else if ((event & 0b110000) == 0b010000) {
					DrawGraph(room_x, room_y, pic_wall[5], FALSE);
					if ((room_x + 100 <= mx && mx < room_x + 590) && (room_y + 170 <= my && my < room_y + 290)) {
						DrawBox(room_x + 100, room_y + 170, room_x + 590, room_y + 290, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 100, room_y + 170, room_x + 590, room_y + 290, &Scene, CutinLeady, Mouse, &cflag); //���P�����N���b�N

					/*----- ���P�����L -----*/
					if (Scene == CutinLeady) {
						sflag = 8;
					}
				}
				/*----- �{�^���̕\�� -----*/
				else if ((event & 0b100000) == 0b100000) {
					DrawGraph(room_x, room_y, pic_wall[6], FALSE);
					if ((room_x + 275 <= mx && mx < room_x + 405) && (room_y + 245 <= my && my < room_y + 345)) {
						DrawBox(room_x + 275, room_y + 245, room_x + 405, room_y + 345, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 275, room_y + 245, room_x + 405, room_y + 345, &Scene, CutinLeady, Mouse, &cflag); //�{�^���N���b�N

					/*----- �R�[�h���͍� -----*/
					if ((Scene == CutinLeady) && ((event & 0b1000000000000) == 0b1000000000000)) {
						sflag = 16;
					}
					/*----- ����ȊO -----*/
					else if (Scene == CutinLeady) {
						sflag = 10;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall2, Mouse, &cflag); //���ړ��p�{�^���N���b�N
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall4, Mouse, &cflag); //�E�ړ��p�{�^���N���b�N
				MouseEND(Mouse, &cflag);
				break;
			}

			case Wall4: {
				/*----- �K�i�̕\�� -----*/
				if ((event & 0b100000) != 0b100000) {
					DrawGraph(room_x, room_y, pic_wall[7], FALSE);
					if ((room_x + 160 <= mx && mx < room_x + 550) && (room_y + 40 <= my && my < room_y + 405)) {
						DrawBox(room_x + 160, room_y + 40, room_x + 550, room_y + 410, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 160, room_y + 40, room_x + 550, room_y + 410, &Scene, CutinLeady, Mouse, &cflag); //�K�i�N���b�N

					/*----- �N���b�N -----*/
					if (Scene == CutinLeady) {
						sflag = 4;
					}
				}
				/*----- �V���b�^�[�̕\�� -----*/
				else if ((event & 0b10000000000) != 0b10000000000) {
					DrawGraph(room_x, room_y, pic_wall[8], FALSE);
					if ((room_x + 140 <= mx && mx < room_x + 570) && (room_y + 40 <= my && my < room_y + 405)) {
						DrawBox(room_x + 140, room_y + 40, room_x + 570, room_y + 410, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 140, room_y + 40, room_x + 570, room_y + 410, &Scene, CutinLeady, Mouse, &cflag); //�V���b�^�[�N���b�N

					/*----- �V���b�^�[�����ς��C���N���� -----*/
					if ((Scene == CutinLeady) && ((event & 0b1100000000) == 0b1100000000)) {
						sflag = 13;
					}
					/*----- ����ȊO -----*/
					else if (Scene == CutinLeady) {
						sflag = 11;
					}
				}
				/*----- �V���b�^�[(�C���N)�̕\�� -----*/
				else if ((event & 0b10000000000) == 0b10000000000) {
					DrawGraph(room_x, room_y, pic_wall[9], FALSE);
					if ((room_x + 140 <= mx && mx < room_x + 570) && (room_y + 40 <= my && my < room_y + 405)) {
						DrawBox(room_x + 140, room_y + 40, room_x + 570, room_y + 410, Red, FALSE);
					}
					MouseXY(mx, my, room_x + 140, room_y + 40, room_x + 570, room_y + 410, &Scene, CutinLeady, Mouse, &cflag); //�V���b�^�[(�C���N)�N���b�N

					/*----- �N���b�N -----*/
					if (Scene == CutinLeady) {
						sflag = 14;
					}
				}

				MouseXY(mx, my, 0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall3, Mouse, &cflag); //���ړ��p�{�^���N���b�N
				MouseXY(mx, my, SCREEN_WIDTH - MOVE_BUTTON_WIDTH, 0, SCREEN_WIDTH, MOVE_BUTTON_HEIGHT, &GScene, Wall1, Mouse, &cflag); //�E�ړ��p�{�^���N���b�N
				MouseEND(Mouse, &cflag);
				break;
			}
			}

			/*----- �H����p�`�� -----*/
			DrawGraph(0, 0, pic_head, TRUE);

			/*----- �ړ��p�{�^���`�� -----*/
			/*----- ���{�^�� -----*/
			if ((0 <= mx && mx < MOVE_BUTTON_WIDTH) && (0 <= my && my < MOVE_BUTTON_HEIGHT)) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
				DrawBox(0, 0, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, Black, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawTriangle((MOVE_BUTTON_WIDTH / 2) - 40, (MOVE_BUTTON_HEIGHT / 2), (MOVE_BUTTON_WIDTH / 2) + 40, (MOVE_BUTTON_HEIGHT / 2) - 80, (MOVE_BUTTON_WIDTH / 2) + 40, (MOVE_BUTTON_HEIGHT / 2) + 80, White, TRUE);
			}
			/*----- �E�{�^�� -----*/
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
					TextBox("���C�R", "�Ȃɂ��́u�Ŕv�H", "���������Ă����H", "", "",  text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "�w������������I�o���͂Ȃ���I�x���ď����Ă���݂������ˁB", "�ǂ���玄�����͕����߂�ꂽ�悤����B", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�Ȃ�Ă��������I", "���Ƃ����ĒE�o���Ă��I", "", "", text);
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
					TextBox("���C�R", "�˂��˂��A�ǂɂ������Ă邱��͂Ȃ��ɁH", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "����́u�̂�����v���Ă����񂾂�B", "���낢��Ȃ��̂�؂邱�Ƃ��ł���񂾁B", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�͂��[�A����Ȃɔ����̂ɂ��������I", "����ł������́u�Ŕv���؂��邩�ȁH", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "�����...����Ă݂Ȃ��ƕ�����Ȃ��ˁB", "���ƂŎ����ɍs�����ˁB", "", "", text);
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
					TextBox("���E�R", "�����ɂ�...�ق��ɉ����Ȃ��ˁB", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���C�R", "�Ȃ��ˁ[�B", "", "", "", text);
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
					TextBox("���C�R", "�킟�A�����̕ǂ�����Ɓu�����܁v�������I", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "�{�����A�悭�������ˁB", "���炢���[�B", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�G�w�w�B", "�����A�����ɉ����������߂Ȃ����ȁH", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "�������̂Ȃ獷�����߂邩������Ȃ��ˁB", "���ƂŒT���Ă݂悤�ˁB", "", "", text);
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
					TextBox("���C�R", "���킟...�Ȃ񂩑̂��ӂ�ӂ킷��`�B", "���͂͂͂́B", "", "", text);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 1, Mouse, &cflag);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "���V�������Ă�[�I", "�s������_���\�I", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�͂��A�킽���͍������I�H", "", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "�����͊댯����...�B", "�߂Â��Ȃ��悤�ɂ��܂��傤�B", "", "", text);
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
					TextBox("���C�R", "���[�񂤁[��B", "�u�̂�����v����������h����Ȃ��悧�B", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "�������I", "�c�����́u�̂�����v������n�������������Ă�񂾂�I", "�ǂ��ɂ����Đn�����Ȃ��Ƃ����Ȃ����...�B", "�߂��ɂ���Ȃ��̂�����������B", text);
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
					TextBox("���C�R", "��[���A�u�Ŕv��؂邼�[�I", "�������������[�I", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "�����I", "�u�̂�����v�̐n���ǂ�ǂ���Ă�I", "�Ƃ������A�ǂꂾ���ł��̂悱�́u�Ŕv�I", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�������������[�I", "", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "������߂ā\�I", "�u�̂�����v���Ȃ��Ȃ����Ⴄ�[�I", "", "", text);
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
					TextBox("���C�R", "�����A�u�̂�����v�h�������I", "�����...�ӂ���I", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "�ǂ��O�ꂽ�I", "���蕿����I", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "���ɂ���������݂������ˁB", "�`���Ă݂悤�B", "", "", text);
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
					TextBox("���E�R", "�����...�u���P�b�g�����`���[�v���Ă���ȁH", "�Ȃ�ł���ȂƂ���ɁH", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("�����̉�", "�K�V���[���I�I�I�I�I�I�I�I�I�I�I�I", "", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�����A�ȂɂȂɁI�H", "�������傫�����I", "�т�����I", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "���́u�X�C�b�`�v��������Ȃ��Ȃ�������A�������������̂ˁB", "������Ƃ܂������Ă݂܂��傤���B", "", "", text);
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
					TextBox("���E�R", "�u�̂�����v�Ő؂�Ȃ��Ă��A����Ȃ�I", "�t�@�C�A�I�I�I�I�I�I�I�I�I�I�I", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���C�R", "���[�I�I�I�I�I�I�I", "�������[���[��I�I�I�I�I�I�I", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���E�R", "����ŐՌ`���Ȃ�����t...��H", "�҂��āI", "�������c���Ă�I", "", text);
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
					TextBox("���C�R", "�ˁ[�ˁ[�A���́u�X�C�b�`�v���Ă�����Ă����H", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "�_�[���B", "�������y�������Ă݂����ǁA�т��Ƃ����Ȃ�������B", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�����[�B", "�܂�Ȃ��́[�B", "", "", text);
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
					TextBox("���C�R", "�������ӂ�ӂ킵���Ƃ���A�����łӂ�����Ă��I", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "����́u�V���b�^�[�v���Ă�ˁB", "�u�X�C�b�`�v��������Ȃ��Ȃ������炵�܂����񂾂�B", "�ɂ��Ă���������v�ˁA����B", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�������Ă��т��Ƃ����ȁ[���I", "��������[���I", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "������ƁA�����������I", "���邳���I", "", "", text);
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
					TextBox("���C�R", "�Ȃ񂾂낤����H", "���������͂ł������I", "", "", text);
					break;
				}
				case 1: {
					DrawGraph(0, 0, pic_all[12], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "���Ԃ�u���͔v���Ă��̂���Ȃ����ȁB", "�����A�������ɂ����������H", "", "", text);
					break;
				}
				case 2: {
					DrawGraph(0, 0, pic_all[13], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�����...�u�C���N�v���[�I", "...�Ȃ�ł��낤�H", "", "", text);
					break;
				}
					  DrawGraph(0, 0, pic_all[13], TRUE);
				case 3: {
					DrawGraph(0, 0, pic_all[13], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "�g�p�p�r���S��������Ȃ�...�B", "�䂾...�B", "", "", text);
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
					TextBox("���C�R", "�ւԂ������������I�I�I�I�I�I", "���ĂĂĂ�...�B", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "���v�I�H", "��...�u�C���N�v���u�V���b�^�[�v�ɂ��������������ˁB", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "����H", "������Ƒ҂��āH", "�����������яオ���Ă���I", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("��l", "����...����́I�I�I�I�I�I�I�I�I�I�I�I", "", "", "", text);
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
					TextBox("���C�R", "�u���͔v�ɑł����ޗp�́u�R�[�h�v���I", "", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "����ȂƂ����...�B", "������u�C���N�v���������̂ˁI", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "������u���͔v�ɑł����񂾂�E�o�ł��邩���I", "��������Ă݂悤�I", "", "", text);
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
					TextBox("���C�R", "...����ł悵���ƁB", "�J���S�}�I", "", "", text);
					break;
				}
				case 1: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "...�����N���Ȃ��ˁB", "�����ق��Ƀg���K�[�ɂȂ���̂�����̂��ȁH", "", "", text);
					break;
				}
				case 2: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("���C�R", "�����I�������́u�X�C�b�`�v�I", "���̂ŉ�����悤�ɂȂ����񂶂�Ȃ��H", "", "", text);
					break;
				}
				case 3: {
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 4, Mouse, &cflag);
					TextBox("���E�R", "���͎����ˁB", "�����Ă݂܂��傤���B", "", "", text);
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
					TextBox("���C�R", "�ۂ����ƂȁB", "", "", "", text);
					break;
				}
				case 1: {
					DrawGraph(0, 0, pic_all[17], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 2, Mouse, &cflag);
					TextBox("���E�R", "�Ȃɂ�������n...", "", "", "", text);
					break;
				}
				case 2: {
					DrawGraph(0, 0, pic_all[18], TRUE);
					MouseXY(mx, my, 0, 0, 1280, 720, &esflag, 3, Mouse, &cflag);
					TextBox("�I�H�I�H�I�H�I�H", "���E�R", "", "", "", text);
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
					/*----- �t�F�[�h�A�E�g -----*/
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

	DxLib_End(); // DX���C�u�����I������
	return 0;

}


void fade(int fade_mode) {
	/*----- �t�F�[�h�A�E�g -----*/
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
	/*----- �t�F�[�h�C�� -----*/
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