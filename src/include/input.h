#include <SDL2/SDL.h>

class Input {
    public: 
        Input();
        ~Input();
        Uint32 getEventType();
        SDL_Keycode getKey();
        SDL_Event event;

        int xInput, yInput;
    
};