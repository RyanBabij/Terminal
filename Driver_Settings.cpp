#pragma once
#ifndef TERMINAL_DRIVER_SETTINGS_CPP
#define TERMINAL_DRIVER_SETTINGS_CPP

/* WorldSim: Driver_Settings.cpp
  #include "Driver_Settings.cpp"

   Gameplay settings and other global settings go here. Some of this stuff will probably end up in an .ini file.
*/

#include <limits.h> // We need ULONG_MAX for absolute coords.

   // GAME FLAGS
   // number of times to cycle a running program each tick. Default 1.
   // Higher value will make programs run faster.
unsigned short int PROGRAM_CYCLES_PER_TICK = 5;

  // SYSTEM STRINGS
const std::string VERSION = "0.0.069 Win32 dev";
const std::string G_WINDOW_TITLE = "Terminal";
const std::string SAVE_FOLDER_PATH = "savedata";

// WINDOW STUFF

unsigned int RESOLUTIONX=800, RESOLUTIONY=600;
const bool MAXIMISE_WINDOW = false;

  // MAIN MENU ENUMS
   // I use a global enum to decide what menu we are in. Makes like much easier.
enum enumMenu { MENU_UNKNOWN, MENU_TITLE };
//Set the menu the game boots into
enumMenu activeMenu = MENU_TITLE;

bool HOTKEYS_ENABLED = false;

bool CLEAN_SAVES_ON_EXIT = true;

/* Will output the framerate. */
const bool OUTPUT_FRAMERATE = false;
const int OUTPUT_FRAMERATE_SAMPLE_SIZE = 50;

const bool LIMIT_FRAMERATE = true;
const double FRAMERATE = 30; // SETTING IT TO A REASONABLE VALUE CAN GREATLY REDUCE STRESS ON GPU
const double POLLSPERSECOND = 30; // NOT CURRENTLY IMPLEMENTED
double LOGIC_PER_SECOND = 1;
const double PHYSICS_PER_SECOND = 10;
double ANIMATION_PER_SECOND = 10; // SETS SPEED OF ANIMATIONS

   // SLOW FRAMERATE CAN BE ACTIVATED TO GET EXTRA CYCLES FOR GAMEPLAY.
int SLOW_FRAMERATE = 1000000/2;
   // TURNS ON SLOW FRAMERATE
bool SLOW_FRAMERATE_ACTIVE = false;
int UFRAMERATE = 1000000/FRAMERATE;
const int UPOLLSPERSECOND = 1000000/POLLSPERSECOND;
int U_LOGIC_PER_SECOND = 1000000/LOGIC_PER_SECOND;
const int U_PHYSICS_PER_SECOND = 1000000/PHYSICS_PER_SECOND;
const int UANIMATIONSPERSECOND = 1000000/ANIMATION_PER_SECOND;
int CURRENT_ANIMATION_FRAME = 0; /* 0 - 99 */


  // RENDER AND FRAME STUFF

/* Double buffering will make the frame transitions smoother, but it is slower. */
const bool DOUBLE_BUFFERING = true;

/* BUSY_WAIT will cause the program to enter a busy wait if rendering is almost ready. Not sure if it works very well. */
const bool BUSY_WAIT = false;
const int BUSY_WAIT_USEC = 2000;

/* If CPU isn't relinquished, then OpenGL tends to run at 100% CPU for some reason. */
/* This uses MsgWaitForMultipleObjects, which will let the OS do other things until any kind of input event occurs */
const bool RELINQUISH_CPU = true;
const unsigned int RELINQUISH_CPU_TIMEOUT = 100; /* Anywhere from 10-100 seems to be okay. Maximum amount of time to relinquish */
  bool NO_BACKLOG=false;

bool RENDER_NEXT_FRAME=true; /* If there has been no input or state change, there's no need to render */
const bool LAZY_RENDERING=false; /* Only render if something happened */


// GLOBAL FLAGS

bool QUIT_FLAG = false;
bool DONT_RENDER = false; /* In debugging, rendering can be disabled by pressing R. */
bool LIMIT_LOGIC = true; /* Press S. */
bool FORCE_LOGIC_FRAME = false; /* True will trigger next logic frame immediately. */
bool PAUSE_LOGIC = false; /* True will suspend logic. */
  // MAKES COUT MUCH FASTER BUT DOES NOT GUARANTEE ORDER OR COMPLETION BEFORE EXIT/CRASH.
const bool FAST_COUT = false;

bool EMULATE_2_BUTTON_MIDDLE_CLICK = true; // Allows middle click with mice without middle click by pressing left and right click.

// MORE STRINGS

const char THOUSANDS_DELIMITER = ',';
const char DECIMAL_DELIMITER = '.';


#endif