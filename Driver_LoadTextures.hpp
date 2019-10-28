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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  value);
}


static Texture TEX_TERMINAL;
static Texture TEX_TERMINAL_GRID;
static Texture TEX_TERMINAL_BKG;


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
  
//#undef THREADED_TEXTURE_LOADING
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  std::thread t1( []
  {
#endif
      // LOAD MENU TEXTURES
    preloadTextureVerbose("Textures/HaruhiTerminal2.png",&TEX_TERMINAL);
    preloadTextureVerbose("Textures/43Grid.png",&TEX_TERMINAL_GRID);
    preloadTextureVerbose("Textures/Background5.png",&TEX_TERMINAL_BKG);

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

  bindNearestNeighbour(&TEX_TERMINAL);
  bindNearestNeighbour(&TEX_TERMINAL_GRID);
  bindNearestNeighbour(&TEX_TERMINAL_BKG);

    
}

#endif
