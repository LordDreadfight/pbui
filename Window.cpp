#include "Window.h"
#include "UIElement.h"

Window::Window(const char* title, unsigned short int width, unsigned short int height) {
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    running = true;
    needsRender = true;
}

Window::~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::addElement(UIElement* element) {
    elements.push_back(element);
    needsRender = true;
}

void Window::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                onResize(event.window.data1, event.window.data2);
            }
            needsRender = true;
        }

        for (auto& element : elements) {
            element->handleEvent(event);
            needsRender = true;
        }
    }
}

void Window::render() {
    if (!needsRender) return;

    SDL_SetRenderDrawColor(renderer, 36, 36, 36, 255);
    SDL_RenderClear(renderer);
    for (auto& element : elements) {
        element->draw(renderer);
    }

    SDL_RenderPresent(renderer);

    needsRender = false;
}

void Window::onResize(unsigned short int width, unsigned short int height) {
    SDL_RenderSetLogicalSize(renderer, width, height);
    needsRender = true;
}

void Window::mainLoop() {
    while (running) {
        handleEvents();
        render();
        SDL_Delay(1);
    }
}
