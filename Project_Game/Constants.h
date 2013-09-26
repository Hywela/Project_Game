#pragma once
#include <string>
using namespace std;



//Settings
extern const char *GAME_NAME; 
const int RESOLUTION_WIDTH = 640;
const int RESOLUTION_HEIGHT = 480;
const int MAX_RESOLUTION_WIDTH = 1920;
const int MAX_RESOLUTION_HEIGHT = 1080;
const int TILE_SIZE = 64;
const string DIR_TEXTURES = "Textures/";
const string DIR_BACKGROUNDS = DIR_TEXTURES + "Backgrounds/";
const string DIR_MODULES = DIR_TEXTURES + "Modules/";
const string DIR_HULLS = DIR_TEXTURES + "Hulls/";

const int SHIP_WIDTH = 5;
const int SHIP_HEIGHT = 5;

enum hull_action {NOTHING, PLACE, MOVE, REMOVE};

enum hull_type {NORMAL, ELECTRICAL, REINFORCED};

