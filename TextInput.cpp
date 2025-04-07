#include "TextInput.h"
#include <iostream>
#include <SDL2/SDL_timer.h>

TextInput::TextInput(short int x, short int y, unsigned short int width, unsigned short int height)
    : x(x), y(y), width(width), height(height), text(""), isFocused(false), caretPos(0), selectionStart(-1), selectionEnd(-1), showCaret(true), lastBlinkTime(SDL_GetTicks()) {
    font = TTF_OpenFont("Ubuntu-Regular.ttf", 14);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

void TextInput::setPlaceholder(const std::string& placeholderText) {
    placeholder = placeholderText;
}

void TextInput::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = {x, y, width, height};
    SDL_Color bgColor = isFocused ? SDL_Color{112, 220, 112, 255} : SDL_Color{47, 211, 71, 255};
    SDL_Color borderColor = SDL_Color{0, 0, 0, 255};

    // Draw background
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);

    // Draw border
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &rect);

    // Render text or placeholder
    std::string renderText = text.empty() && !isFocused ? placeholder : text;
    SDL_Color textColor = text.empty() && !isFocused ? SDL_Color{150, 150, 150, 255} : SDL_Color{0, 0, 0, 255};

    if (font) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, renderText.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = {x + 5, y + (height - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }

    // Draw caret
    if (isFocused && showCaret && caretPos <= text.size()) {
        std::string textBeforeCaret = text.substr(0, caretPos);
        SDL_Surface* caretSurface = TTF_RenderText_Solid(font, textBeforeCaret.c_str(), textColor);
        int caretX = x + 5 + (caretSurface ? caretSurface->w : 0);
        SDL_FreeSurface(caretSurface);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, caretX, y + 4, caretX, y + height - 4);
    }

    // Blink caret logic
    if (SDL_GetTicks() - lastBlinkTime >= 500) {
        lastBlinkTime = SDL_GetTicks();
        showCaret = !showCaret;
    }
}

void TextInput::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        isFocused = mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height;
        
        if (isFocused) {
            selectionStart = -1;
            caretPos = getClosestCharIndex(mouseX);
        }
    }

    if (isFocused && event.type == SDL_TEXTINPUT) {
        text.insert(caretPos, event.text.text);
        caretPos += strlen(event.text.text);
    } else if (isFocused && event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && caretPos > 0) {
            text.erase(caretPos - 1, 1);
            caretPos--;
        } else if (event.key.keysym.sym == SDLK_LEFT && caretPos > 0) {
            caretPos--;
        } else if (event.key.keysym.sym == SDLK_RIGHT && caretPos < text.size()) {
            caretPos++;
        }
    }
}

int TextInput::getClosestCharIndex(int mouseX) const {
    int closestIndex = 0;
    int minDistance = width;

    for (size_t i = 0; i <= text.size(); ++i) {
        std::string partialText = text.substr(0, i);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, partialText.c_str(), SDL_Color{0, 0, 0, 255});
        int textWidth = textSurface ? textSurface->w : 0;
        SDL_FreeSurface(textSurface);

        int distance = abs(x + 5 + textWidth - mouseX);
        if (distance < minDistance) {
            minDistance = distance;
            closestIndex = i;
        }
    }
    return closestIndex;
}