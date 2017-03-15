 #ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H
#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Level.h"
#include "Camera.h"
#include "LTexture.h"
static constexpr size_t NUM_MENU_ITEMS = 2;
static constexpr size_t JOIN = 0;
static constexpr size_t OPTIONS = 1;
static constexpr size_t NUM_TEXT_FIELDS = 2;
static constexpr size_t IP = 0;
static constexpr size_t USERNAME = 1;
static constexpr size_t maxLength = 15;

//Position of color in the fontColor array
static constexpr size_t WHITE = 0;
static constexpr size_t GREEN = 1;
static constexpr size_t BLACK = 2;
static constexpr size_t RED = 3;


class GameStateMenu : public GameState {
public:
    GameStateMenu(Game& g, int gameWidth, int gameHeight);
    virtual ~GameStateMenu();

    virtual bool load();
    virtual void loop();

    TTF_Font* headingFont;
    TTF_Font* textboxFont;
    TTF_Font* menuFont;
    LTexture frameFPSTextTexture;

private:

    Level level;
    Camera camera;

    virtual void sync();
    virtual void handle();
    virtual void update(const float& delta);
    virtual void render();
    void positionElements();
    void renderText(LTexture *fontTexture, const char* text, SDL_Color color, TTF_Font* font, SDL_Rect rect);

    std::string menuItems[NUM_MENU_ITEMS]; //Menu Item text to be displayed in the window
    bool selected[NUM_MENU_ITEMS]; //Whether the menu option has been selected
    SDL_Rect menuItemPos[NUM_MENU_ITEMS]; //Position of the menu items on the screen
    LTexture menuTextTextures[NUM_MENU_ITEMS];

    bool activeTextbox[NUM_MENU_ITEMS]; //Whether the text box has been selected
    SDL_Rect textboxPos[NUM_TEXT_FIELDS]; //Position of the textbox on the screen
    LTexture textboxTextures[NUM_TEXT_FIELDS];

    SDL_Rect textboxTextPos[NUM_TEXT_FIELDS]; //Position of the textbox text on the screen
    std::string defaultText[NUM_TEXT_FIELDS]; //Default text to be displayed in the textbox
    std::string textInput[NUM_TEXT_FIELDS]; //User inputted text in the textbox

    SDL_Color   fontColors[4]; //Colors used in the Window
};

#endif
