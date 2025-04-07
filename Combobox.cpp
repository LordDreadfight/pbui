#include "Combobox.h"
#include <iostream>

Combobox::Combobox(short int x, short int y, unsigned short int width, unsigned short int height, const std::vector<std::string> &options)
    : x(x), y(y), width(width), height(height), options(options), selectedIndex(0), isOpen(false), font(nullptr)
{
    // Load the font
    font = TTF_OpenFont("Ubuntu-Regular.ttf", 14);
    if (!font)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

void Combobox::setOnSelectionChange(std::function<void(const std::string &)> callback)
{
    onSelectionChange = callback;
}

void Combobox::draw(SDL_Renderer *renderer)
{
    // Draw the main rectangle (closed combo box)
    SDL_Rect mainRect = {x, y, width, height};
    SDL_Color bgColor = {47, 211, 71, 255}; // Button background color
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &mainRect);

    // Draw the dropdown indicator (filled triangle)
    SDL_Color triangleColor = {0, 0, 0, 255}; // Black color for the triangle
    SDL_SetRenderDrawColor(renderer, triangleColor.r, triangleColor.g, triangleColor.b, triangleColor.a);

    // Triangle points (downward facing)
    short int triangleBaseX = x + width - 20; // Base of the triangle 20px from the right
    short int triangleBaseY = y + height / 2;
    short int triangleSize = 6; // Size of the triangle (half of the width or height)

    SDL_Point topPoint = {triangleBaseX, triangleBaseY + triangleSize};
    SDL_Point leftPoint = {triangleBaseX - triangleSize, triangleBaseY - triangleSize};
    SDL_Point rightPoint = {triangleBaseX + triangleSize, triangleBaseY - triangleSize};

    // Fill the triangle using horizontal lines
    for (int row = 0; row <= triangleSize * 2; ++row)
    {
        int y = triangleBaseY - triangleSize + row;
        int startX = triangleBaseX - (triangleSize - row / 2);
        int endX = triangleBaseX + (triangleSize - row / 2);
        SDL_RenderDrawLine(renderer, startX, y, endX, y);
    }

    // Draw the selected option text
    if (font && !options.empty())
    {
        SDL_Color textColor = {0, 0, 0, 255}; // Black text
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, options[selectedIndex].c_str(), textColor);
        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = {x + 10, y + (height - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }

    // If open, draw the dropdown
    if (isOpen)
    {
        for (size_t i = 0; i < options.size(); ++i)
        {
            int optionHeight = height; // Each option's height is the same as the main rectangle
            drawOption(renderer, i, x, y + (i + 1) * optionHeight, width, optionHeight, i == selectedIndex);
        }
    }
}

void Combobox::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (isOpen) {
            // Check if clicking inside the dropdown or the main rectangle
            bool clickedInsideMain = mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height;
            bool clickedInsideDropdown = false;

            if (!clickedInsideMain) {
                // Check each dropdown option
                for (size_t i = 0; i < options.size(); ++i) {
                    int optionY = y + (i + 1) * height;
                    if (mouseX > x && mouseX < x + width && mouseY > optionY && mouseY < optionY + height) {
                        clickedInsideDropdown = true;
                        selectedIndex = i; // Update selected index
                        isOpen = false;    // Close dropdown
                        if (onSelectionChange) {
                            onSelectionChange(options[selectedIndex]);
                        }
                        break;
                    }
                }
            }

            // If clicked outside, close the dropdown
            if (!clickedInsideMain && !clickedInsideDropdown) {
                isOpen = false;
            }
        } else {
            // If dropdown is closed, check if clicking on the main rectangle to open it
            if (mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height) {
                isOpen = true; // Open dropdown
            }
        }
    }
}


void Combobox::drawOption(SDL_Renderer *renderer, short int index, short int optionX, short int optionY, short int optionWidth, short int optionHeight, bool isHovered)
{
    SDL_Rect optionRect = {optionX, optionY, optionWidth, optionHeight};
    SDL_Color bgColor = isHovered ? SDL_Color{112, 220, 112, 255} : SDL_Color{47, 211, 71, 255};
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &optionRect);

    // Render option text
    if (font && index < options.size())
    {
        SDL_Color textColor = {0, 0, 0, 255}; // Black text
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, options[index].c_str(), textColor);
        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            short int textWidth = textSurface->w;
            short int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);
            SDL_Rect textRect = {optionX + 10, optionY + (optionHeight - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }
}
