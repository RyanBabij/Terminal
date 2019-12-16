#pragma once
#ifndef TERMINAL_TERMINAL_PROGRAM_CPP
#define TERMINAL_TERMINAL_PROGRAM_CPP

/* Terminal: Terminal_Program
   #include "Terminal_Program.cpp"
   
*/

#include <File/FileManagerStatic.hpp>
#include <File/FileManager.hpp>

#include "Terminal_Program.hpp"
#include "Terminal.hpp"

Terminal_Program::Terminal_Program(Terminal * termPtr)
{
   terminal=termPtr;
   active=false;
   programName="";
   graphicsMode=false;
}

// Terminal_Program::Terminal_Program()
// {
   // terminal=0;
   // active=false;
   // programName="";
   // graphicsMode=false;
// }


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
 
// Program_Write::Program_Write()
// {
   // graphicsMode=true;
   // programName="WRITE";
// }

Program_Write::Program_Write(Terminal * ptrTerminal): Terminal_Program(ptrTerminal)
{
   graphicsMode=true;
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
   if (!active) { return ""; }
   return temp;
}

void Program_Write::cycle()
{  
}

void Program_Write::keyboardEvent (Keyboard* _keyboard)
{
   if (!active) { return; }
   std::string allowedInputs = " !@#$%^&*()\"\'";
   if (_keyboard->keyWasPressed)
   {
      unsigned char c = _keyboard->lastKey;
      if (DataTools::isAlphaNumeric(c) || allowedInputs.find(c) != std::string::npos)
      {
         temp += c;
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
         
         FileManager::createDirectory("storage");
         
         FileManagerStatic::writeFreshString(temp,savePath);
         _keyboard->clearAll();
         active = false;
      }
      else if (_keyboard->lastKey == 3 ) /* CTRL+C to shutdown (passed up to Terminal)*/
      {
         active=false;
      }
      else if (_keyboard->lastKey == 8 ) // Backspace
      {
         if ( temp.size()>0)
         {
            temp.pop_back();
         }
         _keyboard->clearAll();
         
      }
         // CURSOR NAVIGATION
      else if (_keyboard->isPressed(Keyboard::LEFT))
      {
         std::cout<<"LEFT\n";
         //--cursorX;
         _keyboard->clearAll();
      }
      else if (_keyboard->lastKey == Keyboard::RIGHT)
      {
         std::cout<<"RIGHT\n";
         _keyboard->clearAll();
      }
      else if (_keyboard->lastKey == Keyboard::UP)
      {
         std::cout<<"UP\n";
         _keyboard->clearAll();
      }
      else if (_keyboard->lastKey == Keyboard::DOWN)
      {
         std::cout<<"DOWN\n";
         _keyboard->clearAll();
      }
   }
}
 

// Program_Read::Program_Read()
// {
   // graphicsMode=true;
   // programName="READ";
   // fileToRead=0;
// }

// std::string Program_Read::render()
// {
   // //if ( fileToRead==0 || terminal==0 )
   // if ( fileToRead==0 )
   // {
      // return "";
   // }
   
   // //RENDER THE OUTPUT.
   // //terminal->writeString(0,0,"AYY LMAO");
   // return "AYY LMAO";
// }

// Program_Run::Program_Run()
// {
   // graphicsMode=false; //We need to convert this to ASCII mode
   // programName="RUN";
   // fileToRead=0;
   // currentLine=0;
   // output="";
   // vLine=0;
// }

Program_Run::Program_Run(Terminal * term): Terminal_Program(term)
{
   graphicsMode=false; //We need to convert this to ASCII mode
   programName="RUN";
   fileToRead=0;
   currentLine=0;
   output="";
   vLine=0;
}

std::string Program_Run::init (Vector <std::string>* vArg)
{
   std::cout<<"RUN INIT\n";
   output="";
   active=false;
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
      fileContent="";
      std::string _fileName = (*vArg)(1);
      if (_fileName.size() > 0 && DataTools::isAlphaNumeric(_fileName))
      {
         fileName=_fileName;
         
         if (FileManagerStatic::fileExists("storage/"+fileName))
         {
            fileContent = FileManagerStatic::load("storage/"+fileName);
            if (fileContent.size()>0)
            {
               
               active=true;
               currentLine=0;
               
               //EASI CODE MUST ALWAYS BE IN  U P P E R C A S E
               for (auto & c: fileContent) c = toupper(c);
               
               // TOKENIZE CODE INTO LINES BY NEWLINE
               // TOKENISE EACH LINE INTO INSTRUCTIONS BY SPACES
               
               if ( vLine )
               {
                  vLine->clear();
                  delete vLine;
               }

               vLine = Tokenize::tokenize(fileContent,'\n');
               
               if ( vLine==0 )
               {
                  std::cout<<"ERROR: NO TOKENISE\n";
                  active=false;
                  currentLine=0;
                  return "";
               }
               
               for (int i=0;i<vLine->size();++i)
               {
                  std::cout<<i<<" "<<(*vLine)(i)<<"\n";
               }
               output="RUNNING PROGRAM\n";
               return output;
            }
         }
         return "ERROR: FILE DOESN'T EXIST\n";
      }
      else
      {
         fileContent="";
         fileName="";
         active=false;
         return "ERROR: FILENAME MUST BE ALPHANUMERIC\n";
      }
   }
   
   return "NO U\n";
}

void Program_Run::cycle()
{
   std::cout<<"RUN CYCLE\n";
   
   //output+="CYCLE\n";
   
   if (vLine==0)
   {
      std::cout<<"Error: No code vector\n";
      active=false;
      currentLine=0;
      return;
   }
   
   if ( currentLine >= vLine->size() )
   {
      std::cout<<"EXECUTION FINISHED\n";
      active=false;
      currentLine=0;
      return;
   }
   
   std::cout<<"Executing line "<<currentLine<<": "<<(*vLine)(currentLine)<<"\n";
   
   Vector <std::string> * vToken = Tokenize::tokenize((*vLine)(currentLine),' ');
   
   std::cout<<"Tokenized:\n";
   for (int i=0;i<vToken->size();++i)
   {
      std::cout<<(*vToken)(i)<<"\n";
   }
   
   if ( vToken->size()>0)
   {
      // we have at least a basic instruction
      
      if ( (*vToken)(0) == "PRINT" )
      {
         std::cout<<"PRINT FOUND\n";
         
         if ( vToken->size() >= 2 )
         {
            std::cout<<"PRINTING: "<<(*vToken)(1)<<".\n";
            output += (*vToken)(1)+"\n"; 
         }
         else
         {
            // RETURN ERROR
            output += "ERROR";
            //error("ERROR");
         }
      }
      
      
   }
   
   ++currentLine;
   

}

void Program_Run::execute(int lineNumber)
{
   
}

std::string Program_Run::render()
{
   // interpret and run program here.
   
   if (active)
   {
      output="";
      cycle();
      std::cout<<"Current render state: "<<output<<"\n";
      return output;
   }
   
   return output;
}

// Program_Breakout::Program_Breakout()
// {
   // programName="GAME1";
   // paddleX=10;
   // ballX=10;
   // ballY = 20;
   // ballDirection = 0;
   // ballSpeed=0;
// }

// void Program_Breakout::cycle()
// {
   // std::cout<<"cycle\n";
// }

// std::string Program_Breakout::render()
// {
   // return "";
// }
   

#endif