#pragma once
#ifndef TERMINAL_TERMINAL_HPP
#define TERMINAL_TERMINAL_HPP

/* Terminal: Terminal
   #include "Terminal.hpp"

      The terminal menu. Keeps track of internal terminal state, and also handles all input/output.

      Will be expanded to include all kinds of fun stuff.

      Terminal is currently hardcoded with 10x10 font and 64x48 character area, in a 640x480 resolution.
      I'll probably need to add the ability to scale it up otherwise it'll look tiny on huge monitors.

      We'll probably stick with an ASCII table of 256 characters. 128 standard and 128 extended.
      Not sure about colour and other modifiers.

      I was planning to develop multiple versions of the Terminal, however it probably won't be necessary,
      instead that'll be done with servers. Minor cosmetic changes would be fine though.

      CONNECT.  There should be 7, 10, 12, 19, or 22 digits, depending on whether using city extension number,
      and whether including a calling code. CONNECT also returns 0 for failed connection, and 1 for
      successful connection.

      AUTODIAL. Dials random numbers until a connection is made.


      ANSI ESCAPE NOTES
      The ANSI escape sequences are very important for terminals. They allow colours, and also things like
      loading indicators and inputs.

      ANSI codes are stripped before being pushed to Terminal otherwise corruption might screw things up.

      SNOWCRASH
      Special developer debug console. Accessible at any time with ~ key. Has several useful tools, the
      most common one being the packet inspector. Lets you modify, send, rebound packets.

      MEMORY NOTES

      ASCII MODE
      [48][64] -> ASCII space, 8 bit
      [48][64] -> colour space, 8 bit. 4 bit foreground, 4 bit background.
      However in Terminal you can set all of these with a single op.
      
      Pixel mode uses 4x4 pixels -> 160*120. This will fit best with the 8x8 character set. The array will need (160*120)/2 bytes. 9,600 bytes. C64 uses some creative techniques to ensure the screen is able to update regularly... Mainly the use of playfields and sprites.
      
      However Terminal will allow people to make a large number of rendering operations.
      
      C64 has a 16 colour pallete, using hex values 0-F. Terminal will allow the ASCII mode to be overlaid on the pixel mode.
      
      Alternatively we could stick with ASCII games.

      GRAPHICS MODE
      Old computers would sometimes have multiple graphics modes for different purposes. For example
      displaying a bitmap image would use the highest resolution, but playing a game would use a lower one.
      I'm not sure if I'll implement a graphics mode. If I do, it'll probably be low-res 160p like the Atari 2600.
      However in my implementation there will be no sprite limits or playfield. The only major limitation will
      be instructions per frame.
*/

#include <string>

/* Audio stuffs. */
#include <Audio/Sound.hpp> // Generic sound data handler
#include <Audio/Wav.hpp>
#include <Audio/AudioPlayer.hpp> // Generic audio player
#include <Audio/AudioPlayer_OpenAL.hpp>
/* Global OpenAL audio player. */
AudioPlayer_OpenAL globalAudioPlayer;
/* Dial tones. */
Sound * dialTone[10];
Sound* sRing;

#include <Time/Timer.hpp>
Timer toneTimer; /* Keep track of when last tone was played */

#include <Data/Tokenize.hpp> // Tokenize console commands
#include <Data/DataTools.hpp> // Check commands

#include "Server.hpp"
#include "Operator.hpp"
//Operator op;

// EASI interpreter
#include "EASI.cpp"

#include <String/ANSI.hpp>

/* Class to store the Terminal's files which can be default system files,
or created/modified by the user. Files will be read/written to disk
so user can edit them externally if they prefer. Extension .txt is added
for readability but is not required. */

class File
{
   public:

   std::string filename;
   std::string content;

   File ()
   {
   }
};

#include "Terminal_Program.hpp"

#include "MemoryMap.hpp"

// This needs to become timer-based
#define TERM_GLYPH_DELAY 4

class Terminal: public GUI_Interface, public LogicTickInterface
{
   // Full RAM space, contains screen RAM etc. In future probably contains OS etc.
   //char memory [65536]; // = full c64 memory space.
   MemoryMap ram;
   
   
   // Need to change to 40x25 (8x8 font). = 1000 chars. 48x64 = 3,072.
   //unsigned char aNewGlyph[25][40];
   unsigned char aNewGlyph[200][320]; // Actual state of Terminal screen
   unsigned char aNewGlyphBacklog[200][320]; // Desired state of Terminal screen
   
   char aGlyph [48][64]; /* Beware. The x and y are  flipped here because C++ stores arrays in row major. */
   char* pGlyph;

   char aGlyphBacklog[48][64]; /* Character to be loaded onto the screen*/
   char* pGlyphBacklog;
   int loadX, loadY; /* current position the loader is at */

   char aCorrupt[48][64]; /* corrupted characters go here */
   char* pCorrupt;

   short int aGlyphDelay[48][64]; /* number of random glyphs to cycle before displaying correct one. */
   short int * pGlyphDelay;

   Colour foregroundColour [48][64]; /* Keep track of colour for this glyph */

   int cursorX,cursorY;
   int cursorBlink; /* counts up from zero */

   bool pause;

   int intro;

   bool bootScreen; /* Main boot screen */
   bool helpScreen; /* Shows list of commands */
   bool debugConsole; /* Access special features of browser */
   bool errorScreenActive; /* True if Terminal is in error state */
   bool cursorVisible;

   Vector <std::string> vPackets; /* Log of packets sent/recieved */

   std::string dialTones; /* Dialtones to play */
   std::string command; /* command the user has typed */

   std::string currentConnection; /* Active server connection. Empty string means no connection. */

   Vector <File*> vFile;
   /* System default files */
   File fileManual; /* Basic operation manual */

   Vector <Terminal_Program*> vProgram;

   // Container to handle ANSI strings and provide Terminal output.
   ANSI_Grid ansiGrid;

   std::string strMainConsole;

   public:

   Terminal();
   ~Terminal();

   void init();

   void loadAudio();

   void clearScreen(bool forced=false );

   // Movecursor will put the cursor at the end of the string.
   void writeString(int /* _x */, int /* _y */, std::string /* _str */, bool moveCursor=true );

   // Normal "screen wiping" method of loading up a page, typical of old computers
   void loadChar();

   void corrupt();

   void loadChar2(int nIterations = 1);

   // Cycles character through TERM_GLYPH_DELAY random characters before using the correct one
   void loadChar3();

   void randomFill();

   void render();
   bool renderProgram();

   void putCursor(int, int);

   // User has pressed enter and console either goes down by 1, or shuffles everything up by 1.
   void newLine();

   void blinkCursor();

   void hideCursor();

   // put char onto Terminal screen. Returns false if the char is not typable.
   bool typeChar (unsigned char);

   void backspace();

   bool isSafe(int /* _x */, int /* _y */);

   bool keyboardEvent(Keyboard* /* _keyboard */);

   void introStep();

   // Return a random character, but not a newline.
   char getRandomChar();

   void loadHelpScreen();


   /* Show a menu of all the programs available on this computer.
   It'll be cool if it's possible for a user to make their own programs */
   void loadCatalog();

   void loadDebugConsole();

   void bbsDemo();


   /* Shows any mail addressed to you */
   void mailScreen();


   /* Error screen displays if you break the computer. Has flashing red
      border and error message. User must reset computer. */
   void errorScreen(std::string strError="" );


   /* Breakout */
   void game1();


   void loadPage(std::string /* pageData */);

   void sendPacket(std::string _currentConnection, std::string _command);
   void sendTerminalCommand(std::string _command);

   void shiftUp(int amount); //scroll the terminal down by shifting all characters up


   // void screenConnect(std::string _number1="", std::string _number2="")
   // {
   // clearScreen();
   // writeString(0,0,"DIALING...");
   // }

};

#endif
