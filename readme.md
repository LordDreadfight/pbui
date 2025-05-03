Todo list:


-checkbox
-item visibility (visible, hidden, collapsed)
-display priority (combobox dropdown and item laying over each other)
-semi complete code review
-tooltip delay
-iconbutton
-radiobuttons
-toggle switch
-slider
-tableviews
-messagebox
-progressbar


Tutorial on how to use:
This project uses libsdl2, because of licensing reasons with libsdl3, but the main reason is that i didn't get libsdl3 to work within an afternoon
otherwise this project would have been on sdl3.
the base class for all ui elements is UIElement.h, who could've seen that coming. it just initializes two functions draw, and handleevent. the next thing is window, it is a class that renders a window, it has a few functions like addelement or mainloop,  it also has other logic for window rendering on resize or other window updates. the window is only updated when it sees that the states of one of its elements requires it. next is the button, it has a position defined with x and y, width with width and height with height. the button has also a button text, it has a tooltip text too, and the most basic onclick to make something happen when triggered. then i have the combobox, it has basic properties like width, height, x and y too and it has some neat functions like selected index, on selection change, and a vector with options. the label item is very basic it only has x, y, width and height again, it also has a text to make it a label. all ui items of mine have a color theme that is not (easily) changeable (for now). that also applies to the font of the ui lib. i made the concious decision to name all of my items 'views', every language calls it differenlty and i just think that view is the most straight forward. Listview has many functions, partly due to it's nature. i have an additem function, itemselected  function, draw and drawitem function that are more backend related. also you can specify item height besides the usual width, height, x and y. the last item i have right now is the textinput element,it has some fancy caret position stuff, supports placeholders  but is unremarkable otherwise. additionaly i want to prefix that if you have your main function in your main.cpp, you have to be careful because the order in which items are initialized is the priority they are going to have. i.e. if you have a combobox in the top of your window, you might want to init it before the button below it because the combobox list might clip below that button because of the layering.