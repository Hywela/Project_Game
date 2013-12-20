#pragma once
#include <string>
#include <sstream>
using namespace std;



//Settings
const string GAME_NAME = "Space Game";
const int RESOLUTION_WIDTH = 800;
const int RESOLUTION_HEIGHT = 450;
const int MAX_RESOLUTION_WIDTH = 1920;
const int MAX_RESOLUTION_HEIGHT = 1080;
const int TILE_SIZE = 64;
const int SHIELD_SIZE = 192; //Pixels
const int ROCKET_SIZE_W = 32; //Pixels
const int ROCKET_SIZE_H = 18; //Pixels
const int ROCKET_MAX_SPEED = 5;

const string DIR_TEXTURES = "Textures/";
const string DIR_BACKGROUNDS = DIR_TEXTURES + "Backgrounds/";
const string DIR_MODULES = DIR_TEXTURES + "Modules/";
const string DIR_HULLS = DIR_TEXTURES + "Hulls/";
const string DIR_FONTS = DIR_TEXTURES + "Fonts/";
const string DIR_BUTTONS = DIR_TEXTURES + "Buttons/";
const string DIR_EDITTEXTS = DIR_TEXTURES + "EditTexts/";
const string DIR_AUDIO = "Audio/";
const string DIR_EFFECTS = DIR_AUDIO + "Effects/";
const string DIR_MUSIC = DIR_AUDIO + "Music/";
const string DIR_MISC = DIR_TEXTURES + "Misc/";

const int SHIP_WIDTH = 5;
const int SHIP_HEIGHT = 5;

enum hull_action {HUL_NOTHING, HUL_PLACE, HUL_REMOVE};
enum hull_type {NORMAL, ELECTRICAL, REINFORCED, HULL_COUNT};

enum module_action {MOD_NOTHING, MOD_PLACE, MOD_REMOVE};
enum module_type {TURRET, SHIELD, MODULE_COUNT};
enum screenType {SCREEN_LOGIN, SCREEN_MAIN_MENU, SCREEN_SETTINGS, SCREEN_QUEUE};

string numToStr(int n);