#pragma once
#ifndef TERMINAL_EASI_HPP
#define TERMINAL_EASI_HPP

/* Terminal: EASI.hpp
   #include "EASI.hpp"
   
   The EASI language interpreter.
   
*/

#include <stack> // for shunting-yard algorithm

// Simple table to lookup variable names and values
// VarTable is held by EASI.
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
      // BASIC and EASI assume unknown variables are 0.
      return "0";
   }
   void clear()
   {
      vVarName.clear();
      vVarValue.clear();
   }
};

// CodeLine reads and parses lines of EASI code, but does not execute them.
class CodeLine
{
   public:
   std::string label; // The label of the line if applicable.
   std::string lineLabel; //Optional line label
   std::string keyword;
   std::string expression;
   // vector of expression tokens: +-*/^ strings, values, variables.
   // useful because some operators change if they are in a different
   // position, for example = can be comparator or assignment.
   Vector <std::string> vExpressionToken;
   
   std::string strLine; // full code line
   std::string errorMessage;
   std::string assignmentVar; // Only if expression is assignment.
   
   
   // Load and parse line of code.
   CodeLine(std::string _strLine)
   {
      std::cout<<"CodeLine: Loading line: "<<_strLine<<"\n";
      
      label="";
      lineLabel="";
      keyword="";
      expression="";
      strLine = _strLine;
      errorMessage="";
      
      assignmentVar="";
      
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
      
      // Build expression token list
      bool isString2=false;
      std::string strCurrent = "";
      std::string strCurrentVar = "";
      vExpressionToken.clear();
      if (expression.size() > 0)
      {
         for (unsigned int i=0;i<expression.size();++i)
         {
            if ( expression[i]=='"' )
            {
               isString2 = !isString2;
               strCurrent+=expression[i];
               
               if (strCurrent.size() > 1)
               {
                  vExpressionToken.push(strCurrent);
                  strCurrent="";
               }
               if (strCurrentVar.size() > 0)
               {
                  vExpressionToken.push(strCurrentVar);
                  strCurrentVar="";
               }
               
               continue;
            }
            
            if (isString2)
            {
               // build string
               strCurrent+=expression[i];
            }
            
            // push operators
            else if ( expression[i] == '+' || expression[i] == '-' || expression[i] == '*'
               || expression[i] == '/' || expression[i] == '>' || expression[i] == '<'
               || expression[i] == '=')
            {
               
               if (strCurrent.size() > 0)
               {
                  vExpressionToken.push(strCurrent);
                  strCurrent="";
               }
               if (strCurrentVar.size() > 0)
               {
                  vExpressionToken.push(strCurrentVar);
                  strCurrentVar="";
               }
               
               
               vExpressionToken.push(std::string(1,expression[i]));
            }
            else if (!isString2) // build var
            {
               strCurrentVar+=expression[i];
            }
         }
         
         // build any final vars
         if (strCurrent.size() > 0)
         {
            vExpressionToken.push(strCurrent);
            strCurrent="";
         }
         if (strCurrentVar.size() > 0)
         {
            vExpressionToken.push(strCurrentVar);
            strCurrentVar="";
         }
         
      }
      
      //Determine positional operators using expression token vector.
      // Strip assignment code and just keep assigment var.
      
      // Raw assignment expression
      if ( vExpressionToken.size() > 1)
      {
         if ( vExpressionToken(1) == "=" )
         {
            assignmentVar = vExpressionToken(0);
            vExpressionToken.eraseSlot(0);
            vExpressionToken.eraseSlot(0);
         }
      }
      
      // LET assignment expression
      if ( vExpressionToken.size() > 2)
      {
         if ( vExpressionToken(0) == "LET" && vExpressionToken(2) == "=" )
         {
            assignmentVar = vExpressionToken(1);
            vExpressionToken.eraseSlot(0);
            vExpressionToken.eraseSlot(0);
            vExpressionToken.eraseSlot(0);
         }
      }
   }
   
   ~CodeLine()
   { }
   
};

// Maintains VarTable, CodeLines, and executes the lines.
class EASI
{
   public:
   
   int currentLine;
   
   Vector <CodeLine*> vCodeLine;
   Vector <std::string> * vLine; // String for every line of the program, valid or not.
   
   bool terminated;
   
   VarTable varTable;
   
   EASI();
   
   std::string load(std::string _code); /* Load stuff like labels */
   
   std::string cycle(); /* Execute one cycle of the code */
   
   std::string evaluate(CodeLine* _codeLine); // Run code line in current state
   
   std::string shunt(std::string input); /* convert expression to postfix notation */

};


#endif