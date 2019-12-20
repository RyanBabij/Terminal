#pragma once
#ifndef TERMINAL_EASI_HPP
#define TERMINAL_EASI_HPP

/* Terminal: EASI.hpp
   #include "EASI.hpp"
   
   The EASI language interpreter.
   
*/

class CodeLine
{
   public:
   Vector <std::string> * vToken;
   std::string label; // The label of the line if applicable.
   std::string lineLabel; //Optional line label
   std::string keyword;
   std::string expression;
   
   std::string strLine; // full code line
   
   std::string errorMessage;
   
   CodeLine(std::string _strLine)
   {
      label="";
      lineLabel="";
      keyword="";
      expression="";
      strLine = _strLine;
      errorMessage="";
      
      // Evaluate the line
      bool isLineLabel = true;
      bool isKeyword = false;
      bool isExpression = false;
      for (unsigned int i=0;i<_strLine.size();++i)
      {
         if (isLineLabel) // line labels can only be at the beginning of a string.
         {
            if ( DataTools::isAlpha(_strLine[i]))
            {
               // switch to keyword mode.
               isLineLabel=false;
               isKeyword=true;
               // if ( lineLabel.size() > 0 )
               // {
                  
               // }
            }
            else if (DataTools::isNumeric(_strLine[i]))
            {
               lineLabel+=_strLine[i];
            }
            else
            {
               lineLabel="";
               isLineLabel=false;
               //invalid code.
            }
         }
         if (isKeyword) // keywords can only be alpha
         {
            if ( DataTools::isAlpha(_strLine[i]))
            {
               // build and check keyword. max keyword size is 7.
               // check all keywords at once, otherwise we need to good
               // back to evaluate expression.

               if (_strLine.rfind("PRINT",i,5) == i)
               {
                  std::cout<<"FOUND PRINT\n";
               }
               else if (_strLine.rfind("REM",i,3) == i)
               {
                  std::cout<<"FOUND REM\n";
               }
            }
            else if (DataTools::isNumeric(_strLine[i]))
            {
               isKeyword=false;
               isExpression=true;
            }
            else
            {
               lineLabel="";
               isLineLabel=false;
               //invalid code.
            }
         }
         if (isExpression) //expression is evaluated externally
         {
         }
         

         
         
         
      }
      
      vToken = Tokenize::tokenize(_strLine,' ');
      
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

class EASI
{
   public:
   
   EASI();
   
   std::string load(std::string _code); /* Load stuff like labels */
   
   std::string cycle(); /* Execute one cycle of the code */
   
   int currentLine;
   
   Vector <CodeLine*> vCodeLine;
   Vector <std::string> * vLine; // String for every line of the program, valid or not.
};


#endif