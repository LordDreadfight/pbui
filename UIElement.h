#pragma once
#include <SDL2/SDL.h>

struct UIElement{
	virtual ~UIElement() = default;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual void handleEvent(const SDL_Event& event) = 0;
};
