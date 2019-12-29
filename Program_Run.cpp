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
               
               
               
               // currentLine=0;
               
               // //EASI CODE MUST ALWAYS BE IN  U P P E R C A S E
               // for (auto & c: fileContent) c = toupper(c);
               
               // // TOKENIZE CODE INTO LINES BY NEWLINE
               // // TOKENISE EACH LINE INTO INSTRUCTIONS BY SPACES
               
               // if ( vLine )
               // {
                  // vLine->clear();
                  // delete vLine;
               // }

               // vLine = Tokenize::tokenize(fileContent,"\n\r");
               
               // if ( vLine==0 )
               // {
                  // std::cout<<"ERROR: NO TOKENISE\n";
                  // active=false;
                  // currentLine=0;
                  // return "";
               // }
               
               // std::cout<<"Loading program:\n";
               // // Load up each line.
               // for (int i=0;i<vLine->size();++i)
               // {
                  // vCodeLine.push(new CodeLine((*vLine)(i)));
                  // std::cout<<i<<" "<<(*vLine)(i)<<"\n";
               // }
               
               // std::cout<<"CODE LOADED SUCCESSFULLY\n";
               
               // std::cout<<"All labels:\n";
               
               // for (int i=0;i<vCodeLine.size();++i)
               // {
                  // if ( vCodeLine(i)->label != "" )
                  // {
                     // std::cout<<vCodeLine(i)->label<<"\n";
                  // }
               // }

               
               // output="RUNNING PROGRAM\n";
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

void Program_Run::cycle() // for now this is being called directly before render()
{
   output += easi.cycle();
   
   if (easi.terminated)
   {
      currentLine=0;
      active=false;
   }
   
   return;
   
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
   
   if (currentLine < vCodeLine.size())
   {
      std::cout<<" Vcodeline: "<<vCodeLine(currentLine)->strLine<<".\n";
   }
   
   std::string strCurrentLine = (*vLine)(currentLine);
   Vector <std::string> * vToken = Tokenize::tokenize(strCurrentLine,' ');
   
   if (vToken==0)
   {
      std::cout<<"NULL TOKEN\n";
      return;
   }
   
   if ( vToken->size()>0)
   {
      // we have at least a basic instruction
      std::string instruction = (*vToken)(0);
      
      if ( instruction == "PRINT" )
      {
         std::cout<<" PRINT \n";
         
         // replace all instances of "\\n" with '\n'.
         size_t index = 0;
         while (true) //https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
         {
              /* Locate the substring to replace. */
              index = strCurrentLine.find("\\N", index);
              if (index == std::string::npos) break;

              /* Make the replacement. */
              strCurrentLine.replace(index, 2, "\n");

              /* Advance index forward so the next iteration doesn't pick it up as well. */
              index += 2;
         }
         
         //Print only strings enclosed in "
         // The other tokens are variables.
         Vector <std::string> * vPrint = Tokenize::tokenize(strCurrentLine,'"');
         
         if ( vPrint==0 ) { return; }
         
         std::cout<<"All tokens:\n";
         for (int i=0;i<vPrint->size();++i)
         {
            if ( i%2==0 ) // even
            {
               std::cout<<"Getting var: "<<(*vPrint)(i)<<"\n";
               output+=varTable.get( (*vPrint)(i) );
            }
            else //odd
            {
               output+=(*vPrint)(i);
            }
            //std::cout<<(*vPrint)(i)<<"\n";
            //output+=(*vPrint)(i);
         }
         
         for (int i=1;i<vPrint->size();i+=2)
         {
            output+=(*vPrint)(i);
         }
         delete vPrint;
      }
      else if (instruction == "REM")
      {
         std::cout<<" REM \n";
      }
      else if (instruction == "LABEL")
      {
         std::cout<<" LABEL \n";
      }
      else if (instruction == "GOTO")
      {
         std::cout<<" GOTO \n";
         if (vToken->size() > 1)
         {
            std::string strJumpTarget = (*vToken)(1);
            std::cout<<"JUMPING TO "<<strJumpTarget<<".\n";
            for (int i=0;i<vCodeLine.size();++i)
            {
               if ( vCodeLine(i)->label == strJumpTarget )
               {
                  std::cout<<vCodeLine(i)->label<<"\n";
                  currentLine = i;
               }
            }
            
         }

      }
      else if (DataTools::isNumeric(instruction))
      {
         std::cout<<" LINE NUM \n";
      }
      else if (instruction=="END")
      {
         std::cout<<" END \n";
         output+="END OF PROGRAM\n";
         active=false;
         currentLine=0;
         return;
      }
      else if (instruction == "LET")
      {
         std::cout<<" LET \n";
         
         // for now just require the following syntax:
         // LET VAR = VALUE
         // LET VAR$ = "STRING"
         // LET VAR = VALUE1 <+-/*> VALUE2
         // Therefore there can be 4 or 6 tokens
         if (vToken->size() > 3)
         {
            if ( (*vToken)(2) == "=" )
            {
               if ( DataTools::isAlphaNumeric( (*vToken)(1) ) )
               {
                  if ( DataTools::isNumeric( (*vToken)(3) ) )
                  {
                     std::cout<<"VARIABLE "<<(*vToken)(1)<<" WILL EQUAL "<<(*vToken)(3)<<".\n";
                     varTable.set( (*vToken)(1), (*vToken)(3) );
                  }
               }
               
            }
         }
         
      }
      else if (instruction == "INPUT" )
      {
      }
      else
      {
         // If no keyword can be found, then it may be an expression. In this case we basically just use the "LET" code.
         // We need to create an EASI module which takes lines of code.
         std::cout<<" INVALID \n";
      }
      
      
   }
   else
   {
      std::cout<<" NO TOKE \n";
   }
   
   ++currentLine;
   

}

// void Program_Run::execute(int lineNumber)
// {
   
// }

// Program can return update in text mode, or entire screen in graphics mode.
// Output string should be wiped 
std::string Program_Run::render()
{
   // interpret and run program here.
   
   // return a copy of output and wipe the output string.
   std::string retRender = output;
   output="";
   if (active)
   {
      //std::cout<<"rendtest\n";
      //return "test\n";
      return retRender;
   }
   
   return "";
}

#endif