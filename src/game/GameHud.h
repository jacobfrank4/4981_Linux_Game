#ifndef GAME_HUD_H
#define GAME_HUD_H
#include "../sprites/LTexture.h"
class GameHud {

public:

    GameHud();
    virtual ~GameHud();

    void renderRectangle(SDL_Rect *position, SDL_Renderer *renderer, const size_t rgb[3], size_t opacity);
    void setRectPosition(SDL_Rect *rect, size_t x, size_t y);
    void setRectSize(SDL_Rect *rect, size_t w, size_t h);
    SDL_Rect *getHealthbarBackground();
    SDL_Rect *getHealthbarForeground();
    SDL_Rect *getAmmoBackground();
    SDL_Rect *getAmmoForeground();
    SDL_Rect *getAmmoClip();
    LTexture *getAmmoClipTexture();
    LTexture *getActiveWeaponSlotTexture();
    LTexture *getEquippedWeaponTexture();
    LTexture *getWeaponSlotTexture();
    LTexture *getConsumableSlotTexture();
    int getOpacity();
    void decrementOpacity(Uint8 amount);
    void setOpacity(Uint8 opacity);
    void renderWeaponInventory(int activeWeaponSlot, int weaponSlotPosition[3][2], SDL_Renderer* renderer);

private:

    Uint8 inventorySlotOpacity;

    SDL_Rect healthbarBackground;
    SDL_Rect healthbarForeground;

    SDL_Rect ammoBackground;
    SDL_Rect ammoForeground;
    LTexture ammoClipTexture;
    SDL_Rect ammoClip;

    LTexture equippedWeaponTexture;
    LTexture activeWeponSlotTexture;
    LTexture weponSlotTexture;
    LTexture consumableSlotTexture;
};
#endif
