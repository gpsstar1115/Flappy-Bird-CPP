#include<level.h>

Level* Level::instance = nullptr;

Level::Level() {
    Level::instance = this;
}

void Level::createPipe() {
    
    SDL_Texture *pipeImage = loadTexture(renderer, "images/pipe.png");
    SDL_Texture *upperPipeImage = loadTexture(renderer, "images/topPipe.png");

    Pipe *pipe = new Pipe(pipeImage, upperPipeImage);
    pipe->createRandomPipe();

    if(pipeQueue.size() >= queueMaxSize) {
        pipeQueue.pop();
    }
    pipeQueue.push(pipe);
}

void Level::createClouds() {
    this->cloud1 = new Cloud(loadTexture(renderer, "images/cloud.png"), {300, 200, 128, 128});
    this->cloud2 = new Cloud(loadTexture(renderer, "images/cloud.png"), {800, 400, 200, 200});
}

void Level::updateLevel(float deltaTime) {

    this->cloud1->moveCloud(deltaTime);
    this->cloud1->renderCloud(renderer);

    this->cloud2->moveCloud(deltaTime);
    this->cloud2->renderCloud(renderer); 


    std::queue<Pipe*> pipeQueueCopy = pipeQueue;

    while (!pipeQueueCopy.empty()) {
        Pipe* frontPipe = pipeQueueCopy.front();
        frontPipe->movePipe(deltaTime);
        SDL_RenderCopy(renderer, frontPipe->texture, nullptr, &(frontPipe->destinationRect));
        SDL_RenderCopy(renderer, frontPipe->upperPipeTexture, nullptr, &(frontPipe->upperDestinationRect));
        pipeQueueCopy.pop();
    }

    
    

}




SDL_Texture* Level::loadTexture(SDL_Renderer* renderer, const std::string& imagePath) {
    // Load image at specified path
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (surface == nullptr) {
        printf("Error loading image: %s\n", IMG_GetError());
        return nullptr;
    }

    // Create texture from surface pixels
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        printf("Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return nullptr;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(surface);

    return texture;
}




