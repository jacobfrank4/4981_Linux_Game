#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include "GameStateMatch.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Colors.h"
#include "Window.h"

//Modulation component
Uint8 a = 255;

GameStateMatch::GameStateMatch(Game& g,  int gameWidth, int gameHeight) : GameState(g), player(),
                               level(),  base(), camera(gameWidth,gameHeight), hud(){

}

bool GameStateMatch::load() {

    bool success = true;

    //Open the font
    frameFont = TTF_OpenFont( "assets/fonts/kenpixelsquare.ttf", 28 );
    if ( frameFont == NULL ) {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //level = new Level();
    if (!level.levelTexture.loadFromFile("assets/texture/checkerboard.png", game.renderer)) {
        printf("Failed to load the level texture!\n");
        success = false;
    } else {
        level.levelTexture.setDimensions(2000, 2000);
    }

    unsigned int playerMarineID = GameManager::instance()->createMarine();

    // Create Dummy Entitys
    GameManager::instance()->createMarine(game.renderer, 1500, 1500);
    GameManager::instance()->createZombie(game.renderer, 100, 100);
    GameManager::instance()->createZombie(game.renderer, 700, 700);
    GameManager::instance()->createTurret(game.renderer, 1000, 500);
    GameManager::instance()->createWeaponDrop(game.renderer, 1800, 1700);


    //base = Base();
    if (!base.texture.loadFromFile("assets/texture/base.png", game.renderer)) {
        printf("Failed to load the base texture!\n");
        success = false;
    }
    GameManager::instance()->addObject(base);
    Point newPoint = base.getSpawnPoint();

    //player = new Player();
    player.setControl(GameManager::instance()->getMarine(playerMarineID));
    player.marine->setPosition(newPoint.first, newPoint.second);

    if (!player.marine->texture.loadFromFile("assets/texture/arrow.png", game.renderer)) {
        printf("Failed to load the player texture!\n");
        success = false;
    }

    hud.getAmmoClipTexture()->loadFromFile("assets/hud/weaponClip.png", game.renderer);
    hud.getActiveWeaponSlotTexture()->loadFromFile("assets/hud/activeWeaponSlot.png", game.renderer);
    hud.getWeaponSlotTexture()->loadFromFile("assets/hud/weaponSlot.png", game.renderer);
    hud.getEquippedWeaponTexture()->loadFromFile("assets/hud/activeWeaponSlot.png", game.renderer);
    hud.getConsumableSlotTexture()->loadFromFile("assets/hud/consumableSlot.png", game.renderer);

    hud.getActiveWeaponSlotTexture()->setBlendMode(SDL_BLENDMODE_BLEND);
    hud.getWeaponSlotTexture()->setBlendMode(SDL_BLENDMODE_BLEND);


    //camera = Camera(game.window.getWidth(), game.window.getHeight());


    return success;
}

void GameStateMatch::loop() {
    //The frames per second timer
    LTimer fpsTimer;

    //The frames per second cap timer
    LTimer capTimer;

    //Keeps track of time between steps
    LTimer stepTimer;

    //Start counting frames per second
    unsigned long countedFrames = 0;
    float avgFPS = 0;
    fpsTimer.start();

    // State Loop
    while (play) {
        //Start cap timer
        capTimer.start();

        //Calculate and correct fps
        avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );

        //Set FPS text to be rendered
        frameTimeText.str( "" );
        frameTimeText << std::fixed << std::setprecision(0) << "FPS: " << avgFPS;

        // Process frame
        handle();    // Handle user input
        update(stepTimer.getTicks() / 1000.f); // Update state values
        stepTimer.start(); //Restart step timer
        sync();    // Sync game to server
        render();    // Render game state to window

        ++countedFrames;

        //If frame finished early
        int frameTicks = capTimer.getTicks();
        if ( frameTicks < SCREEN_TICK_PER_FRAME ) {
            //Wait remaining time
            SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
        }

    }
}

void GameStateMatch::sync() {

}

void GameStateMatch::handle() {
    const Uint8 *state = SDL_GetKeyboardState(NULL); // Keyboard state
    // Handle movement input
    player.handleKeyboardInput(state);
    player.handleMouseUpdate(game.window, camera.getX(), camera.getY());
    //Handle events on queue
    while ( SDL_PollEvent( &event )) {
        game.window.handleEvent(event);
           switch( event.type ) {
        case SDL_WINDOWEVENT:
            camera.setViewSize(game.window.getWidth(), game.window.getHeight());
            break;
        case SDL_MOUSEWHEEL:
            player.handleMouseWheelInput(&(event));
            hud.setOpacity(OPAQUE);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_RIGHT) {
                player.handlePlacementClick(game.renderer);
            }
            break;
        case SDL_KEYDOWN:
            switch( this->event.key.keysym.sym ) {
                case SDLK_ESCAPE:
                    play = false;
                    break;
                case SDLK_b:
                    player.handleTempBarricade(game.renderer);
                    break;
                case SDLK_1: //Purposeful flow through
                case SDLK_2:
                case SDLK_3:
                    hud.setOpacity(OPAQUE);
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch( event.key.keysym.sym ) {
                default:
                   break;
            }
            break;
        case SDL_QUIT:
            play = false;
            break;
        default:
            break;
        }
    }
}

void GameStateMatch::update(const float& delta) {
    GameManager::instance()->updateCollider();

    // Move player
    GameManager::instance()->updateMarines(delta);
    GameManager::instance()->updateZombies(delta);

    // Move Camera
    camera.move(player.marine->getX(), player.marine->getY());


}

void GameStateMatch::render() {
    //Only draw when not minimized
    if ( !game.window.isMinimized() ) {

        //Clear screen
        SDL_SetRenderDrawColor( game.renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( game.renderer );

        //Render textures
        level.levelTexture.render(game.renderer, 0-camera.getX(), 0-camera.getY());

        //renders objects in game
        GameManager::instance()->renderObjects(game.renderer, camera.getX(), camera.getY());

        SDL_Color textColor = { 0, 0, 0, 255 };

        //Render text
        if ( !frameFPSTextTexture.loadFromRenderedText( frameTimeText.str().c_str(),
                textColor, game.renderer, frameFont ) ) {
            printf( "Unable to render FPS texture!\n" );
        }

        frameFPSTextTexture.render(game.renderer,
            (game.window.getWidth() - frameFPSTextTexture.getWidth()), 0);

        //**************************************************DISPLAY THE HUD********************************************//

        int windowHeight = game.window.getHeight();
        int windowWidth = game.window.getWidth();

        //**************************************************DISPLAY THE HEALTHBAR********************************************//
        float HP = player.marine->getHP();

        size_t rgb[3] = {0};
        if (HP > 0) {
            rgb[0] = static_cast<size_t>(std::round((1 - (HP / 100)) * 255));
            rgb[1] = static_cast<size_t>(HP / 100 * 255);
            rgb[2] = 0;

            hud.getHealthbarBackground()->w = MARINE_WIDTH * 2;
            hud.getHealthbarForeground()->w = static_cast<size_t>(HP / 100 * hud.getHealthbarBackground()->w) - 10;
        }

        SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);

        hud.getHealthbarBackground()->h = 30;
        hud.getHealthbarForeground()->h = hud.getHealthbarBackground()->h - 10;
        hud.setRectPosition(hud.getHealthbarBackground(),
                            player.marine->getX() - camera.getX() - hud.getHealthbarBackground()->w / 2 + MARINE_WIDTH / 2,
                            player.marine->getY() - camera.getY() - hud.getHealthbarBackground()->h - 10);

        hud.setRectPosition(hud.getHealthbarForeground(),
                            player.marine->getX() - camera.getX() - hud.getHealthbarForeground()->w / 2 + MARINE_WIDTH / 2,
                            player.marine->getY() - camera.getY() - hud.getHealthbarForeground()->h - 15);

        hud.renderRectangle(hud.getHealthbarBackground(), game.renderer, BLACK_RGB, 150);
        hud.renderRectangle(hud.getHealthbarForeground(), game.renderer, rgb, 150);

        //**************************************************DISPLAY THE HEALTHBAR********************************************//


        //**************************************************DISPLAY THE AMMOBAR********************************************//

//        int bulletsLeft = player.marine->inventory.getCurrentWeapon().getClip(); // Use once a player can pick up a gun and shoot it.

        int clipWidth = windowWidth / 30;
        int clipHeight = windowWidth / 7.5;
        int clipX = windowWidth - windowWidth / 30 - hud.getAmmoBackground()->w;
        int clipY = windowHeight - windowWidth / 30 - hud.getAmmoBackground()->h;
        hud.setRectSize(hud.getAmmoBackground(), clipWidth, clipHeight);
        hud.setRectSize(hud.getAmmoForeground(), clipWidth, clipHeight);

        hud.setRectPosition(hud.getAmmoBackground(), clipX, clipY);
        hud.setRectPosition(hud.getAmmoForeground(), clipX, clipY);
        hud.renderRectangle(hud.getAmmoBackground(), game.renderer, WHITE_RGB, OPAQUE);
        hud.renderRectangle(hud.getAmmoForeground(), game.renderer, BLUE_RGB, OPAQUE);


        hud.getAmmoClipTexture()->setDimensions(clipWidth, clipHeight);
        hud.getAmmoClipTexture()->render(game.renderer, clipX, clipY);

        //**************************************************DISPLAY THE AMMOBAR********************************************//

        //**************************************************DISPLAY THE INVENTORY********************************************//
        int consumableSize;
        if (windowWidth <= windowHeight) {
            consumableSize =  windowWidth / 10;
        } else {
            consumableSize = windowHeight / 10;
        }
        int weaponSlotWidth = windowWidth / 7.5;
        int weaponSlotHeight = windowHeight / 15;
        int weaponSlotPosY = windowHeight - windowWidth / 30 - weaponSlotHeight;

        hud.getEquippedWeaponTexture()->setDimensions(weaponSlotWidth, weaponSlotHeight);
        hud.getActiveWeaponSlotTexture()->setDimensions(weaponSlotWidth, weaponSlotHeight);
        hud.getWeaponSlotTexture()->setDimensions(weaponSlotWidth, weaponSlotHeight);
        hud.getConsumableSlotTexture()->setDimensions(consumableSize, consumableSize);

        //Cap if below 0
        if( hud.getOpacity() <= 0 )
        {
            hud.setOpacity(0);
        } else {
            hud.decrementOpacity(1);
        }
        hud.getActiveWeaponSlotTexture()->setAlpha(hud.getOpacity());
        hud.getWeaponSlotTexture()->setAlpha(hud.getOpacity());

        int weaponSlotPosition[3][2] =
        {
            {clipX - 2 * weaponSlotWidth - windowWidth / 30 - windowWidth / 30 , weaponSlotPosY},
            {clipX - 3 * weaponSlotWidth - 2 * (windowWidth / 30) - windowWidth / 30, weaponSlotPosY},
            {clipX - 4 * weaponSlotWidth - 3 * (windowWidth / 30) - windowWidth / 30, weaponSlotPosY}
        };

        hud.renderWeaponInventory(player.marine->inventory.getCurrent(), weaponSlotPosition, game.renderer);

        hud.getEquippedWeaponTexture()->render(game.renderer, clipX - weaponSlotWidth - windowWidth / 30, weaponSlotPosY);
        hud.getConsumableSlotTexture()->render(game.renderer, windowWidth / 30, windowHeight - windowWidth / 30 - consumableSize);
        //**************************************************DISPLAY THE INVENTORY********************************************//

        //**************************************************DISPLAY THE HUD********************************************//

        //Update screen
        SDL_RenderPresent( game.renderer );
    }
}

GameStateMatch::~GameStateMatch() {

    // Free texture and font
    delete GameManager::instance();
    frameFPSTextTexture.free();
    TTF_CloseFont(frameFont);
    frameFont = NULL;

}
