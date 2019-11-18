#pragma once
#ifndef TERMINAL_TERMINAL_PROGRAM_CPP
#define TERMINAL_TERMINAL_PROGRAM_CPP

/* Terminal: Terminal_Program
   #include "Terminal_Program.cpp"
   
*/ 

#include "Terminal_Program.hpp"
#include "Terminal.hpp"



 void Terminal_Program::render()
 {
 }
 
Program_Write::Program_Write()
{  
}

Program_Read::Program_Read()
{
   fileToRead=0;
}

void Program_Read::render()
{
   if ( fileToRead==0 || terminal==0 )
   {
      return;
   }
   
   //RENDER THE OUTPUT.
   terminal->writeString(0,0,"AYY LMAO");
   
}

Program_Breakout::Program_Breakout()
{
   paddleX=10;
   ballX=10;
   ballY = 20;
   ballDirection = 0;
   ballSpeed=0;
}

void Program_Breakout::gameTick()
{
   
}
   

#endif