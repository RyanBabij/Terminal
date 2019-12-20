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
      std::cout<<"EASI: Evaluating line: "<<_strLine<<"\n";
      
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
      
      // A line that starts with numbers  has a line number label
      // A line that starts with characters is either a keyword or expression
      // (an expression must start with a variable or keyword, and neither can contain numbers)
      // If the line does not have a valid keyword, it will be evaluated as an expression
      // If the expression is invalid, the code line is invalid.
      // These rules mean that EASI and BASIC are whitespace independent,
      // and you can strip all whitespace (except strings) and assume every char is significant
      //_strLine = DataTools::strip(_strLine,"\n\r");
      
      // The main catch is that a colon indicates a new line of code, but this is easy to deal with.
      // Simply replace every colon (not in a string) with a newline
      
      std::string _strNew = "";
      bool isString=false;
      // Strip all spaces which aren't between quotes
      for (unsigned int i=0;i<_strLine.size();++i)
      {
         if ( _strLine[i] == '\"' )
         {
            isString=!isString;
            _strNew+=_strLine[i];
         }
         else if ( _strLine[i] != ' ' )
         {
            _strNew+=_strLine[i];
         }
      }
      _strLine=_strNew;
      
      std::cout<<"EASI: Stripped line: "<<_strLine<<"\n";
      
      for (unsigned int i=0;i<_strLine.size();++i)
      {
         if (isLineLabel) // line labels can only be at the beginning of a string.
         {
            if ( DataTools::isAlpha(_strLine[i]))
            {
               // switch to keyword mode.
               isLineLabel=false;
               isKeyword=true;
            }
            else if (DataTools::isNumeric(_strLine[i]))
            {
               lineLabel+=_strLine[i];
            }
            else //invalid char, or maybe expression. Go back to start and try evaluate as expression.
            {
               lineLabel="";
               i=0;
               isLineLabel=false;
               isExpression=true;
            }
         }
         if (isKeyword) // keywords can only be alpha
         {
               if (_strLine.rfind("PRINT",i,5) == i)
               {
                  keyword = "PRINT";
                  i+=5;
               }
               else if (_strLine.rfind("REM",i,3) == i)
               {
                  keyword = "REM";
                  i+=3;
               }
               else if (_strLine.rfind("LET",i,3) == i)
               {
                  keyword = "LET";
                  i+=3;
               }
               else if (_strLine.rfind("END",i,3) == i)
               {
                  keyword = "END";
                  i+=3;
               }
               else if (_strLine.rfind("LABEL",i,5) == i)
               {
                  keyword = "LABEL";
                  i+=5;
               }
               isKeyword=false;
               isExpression=true;
         }
         if (isExpression) //expression is evaluated externally
         {
            const std::string allowedInputs = " !@#$%^&*()\"\'\\=+-/";
            if (DataTools::isAlphaNumeric(_strLine[i]) || allowedInputs.find(_strLine[i]) != std::string::npos)
            {
               expression+=_strLine[i];
            }
         }
      }
      
      std::cout<<"EASI RESULTS\n";
      std::cout<<"Label: "<<label<<".\n";
      std::cout<<"Keyword: "<<keyword<<".\n";
      std::cout<<"Expression: "<<expression<<".\n";
      
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