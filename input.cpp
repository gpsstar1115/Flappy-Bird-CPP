
#include<input.h>

Input::Input() {

}

Input::~Input() {
    
}

Uint32 Input::getEventType() {
    return event.type;
}

SDL_Keycode Input::getKey() {
    return event.key.keysym.sym;
}