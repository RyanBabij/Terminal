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
   
   PROGRAM MODES
      There are three modes:
         CONSOLE MODE
         All output goes to console output. This is very simple mode which
         just uses PRINT command and returns string on render calls.
         CURSES MODE
         Program directly modifies ANSI buffer. Program is passed array of 48x64.
         GRAPHICS MODE
         Program draws pixels to screenspace. Program is passed array of 480x640
         (or potentially a smaller array depending on performance considerations).
         120x160 would be a good smaller size.
   
   RENDER
      We can achieve this by simply passing the array to swap.
      
   PROGRAM OVERVIEW
   
      WRITE
      Very basic text editor. Type the name of the file you want to write... For example:
      "write file" will create a file called file. Typing the name of an already existing file
      will edit that file. CTRL+S will exit the application.
*/ 

class Terminal;

class Terminal_Program
{
   public:
   Terminal * terminal; /* Link to Terminal to call system functions and access RAM */
   
   
   bool active; /* true means the program is running */
   //Terminal* terminal;
      /* If true, Terminal will constantly refresh render,
         otherwise render calls will be cumulative */
   bool graphicsMode;
   
   //bool isWaitingInput; // If the program is in ASCII mode, it must be able to flag for input.
   bool isBusy; // flag to terminal to indicate work
   
   std::string programName;
   
   Terminal_Program(Terminal*  test);
   //Terminal_Program();
   virtual ~Terminal_Program();
   
   /* Pass the commands written after the program name.
   Return empty string if init successful, otherwise return error message */
   virtual std::string init (Vector <std::string>* vArg);
   
   // Render and logic cycle are decoupled but will typically run at the same rate. */
   virtual std::string render(); /* render at set framerate */
   virtual void cycle(); /* advance 1 game cycle. Maybe return a status. */
   
   virtual void keyboardEvent (Keyboard*);
};

/* Write allows the user to write documents and programs. They can write
as much as they want, and then save with CTRL+S. This will prompt them
to write a file name. The Terminal will then save it in the root directory. */
class Program_Write: public Terminal_Program
{
   public:
   
   File * currentFile;
   std::string temp;
   std::string fileName;
   
   //Program_Write();
   Program_Write(Terminal * ptrTerminal);
   ~Program_Write();
   
   std::string init (Vector <std::string>* vArg) override;
   
   std::string render() override;
   void cycle() override;
   
   void keyboardEvent (Keyboard*) override;
};

// /* Program that allows reading of files. Can scroll up/down with arrows
// and search by typing */
// class Program_Read: public Terminal_Program
// {
   // public:
   
   // File* fileToRead;
   
   // Program_Read();
   // Program_Read(Terminal * ptrTerminal);
   
   // std::string render() override;

// };

#include "Program_Run.hpp"

/*
Let's start by just drawing a dot bouncing around the screen.
*/
// class Program_Breakout: public Terminal_Program
// {
   // public:
   
   // unsigned char aBoard [48][64];
   
   // int paddleX;
   // int ballX, ballY;
   // int ballDirection;
   // int ballSpeed;
   
   // Program_Breakout();
   
   // std::string render() override;
   
   // void cycle() override;
   
// };

// class Program_Tetris: public Terminal_Program
// {
   // public:
// };

// class Program_Dungeon: public Terminal_Program
// {
   // public:
   
// };


#endif