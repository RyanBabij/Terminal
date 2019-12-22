#pragma once
#ifndef TERMINAL_EASI_CPP
#define TERMINAL_EASI_CPP

/* Terminal: EASI.cpp
   #include "EASI.cpp"
   
   Implementation of the EASI language interpreter.
   
*/

#include "EASI.hpp"

EASI::EASI()
{
   currentLine=0;
}

std::string EASI::load(std::string _code)
{
   
   //EASI CODE MUST ALWAYS BE IN  U P P E R C A S E
   _code = DataTools::toUpper(_code);
   
   // TOKENIZE CODE INTO LINES BY NEWLINE
   // TOKENISE EACH LINE INTO INSTRUCTIONS BY SPACES
   
   if ( vLine )
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
      std::cout<<"ERROR: NO TOKENISE\n";
      currentLine=0;
      return "";
   }
   
   std::cout<<"Loading program:\n";
   // Load up each line.
   for (int i=0;i<vLine->size();++i)
   {
      vCodeLine.push(new CodeLine((*vLine)(i)));
      std::cout<<i<<" "<<(*vLine)(i)<<"\n";
   }
   return "";
}

std::string EASI::cycle()
{
   //run the current line of code
   
   if (vCodeLine.isSafe(currentLine)==false)
   { return "END OF PROGRAM\n"; }


//run line

   return "";
}

#endif      