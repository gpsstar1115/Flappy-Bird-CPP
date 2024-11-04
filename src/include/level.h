#include<iostream>
#include <queue>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

struct Pipe {
    SDL_Texture *texture;
    SDL_Texture *upperPipeTexture;

    SDL_Rect destinationRect;
    SDL_Rect upperDestinationRect;
    
    float xVelocity = 400;
    float xPos = 0;  // actually destinationRect.x is type of x, so won't work updating that gradually

    public: 

        Pipe(SDL_Texture *texture, SDL_Texture *upperPipe) {
            this->texture = texture;
            this->upperPipeTexture = upperPipe;
        }

        Pipe(SDL_Texture *texture, SDL_Rect destinationRect) {
            this->texture = texture;
            this->destinationRect = destinationRect;
        }

        void createRandomPipe() {
            int screenWidth = 1280;
            int screenHeight = 720;

            int gap = 200;

            this->xPos = screenWidth - 100;
            
            int maxSize = 500;
            int minSize = 200;
            
            // Generate a random number in the range 200 to 500
            int yPos = rand() % (maxSize - minSize + 1) + minSize;


            this->destinationRect = {static_cast<int>(this->xPos), screenHeight - yPos , 64, yPos};

            this->upperDestinationRect = {static_cast<int>(this->xPos), 0 , 64, screenHeight - destinationRect.h - gap};
            
        }

        void movePipe(float deltaTime) {
            this->xPos -= xVelocity * deltaTime;
            this->destinationRect.x = static_cast<int>(this->xPos);
            this->upperDestinationRect.x = destinationRect.x;
        }

       
};

struct Cloud {
    SDL_Texture *texture;
    SDL_Rect destinationRect;

    float xVelocity = 300;
    float xPos = 0; 

    Cloud(SDL_Texture *texture, SDL_Rect destinationRect) {
        this->texture = texture;
        this->destinationRect = destinationRect;

        this->xPos = destinationRect.x;
    }

    void moveCloud(float deltaTime) {
        this->xPos -= xVelocity * deltaTime;
        this->destinationRect.x = static_cast<int>(this->xPos);

        if(this->destinationRect.x + this->destinationRect.w < 0) {
            respawnCloud();
        }
    }

    void respawnCloud() {
        this->destinationRect.x = 1280;
        this->xPos = this->destinationRect.x;
        this->destinationRect.y = getRandomIntInRange(100, 640);
        this->destinationRect.w = getRandomIntInRange(64, 200);
        this->destinationRect.h = this->destinationRect.w;
    }

    void renderCloud(SDL_Renderer *renderer) {
         SDL_RenderCopy(renderer, texture, nullptr, &destinationRect);
    }

    int getRandomIntInRange(int min, int max) {
        return rand() % (max - min + 1) + min;
    }
};




class Level {
    public: 
        static Level *instance;
        Level();
        SDL_Renderer *renderer;
        void createPipe();
        void updateLevel(float deltaTime);
        SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& imagePath);
        void createRoad();
        std::queue<Pipe*> pipeQueue;
        void createClouds();

    private:
        int ySpeed;
        struct Pipe *pipe;
        int queueMaxSize = 3;
        struct Cloud *cloud1, *cloud2;
        
        
};


