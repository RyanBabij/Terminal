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

std::string Terminal_Program::init (Vector <std::string> vArg)
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

std::string Program_Write::init (Vector <std::string> vArg)
{
   return "";
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