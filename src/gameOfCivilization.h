#ifndef __GAME_OF_CIVILIZATION_H__
#define __GAME_OF_CIVILIZATION_H__

class Game {

public:

    //virtual Game(uint32_t width, uint32_t height, char* filename) { };
    //virtual ~Game() { };

    virtual void printGrid() = 0;

    virtual void advanceGame() = 0;

    //virtual void render() = 0;


    //virtual void dumpParticles(const char* filename) {}

};


#endif
