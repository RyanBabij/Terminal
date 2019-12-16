#pragma once
#ifndef TERMINAL_TERMINAL_CPP
#define TERMINAL_TERMINAL_CPP

/* Terminal: Terminal
#include "Terminal.cpp"
*/

#include "Terminal.hpp"

Terminal::Terminal()
{
   pGlyph = &aGlyph[0][0];
   pGlyphBacklog = &aGlyphBacklog[0][0];
   pCorrupt = &aCorrupt[0][0];
   pGlyphDelay = &aGlyphDelay[0][0];

   dialTones="";

   errorScreenActive=false;
   cursorVisible=true;

   init();
}

Terminal::~Terminal()
{
   globalAudioPlayer.close();
}


void Terminal::init()
{
   int i=0;
   for (int _y=0;_y<48;++_y)
   {
      for (int _x=0;_x<64;++_x)
      {
         aGlyph[_y][_x] = ' ';
         aGlyphBacklog[_y][_x] = ' ';
         aCorrupt[_y][_x] = ' ';
         aGlyphDelay[_y][_x]=TERM_GLYPH_DELAY;
      }
   }

   cursorX = -1; cursorY = -1;
   cursorBlink = 0;
   pause=false;
   intro=0;
   debugConsole=false;
   bootScreen=true;
   helpScreen=false;
   vPackets.clear();
   command = "";
   currentConnection="";
   errorScreenActive=false;
   cursorVisible=true;

   loadX=0; loadY=0;

   putCursor(0,0);
   //bootSystem1();

   frameRateTimer.init();
   frameRateTimer.start();

   loadAudio();

   fileManual.filename = "manual.txt";
   vFile.push(&fileManual);

   //load basic programs
   vProgram.clearPtr();
   vProgram.push(new Program_Write(this));
   vProgram.push(new Program_Run(this));

   clearScreen();
   strMainConsole = "                    *** SUDACHI SYSTEM 1 ***                    \n";

}

void Terminal::loadAudio()
{
   globalAudioPlayer.init();
   globalAudioPlayer.globalVolume=25;

   Wav w1;
   w1.readFile("wav/1.wav");

   Wav w2;
   w2.readFile("wav/2.wav");

   Wav w3;
   w3.readFile("wav/3.wav");

   Wav w4;
   w4.readFile("wav/4.wav");

   Wav w5;
   w5.readFile("wav/5.wav");

   Wav w6;
   w6.readFile("wav/6.wav");

   Wav w7;
   w7.readFile("wav/7.wav");

   Wav w8;
   w8.readFile("wav/8.wav");

   Wav w9;
   w9.readFile("wav/9.wav");

   Wav w0;
   w0.readFile("wav/0.wav");

   Wav wRing;
   wRing.readFile("wav/ringtone.wav");
   sRing = wRing.toSound();

   dialTone[0] = w0.toSound();
   dialTone[1] = w1.toSound();
   dialTone[2] = w2.toSound();
   dialTone[3] = w3.toSound();
   dialTone[4] = w4.toSound();
   dialTone[5] = w5.toSound();
   dialTone[6] = w6.toSound();
   dialTone[7] = w7.toSound();
   dialTone[8] = w8.toSound();
   dialTone[9] = w9.toSound();
}

void Terminal::clearScreen(bool forced) /* forced will instantly clear the screen, instead of using backlog */
{
   int i=0;
   for (int _y=0;_y<48;++_y)
   {
      for (int _x=0;_x<64;++_x)
      {
         aGlyphBacklog[_y][_x] = ' ';
         ansiGrid.aGlyph[_y][_x]=' ';
         if ( forced )
         {
            aGlyph[_y][_x] = ' ';
         }
      }
   }
   //command="";
   //strMainConsole="";
}

//Writes the string to the backlog.
// Todo: Add instant option.
void Terminal::writeString(int _x, int _y, std::string _str, bool moveCursor)
{
   ansiGrid.cursorX=_x;
   ansiGrid.cursorY=_y;
   ansiGrid.read(_str);

   for (int _y2=0;_y2<48;++_y2)
   {
      for (int _x2=0;_x2<64;++_x2)
      {
         aGlyphBacklog[_y2][_x2] = ansiGrid.aGlyph[_y2][_x2];
         foregroundColour[_y2][_x2] = ansiGrid.aColour[_y2][_x2];
      }
   }
   cursorX=ansiGrid.cursorX;
   cursorY=ansiGrid.cursorY;
   //putCursor(ansiGrid.cursorX,ansiGrid.cursorY);
}

void Terminal::loadChar()
{
   int maxSkip = 10;
   while (maxSkip-- > 0)
   {
      if ( isSafe(loadX,loadY) )
      {
         if ( aGlyph[loadY][loadX] != aGlyphBacklog[loadY][loadX] )
         {
            aGlyph[loadY][loadX] = aGlyphBacklog[loadY][loadX];
            maxSkip=0;
         }
         ++loadX;
         if (loadX > 63)
         {
            if ( loadY < 47 )
            {
               loadX=0; ++loadY;
            }
            else
            {
               loadX=0; loadY=0;
            }
         }
      }
   }
}

void Terminal::corrupt()
{
   int rY = Random::randomInt(47);
   int rX = Random::randomInt(63);

   aGlyph[rY][rX]-= Random::randomInt(10);
   if (aGlyph[rY][rX] < 0)
   {
      aGlyph[rY][rX] = 0;
   }
}

//Variant which loads all of screen at once, but cycles each character through the
// character table until it hits the right one.
// This should cycle through random chars to get consistent timing.
void Terminal::loadChar2(int nIterations /* =1 */)
{
   for (int i2=0;i2<nIterations;++i2)
   {
      // fill terminal with random glyphs.
      for (int i=0;i<3072;++i)
      {
         if (pGlyph[i] != pGlyphBacklog[i])
         {
            pGlyph[i]++;
            if (pGlyph[i] < 0) { pGlyph[i]=0; }
            if (pGlyph[i] == '\n') { pGlyph[i]++; }
         }
      }
   }
}

//Variant which loads all of screen at once, but cycles each character through the
// TERM_GLYPH_DELAY random characters before hitting the right one
void Terminal::loadChar3()
{
   for (int i=0;i<3072;++i)
   {
      if (pGlyph[i] != pGlyphBacklog[i])
      {
         if ( pGlyphDelay[i]==0 )
         {
            pGlyph[i] = pGlyphBacklog[i];
            pGlyphDelay[i]=TERM_GLYPH_DELAY;
         }
         else
         {
            pGlyphDelay[i]--;
            unsigned char randGlyph = Random::randomInt(253);
            if (randGlyph == '\n') { randGlyph=255; }
            if (randGlyph == pGlyphBacklog[i]) { randGlyph=254; }
            pGlyph[i]=randGlyph;
         }
      }
   }
}

void Terminal::randomFill()
{
   // fill terminal with random glyphs.
   for (int i=0;i<3072;++i)
   {
      pGlyph[i] = Random::randomInt(127);
      if (pGlyph[i] == '\n' ) { pGlyph[i] = ' '; }
      pGlyphBacklog[i] = ' ';
   }
}

bool Terminal::renderProgram()
{
   for (int i=0;i<vProgram.size();++i)
   {
      // WE NEED 2 STRINGS:
      // ASCII MODE STRING AND GRAPHICS MODE STRING
      // WE ALSO NEED TO HANDLE PROGRAM EXIT (OR MAYBE NOT NECESSARY
      // IF USING A GRAPHICS BUFFER)
      
      // We really should just have a ptr to the active program (if any).
      if (vProgram(i)->active)
      {
         if ( vProgram(i)->graphicsMode )
         {
            clearScreen();
            strMainConsole = vProgram(i)->render();
            //writeString(0,0,vProgram(i)->render(),true);
            writeString(0,0,strMainConsole,true);
         }
         else // ASCII mode
         {
            clearScreen();
            strMainConsole += vProgram(i)->render();
            writeString(0,0,strMainConsole,true);
         }
         
         // check if programs terminated, so we can reset screen.
         for (int i2=0;i2<vProgram.size();++i2)
         {
            if (vProgram(i2)->active)
            {
               //clearScreen(true);
               return true;
            }
         }
         std::cout<<"TERMINATE DETECTED\n";
         
         // terminal reset code
         
         
         //clearScreen();
         //putCursor(0,0);
         //strMainConsole+=".";
         
         //writeString(0,0,vProgram(i)->render());
         return true;
      }
   }
   return false;
}

// Terminal only renders text, not any decoration.
void Terminal::render()
{
   //loadChar();
   //loadChar();   
   loadChar3();

   blinkCursor();



   if (dialTones.size() > 0)
   {
      toneTimer.update();

      if ( toneTimer.fullSeconds > 0.12)
      { globalAudioPlayer.stopAllSounds();
      }

      if (toneTimer.fullSeconds > 0.20)
      {
         int nextTone = (int)dialTones[0]-48;

         if ( nextTone >= 0 && nextTone <= 9)
         { globalAudioPlayer.playSoundOnce(dialTone[nextTone]);
         }
         else if (dialTones[0] == 'R')
         {
            globalAudioPlayer.playSoundOnce(sRing);
         }

         toneTimer.start();
         dialTones.erase(0,1);
      }
   }
   
   if (renderProgram() == false)
   {
      clearScreen();
      writeString(0,0,strMainConsole,true);
   }


   //Update: Glyphs now render on a grid instead of using error-prone string.
   // Also foreground colour support added.
   int i=0;
   for (int _y=0;_y<48;++_y)
   {
      for (int _x=0;_x<64;++_x)
      {
         font8x8.putChar(aGlyph[_y][_x],panelX1+(10*_x),panelY2-(10*_y),foregroundColour[_y][_x]);
      }
   }

   if (errorScreenActive)
   {
      //clearScreen(true);
      errorScreen();
   }
}

// Simply placing the cursor over a glyph will erase it, which is kinda cool
void Terminal::putCursor(int _x, int _y)
{
   if ( isSafe(cursorX,cursorY) )
   {
      aGlyph[cursorY][cursorX] = ' ';
      aGlyphBacklog[cursorY][cursorX] = ' ';
      ansiGrid.aGlyph[cursorY][cursorX] = ' ';
   }

   if (isSafe(_x,_y))
   {
      cursorX = _x;
      cursorY = _y;
      aGlyph[cursorY][cursorX] = ' ';
      aGlyphBacklog[cursorY][cursorX] = ' ';
      ansiGrid.aGlyph[cursorY][cursorX] = ' ';
   }
}

void Terminal::newLine()
{
   if (isSafe(0,cursorY+1))
   {
      putCursor(0,cursorY+1);
   }
   else
   {
      //Scroll terminal by 1 row.
      std::cout<<"SHIFT\n";
      shiftUp(1); //ANSI_Grid handles scrolling, but we need to handle the backlog scrolling
   }
}

void Terminal::blinkCursor()
{
   if ( cursorVisible)
   {
      if (isSafe(cursorX,cursorY))
      {
         if (++cursorBlink > 40)
         {
            cursorBlink = 0;
         }
         if ( cursorBlink > 20)
         {
            aGlyph[cursorY][cursorX] = ' ';
         }
         else
         {
            aGlyph[cursorY][cursorX] = 1;
         }
      }
   }
}

void Terminal::hideCursor()
{
   cursorVisible=false;
   if (isSafe(cursorX,cursorY))
   {
      if (aGlyph[cursorY][cursorX]==1)
      {
         aGlyph[cursorY][cursorX] = ' ';
      }
   }
}

bool Terminal::typeChar (const unsigned char c)
{
   //if ( _keyboard->isAlphaNumeric(_keyboard->lastKey) || _keyboard->lastKey == Keyboard::SPACE)
  
   std::string allowedInputs = " !@#$%^&*()\"\'";

   if (DataTools::isAlphaNumeric(c) || allowedInputs.find(c) != std::string::npos)
   {
      strMainConsole += c;
      
      // Make sure we're on an input space before we type.
      if (isSafe(cursorX,cursorY) && isSafe(cursorX+1,cursorY))
      {
         putCursor(cursorX+1,cursorY);
         aGlyph[cursorY][cursorX-1] = c;
         aGlyphBacklog[cursorY][cursorX-1] = c;

         ansiGrid.aGlyph[cursorY][cursorX-1]=c;
         command+=c;
         return true;
      }
   }
   return false;
}

void Terminal::backspace()
{
   if ( strMainConsole[strMainConsole.size()-1] != '\n')
   {
      strMainConsole.pop_back();

      if (isSafe(cursorX-1,cursorY))
      {
         putCursor(cursorX-1,cursorY);
         aGlyph[cursorY][cursorX+1] = ' ';
         aGlyphBacklog[cursorY][cursorX+1] = ' ';
         ansiGrid.aGlyph[cursorY][cursorX+1]= ' ';

         if ( command.size () > 0 )
         { command = command.substr(0, command.size()-1);
         }
      }
   }
}

bool Terminal::isSafe(int _x, int _y)
{
   return ( _x >= 0 && _x <= 63 && _y  >= 0 && _y <= 47);
}

bool Terminal::keyboardEvent(Keyboard* _keyboard)
{
   for (int i=0;i<vProgram.size();++i)
   {
      if (vProgram(i)->active)
      {
         vProgram(i)->keyboardEvent(_keyboard);
         
         
         // check if programs terminated, so we can reset screen.
         for (int i2=0;i2<vProgram.size();++i2)
         {
            if (vProgram(i2)->active)
            {
               
               return true;
            }
         }
         std::cout<<"TERMINATE DETECTED\n";
         clearScreen(true);
         strMainConsole="";
         _keyboard->clearAll();
         return true;
      }
   }

   if (_keyboard->keyWasPressed)
   {
   std::cout<<"Keypress: "<<(int) _keyboard->lastKey<<".\n";

   if ( typeChar(_keyboard->lastKey))
   {  }
   else if (_keyboard->lastKey == 96) /* TILDE */
   {
      debugConsole=!debugConsole;

      if ( debugConsole )
      {
         loadDebugConsole();
      }
   }
   else if (_keyboard->lastKey == 18) /* CTRL + R */
   {
      init();
   }
   else if (_keyboard->lastKey == 19)
   {
      std::cout<<"Save\n";
   }
   else if (_keyboard->lastKey == 8 )
   {
      backspace();
   }
   else if (_keyboard->lastKey == 3 ) /* CTRL+C */
   {
      shutDown();
   }
   // Get whatever the user typed.
   else if (_keyboard->lastKey == Keyboard::ENTER )
   {
      strMainConsole+='\n';
      newLine();
      // Convert string to upper case
      for (auto & c: command) c = toupper(c);
      std::cout<<"Entered command: "<<command<<".\n";

      if ( command == "" )
      {
         _keyboard->clearAll();
      }

      // If we're connected to a server, send the input to the server
      else if (currentConnection != "")
      {
         sendPacket(currentConnection,command);
         command = "";
      }
      // We are sending command to Terminal, so tokenise and process it
      else
      {
         sendTerminalCommand(command);
         command = "";
      }
      intro=0;

      _keyboard->clearAll();
      return true;
   }
   _keyboard->clearAll();
   }
   return false;
}

void Terminal::introStep()
{
   if ( intro < 11 )
   {
      // fill terminal with random glyphs.
      for (int i=0;i<3072;++i)
      {
         if (pGlyph[i] != ' ')
         {
            pGlyph[i] = Random::randomInt(127);
            if (pGlyph[i] == '\n' || Random::oneIn(4)) { pGlyph[i] = ' '; }
         }
      }
   }
   else
   {
      // fill terminal with random glyphs.
      for (int i=0;i<3072;++i)
      {
         pGlyph[i]= ' ';
      }
   }
   ++intro;
}

char Terminal::getRandomChar()
{
   char randChar = Random::randomInt(127);
   if (randChar == '\n') { return ' '; }
   return randChar;
}

void Terminal::loadHelpScreen()
{
   clearScreen();
   //randomFill();
   writeString(0,0,"                    *** SUDACHI SYSTEM 1 ***                    ");
   writeString(0,2,"CATALOG - LIST PROGRAMS");
   writeString(0,3,"LIST - LIST FILES");
   writeString(0,4,"RUN - RUN PROGRAM");
   writeString(0,5,"REBOOT - REBOOT COMPUTER");
   writeString(0,6,"POWEROFF - POWER OFF COMPUTER");

}

void Terminal::loadCatalog()
{
   writeString(0,0,"                    *** SUDACHI SYSTEM 1 ***                    ");
   writeString(0,2,"CONNECT"); /* Allows connection to servers with WireNet protocol */
   writeString(0,3,"GAME1"); /* Some game probably breakout. */
   writeString(0,4,"GAME2"); /* Maybe tetris or smth idk */
   writeString(0,5,"DEBUG"); /* Snowcrash console, contains set of network programs */
   writeString(0,6,"SONG"); /* A realistic elephant. */
   writeString(0,7,"WRITE"); /* Take notes or write program */
   writeString(0,8,"MAIL"); /* Check/send mail */
}

void Terminal::loadDebugConsole()
{
   randomFill();
   writeString(0,0,"                    *** SNOWCRASH CONSOLE ***                    ");

   for (int i=0;i<vPackets.size() && i < 20;++i)
   {
      writeString(0,i+2,vPackets(i));
   }
}

void Terminal::bbsDemo()
{
   dialTones = "1234567890";

   vPackets.push("SENT 111 111 1111 CONNECT");
   vPackets.push("RECV 111 111 1111 ACK");
   vPackets.push("RECV 111 111 1111 [SITE DATA]");

   clearScreen();
   //init();
   //randomFill();

   writeString(0,0,    "                      .__                         ");
   writeString(0,1, "_____  ___.__.___.__. |  |   _____ _____    ____  ");
   writeString(0,2, "\\__  \\<   |  <   |  | |  |  /     \\\\__  \\  /  _ \\ ");
   writeString(0,3, " / __ \\\\___  |\\___  | |  |_|  Y Y  \\/ __ \\(  <_> )");
   writeString(0,4, "(____  / ____|/ ____| |____/__|_|  (____  /\\____/ ");
   writeString(0,5, "     \\/\\/     \\/                 \\/     \\/        ");

   writeString(0,10, "@@@@@@@@@@@@@@@@@&,//////,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
   writeString(0,11, "@@@@@@@@@&@&/.@/////////////*,&////@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
   writeString(0,12, "@@@@@@@&**///////////////////***********,@@@@@@@@@@@@@@@@@@@@@@");
   writeString(0,13, "@@@@@@**//////////////////////(***************/&@@@@@@@@@@@@@@@");
   writeString(0,14, "@@@@/***///////////****////////,****************** &@@@@@@@@@@@");
   writeString(0,15, "@@@,*****//////////////(////////*********************&@@@@@@@@@");
   writeString(0,16, "@@/***********./*///////**////*************************/@@@@@@@");
   writeString(0,17, "@/****************/////,***********.//////////////*******@@@@@@");
   writeString(0,18, "&*****************************/////////////////////********@@@@");
   writeString(0,19, "*****************************/////////////////////**********,&@");
   writeString(0,20, "***************************** ///////////////@/ **************@");
   writeString(0,21, "*****************,/////.**************///////*****************/");
   writeString(0,22, "****************/////////////.*********@///********************");
   writeString(0,23, "@***************//////////////////@****************************");
   writeString(0,24, "@@&**************(////////////////////////*********************");
   writeString(0,25, "@@&&********......******(//////////////////////@***************");
   writeString(0,26, "@@@@@,*****(.......*********** ///////////////////////*********");
   writeString(0,27, "@@@@@@@*****........****************,//////////////////////****");
   writeString(0,28, "@@@@@@@@/***.......(************************@///////////////@**");
   writeString(0,29, "@@@@@@@@@@,*........((...(......((..(.......(****@///////////**");
   writeString(0,30, "@@@@@@@@@@@ ................................(*********/////.***");
   writeString(0,31, "@@@@@@@@@@&..................................******************");
   writeString(0,32, "@@@@@@@@@@@*...................(..........********************&");
   writeString(0,33, "@@@@@@@@@@@@&.................*************************.&@@@@@@");
   writeString(0,34, "@@@@@@@@@@@@@@&&*....(.(......********************&&@@@@@@@@@@@");
   writeString(0,35, "@@@@@@@@@@@@@@@@@@@&/@@@/.((((**************/&@&@@@@@@@@@@@@@@@");

   writeString(0,7,"Welcome to AYYBBS");
   writeString(0,8,"Login:");
}


void Terminal::mailScreen()
{
   char r = 1;
   std::string redacted  = std::string() + r + "" + r + "/" + r + r + "/" +r +r +r +r;
   clearScreen(); 
   writeString(0,0,"*** MAIL. Type number to read. ***");
   writeString(0,1,"1. "+redacted+" - WELCOME TO MAIL");
   writeString(0,2,"2. "+redacted+" - SUP");
}

void Terminal::errorScreen(std::string strError)
{
   hideCursor();

   clearScreen(true);
   
   char errorBorder = ' ';

   Colour currentColour;
   
   // Note: Colours are currently broken on some systems
   currentColour.set(255,255,255,255);

   gameTimer.update();


   if (gameTimer.seconds % 2 == 0)
   {
      currentColour.set(255,0,0,255);
      errorBorder=1;
   }
   
   // Draw flashing red border
   for (int _x=0;_x<64;++_x)
   {
      aGlyph[0][_x]=errorBorder;
      aGlyph[47][_x]=errorBorder;

      aGlyphBacklog[0][_x]=errorBorder;
      aGlyphBacklog[47][_x]=errorBorder;

      foregroundColour[0][_x] = currentColour;
      foregroundColour[47][_x] = currentColour;
   }
   for (int _y=0;_y<48;++_y)
   {
      aGlyph[_y][0]=errorBorder;
      aGlyph[_y][63]=errorBorder;

      aGlyphBacklog[_y][0]=errorBorder;
      aGlyphBacklog[_y][63]=errorBorder;

      foregroundColour[_y][0] = currentColour;
      foregroundColour[_y][63] = currentColour;
   }
}

void Terminal::game1()
{
   putCursor(-1,-1);
   char block = 1;
   std::string blocks = "   ";
   for (int i=0;i<58;++i)
   {
      blocks+=block;
   }
   blocks+="   ";

   writeString(0,1,blocks);
   writeString(0,2,blocks);
   writeString(0,3,blocks);

   std::string paddle = "";
   paddle+=block; paddle+=block; paddle+=block; paddle+=block;

   writeString(30,47,paddle);

   std::string ball = "";
   ball += 1;

   writeString(32,24,ball);
}

void Terminal::loadPage(std::string pageData)
{
   clearScreen();
   putCursor(0,0);
   writeString(0,0,pageData);
}

void Terminal::sendPacket(std::string _currentConnection, std::string _command)
{
   std::string response = op.sendPacket(_currentConnection,_command);

   std::cout<<"Packet: "<<_command<<" send to "<<_currentConnection<<".\n";
   std::cout<<"Response: "<<response<<".\n";

   if ( response == "[RDR]" )
   {
      clearScreen();
      loadPage(op.servePage(_currentConnection));
   }
}

void Terminal::sendTerminalCommand(std::string _command)
{
   std::cout<<"Terminal command recieved: "<<_command<<"\n";
   
   // Any valid command must have length at least 1.
   if ( _command.size() == 0 )
   {
      return;
   }
   //Tokenise
   Vector <std::string> * vToken = Tokenize::tokenize(command,' ');
   
   if (vToken==0) // Tokenize returns null obj if no tokens.
   {
      return;
   }

std::cout<<"vtoken size: "<<vToken->size()<<".\n";
   for (int i=0;i<vToken->size();++i)
   {
      std::cout<<"Token: "<<(*vToken)(i)<<"\n";
   }

   //First token is command.
   if (vToken->size() > 0)
   {
      command = (*vToken)(0);
   }
   else
   {
      command = "";
   }

   //Check system commands.
   if (command == "HELP" && bootScreen == true)
   {
      helpScreen=true;
      bootScreen=false;
      debugConsole=false;
      command = "";
      loadHelpScreen();
   }
   else if (command == "REBOOT" || command == "RESET")
   {
      init();
   }
   else if (command == "SHUTDOWN" || command == "POWEROFF")
   {
      shutDown();
   }
   else if (command=="ERROR")
   {
      std::cout<<"error\n";
      errorScreenActive=true;
   }
   else if (command == "CONNECT") // This should really be a program
   {
      // strip everything except numbers. There should be 7, 10, 12, 19, or 22 digits.
      // phonecards will be 12 digits.

      std::string connectPacket = "[CON]";

      if (vToken->size() == 1)
      {
         strMainConsole+="\033[1;31mDestination number required\033[0m\n";
      }
      else if (vToken->size() == 2)
      {
         std::string targetDial = (*vToken)(1);
         std::cout<<"Dial arg: "<<targetDial<<".\n";



         // dial must be 10 or 7 digits. (3 digits are city code)
         if ( DataTools::isNumber(targetDial) )
         {
            // Auto-add city code if necessary.
            if (targetDial.size()==7)
            {
               targetDial = "001" + targetDial;
            }

            if (targetDial.size() == 10)
            {
               dialTones = targetDial+"R";

               vPackets.push("[CON]["+targetDial+"]");

               if ( op.dial(targetDial) )
               {
                  vPackets.push("[ACK]["+targetDial+"]");
                  loadPage(op.servePage(targetDial));
                  currentConnection= targetDial;
               }
               else
               {
                  vPackets.push("[404]["+targetDial+"]");
                  currentConnection="";
               }
            }
            else
            {
               strMainConsole+="\033[1;31mDestination number must be 7 or 10 digits, no spaces.\033[0m\n";
            }
         }
         else
         {
            strMainConsole+="\033[1;31mParameter must be a number.\033[0m\n";
            strMainConsole+="\033[1;31mParameter must be a number.\033[0m\n";
         }
      }
      else
      {
      strMainConsole+="\033[1;31mError.\033[0m\n";
      }
      //screenConnect("","");   
      //command = "";
   }

   else //Iterate through program list to find match.
   {
      std::cout<<"Checking programs\n";
      for (int i=0;i<vProgram.size();++i)
      {
         std::cout<<vProgram(i)->programName<<".\n";
         if (command == vProgram(i)->programName)
         {
            //init this program.
            std::cout<<"Program match. Running "<<command<<".\n";

            std::string strReturn = vProgram(i)->init(vToken);

            if ( strReturn != "")
            {
               //writeString(cursorX,cursorY,strReturn,true);
               strMainConsole+=strReturn;
            }
            return;
         }
      }
      strMainConsole+="\033[1;31mUnknown command \033[0m"+command+"\n";
   }
}

void Terminal::shiftUp(int amount)
{
   if (isSafe(cursorX,cursorY))
   {
      aGlyph[cursorY][cursorX] = ' ';
   }

   for (int _x=0;_x<64;++_x)
   {
      for (int _y=0;_y<47;++_y)
      {
         aGlyph[_y][_x] = aGlyph[_y+1][_x];
         aGlyphBacklog[_y][_x] = aGlyphBacklog[_y+1][_x];
         //ansiGrid.aGlyph[_y][_x] = ansiGrid.aGlyph[_y+1][_x];
         //ansiGrid.aColour[_y][_x]=ansiGrid.aColour[_y+1][_x];
         foregroundColour[_y][_x]=foregroundColour[_y+1][_x];
      }
   }

   for (int _x=0;_x<64;++_x)
   {
      aGlyph[47][_x] = ' ';
      aGlyphBacklog[47][_x] = ' ';
      //ansiGrid.aGlyph[47][_x] = ' ';
      //ansiGrid.aColour[47][_x].set(255,255,255,255);
      foregroundColour[47][_x].set(255,255,255,255);
      putCursor(0,47);
   }

}

#endif