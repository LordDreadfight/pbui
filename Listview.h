#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct ListView : public UIElement {
    short int x, y;
    unsigned short int width, height;
    unsigned short int itemHeight;
    std::vector<std::string> items;
    int selectedIndex;
    int scrollOffset;
    bool isFocused;

    TTF_Font* font;
    std::function<void(const std::string&)> onItemSelected;

    ListView(short int x, short int y, unsigned short int width, unsigned short int height, unsigned short int itemHeight);
    void setOnItemSelected(std::function<void(const std::string&)> callback);
    void addItem(const std::string& item);
    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event) override;
    void drawItem(SDL_Renderer* renderer, const std::string& text, short int itemY, bool isSelected);
};
