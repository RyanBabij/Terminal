#pragma once
#ifndef TERMINAL_PROGRAM_RUN_HPP
#define TERMINAL_PROGRAM_RUN_HPP

/* Terminal: Program_Run
   #include "Program_Run.hpp"
   
   RUN is an EASI interpreter.
   
   Instructions:
   
      PRINT - Prints text, variables, and/or strings.
      LET - Set a variable value. Variables followed by $ are strings.
      
   Render and CPU cycles are decoupled, although the C64 I think was mostly tied to clock rate.
   Render rate will probably be base framerate. However the amount of access to graphics RAM
   will be limited.
   
*/

/* Program that interprets files containing EASI language */
class Program_Run: public Terminal_Program
{
   public:
   

   Vector <CodeLine*> vCodeLine;
   
   EASI easi; /* EASI interpreter */
   

   VarTable varTable;
   
   File* fileToRead;
   std::string fileContent;
   int currentLine;
   
   std::string fileName;
   std::string output; /* Program output, typically from PRINT */
   std::string input; // Stores keyboard input during INPUT command
   
   Vector <std::string> * vLine; // String for every line of the program, valid or not.
   
   //Program_Run();
   Program_Run(Terminal * ptrTerminal);
   
   std::string init (Vector <std::string>* vArg) override;
   
   std::string render() override;
   
   //run the code for 1 emulated cycle returns possible output
   void cycle() override;
   //void execute(int lineNumber);
   
   void keyboardEvent (Keyboard* _keyboard) override;

};


#endif