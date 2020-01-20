#pragma once
#ifndef TERMINAL_DRIVER_INIT_HPP
#define TERMINAL_DRIVER_INIT_HPP

/* Terminal: Driver_Init.hpp
  #include "Driver_Init.hpp"

Driver initialization goes here.

Initialize main menus, start timers, initialise any important data.

*/

#include <Math/Shunting/Shunting.cpp>

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
   unsigned char* fileData = FileManager::getFile("Textures/Font/8x8/8x8 Transparent v4 plus junk white.png",&fileSize);   
   
   if ( fileData == 0 )
   {
      std::cout<<"ERROR: Font PNG did not load.\n";
   }
   else
   {   
      fontPng.load(fileData,fileSize);
      if(font8x8.loadData(&fontPng,8,8)==false)
      { std::cout<<"ERROR: Font did not load.\n"; }
      delete [] fileData;
   }
   
   


   /* Start timers. */
   frameRateTimer.init();
   frameRateTimer.start();
   frameRateTimer2.init();
   frameRateTimer2.start();
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
   
   //SHUNT TEST CASES
   Shunting shunt;
   shunt.buildDefaults();
      // some errors to fix
   //shunt.shunt("-(1*2*-3)-(5+5)+-(2)");
   //shunt.shunt("-ABS(-100)=-ABS(100-200)");
   //shunt.shunt("--ABS(-100)=ABS(100-200)");
   //shunt.shunt("ABS(-100)=ABS(-50)-100");
   shunt.shunt("RND(100)<RND(100)");
   //shunt.evaluate();


   std::cout<<"Shunt: "<<shunt.toString()<<"\n";
   shunt.evaluate();
   
   //exit(0);
   
   //#define EASI_TEST_CASES
   #ifdef EASI_TEST_CASES

   std::cout<<" *** EASI TEST CASES ***\n";
   EASI easi;
   
   std::string strTestProg = FileManagerStatic::load("storage/LOOP");

   easi.load(strTestProg);

   for (int i=0; easi.terminated==false && i < 50; ++i)
   {
      std::string strCycle = easi.cycle();
   }

   std::cout<<"TESTING DONE, EXITING\n";
   exit(0);
   
   #endif
   
}

#endif
