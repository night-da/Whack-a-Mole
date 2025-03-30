#ifndef _BUTTON_H_
#define _BUTTON_H_

// 头文件：按钮类

#include <string>		// 文本
#include <functional>	// 回调
#include <graphics.h>	// EasyX 头文件

using namespace std;

class Button {
public:
	// 构造函数
	// 用 wstring (EasyX 宽字符)
	Button(const wstring& text, const RECT& rect) {
		this->text = text;
		this->rect = rect;
	}

	// 析构函数
	~Button() = default;

	// 消息
	void on_input(const ExMessage& msg) {
		switch (msg.message) {
			// 鼠标移动 (msg)
		case WM_MOUSEMOVE:
			is_hovered = msg.x >= rect.left && msg.x <= rect.right
				&& msg.y >= rect.top && msg.y <= rect.bottom;
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if (is_hovered) {
				is_pushed = true;
			}
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			if (is_pushed) {
				on_click();
			}
			is_pushed = false;
			break;

		default:
			break;
		}
	}

	void on_render() {
		// 设置边框颜色
		setlinecolor(RGB(20, 20, 20));
		// 按下
		if (is_pushed) {
			setfillcolor(RGB(185, 185, 185));
		}
		// 悬停
		else if (is_hovered) {
			setfillcolor(RGB(235, 235, 235));
		}
		else {
			setfillcolor(RGB(205, 205, 205));
		}

		// 矩形填充
		fillrectangle(rect.left, rect.top, rect.right, rect.bottom);

		setbkmode(TRANSPARENT);
		settextcolor(RGB(20, 20, 20));	// 文本颜色
		// 水平居中 | 单行文本 | 竖向居中
		drawtext(text.c_str(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	void set_on_click(function<void()> on_click) {
		this->on_click = on_click;
	}

private:
	RECT rect;
	wstring text;

	bool is_pushed = false;		// 按下
	bool is_hovered = false;	// 悬停
	// 当点到的时候，回调
	function<void()> on_click;
};

#endif