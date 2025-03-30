#include "button.h"

// 界面 + 背景 + 按钮（类）

#include <graphics.h>

// 链接
#pragma comment(lib, "WINMM.lib")	// 音频
#pragma comment(lib, "MSIMG32.lib")	// 微软内置库

// 素材

// 有地鼠，无地鼠的格子；锤子，背景音乐，敲击音效

IMAGE img_menu;							// 主菜单背景图
IMAGE img_mole;							// 地鼠图片
IMAGE img_empty;						// 坑位图片
IMAGE img_hammer_idle;					// 默认状态锤子
IMAGE img_hammer_down;					// 敲下状态锤子

// 游戏是否开始和结束的全局变量
bool is_quit = false;					// 游戏是否退出
bool is_started = false;				// 游戏是否开始

// 坑位有点大，偏移（居中）
const POINT offset_map = { 32, 75 };	// 地图偏移坐标

// 地图（数组）
bool map[4][4] = {
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
};										// 游戏地图
int idx_x, idx_y;						// 当前地鼠索引

// 锤子的状态
bool is_hammer_down = false;			// 锤子是否落下
// 锤子的坐标
POINT pos_hammer = { 0, 0 };			// 锤子的坐标位置

// 得分
int score = 0;							// 当前玩家得分

// 开始菜单的 3 个按钮
Button btn_play(L"开 始 游 戏", { 300, 450, 500, 500 });
Button btn_info(L"游 戏 介 绍", { 300, 550, 500, 600 });
Button btn_quit(L"退 出 游 戏", { 300, 650, 500, 700 });

// 绘制透明贴图
void my_putimage(int x, int y, IMAGE* img) {
	// 获取宽度、高度
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

// 加载素材
void load_resources() {
	// 设置图片素材
	loadimage(&img_menu, _T("resources/menu.jpg"));
	loadimage(&img_mole, _T("resources/mole.jpg"));
	loadimage(&img_empty, _T("resources/empty.jpg"));
	loadimage(&img_hammer_idle, _T("resources/hammer_idle.png"));
	loadimage(&img_hammer_down, _T("resources/hammer_down.png"));

	// 加载音频
	mciSendString(_T("open resources/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open resources/hit.mp3 alias hit"), NULL, 0, NULL);
}

// 菜单输入
void input_menu_scene(const ExMessage& msg) {
	// 3 个按钮
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
		// 判定范围，得分加减
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

// 渲染菜单场景
void render_menu_scene() {
	// 放菜单背景图
	putimage(0, 0, &img_menu);

	// 渲染
	btn_play.on_render();
	btn_info.on_render();
	btn_quit.on_render();
}

// 渲染游戏场景
void render_game_scene() {
	// 绘制游戏地图
	static const int width_img_mole = img_mole.getwidth();
	static const int height_img_mole = img_mole.getheight();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// 如果当前位置存在地鼠，就绘制地鼠贴图，否则绘制空贴图
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

	// 绘制锤子
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

	// 绘制游戏得分
	wstring str_score = L"玩家得分 " + to_wstring(score);
	settextstyle(25, 0, _T("黑体"));
	settextcolor(RGB(255, 15, 105));
	outtextxy(25, 25, str_score.c_str());
}

// 更新，打了消失，不打的话过一段时间也消失，动态更新游戏数据
void update_game_scene() {
	static int timer = 0;

	// 增加定时器时间
	timer = ++timer % 100;

	// 如果到达时间，刷新地图上的地鼠位置
	if (timer == 0) {
		map[idx_x][idx_y] = false;
		idx_x = rand() % 4, idx_y = rand() % 4;
		map[idx_x][idx_y] = true;
	}
}

int main(int argc, char** argv) {
	// 创建游戏窗口
	HWND hwnd = initgraph(800, 950);
	// 设置标题
	SetWindowText(hwnd, _T("打地鼠"));

	// 加载素材
	load_resources();

	// 初始化按钮
	btn_play.set_on_click([&]()
		{	// lambda 函数
			is_started = true;
			mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
		});
	btn_info.set_on_click([&]()
		{
			MessageBox(hwnd, _T("地图中会有随机刷新的地鼠\n点击鼠标左键敲击地鼠得分"),
				_T("玩法介绍"), MB_OK);
		});
	btn_quit.set_on_click([&]()
		{
			is_quit = true;
		});

	// 开启批量绘图（不重要）
	BeginBatchDraw();

	while (!is_quit) {
		// 用户输入
		ExMessage msg;
		while (peekmessage(&msg)) {
			// 如果没有开始游戏
			if (!is_started) {
				input_menu_scene(msg);
			}
			else {
				input_game_scene(msg);
			}
		}

		// 如果开始游戏，就开始更新
		if (is_started) {
			update_game_scene();
		}

		// 游戏背景颜色
		setbkcolor(RGB(251, 251, 251));
		// 清空屏幕
		cleardevice();

		// 如果开始游戏
		if (!is_started) {
			render_menu_scene();
		}
		else {
			render_game_scene();
		}

		FlushBatchDraw();

		// 静态延时（5毫秒）
		Sleep(5);
	}

	// 结束批量绘图
	EndBatchDraw();

	return 0;
}