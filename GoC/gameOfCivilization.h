#ifndef __GAME_OF_CIVILIZATION_H__
#define __GAME_OF_CIVILIZATION_H__

class GoC {

public:

    //virtual void setup();

    virtual void printGrid() = 0;

    virtual void advanceGame() = 0;

    virtual void render() = 0;

};


#endif
