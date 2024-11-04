#include<game.h>


Game::Game() {

}

Game::~Game() {
    
}

// Definition for setPlayerSprite function
void Game::setPlayerSprite(SDL_Texture* texture) {

    player = new Player(texture, {100, Game::SCREEN_HEIGHT/2, 64, 64});

    /* this->playerSprite = texture;
    this->xPos = 100;
    this->yPos = Game::SCREEN_HEIGHT/2;

    this->yVelocity = 0;
    this->jumpSpeed = 3;
    this->powerBoost = 5; */

    this->input = new Input();
}

void Game::initFont() {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    gameOverFont  = TTF_OpenFont("ARCADECLASSIC.TTF", 128); // Change the path and size as needed
    if (gameOverFont == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    scoreFont = TTF_OpenFont("ARCADECLASSIC.TTF", 32); // Change the path and size as needed
    if (scoreFont == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }
}

Text* Game::createText(SDL_Renderer* renderer, TTF_Font *font, const std::string& text, int x, int y, SDL_Color color) {
   
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        std::cerr << "Failed to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return nullptr;
    }

    SDL_Rect destinationRect = {x - surface->w/2, y - surface->h/2, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &destinationRect);

    SDL_FreeSurface(surface);

    return new Text(texture, destinationRect);
}

void Game::resetGame(SDL_Renderer *renderer) {
    // clearing pipes
    while(!Level::instance->pipeQueue.empty()) {
        Level::instance->pipeQueue.pop();
    }
    
    isGameOver = false;
    isPlayerActive = false;
    this->player->yVelocity = 0;
    this->player->destinationRect = {100, Game::SCREEN_HEIGHT/2, 64, 64};
    gameScore = 0;
    gameTimer = 0;
    pipeSpawnCurrentTime = pipeSpawnTime;
    isPlayAgainClicked = false;
}

void Game::start(SDL_Renderer *renderer) {
    Level::instance->createPipe();
    Level::instance->createClouds();
    
    // TTF for Game Over Text
    initFont();
    
    gameOverText = createText(renderer, gameOverFont,  "Game Over", Game::SCREEN_WIDTH/2, Game::SCREEN_HEIGHT/2, {240, 175, 53});
    SDL_RenderCopy(renderer, gameOverText->texture, nullptr, &(gameOverText->destinationRect));

    gameScore = 0;
    scoreText = createText(renderer, scoreFont, "Score: " + gameScore, 100, 100, {255, 255, 255});
    SDL_RenderCopy(renderer, scoreText->texture, nullptr, &(scoreText->destinationRect));

    SDL_Rect playAgainRect = {Game::SCREEN_WIDTH/2 - 100, Game::SCREEN_HEIGHT/2 + 100, 200, 50};
    Text *playAgainText =  createText(renderer, scoreFont, "Play Again", playAgainRect.x + 100, playAgainRect.y + 25, {29, 59, 13});
    playAgain = new Button(playAgainRect, playAgainText);
}

// Definition for update function
void Game::update(SDL_Renderer *renderer, float deltaTime) {
    

    gameTimer += deltaTime;
    pipeSpawnCurrentTime -= deltaTime;

    SDL_SetRenderDrawColor(renderer, 78, 186, 196, 255);
    // Rendering the renderer and sprites------->>>>>> 
    SDL_RenderClear(renderer);  // all rendering between RenderClear and RenderPresent


// -------------------------- Game Over ------------------------
    if(isGameOver) {
        SDL_RenderCopy(renderer, gameOverText->texture, nullptr, &(gameOverText->destinationRect));
        
        if(isPlayAgainClicked) {
            resetGame(renderer);
            isGameOver = false;
        }
        // Render "Play Again" button
        // draw bg first

        playAgain->renderButton(renderer);

        SDL_RenderPresent(renderer);

        
        return;
    }

// -------------------------------------------------------------
    
    Level::instance->updateLevel(deltaTime);
    if(static_cast<int>(pipeSpawnCurrentTime) == 0) {
        pipeSpawnCurrentTime = pipeSpawnTime;
        Level::instance->createPipe();
    }

    bool isPowerBoost=  this->handleInput(deltaTime);
    if(!isPowerBoost)
        gravityFall(deltaTime);

    //printf("yVelocity = %f \n", this->player->yVelocity);
    SDL_Delay(1);
    

// ---------------- Rendering ------------------------------------------------
    this->player->updateYposition();
    this->player->updateDestinationRect();
    
    updateScoreAndText(renderer, static_cast<int>(gameTimer));
    SDL_RenderCopy(renderer, scoreText->texture, nullptr, &(scoreText->destinationRect));
    SDL_RenderCopy(renderer, this->player->texture, nullptr, &(this->player->destinationRect));
    SDL_RenderPresent(renderer);

    isPlayerColliding();
    // check for fall and going above ... y bounds check. this.player->yPos < 0 || this.player->yPos > screenHeight 
    // or in player class create method -> CheckPlayerYOutOfBounds();
}

float gravity = 9.8f * 8;
void Game::gravityFall(float deltaTime) {
    if(!isPlayerActive) return;
    this->player->yVelocity = this->player->yVelocity + gravity * deltaTime;
    
}

void Game::isPlayerColliding() {
   
    std::queue<Pipe*> pipeQueueCopy = Level::instance->pipeQueue;

    while (!pipeQueueCopy.empty()) {
        Pipe* frontPipe = pipeQueueCopy.front();
        if(SDL_HasIntersection(&(this->player->destinationRect), &(frontPipe->destinationRect)) 
            || SDL_HasIntersection(&(this->player->destinationRect), &(frontPipe->upperDestinationRect))) {
            // game over;
            isGameOver = true;
            isPlayerActive = false;
            return;
        }
        pipeQueueCopy.pop();
    }
}

bool Game::handleInput(float deltaTime) {
    int yInput;
    yInput = input->yInput;
    bool isPowerBoost = false;
    if(yInput != 0) {
        if(this->player->yVelocity < 0) {
            // maybe player can be keeping pressing up, so giving a powerboost overtime
            this->player->yVelocity -= this->player->powerBoost * deltaTime;
            isPowerBoost = true;
        } else {
            this->player->yVelocity = -this->player->jumpSpeed;
            isPowerBoost = false;
        }
    } 
    return isPowerBoost;
}
