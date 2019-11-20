#pragma once
#ifndef TERMINAL_TERMINAL_PROGRAM_CPP
#define TERMINAL_TERMINAL_PROGRAM_CPP

/* Terminal: Terminal_Program
   #include "Terminal_Program.cpp"
   
*/ 

#include "Terminal_Program.hpp"
#include "Terminal.hpp"

Terminal_Program::Terminal_Program()
{
   programName="";
}

Terminal_Program::~Terminal_Program()
{
}

std::string Terminal_Program::init (Vector <std::string>* vArg)
{
   return "Error: This is a virtual function.";
}

std::string Terminal_Program::render()
{
   return "";
}
 
 void Terminal_Program::cycle()
 {
 }
 
Program_Write::Program_Write()
{
   programName="WRITE";
}

Program_Write::~Program_Write()
{
}

std::string Program_Write::init (Vector <std::string>* vArg)
{
   if (vArg==0)
   {
      return "INTERNAL ERROR: NULL ARGS\n";
   }
   
   if (vArg->size()<2)
   {
      return "Error: No filename specified\n";
   }
   else
   {
      std::string _fileName = (*vArg)(1);
      std::cout<<"fname "<<_fileName<<".\n";
      if (_fileName.size() > 0 && DataTools::isAlphaNumeric(_fileName))
      {
         return "";
      }
      else
      {
         return "ERROR: FILENAME MUST BE ALPHANUMERIC\n";
      }
   }
   
   return "NO U\n";
}

std::string Program_Write::render()
{
   return "";
}

void Program_Write::cycle()
{  
}

Program_Read::Program_Read()
{
   programName="READ";
   fileToRead=0;
}

std::string Program_Read::render()
{
   //if ( fileToRead==0 || terminal==0 )
   if ( fileToRead==0 )
   {
      return "";
   }
   
   //RENDER THE OUTPUT.
   //terminal->writeString(0,0,"AYY LMAO");
   return "AYY LMAO";
}

Program_Breakout::Program_Breakout()
{
   programName="GAME1";
   paddleX=10;
   ballX=10;
   ballY = 20;
   ballDirection = 0;
   ballSpeed=0;
}

void Program_Breakout::cycle()
{
   std::cout<<"cycle\n";
}

std::string Program_Breakout::render()
{
   return "";
}
   

#endif