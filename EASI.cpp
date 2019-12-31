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
   //EASI CODE MUST ALWAYS BE IN  U P P E R C A S E
   _code = DataTools::toUpper(_code);
   
   std::cout<<"EASI:load\n\n";
   std::cout<<_code<<"\n\n";
   
   terminated=false;
   
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
   
   // Tokenise by newline
   vLine = Tokenize::tokenize(_code,"\n\r");
   
   if ( vLine==0 )
   {
      currentLine=0;
      return "";
   }

   // Load up each line.
   for (int i=0;i<vLine->size();++i)
   {
      vCodeLine.push(new CodeLine((*vLine)(i)));
   }
   
   // Debug output of each CodeLine
   for (int i=0;i<vCodeLine.size();++i)
   {
      std::cout<<"\nLine "<<i<<": "<<vCodeLine(i)->strLine<<" -> "<<vCodeLine(i)->strLineStripped<<"\n";
      std::cout<<"           Label:  "<<vCodeLine(i)->label<<"\n";
      std::cout<<"       Linelabel:  "<<vCodeLine(i)->lineLabel<<"\n";
      std::cout<<"         Keyword:  "<<vCodeLine(i)->keyword<<"\n";
      std::cout<<"  Assignment var:  "<<vCodeLine(i)->assignmentVar<<"\n";
      std::cout<<"      Expression:  "<<vCodeLine(i)->expression<<"\n";
      std::cout<<"             Arg:  "<<vCodeLine(i)->arg<<"\n";
      
      
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
   if ( _codeLine == 0 )
   {
      return "ERROR: Null CodeLine ptr\n";
   }

   std::cout<<"Executing line: "<<_codeLine->strLineStripped<<"\n";

   // 1: Sub all variables into expressions, replace uninitialized vars with 0 or null string.
   // 2: Evaluate expression, assign if necessary.
   //    There's no need to worry about combining strings and expressions except with PRINT,
   //    as mixing of var types is forbidden.
   //    todo: Account for positive/negative prefixes, which can be found either at the
   //    start of an expression, or following other operators.
   // 3: Evaluate and execute keywords.

   // sub all variables using token vector
   //  should be moved into a function
   Vector <std::string> vSubbedToken = _codeLine->vExpressionToken;
   for (int i=0;i<vSubbedToken.size();++i)
   {
      if ( varTable.isRealVar(vSubbedToken(i)) ) // get variable as-is
      {
         vSubbedToken(i) = varTable.get(vSubbedToken(i));
      }
      else if ( varTable.isStringVar(vSubbedToken(i)) ) // add quotes to sub so we know it's a string
      {
         vSubbedToken(i) = "\"" + varTable.get(vSubbedToken(i)) + "\"";
      }
   }
   
      // Assignment expression
   if ( _codeLine->assignmentVar != "" )
   {
      // rebuild subbed expression into string
      std::string strEvalExpression = "";
      for (int i=0;i<vSubbedToken.size();++i)
      {
         strEvalExpression += vSubbedToken(i);
      }
      
      // real variable evaluation
      if ( varTable.isRealVar(_codeLine->assignmentVar) )
      {
         shunt.shunt(strEvalExpression);
         std::string strResult = DataTools::toString(shunt.evaluate());
         varTable.set(_codeLine->assignmentVar,DataTools::toString(strResult));
      }
      // string variable evaluation
      else if (varTable.isStringVar(_codeLine->assignmentVar) )
      {
         // assigning string vars is simpler because there's only the + operator
         bool isString = false;
         std::string strVarAssignment = "";
         
         for (unsigned int i=0;i<strEvalExpression.size();++i)
         {
            if ( strEvalExpression[i] == '"' )
            {
               isString = !isString;
               continue;
            }
            
            if ( isString )
            {
               strVarAssignment += strEvalExpression[i];
            }
         }
         
         varTable.set(_codeLine->assignmentVar,strVarAssignment);
      }

   std::cout<<varTable.toString();
   }
   // IF <expression> THEN <linenumber>
   // or:
   // IF <expression> THEN <command>
   else if ( _codeLine->keyword == "IF" )
   {
      // evaluate everything between IF and THEN.
      std::cout<<"EVALUATING IF\n";
      
      // sub in vars
      // sub all variables using token vector
      // vSubbedToken.clear();
      // vSubbedToken = _codeLine->vExpressionToken;
      // for (int i=0;i<vSubbedToken.size();++i)
      // {
         // if ( varTable.isRealVar(vSubbedToken(i)) ) // get variable as-is
         // {
            // vSubbedToken(i) = varTable.get(vSubbedToken(i));
         // }
         // else if ( varTable.isStringVar(vSubbedToken(i)) ) // add quotes to sub so we know it's a string
         // {
            // vSubbedToken(i) = "\"" + varTable.get(vSubbedToken(i)) + "\"";
         // }
      // }
      // rebuild subbed expression into string
      std::string strEvalExpression = "";
      for (int i=0;i<vSubbedToken.size();++i)
      {
         strEvalExpression += vSubbedToken(i);
      }
      
      shunt.shunt(strEvalExpression);
      std::string strResult = DataTools::toString(shunt.evaluate());
      //varTable.set(_codeLine->assignmentVar,DataTools::toString(strResult));
      
      std::cout<<"RESULT: "<<strResult<<"\n";
      
      // -1 is true, execute arg.
      if (strResult == "-1")
      {
         std::cout<<"TRUE, EXECUTING ARG: "<<_codeLine->arg<<"\n";
         // if arg is a number, jump to that label
         
         if (DataTools::isNumeric(_codeLine->arg))
         {
            std::cout<<"JUMP TO LINE : "<<_codeLine->arg<<"\n";
            jumpToLabel(_codeLine->arg);
         }
         
         // if arg is not a number, execute the line like a normal code line.
      }
      else
      {
         std::cout<<"FALSE, CONTINUING\n";
      }

      // 0 is false, continue next line.
      
   }
   else if ( _codeLine->keyword == "GOTO" )
   {
      jumpToLabel(_codeLine->expression);
   }
   else if ( _codeLine->keyword == "PRINT" )
   {
      // PRINT can return strings, evaluate an expression, or a combination of both.
      // Strings can be string var or text enclosed in quotes.
      // adjacent strings are automatically appended, even without a + operator
      // Any operations between strings and non-strings are invalid.
      // PRINT will also evaluate normal expressions.
      
      // rebuild subbed expression into string
      std::string strEvalExpression = "";
      
      std::string expressionToEvaluate="";
      
      bool lastWasString = false;
      bool appendingString = false;
      
      for (int i=0;i<vSubbedToken.size();++i)
      {
         // replace all instances of "\\n" with '\n'.
         size_t index = 0;
         while (true) //https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
         {
              /* Locate the substring to replace. */
              index = vSubbedToken(i).find("\\N", index);
              if (index == std::string::npos) break;

              /* Make the replacement. */
              vSubbedToken(i).replace(index, 2, "\n");

              /* Advance index forward so the next iteration doesn't pick it up as well. */
              index += 2;
         }
         
         //number variable
         if (DataTools::isNumeric(vSubbedToken(i)))
         {
            std::cout<<"numeric\n";
            //if last was string, then invalid operation.
            if (appendingString) //invalid
            {
               return "ERROR\n";
            }
            else
            {
               //strEvalExpression+=vSubbedToken(i);
               expressionToEvaluate+=vSubbedToken(i);
            }
         }
         else if (vSubbedToken(i).size()>1 && vSubbedToken(i)[0] == '"' && vSubbedToken(i).back()=='"')
         { // is a string
            //this is a string, only appending is possible.
            
            
            //EVALUATE EXPRESSION
            if (expressionToEvaluate.size() > 0)
            {
               //EVALUATE
               shunt.shunt(expressionToEvaluate);
               std::string strResult = DataTools::toString(shunt.evaluate());
               std::cout<<"     APPEND: "<<strResult<<"\n";
               strEvalExpression+=strResult;
               expressionToEvaluate="";
            }
            
            // remove first and last char
            strEvalExpression+=vSubbedToken(i).substr(1, vSubbedToken(i).size() - 2);
            
            //Strings can only be followed by + and another string, another string, or a number.
            lastWasString=true;
            appendingString=false;
         }
         else // operator or invalid.
         {
            expressionToEvaluate+=vSubbedToken(i);
         }
      }
      
      //FINAL CASE EVALUATE EXPRESSION
      if (expressionToEvaluate.size() > 0)
      {
            //EVALUATE
            shunt.shunt(expressionToEvaluate);
            std::string strResult = DataTools::toString(shunt.evaluate());
            std::cout<<"     APPEND2: "<<strResult<<"\n";
            strEvalExpression+=strResult;
            expressionToEvaluate="";
      }
      return strEvalExpression;
   }
   
   
   return "";

   
   

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


void EASI::jumpToLabel(std::string _label)
{
   for (int i=0;i<vCodeLine.size();++i)
   {
      if ( vCodeLine(i)->label == _label || vCodeLine(i)->lineLabel == _label )
      {
         currentLine = i;
      }
   }
}

#include <list> 
#include <iterator> 

//#include <Algorithm/Shunting.cpp>

// std::string EASI::shunt(std::string _input)
// {
   // //Shunting shunt;
   // shunt.shunt(_input);
   
   // std::cout<<"EASI: shunted: "<<shunt.toString()<<".\n";
   // return shunt.toString();

// }

#endif      