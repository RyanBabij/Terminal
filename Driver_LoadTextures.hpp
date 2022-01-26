#pragma once
#ifndef TERMINAL_LOAD_TEXTURES_HPP
#define TERMINAL_LOAD_TEXTURES_HPP

#include <string>

#define THREADED_TEXTURE_LOADING
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  #include <thread>
#endif

#include <Graphics/Texture/TextureLoader.hpp>
#include <Graphics/Texture/Texture.hpp>

/* WorldSim: Driver_LoadTextures.hpp
  #include "Driver_LoadTextures.hpp"

   Called from Driver::init().
   These vars are global.
   All texture refs start with TEX_, and then use the path and filename from the textures folder.
   Requires OpenGL headers.
*/

void SetColor(int value){
#ifdef WILDCAT_WINDOWS
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  value);
#endif
}



static Texture TEX_TERMINAL;
static Texture TEX_TERMINAL_GRID;
static Texture TEX_TERMINAL_BKG;

static Texture TEX_MCOM_CITY;
static Texture TEX_MCOM_CITY_RIP;
static Texture TEX_MCOM_MISSILE;
static Texture TEX_MCOM_MISSILE_RIGHT;
static Texture TEX_MCOM_MISSILE_RIP;

Texture texRuntime; /* Test of runtime graphics creation. */


void loadTextureVerbose(const std::string _path, Texture* _texture)
{
   std::cout<<"Loading: "<<_path<<".\n";
   
   if(loadTextureNearestNeighbour(_path,_texture)==false)
   { std::cout<<"Error loading "<<_path<<".\n"; }
}

void preloadTextureVerbose(const std::string _path, Texture* _texture)
{

   
   if(loadTextureNearestNeighbour(_path,_texture)==false)
   {
   SetColor(4);
    std::cout<<"Loading: "<<_path<<": FAIL.\n";
  SetColor(7);
  }
  else
  {
    std::cout<<"Loading: "<<_path<<": SUCCESS.\n";
  }

}


void loadTextures() // Testing some multithreading here. Probably shouldn't because a texture atlas would be better
{
		if(const char* env_p = std::getenv("WILDCAT_TEXTURES"))
	std::cout << "Your PATH is: " << env_p << '\n';

static std::string texPath = std::getenv("WILDCAT_TEXTURES");
texPath+="\\Terminal\\";
std::cout<<"texpath: "<<texPath<<"\n";
  
//#undef THREADED_TEXTURE_LOADING
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  std::thread t1( []
  {
#endif
      // LOAD MENU TEXTURES
    preloadTextureVerbose(texPath+"HaruhiTerminal2.png",&TEX_TERMINAL);
    preloadTextureVerbose(texPath+"43Grid.png",&TEX_TERMINAL_GRID);
    preloadTextureVerbose(texPath+"Background5.png",&TEX_TERMINAL_BKG);
	 
    // preloadTextureVerbose("Textures/Game/cityt2.png",&TEX_MCOM_CITY);
    // preloadTextureVerbose("Textures/Game/cityrip.png",&TEX_MCOM_CITY_RIP);
    // preloadTextureVerbose("Textures/Game/missilet2.png",&TEX_MCOM_MISSILE);
    // preloadTextureVerbose("Textures/Game/missilert.png",&TEX_MCOM_MISSILE_RIGHT);
    // preloadTextureVerbose("Textures/Game/missileript.png",&TEX_MCOM_MISSILE_RIP);

#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  });
  
  std::thread t2( []
  {
#endif

#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  });
#endif
    
    
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  t1.join();
  t2.join();
#endif

  bindNearestNeighbour(&TEX_TERMINAL,COMPRESS_TEXTURES);
  bindNearestNeighbour(&TEX_TERMINAL_GRID,COMPRESS_TEXTURES);
  bindNearestNeighbour(&TEX_TERMINAL_BKG,COMPRESS_TEXTURES);
  
  texRuntime.create(320,200,0,true);

// load font
   /* Load font */
   Png fontPng;
   int fileSize;
   //unsigned char* fileData = FileManager::getFile(texPath+"Font/8x8/8x8 Transparent v4 plus junk white.png",&fileSize);   
   unsigned char* fileData = FileManager::getFile(texPath+"Font/8x8/C64-6.png",&fileSize);   
   
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
    
}

#endif
