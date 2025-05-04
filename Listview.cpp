#include "Listview.h"
#include <iostream>

ListView::ListView(short int x, short int y, unsigned short int width, unsigned short int height, unsigned short int itemHeight)
    : x(x), y(y), width(width), height(height), itemHeight(itemHeight), selectedIndex(-1), scrollOffset(0), isFocused(false) {
    font = TTF_OpenFont("Roboto-Regular.ttf", 14);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

void ListView::setOnItemSelected(std::function<void(const std::string&)> callback) {
    onItemSelected = callback;
}
void ListView::addItem(const std::string& item) {
    items.push_back(item);
}
void ListView::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = {x, y, width, height};
    SDL_Color bgColor = {47, 211, 71, 255};
    SDL_Color borderColor = {0, 0, 0, 255};
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &rect);
    int visibleItems = height / itemHeight;
    for (int i = 0; i < visibleItems && i + scrollOffset < items.size(); ++i) {
        drawItem(renderer, items[i + scrollOffset], y + i * itemHeight, i + scrollOffset == selectedIndex);
    }
}

void ListView::drawItem(SDL_Renderer* renderer, const std::string& text, short int itemY, bool isSelected) {
    SDL_Rect itemRect = {x, itemY, width, itemHeight};
    SDL_Color bgColor = isSelected ? SDL_Color{112, 220, 112, 255} : SDL_Color{47, 211, 71, 255};
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &itemRect);

    if (font) {
        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = {x + 10, itemY + (itemHeight - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void ListView::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height) {
            isFocused = true;
            int clickedIndex = (mouseY - y) / itemHeight + scrollOffset;

            if (clickedIndex >= 0 && clickedIndex < items.size()) {
                selectedIndex = clickedIndex;
                if (onItemSelected) {
                    onItemSelected(items[selectedIndex]);
                }
            }
        } else {
            isFocused = false;
        }
    } else if (event.type == SDL_MOUSEWHEEL && isFocused) {
        scrollOffset -= event.wheel.y;
        if (scrollOffset < 0) {
            scrollOffset = 0;
        } else if (scrollOffset + height / itemHeight >= items.size()) {
            scrollOffset = items.size() - height / itemHeight;
        }
    }
}
