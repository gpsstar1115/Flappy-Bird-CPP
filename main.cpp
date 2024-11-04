
#include<main.h>

float x_pos, y_pos, dx;

SDL_Renderer *renderer;
SDL_Texture* playerSprite;
Game *game;

Uint32 previousTimeMillis;


void printCredits() {
    std::cout << "  GGG   PPPP   SSS \n";
    std::cout << " G   G  P   P  S    \n";
    std::cout << " G      P   P   SS  \n";
    std::cout << " G  GG  PPPP     S  \n";
    std::cout << " G   G  P         S  \n";
    std::cout << "  GGG   P      SSS  \n";
}

int main(int argc, char *argv[]) {
    printf(" Made By -  \n\n");
    printCredits();
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    } // else ->

    SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0);

    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    Level::instance = nullptr;
    Level *level = new Level();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        destroy(window);
        return 1;
    }

    level->renderer = renderer;

    game = new Game();
    start(renderer);

    playerSprite = level->loadTexture(renderer, "images/flappy.png");
    if (playerSprite == nullptr) {
        // Texture loading failed
        // Handle error
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    game->setPlayerSprite(playerSprite);

    bool running = true;
    while(running){
       // SDL_Event event;
        while(SDL_PollEvent(&(game->input->event))){
            switch(game->input->event.type){
                case SDL_QUIT:
                    running = false;
                    printf("Quit Event");
                    break;
                
                case SDL_KEYDOWN: 
                     switch(game->input->getKey()) {
                        case SDLK_w:
                        case SDLK_SPACE: game->input->yInput = 1; break;

                        } 
                        game->isPlayerActive = true;
                    break;

                case SDL_KEYUP: 
                     switch(game->input->getKey()) {
                        case SDLK_w:
                        case SDLK_SPACE: game->input->yInput = 0; break;
                    } 
                    break;

                case SDL_MOUSEBUTTONDOWN: 
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // Check if the mouse click is within the "Play Again" button area
                    if (mouseX >= game->playAgain->destinationRect.x && mouseX <= game->playAgain->destinationRect.x + game->playAgain->destinationRect.w
                        && mouseY >= game->playAgain->destinationRect.y && mouseY <= game->playAgain->destinationRect.y + game->playAgain->destinationRect.h) {
                        game->isPlayAgainClicked = true;
                    }

                    

                default:
                    break;
            }
        }
        

        int error = update(window);

        if(error != 0) {
            destroy(window);
            return error;
        }

        SDL_GL_SwapWindow(window);
        
    }

    destroy(window);

    return 0;
}

void start(SDL_Renderer *renderer) {
    x_pos = 0;
    y_pos = 0;
    dx = 1;

    previousTimeMillis = SDL_GetTicks();

    

    SDL_SetRenderDrawColor(renderer, 78, 186, 196, 255);
    // Clear the window with the background color
    SDL_RenderClear(renderer);

    game->start(renderer);
    
    // Present the renderer
    SDL_RenderPresent(renderer);
}

int update(SDL_Window *window) {
    Uint32 currentTimeMillies = SDL_GetTicks();
    
    float deltaTime = (currentTimeMillies - previousTimeMillis)/1000.0f; // millies to seconds conversion by divide by 1000
    if(deltaTime > 1) { printf("deltaTime is too high deltaTime = %f", deltaTime); deltaTime = 0.001;  } // sometimes previosTimeMillies can take longer, in first frame, causing deltaTime to have a very large value, so fixing this by limited for the first time only to make sure nothing go too fast in game
    previousTimeMillis = currentTimeMillies;

    float move_speed = 50;

    x_pos += dx * deltaTime * move_speed;

    game->update(renderer, deltaTime);

    return 0;
}

void destroy(SDL_Window *window) {
    if(renderer) SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();     
}

