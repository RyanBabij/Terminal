#pragma once
#ifndef TERMINAL_PROGRAM_RUN_CPP
#define TERMINAL_PROGRAM_RUN_CPP

/* Terminal: Program_Run.cpp
   #include "Program_Run.cpp"
   
*/

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
               std::cout<<"EASI: Load\n";
               easi.load(fileContent);
               active=true;

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
         return "ERROR: FILENAME: "+fileName+" IS NOT ALPHANUMERIC 2\n";
      }
   }
   
   return "NO U\n";
}

void Program_Run::cycle() // for now this is being called directly before render()
{
   
   if ( easi.isWaitingInput == 0 )
   {
      isBusy=true;
      if (input.size() > 0 )
      {
         // make sure EASI gets the input.
         easi.input=input;
         input="";
      }
      
      for (int i=0;i<PROGRAM_CYCLES_PER_TICK;++i)
      {
         output+=easi.cycle();
      }
   }
   else
   {
      isBusy=false;
   }
   
   if (easi.terminated)
   {
      currentLine=0;
      active=false;
      isBusy=false;
   }
   
   return;

}

void Program_Run::keyboardEvent (Keyboard* _keyboard)
{
   if (!active) { return; }
   //std::string allowedInputs = " !@#$%^&*()\"\'\\=+-/";
   if (easi.isWaitingInput > 0 && _keyboard->keyWasPressed)
   {
      if (_keyboard->lastKey == Keyboard::ENTER)
      {
         easi.isWaitingInput--;
         std::cout<<"INPUT is: "<<input<<"\n";
         easi.vInput.push(input);
         input="";
         output+="\n";
         _keyboard->clearAll();
      }
      else if (_keyboard->lastKey == Keyboard::BACKSPACE)
      {
         if ( input.size() > 0 ) { input.pop_back(); output+='\b'; }
         _keyboard->clearAll();
      }
      // ANSI lets us pass backspaces, so we can just return whatever keys we recieve.
      else if (DataTools::isAlphaNumeric(_keyboard->lastKey))
      {
         output+=_keyboard->lastKey;
         input+=_keyboard->lastKey;
         _keyboard->clearAll();
         return;
      }
   }
}

// Program can return update in text mode, or entire screen in graphics mode.
// Output string should be wiped 
std::string Program_Run::render()
{
   //Protip: We can't necessarily return if inactive, because there may be a final render call
   // or batch cycles.
   // Intead just check if we have output to return.
   if ( output.size()==0 )
   { return ""; }
   
   // return a copy of output and wipe the output string.
   std::string retRender = output;
   output="";
   return retRender;
}

#endif