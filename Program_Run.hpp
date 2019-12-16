#pragma once
#ifndef TERMINAL_PROGRAM_RUN_HPP
#define TERMINAL_PROGRAM_RUN_HPP

/* Terminal: Program_Run
   #include "Program_Run.hpp"
   
   RUN is an EASI interpreter.
   
   Instructions:
   
      PRINT - Prints text, variables, and/or strings.
      LET - Set a variable value. Variables followed by $ are strings.
   
*/

/* Program that interprets files containing EASI language */
class Program_Run: public Terminal_Program
{
   public:
   
   File* fileToRead;
   std::string fileContent;
   int currentLine;
   
   std::string fileName;
   std::string output; /* Program output, typically from PRINT */
   
   Vector <std::string> * vLine;
   
   //Program_Run();
   Program_Run(Terminal * ptrTerminal);
   
   std::string init (Vector <std::string>* vArg) override;
   
   std::string render() override;
   
   //run the code for 1 emulated cycle returns possible output
   void cycle() override;
   void execute(int lineNumber);

};


#endif