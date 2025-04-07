#pragma once
#include <SDL2/SDL.h>
#include <vector>

class UIElement;

class Window {
public:
    Window(const char* title, unsigned short int width, unsigned short int height);
    ~Window();

    void addElement(UIElement* element);
    void mainLoop();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    bool needsRender;
    std::vector<UIElement*> elements;

    void handleEvents();
    void render();
    void onResize(unsigned short int width, unsigned short int height);
};
