#pragma once
#ifndef TERMINAL_DRIVER_INIT_HPP
#define TERMINAL_DRIVER_INIT_HPP

/* Terminal: Driver_Init.hpp
  #include "Driver_Init.hpp"

Driver initialization goes here.

Initialize main menus, start timers, initialise any important data.

*/

#include <Algorithm/Shunting.cpp>

#include "Driver_InitServers.hpp"

void init()
{   
  Random::seed();

   //Makes cout faster but less reliable
   if ( FAST_COUT )
   {
      std::ios_base::sync_with_stdio(false);
      std::cin.tie(NULL);
   }
   
   globalKeyboard.init(); 

   /* Load textures (Tex pointers from Driver_TextureList.hpp). */
   loadTextures();
  
   /* Load font */
   Png fontPng;
   int fileSize;
   unsigned char* fileData = FileManager::getFile("Textures/Font/10x10/10x10 white v4.png",&fileSize);   
   
   if ( fileData == 0 )
   {
      std::cout<<"ERROR: Font PNG did not load.\n";
   }
   else
   {   
      fontPng.load(fileData,fileSize);
      if(font8x8.loadData(&fontPng,10,10)==false)
      { std::cout<<"ERROR: Font did not load.\n"; }
      delete [] fileData;
   }
   
   


   /* Start timers. */
   frameRateTimer.init();
   frameRateTimer.start();
   pollRateTimer.init();
   pollRateTimer.start();
   logicRateTimer.init();
   logicRateTimer.start();
   physicsRateTimer.init();
   physicsRateTimer.start();
  animationTimer.init();
  animationTimer.start();
   debugTimer.init();
   debugTimer.start();
  
   playerKeypressTimer.init();
   playerKeypressTimer.start();
   
   gameTimer.init();
   gameTimer.start();

   // /* Initialise the main menu */
   menuTitle.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
   menuTitle.init();
   menuTitle.setFont(&font8x8);
   menuTitle.backgroundTexture=&TEX_TERMINAL;
   menuTitle.active=true;

   mouseInterfaceManager.add(&menuTitle);
   globalGuiManager.add(&menuTitle);
   
   initServers();
   
   std::cout<<"Running test cases\n";
   
   // SHUNT TEST CASES
   //Shunting shunt;
   //shunt.buildDefaults();
   //shunt.shunt("-1+(1-2)*(4/2)");
   //shunt.test();
   
   // EASI TEST CASES
   std::cout<<" *** EASI TEST CASES ***\n";
   EASI easi;
   std::cout<<"loading\n";
   
   std::string strTestProg = FileManagerStatic::load("testprog");
   
   
   // std::string prog1 = "10 TEST\n
   // 20 PRINT \"HELLO\"\n
   // ";
   
   easi.load(strTestProg);
   std::cout<<"end loading\n";
   
   int i=0;
   
   while (easi.terminated==false && i++ < 1000)
   {
      std::string strCycle = easi.cycle();
      std::cout<<"cycle output: "<<strCycle<<"\n";
   }
   
   std::cout<<"TESTING DONE, EXITING\n";
   exit(0);
   
}

#endif
