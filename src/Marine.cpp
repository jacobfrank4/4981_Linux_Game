#include "Marine.h"

Marine::Marine() : Movable(MARINE_VELOCITY) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
    printf("Create Marine\n");
}

Marine::~Marine() {
    printf("Destroy Marine\n");
}

void Marine::onCollision() {
    printf("COLISION DETECTED\n");
    // Do nothing for now
}

void Marine::collidingProjectile(int damage) {
    health = health - damage;
}

/**
* Function: getHP
*
* Date:
* JF: March 11, 2017: Created
*
* Designer:
* Jacob Frank
*
* Programmer:
* Jacob Frank
*
* Interface: getHP()
*
* Returns: The integer value of the Marine's health (0 - 100)
*
* Notes:
* function returns the current value of the health variable of the Marine
*/
int Marine::getHP() {
    return health;
}
