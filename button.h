#ifndef _BUTTON_H_
#define _BUTTON_H_

// ͷ�ļ�����ť��

#include <string>		// �ı�
#include <functional>	// �ص�
#include <graphics.h>	// EasyX ͷ�ļ�

using namespace std;

class Button {
public:
	// ���캯��
	// �� wstring (EasyX ���ַ�)
	Button(const wstring& text, const RECT& rect) {
		this->text = text;
		this->rect = rect;
	}

	// ��������
	~Button() = default;

	// ��Ϣ
	void on_input(const ExMessage& msg) {
		switch (msg.message) {
			// ����ƶ� (msg)
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
		// ���ñ߿���ɫ
		setlinecolor(RGB(20, 20, 20));
		// ����
		if (is_pushed) {
			setfillcolor(RGB(185, 185, 185));
		}
		// ��ͣ
		else if (is_hovered) {
			setfillcolor(RGB(235, 235, 235));
		}
		else {
			setfillcolor(RGB(205, 205, 205));
		}

		// �������
		fillrectangle(rect.left, rect.top, rect.right, rect.bottom);

		setbkmode(TRANSPARENT);
		settextcolor(RGB(20, 20, 20));	// �ı���ɫ
		// ˮƽ���� | �����ı� | �������
		drawtext(text.c_str(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	void set_on_click(function<void()> on_click) {
		this->on_click = on_click;
	}

private:
	RECT rect;
	wstring text;

	bool is_pushed = false;		// ����
	bool is_hovered = false;	// ��ͣ
	// ���㵽��ʱ�򣬻ص�
	function<void()> on_click;
};

#endif