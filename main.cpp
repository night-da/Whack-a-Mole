#include "button.h"

// ���� + ���� + ��ť���ࣩ

#include <graphics.h>

// ����
#pragma comment(lib, "WINMM.lib")	// ��Ƶ
#pragma comment(lib, "MSIMG32.lib")	// ΢�����ÿ�

// �ز�

// �е����޵���ĸ��ӣ����ӣ��������֣��û���Ч

IMAGE img_menu;							// ���˵�����ͼ
IMAGE img_mole;							// ����ͼƬ
IMAGE img_empty;						// ��λͼƬ
IMAGE img_hammer_idle;					// Ĭ��״̬����
IMAGE img_hammer_down;					// ����״̬����

// ��Ϸ�Ƿ�ʼ�ͽ�����ȫ�ֱ���
bool is_quit = false;					// ��Ϸ�Ƿ��˳�
bool is_started = false;				// ��Ϸ�Ƿ�ʼ

// ��λ�е��ƫ�ƣ����У�
const POINT offset_map = { 32, 75 };	// ��ͼƫ������

// ��ͼ�����飩
bool map[4][4] = {
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
};										// ��Ϸ��ͼ
int idx_x, idx_y;						// ��ǰ��������

// ���ӵ�״̬
bool is_hammer_down = false;			// �����Ƿ�����
// ���ӵ�����
POINT pos_hammer = { 0, 0 };			// ���ӵ�����λ��

// �÷�
int score = 0;							// ��ǰ��ҵ÷�

// ��ʼ�˵��� 3 ����ť
Button btn_play(L"�� ʼ �� Ϸ", { 300, 450, 500, 500 });
Button btn_info(L"�� Ϸ �� ��", { 300, 550, 500, 600 });
Button btn_quit(L"�� �� �� Ϸ", { 300, 650, 500, 700 });

// ����͸����ͼ
void my_putimage(int x, int y, IMAGE* img) {
	// ��ȡ��ȡ��߶�
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

// �����ز�
void load_resources() {
	// ����ͼƬ�ز�
	loadimage(&img_menu, _T("resources/menu.jpg"));
	loadimage(&img_mole, _T("resources/mole.jpg"));
	loadimage(&img_empty, _T("resources/empty.jpg"));
	loadimage(&img_hammer_idle, _T("resources/hammer_idle.png"));
	loadimage(&img_hammer_down, _T("resources/hammer_down.png"));

	// ������Ƶ
	mciSendString(_T("open resources/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open resources/hit.mp3 alias hit"), NULL, 0, NULL);
}

// �˵�����
void input_menu_scene(const ExMessage& msg) {
	// 3 ����ť
	btn_play.on_input(msg);
	btn_info.on_input(msg);
	btn_quit.on_input(msg);
}

void input_game_scene(const ExMessage& msg) {
	switch (msg.message) {
	case WM_MOUSEMOVE:
		pos_hammer.x = msg.x;
		pos_hammer.y = msg.y;
		break;

	case WM_LBUTTONDOWN: {
		is_hammer_down = true;

		static const int width_img_mole = img_mole.getwidth();
		static const int height_img_mole = img_mole.getheight();
		// �ж���Χ���÷ּӼ�
		if (map[idx_x][idx_y] &&
			(pos_hammer.x >= idx_x * width_img_mole + offset_map.x
				&& pos_hammer.x <= (idx_x + 1) * width_img_mole + offset_map.x
				&& pos_hammer.y >= idx_y * height_img_mole + offset_map.y
				&& pos_hammer.y <= (idx_y + 1) * height_img_mole + offset_map.y)) {
			score += 10;
			map[idx_x][idx_y] = false;
			mciSendString(_T("play hit repeat from 0"), NULL, 0, NULL);
		}
		else {
			score -= 10;
		}
	}
		break;

	case WM_LBUTTONUP:
		is_hammer_down = false;
		break;

	default:
		break;
	}
}

// ��Ⱦ�˵�����
void render_menu_scene() {
	// �Ų˵�����ͼ
	putimage(0, 0, &img_menu);

	// ��Ⱦ
	btn_play.on_render();
	btn_info.on_render();
	btn_quit.on_render();
}

// ��Ⱦ��Ϸ����
void render_game_scene() {
	// ������Ϸ��ͼ
	static const int width_img_mole = img_mole.getwidth();
	static const int height_img_mole = img_mole.getheight();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// �����ǰλ�ô��ڵ��󣬾ͻ��Ƶ�����ͼ��������ƿ���ͼ
			if (map[i][j]) {
				putimage(i * width_img_mole + offset_map.x,
					j * height_img_mole + offset_map.y, &img_mole);
			}
			else {
				putimage(i * width_img_mole + offset_map.x,
					j * height_img_mole + offset_map.y, &img_empty);
			}

		}
	}

	// ���ƴ���
	static const int width_img_hammer_idle = img_hammer_idle.getwidth();
	static const int height_img_hammer_idle = img_hammer_idle.getheight();
	static const int width_img_hammer_down = img_hammer_down.getwidth();
	static const int height_img_hammer_down = img_hammer_down.getheight();
	if (!is_hammer_down) {
		my_putimage(pos_hammer.x - width_img_hammer_idle / 2,
			pos_hammer.y - height_img_hammer_idle / 2, &img_hammer_idle);
	}
	else {
		my_putimage(pos_hammer.x - width_img_hammer_down / 2,
			pos_hammer.y - height_img_hammer_down / 2, &img_hammer_down);
	}

	// ������Ϸ�÷�
	wstring str_score = L"��ҵ÷� " + to_wstring(score);
	settextstyle(25, 0, _T("����"));
	settextcolor(RGB(255, 15, 105));
	outtextxy(25, 25, str_score.c_str());
}

// ���£�������ʧ������Ļ���һ��ʱ��Ҳ��ʧ����̬������Ϸ����
void update_game_scene() {
	static int timer = 0;

	// ���Ӷ�ʱ��ʱ��
	timer = ++timer % 100;

	// �������ʱ�䣬ˢ�µ�ͼ�ϵĵ���λ��
	if (timer == 0) {
		map[idx_x][idx_y] = false;
		idx_x = rand() % 4, idx_y = rand() % 4;
		map[idx_x][idx_y] = true;
	}
}

int main(int argc, char** argv) {
	// ������Ϸ����
	HWND hwnd = initgraph(800, 950);
	// ���ñ���
	SetWindowText(hwnd, _T("�����"));

	// �����ز�
	load_resources();

	// ��ʼ����ť
	btn_play.set_on_click([&]()
		{	// lambda ����
			is_started = true;
			mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
		});
	btn_info.set_on_click([&]()
		{
			MessageBox(hwnd, _T("��ͼ�л������ˢ�µĵ���\n����������û�����÷�"),
				_T("�淨����"), MB_OK);
		});
	btn_quit.set_on_click([&]()
		{
			is_quit = true;
		});

	// ����������ͼ������Ҫ��
	BeginBatchDraw();

	while (!is_quit) {
		// �û�����
		ExMessage msg;
		while (peekmessage(&msg)) {
			// ���û�п�ʼ��Ϸ
			if (!is_started) {
				input_menu_scene(msg);
			}
			else {
				input_game_scene(msg);
			}
		}

		// �����ʼ��Ϸ���Ϳ�ʼ����
		if (is_started) {
			update_game_scene();
		}

		// ��Ϸ������ɫ
		setbkcolor(RGB(251, 251, 251));
		// �����Ļ
		cleardevice();

		// �����ʼ��Ϸ
		if (!is_started) {
			render_menu_scene();
		}
		else {
			render_game_scene();
		}

		FlushBatchDraw();

		// ��̬��ʱ��5���룩
		Sleep(5);
	}

	// ����������ͼ
	EndBatchDraw();

	return 0;
}