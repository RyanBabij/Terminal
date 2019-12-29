#pragma once
#ifndef TERMINAL_EASI_CPP
#define TERMINAL_EASI_CPP

/* Terminal: EASI.cpp
   #include "EASI.cpp"
   
   Implementation of the EASI language interpreter.
   
*/

#include "EASI.hpp"

#include <queue> // shunting alorithm

EASI::EASI()
{
   currentLine=0;
   terminated=false;
   vLine=0;
}

std::string EASI::load(std::string _code)
{
   std::cout<<"EASI:load\n";
   std::cout<<_code<<"\n\n";
   
   terminated=false;
   
   //EASI CODE MUST ALWAYS BE IN  U P P E R C A S E
   _code = DataTools::toUpper(_code);
   
   // TOKENIZE CODE INTO LINES BY NEWLINE
   // TOKENISE EACH LINE INTO INSTRUCTIONS BY SPACES
   
   if ( vLine != 0 )
   {
      vLine->clear();
      delete vLine;
      vLine=0;
   }

   for (int i=0;i<vCodeLine.size();++i)
   {
      delete vCodeLine(i);
      vCodeLine(i)=0;
   } vCodeLine.clear();
   
   vLine = Tokenize::tokenize(_code,"\n\r");
   
   if ( vLine==0 )
   {
      currentLine=0;
      return "";
   }
   
   std::cout<<"Loading program:\n";
   // Load up each line.
   
   std::cout<<"Codelines loaded:\n";
   for (int i=0;i<vLine->size();++i)
   {
      vCodeLine.push(new CodeLine((*vLine)(i)));
      std::cout<<i<<" "<<(*vLine)(i)<<"\n";
   }
   
   for (int i=0;i<vCodeLine.size();++i)
   {
      std::cout<<"\nLine "<<i<<":\n";
      std::cout<<"           Label:  "<<vCodeLine(i)->label<<"\n";
      std::cout<<"       Linelabel:  "<<vCodeLine(i)->lineLabel<<"\n";
      std::cout<<"         Keyword:  "<<vCodeLine(i)->keyword<<"\n";
      std::cout<<"  Assignment var:  "<<vCodeLine(i)->assignmentVar<<"\n";
      std::cout<<"      Expression:  "<<vCodeLine(i)->expression<<"\n";
      
      
      std::cout<<"Expression tokens: ";
      for (int i2=0; i2<vCodeLine(i)->vExpressionToken.size(); ++i2)
      {
         std::cout<<vCodeLine(i)->vExpressionToken(i2)<<" ";
      } std::cout<<"\n";
      
   }
   return "";
}

std::string EASI::cycle()
{
   //run the current line of code
   
   if (vCodeLine.isSafe(currentLine)==false)
   {
      std::cout<<"EASI: End of program\n";
      terminated=true;
      currentLine=0;
      return "END OF PROGRAM\n";
   }
   return evaluate (vCodeLine(currentLine++));
}

std::string EASI::evaluate(CodeLine* _codeLine)
{   
   // varTable.update("A","100");
   // varTable.update("B","2");
   // varTable.update("C","33");

   //Step 1: Sub all variables to prevent problems down the line with string variables
      // variables are alphabet chars with no numbers or symbols, so they're easy to identify.
      // strings end with a $.
   //Step 2: Sort strings and expressions between strings.
   //Step 3: Evaluate each expression.
   //Step 4: Append results and strings and return final result.
   
   // strip strings and build vector of sub-expressions.
   // expression always gets pushed first to preserve order.
   
   //std::cout<<"EASI: Evaluating: "<<_strExpression<<".\n";
   
   if ( _codeLine==0 )
   { return "ERROR"; }

   std::string _str = "";
   std::string _strSubExpression = "";
   
   Vector <std::string> _vString;
   Vector <std::string> _vSubExpression;
   
   bool isString=false;
   // we need to sub all variables here to prevent confusion with string variables in expressions.
   // variables are alphabetical, so we just scan for any alphabetical chars.
   // in future we must make sure we aren't reading in keywords or functions
   // BASIC and EASI set unknown variables to 0, this includes throwing /0 errors.
   std::string _strSubbedCode = "";
   std::string _strVar = "";
   
         // need to account for assignment expressions here.
         // assignment can have a single lvalue, but multiple rvalues.
   std::string _strAssignment = ""; // Variable to assign if required.
   //bool isLvalue=true; // only true for first element.
   
   std::string _strExpression = _codeLine->expression;
   
   
   // step 1: determine if this is an assignment expression.
   // This can only happen if there is no keyword, or the keyword is LET
   
   Vector <std::string> expressionTokens; // vector of tokens in expression
   
   
   // Raw expression
   if (_codeLine->keyword=="" ) // raw expression must have assignment, or it doesn't do anything
   {
      // split expression into tokens. A variable assignment cannot mix types.
      // string assignment may only contain strings separated by + operator.
      // integer/real assignment may contain expressions.
      
      if (_codeLine->assignmentVar.size() > 0 )
      {
         if (_codeLine->assignmentVar.back() == '$' )
         {
            // string assignment
            // string expressions may only have strings and + operator.
         }
         else
         {
            // var assignment
         }
      }
      

   }


   
   
   //Step 0: figure out if this is an assignment expression, for example:
   // A = B or
   // VAR = 123+456*2
   // Assignment expressions may only use a single =.
   // Invalid assignments still run in BASIC. Assignment appears to become a comparator,
   // with true returning -1 (11111111 11111111) TRUE
   // and false returning 0 (0000000 00000000) FALSE
   // TRUE appears to be a special case which converts to value -1. (10000000 00000001).
   // however this is unlikely to appear in normal code.

   
   // Step 1: Sub in all variable values (except strings and assignment variable).
   for (unsigned int i=0;i<_strExpression.size();++i)
   {
      if ( _strExpression[i] == '\"' ) // flip between expression and string
      {
         isString = !isString;
         _strSubbedCode += '\"';
         continue;
      }
         
      if ( isString ) // ignore
      {
         _strSubbedCode += _strExpression[i];
      }
      else // scan for variables
      {
         // check is alpha
         //check is dollarsign
         
         if ( DataTools::isAlpha(_strExpression[i]) )
         {
            _strVar+=_strExpression[i];
         }
         else if (_strExpression[i] == '$')
         {
            //STRING VAR
            // pull from vartable
            //std::cout<<"Stringvar\n";
            _strVar+='$';
            _strSubbedCode += varTable.get(_strVar);
            _strVar="";
         }
         else
         {
            // variable terminates.
            if (_strVar != "")
            {
               // pull from vartable.
               _strSubbedCode += varTable.get(_strVar);
               _strVar="";
            }
            _strSubbedCode += _strExpression[i];
         }

      }

   }
   //end case: final var
   if ( _strVar.size() > 0 )
   {
      _strSubbedCode += varTable.get(_strVar);
   }
   
   //create sub expression and string vector.
   
   isString=false;
   for (unsigned int i=0;i<_strExpression.size();++i)
   {
      if ( _strExpression[i] == '\"' ) // flip between expression and string
      {
         isString = !isString;
         
         if ( isString ) // push the expression
         {
            _vSubExpression.push(_strSubExpression);
            _strSubExpression="";
         }
         else // push the string
         {
            _vString.push(_str);
            _str="";
         }
      }
      else if (isString)
      {
         _str += _strExpression[i];
      }
      else
      {
         _strSubExpression += _strExpression[i];
      }
   }
   _vSubExpression.push(_strSubExpression);
   
   std::cout<<"Sub expressions\n";
   for (int i=0;i<_vSubExpression.size();++i)
   {
      std::cout<<_vSubExpression(i)<<"\n";
      //convert each sub expression with shunting
      
      std::stack <std::string> stkValue;
      std::stack <std::string> stkOperator;
      
      std::queue <std::string> qVal;
      
      
   }
   
   std::cout<<"Sub strings\n";
   for (int i=0;i<_vString.size();++i)
   {
      std::cout<<_vString(i)<<"\n";
   }

   // EVALUATE SUBEXPRESSIONS

   // std::cout<<"Sub expressions\n";
   // for (int i=0;i<_vSubExpression.size();++i)
   // {
      // std::cout<<_vSubExpression(i)<<"\n";
      // std::string shunted = shunt(_vSubExpression(i));
      // std::cout<<"Shunted to: "<<shunted<<"\n";
   // }
   

   return _strExpression+"\n"+_strSubbedCode+"\n";

}

#include <list> 
#include <iterator> 

#include <Algorithm/Shunting.cpp>

std::string EASI::shunt(std::string _input)
{
   
   Shunting shunt;
   shunt.shunt(_input);
   
   std::cout<<"EASI: shunted: "<<shunt.toString()<<".\n";
   return shunt.toString();

}

#endif      