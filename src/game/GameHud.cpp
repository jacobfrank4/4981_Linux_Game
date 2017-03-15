#include "GameHud.h"

GameHud::GameHud(): inventorySlotOpacity(0){}

void GameHud::renderRectangle(SDL_Rect *position, SDL_Renderer* renderer, const size_t rgb[3], size_t opacity) {

    for (size_t i = 0; i < 3; ++i) {
        if (rgb[i] < 0 || rgb[i] > 255) {
            printf("rgb[%zu] value is invalid (less than 0 or greater than 255): %zu", i, rgb[i]);
            return;
        }
    }

    if (opacity < 0 || opacity > 255) {
        printf("Opacity value is invalid (less than 0 or greater than 255): %zu", opacity);
        return;
    }

    SDL_SetRenderDrawColor(renderer, rgb[0], rgb[1], rgb[2], opacity);
    SDL_RenderFillRect(renderer, position);
}

void GameHud::setRectPosition(SDL_Rect *rect, size_t x, size_t y) {
    rect->x = x;
    rect->y = y;
}

void GameHud::setRectSize(SDL_Rect *rect, size_t w, size_t h) {
    rect->w = w;
    rect->h = h;
}

SDL_Rect* GameHud::getHealthbarBackground() {
    return &healthbarBackground;
}

SDL_Rect* GameHud::getHealthbarForeground() {
    return &healthbarForeground;
}

SDL_Rect* GameHud::getAmmoBackground() {
    return &ammoBackground;
}

SDL_Rect* GameHud::getAmmoForeground() {
    return &ammoForeground;
}

SDL_Rect* GameHud::getAmmoClip() {
    return &ammoClip;
}

LTexture* GameHud::getAmmoClipTexture() {
    return &ammoClipTexture;
}

LTexture* GameHud::getActiveWeaponSlotTexture() {
    return &activeWeponSlotTexture;
}

LTexture* GameHud::getWeaponSlotTexture() {
    return &weponSlotTexture;
}

LTexture* GameHud::getConsumableSlotTexture() {
    return &consumableSlotTexture;
}

LTexture* GameHud::getEquippedWeaponTexture() {
    return &equippedWeaponTexture;
}

int GameHud::getOpacity() {
    return inventorySlotOpacity;
}

void GameHud::decrementOpacity(Uint8 amount) {
    inventorySlotOpacity -= amount;
}

void GameHud::setOpacity(Uint8 newOpacity) {
    inventorySlotOpacity = newOpacity;
}

void GameHud::renderWeaponInventory(int activeWeaponSlot, int weaponSlotPosition[3][2], SDL_Renderer* renderer) {
    switch (activeWeaponSlot) {
        case 0:
            getWeaponSlotTexture()->render(renderer, weaponSlotPosition[0][0], weaponSlotPosition[0][1]);
            getWeaponSlotTexture()->render(renderer, weaponSlotPosition[1][0], weaponSlotPosition[1][1]);
            getActiveWeaponSlotTexture()->render(renderer, weaponSlotPosition[2][0], weaponSlotPosition[2][1]);
            break;
        case 1:
            getWeaponSlotTexture()->render(renderer, weaponSlotPosition[0][0], weaponSlotPosition[0][1]);
            getActiveWeaponSlotTexture()->render(renderer, weaponSlotPosition[1][0], weaponSlotPosition[1][1]);
            getWeaponSlotTexture()->render(renderer, weaponSlotPosition[2][0], weaponSlotPosition[2][1]);
            break;
        case 2:
            getActiveWeaponSlotTexture()->render(renderer, weaponSlotPosition[0][0], weaponSlotPosition[0][1]);
            getWeaponSlotTexture()->render(renderer, weaponSlotPosition[1][0], weaponSlotPosition[1][1]);
            getWeaponSlotTexture()->render(renderer, weaponSlotPosition[2][0], weaponSlotPosition[2][1]);
            break;
        default:
            break;
    }
}

GameHud::~GameHud(){}
