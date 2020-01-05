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
   
   // return true if var is a valid numeric or string variable name
   bool isValid(std::string _varName)
   {
      if (_varName.size() == 0 )
      { return false; }
      
      for (unsigned int i=0;i<_varName.size()-1;++i)
      {
         if ( _varName[i] < 65 || _varName[i] > 90 )
         {
            return false;
         }
      }
      
      if ( _varName.back() == '$'
        || (_varName.back() > 64 && _varName.back() < 91) )
      { return true; }
 
      
      return false;
   }
   
   // real vars must be uppercase alpha characters only
   bool isRealVar ( std::string _varName )
   {
      if ( _varName.size() == 0 )
      { return false; }
      
      for (unsigned int i=0;i<_varName.size();++i)
      {
         if ( _varName[i] < 65 || _varName[i] > 90 )
         {
            return false;
         }
      }
      
      return true;
   }

   // string vars must be uppercase alpha and end with a $
   bool isStringVar (std::string _varName)
   {
      if (_varName.size() == 0 )
      { return false; }
      
      for (unsigned int i=0;i<_varName.size()-1;++i)
      {
         if ( _varName[i] < 65 || _varName[i] > 90 )
         {
            return false;
         }
      }
      
      if ( _varName.back() != '$' )
      { return false; }
 
      
      return true;
   }
   
   
   // Automatically adds the variable or updates it as required.
   void set(std::string _varName, std::string _varValue)
   {
      std::cout<<"Set "<<_varName<<" -> "<<_varValue<<".\n";
      
      for (int i=0;i<vVarName.size();++i)
      {
         if ( vVarName(i) == _varName )
         {
            if ( DataTools::isNumeric(_varValue)==false && isRealVar(_varName) )
            {
               // convert invalid values to 0.
               vVarValue(i) = "0";
            }
            else
            { vVarValue(i) = _varValue;
            }
            
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
   
   std::string toString()
   {
      std::string strRet = "";
      for (int i=0;i<vVarName.size();++i)
      {
         strRet+=" "+vVarName(i)+": "+vVarValue(i)+"\n";
      }
      
      return strRet;
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
   std::string arg; // Commands or sub-code to be executed
   // vector of expression tokens: +-*/^ strings, values, variables.
   // useful because some operators change if they are in a different
   // position, for example = can be comparator or assignment.
   Vector <std::string> vExpressionToken;
   
   std::string strLine; // full code line
   std::string strLineStripped; // above but with non-string whitespace removed.
   std::string errorMessage;
   std::string assignmentVar; // Only if expression is assignment.
   
   
   // Load and parse line of code.
   CodeLine(std::string _strLine)
   {
      std::cout<<"Loading line: "<<_strLine<<"\n";
      
      label="";
      lineLabel="";
      keyword="";
      expression="";
      arg="";
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
         else if (isString==true || _strLine[i] != ' ' )
         {
            _strNew+=_strLine[i];
         }
      }
      _strLine=_strNew;
      strLineStripped=_strNew;
      
      std::cout<<"Stripped to: "<<strLineStripped<<"\n";
      
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
               else if (_strLine.rfind("IF",i,2) == i)
               {
                  std::size_t found = _strLine.find("THEN",i+2);
                  
                  // IF is only valid if followed by a THEN
                  // Code between IF and THEN becomes an expression
                  // Code after THEN becomes an arg.
                  // THEN may be followed by a number, in which case EASI
                  // jumps to that line if true, or continues if false.
                  // THEN may instead be followed by a command like PRINT,
                  // which executes if true, and does not execute if false.
                  if ( found != std::string::npos )
                  {
                     std::cout<<" IF CONTAINS A THEN\n";
                     keyword = "IF";
                     i+=2;
                     expression = _strLine.substr(i,found-i);
                     std::cout<<"EXPRESSOIN: "<<expression<<"\n";
                     i=found+4;
                     
                     arg = _strLine.substr(i);
                     
                     break;
                  }
                  else
                  {
                     std::cout<<" IF BUT NO THEN\n";
                  }
                  
                  
                  // expression is everything between IF and THEN.
                  // after THEN can be line number or command.
                  //keyword = "IF";
                  
                  
               }
               // REM - Remark
               // A comment line, should be ignored by EASI.
               else if (_strLine.rfind("REM",i,3) == i)
               {
                  keyword = "REM";
                  i+=3;
                  return;
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
               else if (_strLine.rfind("GOTO",i,4) == i)
               {
                  keyword = "GOTO";
                  i+=4;
               }
               else if (_strLine.rfind("INPUT",i,5) == i)
               {
                  std::cout<<"Found INPUT\n";
                  keyword = "INPUT";
                  i+=5;
                  
                  // INPUT may contain a string, which must be terminated with a ;.
                  // INPUT may then contain multiple input variables, separated with commas.
                  // The first expression token will be the output string.
                  // Any following tokens will be variables to assign.
                  
                  std::string strOutput = "";
                  
                  if ( _strLine.size() > 5 )
                  {
                     if ( _strLine[5] == '\"' )
                     {
                        std::cout<<"INPUT contains a string\n";
                        bool validOutput = false;
                        //build the output string until we find terminating quote
                        for (i=6;i<_strLine.size();++i)
                        {
                           if ( _strLine[i] == '\"' )
                           {
                              // output string is built.
                              validOutput=true;
                              ++i;
                              break;
                           }
                           else
                           {
                              strOutput+=_strLine[i];
                           }
                        }
                        if ( validOutput==false)
                        {
                           std::cout<<"Invalid output\n";
                           // ERROR
                           strOutput="";
                           vExpressionToken.clear();
                           return;
                        }
                     }
                  }
                  std::cout<<"INPUT string built\n";
                  
                  //check for terminating semicolon
                  if (i < _strLine.size() && _strLine[i] == ';')
                  {
                     std::cout<<"Advancing past semicolon\n";
                     ++i;
                  }
                  
                  // push output string, even if it's a null string.
                  // so EASI can always print index 0.
                  std::cout<<"Pushing output string: "<<strOutput<<"\n";
                  vExpressionToken.push(strOutput);
                  
                  // build all vars, which should be separated by commas.
                  // There must be at least 1 variable, otherwise the line
                  // is invalid.
                  // all chars should be alpha or comma.
                  
                  std::string currentVarName = "";
                  for (;i<_strLine.size();++i)
                  {
                     std::cout<<"current i: "<<i<<"\n";
                     if ( _strLine[i] == ',' )
                     {
                        // add var
                        if ( currentVarName.size() > 0 )
                        {
                           std::cout<<"Pushing var: "<<currentVarName<<"\n";
                           vExpressionToken.push(currentVarName);
                           currentVarName="";
                        }
                     }
                     else if (DataTools::isAlpha(_strLine[i]) || _strLine[i]=='$') // build var
                     {
                        currentVarName+=_strLine[i];
                     }
                     else //invalid code
                     {
                        // ERROR
                        std::cout<<"Error building var list. Fail char was: "<<(int)_strLine[i]<<"\n";
                        vExpressionToken.clear();
                        return;
                     }
                  }
                  // add var
                  if ( currentVarName.size() > 0 )
                  {
                     std::cout<<"Pushing var final: "<<currentVarName<<"\n";
                     vExpressionToken.push(currentVarName);
                     currentVarName="";
                  }

                  return;
               }
               //else
               //{
                  isKeyword=false;
                  isExpression=true;
               //}
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
               || expression[i] == '=' || expression[i] == '(' || expression[i] == ')')
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
      if ( keyword=="" && vExpressionToken.size() > 1)
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

#include <Algorithm/Shunting.cpp>

// Maintains VarTable, CodeLines, and executes the lines.
class EASI
{
   public:
   
   Shunting shunt; // Constructs with default operators
   
   int currentLine;
   
   Vector <CodeLine*> vCodeLine;
   Vector <std::string> * vLine; // String for every line of the program, valid or not.
   
   bool terminated;
   short int isWaitingInput; // 0 = not waiting for input. other values are the number of inputs to get.
   Vector <std::string> vInputVar; // variables to put input into
   Vector <std::string> vInput; // user input from INPUT command. Can hold multiple inputs.
   std::string input; // holds current input.
   
   VarTable varTable;
   
   EASI();
   
   std::string load(std::string _code); // Load code into the CodeLine Vector
   
   std::string cycle(); // Execute one cycle of the code, return any output.
   
   std::string evaluate(CodeLine* _codeLine); // Run code line in current state
   
   void jumpToLabel(std::string _label);
   
   //std::string shunt(std::string input); // convert expression to postfix notation

};


#endif