#pragma once

#include "context.h"
#include <string>

enum class ButtonState
{
	Normal,		// 正常状态
	Hovered,	// 焦点在按键上
	Pressed,	// 按键被按下
	Disabled,	// 按键不启用
	Count,
};

class Button
{
	TextureName textureName[4];	// 4个状态的材质
	ButtonState state;			// 按键当前显示状态
	Rectangle area;				// 按键所在位置和大小（按键所在区域）
	bool captured;			// 鼠标是否从这个按键内部开始按下
	std::string text;			// 按键中央显示的文本
	float fontSize;				// 文本字号
	Color textColor;			// 文本颜色

public:
	Button(Vector2 position, std::string text = {});				// 先传入位置 文本可选
	Button(Vector2 position, Vector2 size, std::string text = {});	// 先传入位置和大小 文本可选
	Button(Vector2 position, float scale, std::string text = {}); //位置 缩放比例 文本可选

	bool Update();						// 更新 返回真说明按键被点击
	void Draw() const;					// 绘制按键自身
	ButtonState GetState() const;		// 获得按键状态
	void Disable();						// 禁用按键
	void Enable();						// 启用按键
	void SetText(std::string text);		// 设置文本
	void SetFontSize(float fontSize);	// 设置字体大小
	void SetTextColor(Color color);		// 设置字体颜色
};
