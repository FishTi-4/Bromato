#include "Button.h"

const float DEFAULT_BUTTON_WIDTH = 450.0;
const float DEFAULT_BUTTON_HEIGHT = 140.0;

Button::Button(Vector2 position, std::string buttonText) // 构造函数
{
	// 设置位置和大小
	area.x = position.x;
	area.y = position.y;
	area.width = DEFAULT_BUTTON_WIDTH;
	area.height = DEFAULT_BUTTON_HEIGHT;

	// 设置材质
	textureName[Idx(ButtonState::Normal)] = TextureName::ButtonNormal;
	textureName[Idx(ButtonState::Hovered)] = TextureName::ButtonHovered;
	textureName[Idx(ButtonState::Pressed)] = TextureName::ButtonPressed;
	textureName[Idx(ButtonState::Disabled)] = TextureName::ButtonDisabled;

	state = ButtonState::Normal;
	captured = false;
	text = buttonText;
	fontSize = area.height * 0.3;
	textColor = BLACK;
}

Button::Button(Vector2 position, Vector2 size, std::string buttonText)
{
	area.x = position.x;
	area.y = position.y;
	area.width = size.x;
	area.height = size.y;

	textureName[Idx(ButtonState::Normal)] = TextureName::ButtonNormal;
	textureName[Idx(ButtonState::Hovered)] = TextureName::ButtonHovered;
	textureName[Idx(ButtonState::Pressed)] = TextureName::ButtonPressed;
	textureName[Idx(ButtonState::Disabled)] = TextureName::ButtonDisabled;

	state = ButtonState::Normal;
	captured = false;
	text = buttonText;
	fontSize = area.height * 0.3;
	textColor = BLACK;
}

Button::Button(Vector2 position, float scale, std::string buttonText)
{
	area.x = position.x;
	area.y = position.y;
	area.width = DEFAULT_BUTTON_WIDTH * scale;
	area.height = DEFAULT_BUTTON_HEIGHT * scale;

	textureName[Idx(ButtonState::Normal)] = TextureName::ButtonNormal;
	textureName[Idx(ButtonState::Hovered)] = TextureName::ButtonHovered;
	textureName[Idx(ButtonState::Pressed)] = TextureName::ButtonPressed;
	textureName[Idx(ButtonState::Disabled)] = TextureName::ButtonDisabled;

	state = ButtonState::Normal;
	captured = false;
	text = buttonText;
	fontSize = area.height * 0.3;
	textColor = BLACK;
}

bool Button::Update()
{ 
	if (state == ButtonState::Disabled) // 已禁用，直接返回
	{
		captured = false;
		return false;
	}
	bool inside = CheckCollisionPointRec(data.mouse, area);	// 光标是否在按键上
	if (inside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // 这一帧在鼠标内部按下鼠标，还没松开
	{	
		captured = true;
		state = ButtonState::Pressed;
		return false;
	}
	if (captured)
	{
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) // 已在案件内按下鼠标，这一帧刚好松开
		{
			captured = false;
			state = inside ? ButtonState::Hovered : ButtonState::Normal; // 是否在按键内部松开
			return inside;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))	// 已在按键内按下鼠标，这一帧还没松开
		{
			state = ButtonState::Pressed;
			return false;
		}
		// 其他神秘情况
		captured = false;
	}
	state = inside ? ButtonState::Hovered : ButtonState::Normal; // 是否在按键上
	return false;
}

void Button::Draw() const
{
	const Texture2D& texture = assets.GetTexture(textureName[Idx(state)]);			// 找到材质
	const Rectangle source = { 0,0,(texture.width),(texture.height) };				// 被拉伸素材矩形
	DrawTexturePro(texture, source, area, { 0,0 }, 0, WHITE);						// 把材质拉伸到有效区域
	if (text.empty()) return;			// 没文本直接返回
	const Font font = GetFontDefault(); // 获得字体
	const float spacing = 1;			// 字间距
	const Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);	// 字体长宽
	const Vector2 textPosition = { area.x + (area.width - textSize.x) / 2,area.y + (area.height - textSize.y) / 2 }; // 计算显示位置
	DrawTextEx(fontCN, text.c_str(), textPosition, fontSize, spacing, textColor);		// 绘制
}

ButtonState Button::GetState() const { return state; }

void Button::Enable() { if (state == ButtonState::Disabled) state = ButtonState::Normal; }

void Button::Disable() { state = ButtonState::Disabled; }

void Button::SetText(std::string newText) { text = newText; }

void Button::SetFontSize(float newFontSize) { fontSize = newFontSize; }

void Button::SetTextColor(Color color) { textColor = color; }