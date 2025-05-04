#include <iostream>
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "TextInput.h"
#include "Combobox.h"
#include "Listview.h"

using std::cout;
using std::cin;

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    Window window("Playground ui showcaseOllama Messenger", 1280, 720);
    Button startBtn(10, 10, 150, 40, "Push it to the limit");
    startBtn.setOnClick([]() {});
    startBtn.setTooltip("this is a tooltip");
    std::vector<std::string> itemslist = {"test","stest","test3"};
    Combobox combi(170, 10, 250, 40, itemslist);
    TextInput textinput(260, 670, 800, 40);
    textinput.setPlaceholder("This is a placeholder text");
    
    Button sendBtn(1080, 670, 100, 40, "Send");
    Button settingsBtn(10, 670, 150, 40, "Settings");
    ListView listview(10, 60, 250, 400, 20);
    listview.addItem("test");
    listview.addItem("stest");
    listview.addItem("test3");


    window.addElement(&listview);
    window.addElement(&combi);
    window.addElement(&textinput);
    window.addElement(&startBtn);
    window.addElement(&sendBtn);
    window.addElement(&settingsBtn);
    window.mainLoop();    
    TTF_Quit();
    SDL_Quit();

	return 0;
}
