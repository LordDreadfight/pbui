#pragma once
#include "UIElement.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Label : public UIElement {
    Label(short int x, short int y, unsigned short int width, unsigned short int height, const std::string& text);

    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event) override;

    short int x, y;
    unsigned short int width, height;
    std::string text;
    TTF_Font* font;
};
