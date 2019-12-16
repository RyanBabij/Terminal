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

#endif