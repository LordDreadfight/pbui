#pragma once
#include "UIElement.h"
#include <functional>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

struct Combobox : public UIElement {
    Combobox(short int x, short int y, unsigned short int width, unsigned short int height, const std::vector<std::string>& options);
    void setOnSelectionChange(std::function<void(const std::string&)> callback);
    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event) override;
    short int x, y;
    unsigned short int width, height;
    std::vector<std::string> options;
    int selectedIndex;                
    bool isOpen;                     
    std::function<void(const std::string&)> onSelectionChange;
    TTF_Font* font;
    void drawOption(SDL_Renderer* renderer, short int index, short int optionX, short int optionY, short int optionWidth, short int optionHeight, bool isHovered);
};
