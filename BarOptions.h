#pragma once

#include <functional>
#include <iostream>
#include "context.h"

extern RuntimeData data;

class BarOptions {
public:
    BarOptions(int* tg, Vector2 p, int x, int y, const char* name) : target_val(tg), hig(y), wid(x), pos(p), name(name) {
        Coll = { p.x, p.y, (float)x, (float)y };
    }

    void update() {
        Vector2 mouse_pos = data.mouse;

        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return;
        if (!CheckCollisionPointRec(mouse_pos, Coll))  return;

        //std::cout << "clck  ";
        float bit = (mouse_pos.x - pos.x) / wid * 100 + 0.5;
        //std:: cout << bit << std:: endl;
        *target_val = bit;
    }

    void Draw() {
        Rectangle bar = { pos.x, pos.y, (float)wid, (float)hig };
        Rectangle valueBar = { pos.x + 5, pos.y + 5, *target_val / 100.0f * (wid - 10), (float)hig - 10 };

        DrawTextEx(fontCN, name, { pos.x,pos.y - 50 }, 32, 1, WHITE);
        DrawRectangleRounded(bar, 0.3f, 8, Color{ 35, 42, 55, 255 });
        DrawRectangleRounded(valueBar, 0.3f, 8, Color{ 65, 170, 255, 255 });
        DrawRectangleLinesEx(bar, 3, Color{ 120, 135, 155, 255 });
        DrawTextEx(fontCN, TextFormat("%d", *target_val), { pos.x + wid + 25,pos.y + 8 }, 30, 1, WHITE);
    }

private:

    int* const target_val;
    int hig, wid;
    Vector2 pos;
    Rectangle Coll;
    const char* name;
    };

class ButtonBase {
public:

    virtual ~ButtonBase() = default;
    virtual void draw() = 0;
    virtual void handle_input() = 0;
    virtual Rectangle getRec() = 0;
    virtual bool isMouseColliding() = 0;

private:
};


template<typename Signture>
class RecButton;

template<typename R, typename ... Args>
class RecButton<R(Args ...)> : public ButtonBase
{
public:
    RecButton(const char* s, float x, float y, float zoom) : x(x), y(y), CurButtonZoom(zoom) {
        button_texture = LoadTexture(s);
        update_rect();
    }

    RecButton(const char* s, float x, float y, float zoom, std::function<R(Args ...)> subfuncion) : CurButtonZoom(zoom), x(x), y(y), callback(subfuncion) {
        button_texture = LoadTexture(s);
        update_rect();
    }

    ~RecButton() {
        UnloadTexture(button_texture);
    }

    void draw() override {
        Vector2 pos = { x, y };
        update_rect();

        DrawTextureEx(button_texture, pos, 0.0, CurButtonZoom, WHITE);

        if (CheckCollisionPointRec(data.mouse, button_rect)) {
            DrawRectangleRec(button_rect, { 0, 0, 0, 100 });
        }

#ifdef DEBUG
        DrawRectangleLinesEx(button_rect, 1, RED);
#endif

    }

    virtual R handle_input(Args ...args) override {
        if (callback) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(data.mouse, getRec())) {
                    //std::cout << "Click " << std::endl;
                    return callback(std::forward<Args>(args)...);
                }
            }
        }
    }

    Rectangle getRec() override {
        return button_rect;
    }

    bool isMouseColliding() override {
        return CheckCollisionPointRec(data.mouse, button_rect);
    }


private:
    float CurButtonZoom = 1.0;
    std::function<R(Args ...)> callback;

    float x, y;
    void update_rect() {
        button_rect = { x , y, (float)button_texture.width * CurButtonZoom, (float)button_texture.height * CurButtonZoom };
    }
    Texture button_texture;
    Rectangle button_rect;
};












template<typename Signture>
class TextButton;

template<typename R, typename ... Args>
class TextButton<R(Args ...)> : public ButtonBase
{
public:

    TextButton(std::string s, float x, float y) :print_txt(s), x(x), y(y) {
        button_texture = LoadTexture("assets/textures/ui/button-disabled.png");
        update_rect();
    }

    TextButton(std::string s, float x, float y, std::function<R(Args ...)> subfuncion) : print_txt(s), x(x), y(y), callback(subfuncion) {
        button_texture = LoadTexture("assets/textures/ui/button-disabled.png");
        update_rect();
    }

    ~TextButton() {
        UnloadTexture(button_texture);
    }

    void draw() override {
        Vector2 pos = { x , y };
        update_rect();
        DrawTextureEx(button_texture, pos, 0, CurButtonZoom, WHITE);

        if (CheckCollisionPointRec(data.mouse, button_rect)) {
            DrawRectangleRec(button_rect, { 0, 0, 0, 100 });
        }

        int textWidth = MeasureTextEx(fontCN, print_txt.c_str(), 40, 1).x;
        DrawTextEx(fontCN, print_txt.c_str(), { x + (button_rect.width - textWidth) / 2,y + (button_rect.height - 40) / 2 }, 40, 1, BLACK);

#ifdef DEBUG
        DrawRectangleLinesEx(button_rect, 1, RED);
#endif
    }
    virtual R handle_input(Args ...args) override {
        if (callback) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(data.mouse, getRec())) {
                    std::cout << "Click " << std::endl;


                    return callback(std::forward<Args>(args)...);
                }
            }
        }
    }

    Rectangle getRec() override {
        return button_rect;
    }

    bool isMouseColliding() override {
        return CheckCollisionPointRec(data.mouse, button_rect);
    }


private:

    std::function<R(Args ...)> callback;
    float x, y;
    float CurButtonZoom = 0.4f;
    std::string print_txt;
    void update_rect() {
        button_rect = { x , y , button_texture.width * CurButtonZoom, button_texture.height * CurButtonZoom };
    }
    Texture button_texture;
    Rectangle button_rect;

};
