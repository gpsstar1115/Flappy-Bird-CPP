#ifndef Game_h
#define Game_h


#include<level.h>
#include<input.h>

struct Text {
    SDL_Texture *texture;
    SDL_Rect destinationRect;

    Text(SDL_Texture *texture, SDL_Rect destinationRect) {
        this->texture = texture;
        this->destinationRect = destinationRect;
    }
};

struct Button {
    SDL_Rect destinationRect;
    Text *text;

    Button(SDL_Rect destinationRect, Text *text) {
        this->destinationRect = destinationRect;
        this->text = text;
    }

    void renderButton(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 216, 241, 132, 255); // Green color
        SDL_RenderFillRect(renderer, &destinationRect);

        SDL_RenderCopy(renderer, text->texture, nullptr, &(text->destinationRect));
    }
};

class Game {
    public: 
        bool isPlayerActive = false; // is player pressed a key to start the game, we do not direct apply gravity to fall
        bool isGameOver = false;
        bool isPlayAgainClicked = false;
        static const int SCREEN_WIDTH = 1280;
        static const int SCREEN_HEIGHT = 720;
        Game();
        ~Game();
        void setPlayerSprite(SDL_Texture *texture);
        void resetGame(SDL_Renderer *renderer);
        void start(SDL_Renderer *renderer);
        void update(SDL_Renderer *renderer, float deltaTime);
        void gravityFall(float deltaTime);
        void isPlayerColliding();
        Input *input;
        Text *gameOverText;
        Text *scoreText;
        Button *playAgain;

        TTF_Font *gameOverFont;
        TTF_Font *scoreFont;

        void initFont();
        Text* createText(SDL_Renderer* renderer, TTF_Font *font, const std::string& text,  int x, int y, SDL_Color color);
        bool handleInput(float deltaTime);

        int gameScore;

        void updateScoreAndText(SDL_Renderer* renderer, int setScore) {
            gameScore = setScore;
            std::string score = "Score  " + std::to_string(gameScore);
            scoreText = createText(renderer, scoreFont, score, 100, 100, {255, 255, 255});
        }

    private:
        struct Player *player;

        float gameTimer = 0;
        float pipeSpawnTime = 2;
        float pipeSpawnCurrentTime = pipeSpawnTime;
        
};

struct Player {
    SDL_Texture *texture;
    float yPos;
    float yVelocity;

    float jumpSpeed = 10;
    float powerBoost = 10;

    SDL_Rect destinationRect;

    Player(SDL_Texture *texture, SDL_Rect destinationRect) {
        this->texture = texture;
        this->destinationRect = destinationRect;
        this->yPos = destinationRect.y;
    }

    void updateYposition() {
        yPos += yVelocity * 0.05;  
    }

    void updateDestinationRect(){  // as flappy's only positino change in y axis
        this->destinationRect.y = static_cast<int>(yPos);
    }

};

#endif /* Game_h */