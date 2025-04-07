#include "Button.h"
#include <iostream>

Button::Button(short int x, short int y, unsigned short int width, unsigned short int height, const std::string& text)
    : x(x), y(y), width(width), height(height), text(text), isHovered(false), hoverStartTime(0), showTooltip(false)
{
    onClick = []() {};
    font = TTF_OpenFont("Ubuntu-Regular.ttf", 14);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Button::setTooltip(const std::string& tooltipText) {
    this->tooltipText = tooltipText;
}

void Button::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = {x, y, width, height};
    SDL_Color buttonColor = isHovered ? SDL_Color{112, 220, 112, 255} : SDL_Color{47, 211, 71, 255};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &rect);
    
    if (font) {
        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            unsigned short int textWidth = textSurface->w;
            unsigned short int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = {x + (width - textWidth) / 2, y + (height - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }

    if (showTooltip && !tooltipText.empty() && font) {
        SDL_Color tooltipColor = {255, 255, 255, 255};
        SDL_Surface* tooltipSurface = TTF_RenderText_Solid(font, tooltipText.c_str(), tooltipColor);
        if (tooltipSurface) {
            SDL_Texture* tooltipTexture = SDL_CreateTextureFromSurface(renderer, tooltipSurface);
            unsigned short int tooltipWidth = tooltipSurface->w;
            unsigned short int tooltipHeight = tooltipSurface->h;
            SDL_FreeSurface(tooltipSurface);
            int mousex, mousey;
            SDL_GetMouseState(&mousex, &mousey);
            SDL_Rect tooltipRect = {x + mousex + 10, mousey, tooltipWidth, tooltipHeight};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect bgRect = {tooltipRect.x - 5, tooltipRect.y - 5, tooltipWidth + 10, tooltipHeight + 10};
            SDL_RenderFillRect(renderer, &bgRect);
            SDL_RenderCopy(renderer, tooltipTexture, nullptr, &tooltipRect);
            SDL_DestroyTexture(tooltipTexture);
        }
    }
}

void Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;
        bool inside = (mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height);

        if (inside) {
            if (!isHovered) {
                // Just entered the button
                isHovered = true;
                hoverStartTime = SDL_GetTicks();
                showTooltip = false;
            } else if (!showTooltip && SDL_GetTicks() - hoverStartTime > 250) {
                // Show tooltip after 250ms of initial hover
                showTooltip = true;
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                onClick();
            }
        } else {
            isHovered = false;
            showTooltip = false;
        }
    }
}
