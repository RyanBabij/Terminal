#pragma once
#ifndef TERMINAL_TERMINAL_PROGRAM_HPP
#define TERMINAL_TERMINAL_PROGRAM_HPP

/* Terminal: Terminal_Program
   #include "Terminal_Program.hpp"

   Interface for Terminal programs, mostly dedicated applications like games.
   Dedicated applications recieve all keyboard input, and render to full screen.
   Output can generally be returned as a string.
   Currently hardcoded, but introduction of a coding language would technically
   allow them to be coded in that language.

*/ 

class Terminal;

class Terminal_Program
{
   public:
   bool active; /* true means the program is running */
   Terminal* terminal;
   
   virtual void render();
};

/* Write allows the user to write documents and programs. They can write
as much as they want, and then save with CTRL+S. This will prompt them
to write a file name. The Terminal will then save it in the root directory. */
class Program_Write: public Terminal_Program
{
   public:
   
   Program_Write();
   
};

/* Program that allows reading of files. Can scroll up/down with arrows
and search by typing */
class Program_Read: public Terminal_Program
{
   public:
   
   File* fileToRead;
   
   Program_Read();
   
   void render() override;

};

class Program_Breakout: public Terminal_Program
{
   public:
   
   unsigned char aBoard [48][64];
   
   int paddleX;
   int ballX, ballY;
   int ballDirection;
   int ballSpeed;
   
   Program_Breakout();
   
   void gameTick();
};

class Program_Tetris: public Terminal_Program
{
   public:
};

class Program_Dungeon: public Terminal_Program
{
   public:
   
};


#endif