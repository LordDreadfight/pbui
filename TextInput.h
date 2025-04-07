#pragma once
#include "UIElement.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct TextInput : public UIElement {
    // Position and size
    short int x, y;
    unsigned short int width, height;

    // Text management
    std::string text;
    std::string placeholder;
    TTF_Font* font;

    // Caret and selection state
    int caretPos;            // Current caret position
    int selectionStart;      // Start of text selection
    int selectionEnd;        // End of text selection

    // Focus and caret blinking
    bool isFocused;          // Whether the input is focused
    bool showCaret;          // Whether to render the caret
    Uint32 lastBlinkTime;    // Tracks the last caret blink timestamp

    // Constructor
    TextInput(short int x, short int y, unsigned short int width, unsigned short int height);

    // UIElement interface overrides
    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event) override;

    // Custom methods
    void setPlaceholder(const std::string& placeholderText);

private:
    int getClosestCharIndex(int mouseX) const;  // Calculates caret position based on mouse click
};
