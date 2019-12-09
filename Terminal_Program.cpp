#pragma once
#ifndef TERMINAL_TERMINAL_PROGRAM_CPP
#define TERMINAL_TERMINAL_PROGRAM_CPP

/* Terminal: Terminal_Program
   #include "Terminal_Program.cpp"
   
*/

#include <File/FileManagerStatic.hpp>

#include "Terminal_Program.hpp"
#include "Terminal.hpp"

Terminal_Program::Terminal_Program()
{
   active=false;
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
   return "TEXT RETURN\n\nlol";
}
 
 void Terminal_Program::cycle()
 {
 }
 
void Terminal_Program::keyboardEvent (Keyboard* _keyboard)
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
      temp="";
      std::string _fileName = (*vArg)(1);
      if (_fileName.size() > 0 && DataTools::isAlphaNumeric(_fileName))
      {
         active = true;
         fileName=_fileName;
         
         if (FileManagerStatic::fileExists("storage/"+fileName))
         {
            temp = FileManagerStatic::load("storage/"+fileName);
         }
         return "";
      }
      else
      {
         temp="";
         fileName="";
         active=false;
         return "ERROR: FILENAME MUST BE ALPHANUMERIC\n";
      }
   }
   
   return "NO U\n";
}

std::string Program_Write::render()
{
   return temp;
}

void Program_Write::cycle()
{  
}

void Program_Write::keyboardEvent (Keyboard* _keyboard)
{
   if (_keyboard->keyWasPressed)
   {
      if ( _keyboard->isAlphaNumeric(_keyboard->lastKey) || _keyboard->lastKey == Keyboard::SPACE)
      {
         temp+=_keyboard->lastKey;
         _keyboard->clearAll();
      }
      else if (_keyboard->lastKey == Keyboard::ENTER)
      {
         temp+='\n';
         _keyboard->clearAll();
      }
      else if (_keyboard->lastKey == 19 ) // CTRL + S to save
      {
         std::string savePath = "storage/"+fileName;
         std::cout<<"Saving file as "<<savePath<<".\n";
         FileManagerStatic::writeFreshString(temp,savePath);
         active = false;
      }
      else if (_keyboard->lastKey == 8 ) // Backspace
      {
         if ( temp.size()>0)
         {
            temp.pop_back();
         }
         _keyboard->clearAll();
         
      }
   }
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

Program_Run::Program_Run()
{
   programName="RUN";
   fileToRead=0;
}

std::string Program_Run::init (Vector <std::string>* vArg)
{
   return "";
}

std::string Program_Run::render()
{
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