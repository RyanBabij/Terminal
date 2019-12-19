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
   
   class CodeLine
   {
      public:
      Vector <std::string> * vToken;
      std::string label; // The label of the line if applicable.
      
      std::string strLine; // full code line
      
      CodeLine(std::string _strLine)
      {
         label="";
         //strLine="";
         
         std::string strCurrentLine = _strLine;
         vToken = Tokenize::tokenize(strCurrentLine,' ');
         
         strLine = strCurrentLine;
         
         if (vToken==0)
         {
            return;
         }
   
         if ( vToken->size()>0)
         {
            // we have at least a basic instruction
            std::string instruction = (*vToken)(0);
            
            if ( instruction == "PRINT" )
            {

            }
            else if (instruction == "REM")
            {
            }
            else if (instruction == "LABEL")
            {
               if (vToken->size()>1)
               {
                  label = (*vToken)(1);
               }
            }
            else if (instruction == "GOTO")
            {
            }
            else if (DataTools::isNumeric(instruction))
            {
               label=instruction;
            }
            else
            { //invalid
            }
         }
      }
      
      void process(std::string _line)
      {
      }
      
      ~CodeLine()
      {
         std::cout<<"Codeline died\n";
      }
   };
   Vector <CodeLine*> vCodeLine;
   
   // Simple table to lookup variable names and values
   class VarTable
   {
      public:
      Vector <std::string> vVarName;
      Vector <std::string> vVarValue;
      
      VarTable()
      {
      }
      
      // Automatically adds the variable or updates it as required.
      void update(std::string _varName, std::string _varValue)
      {
         
         for (int i=0;i<vVarName.size();++i)
         {
            if ( vVarName(i) == _varName )
            {
               vVarValue(i) = _varValue;
               return;
            }
         }
         vVarName.push(_varName);
         vVarValue.push(_varValue);
      }
      std::string get(std::string _varName)
      {
         for (int i=0;i<vVarName.size();++i)
         {
            if (vVarName(i) == _varName)
            {
               return vVarValue(i);
            }
         }
         return "";
      }
      void clear()
      {
         vVarName.clear();
         vVarValue.clear();
      }
   };
   VarTable varTable;
   
   File* fileToRead;
   std::string fileContent;
   int currentLine;
   
   std::string fileName;
   std::string output; /* Program output, typically from PRINT */
   
   Vector <std::string> * vLine; // String for every line of the program, valid or not.
   
   //Program_Run();
   Program_Run(Terminal * ptrTerminal);
   
   std::string init (Vector <std::string>* vArg) override;
   
   std::string render() override;
   
   //run the code for 1 emulated cycle returns possible output
   void cycle() override;
   void execute(int lineNumber);

};


#endif