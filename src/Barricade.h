#ifndef BARRICADE_H
#define BARRICADE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Object.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "Window.h"

constexpr int BARRICADE_WIDTH = 100;
constexpr int BARRICADE_HEIGHT = 100;

class Barricade : public Object {
public:
    Barricade(int32_t nid, const SDL_Rect dest, const SDL_Rect &spriteSize, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize, int health = 100, int state = 0, bool placeable = false, bool placed = false);
    virtual ~Barricade();

    void move(const float, const float, const float, const float, CollisionHandler&); // Moves Zombie
    void onCollision();
    void collidingProjectile(const int damage);
    bool isPlaceable();
    bool isPlaced();
    bool checkPlaceablePosition(const float,const float,const float,const float, CollisionHandler&);
    void placeBarricade();

private:
    int health;
    int state; //used to select sprite to display
    bool placeable;
    bool placed;
};

#endif
