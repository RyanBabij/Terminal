#pragma once
#ifndef TERMINAL_TERMINAL_HPP
#define TERMINAL_TERMINAL_HPP

/* Terminal: Terminal
	#include "Terminal.hpp"
	
	The terminal menu. Keeps track of internal terminal state, and also handles all input/output.

	Will be expanded to include all kinds of fun stuff.
	
	Terminal is currently hardcoded with 10x10 font and 64x48 character area, in a 640x480 resolution.
	I'll probably need to add the ability to scale it up otherwise it'll look tiny on huge monitors.
	
	We'll probably stick with an ASCII table of 256 characters.
	
	
	Intro idea: set all glyphs to random value, and cycle them randomly until they hit space.
	
*/

class Terminal: public GUI_Interface, public LogicTickInterface
//class Terminal
{
	
	char aGlyph [48][64]; /* Beware. The x and y are  flipped here because C++ stores arrays in row major. */
	char* pGlyph;
	
	int cursorX,cursorY;
	int cursorBlink; /* counts up from zero */
	
	bool pause;
	
	int intro;
	
	public:
	
	Terminal()
	{
		pGlyph = &aGlyph[0][0];
		init();
	}
	
	void init()
	{
		int i=0;
		for (int _y=0;_y<48;++_y)
		{
			for (int _x=0;_x<64;++_x)
			{
					aGlyph[_y][_x] = ' ';
			}
		}
		
		// aGlyph[0][0] = '@';
		// aGlyph[0][1] = '@';
		// aGlyph[0][2] = '@';
		// aGlyph[47][0] = '@';
		// aGlyph[46][0] = '@';
		// aGlyph[45][0] = '@';
		
		cursorX = -1; cursorY = -1;
		cursorBlink = 0;
		pause=false;
		intro=0;
		
		putCursor(7,8);
		
		
		writeString(0,0, 	"                      .__                         ");
		writeString(0,1, "_____  ___.__.___.__. |  |   _____ _____    ____  ");
		writeString(0,2, "\\__  \\<   |  <   |  | |  |  /     \\\\__  \\  /  _ \\ ");
		writeString(0,3, " / __ \\\\___  |\\___  | |  |_|  Y Y  \\/ __ \\(  <_> )");
		writeString(0,4, "(____  / ____|/ ____| |____/__|_|  (____  /\\____/ ");
		writeString(0,5, "     \\/\\/     \\/                 \\/     \\/        ");
	


		writeString(0,10, "@@@@@@@@@@@@@@@@@&,//////,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		writeString(0,11, "@@@@@@@@@&@&/.@/////////////*,&////@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		writeString(0,12, "@@@@@@@&**///////////////////***********,@@@@@@@@@@@@@@@@@@@@@@");
		writeString(0,13, "@@@@@@**//////////////////////(***************/&@@@@@@@@@@@@@@@");
		writeString(0,14, "@@@@/***///////////****////////,****************** &@@@@@@@@@@@");
		writeString(0,15, "@@@,*****//////////////(////////*********************&@@@@@@@@@");
		writeString(0,16, "@@/***********./*///////**////*************************/@@@@@@@");
		writeString(0,17, "@/****************/////,***********.//////////////*******@@@@@@");
		writeString(0,18, "&*****************************/////////////////////********@@@@");
		writeString(0,19, "*****************************/////////////////////**********,&@");
		writeString(0,20, "***************************** ///////////////@/ **************@");
		writeString(0,21, "*****************,/////.**************///////*****************/");
		writeString(0,22, "****************/////////////.*********@///********************");
		writeString(0,23, "@***************//////////////////@****************************");
		writeString(0,24, "@@&**************(////////////////////////*********************");
		writeString(0,25, "@@&&********......******(//////////////////////@***************");
		writeString(0,26, "@@@@@,*****(.......*********** ///////////////////////*********");
		writeString(0,27, "@@@@@@@*****........****************,//////////////////////****");
		writeString(0,28, "@@@@@@@@/***.......(************************@///////////////@**");
		writeString(0,29, "@@@@@@@@@@,*........((...(......((..(.......(****@///////////**");
		writeString(0,30, "@@@@@@@@@@@ ................................(*********/////.***");
		writeString(0,31, "@@@@@@@@@@&..................................******************");
		writeString(0,32, "@@@@@@@@@@@*...................(..........********************&");
		writeString(0,33, "@@@@@@@@@@@@&.................*************************.&@@@@@@");
		writeString(0,34, "@@@@@@@@@@@@@@&&*....(.(......********************&&@@@@@@@@@@@");
		writeString(0,35, "@@@@@@@@@@@@@@@@@@@&/@@@/.((((**************/&@&@@@@@@@@@@@@@@@");



		
		writeString(0,7,"Welcome to AYYBBS");
		writeString(0,8,"Login:");
	}
	
	void setGlyph ( int x, int y, char val)
	{
	}
	
	void writeString(int _x, int _y, std::string _str)
	{
		for (unsigned int i=0;i<_str.length();++i)
		{
			if ( isSafe(_x,_y) )
			{
				aGlyph[_y][_x] = _str[i];
				++_x;
			}
		}
		// while (isSafe(_x,_y)
		// {
			// aGlyph[_y][_x] = 
			// ++_x;
		// }
	}
	
	void randomFill()
	{
		// fill terminal with random glyphs.
		for (int i=0;i<3072;++i)
		{
			pGlyph[i] = Random::randomInt(127);
			if (pGlyph[i] == '\n' ) { pGlyph[i] = ' '; }
		}
	}
	
	// Terminal only renders text, not any decoration.
	void render()
	{
		int centerX = panelX1 + (panelNX / 2);
		int centerY = panelY1 + (panelNY / 2);
	//	Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,&TEX_TERMINAL_BKG,false);
	
	// if (intro==0)
	// {
		// randomFill();
		// ++intro;
	// }
	// else
	// {
		// introStep();
	// }
	//rainDemo();
	blinkCursor();


	      //Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
   //int linesDrawn = font8x8.drawText(randomText,panelX1,panelY1,panelX2,panelY2,false,false,true,180,180,180);
  int linesDrawn = font8x8.drawText(&aGlyph[0][0],3072,panelX1,panelY1,panelX2,panelY2,false,false,true,180,180,180);
	}
	
	void putCursor(int _x, int _y)
	{
		if ( _x < 0 || _x > 63 || _y  < 0 || _y > 47)
		{
			_x = -1;
			_y = -1;
		}
		
		cursorX = _x;
		cursorY = _y;
		
		aGlyph[cursorY][cursorX] = 1;
	}
	
	void blinkCursor()
	{
		if (isSafe(cursorX,cursorY))
		{
			if (++cursorBlink > 30)
			{
				cursorBlink = 0;
			}
			if ( cursorBlink > 15)
			{
				aGlyph[cursorY][cursorX] = ' ';
			}
			else
			{
				aGlyph[cursorY][cursorX] = 1;
			}
		}

	}
	
	bool isSafe(int _x, int _y)
	{
		return ( _x >= 0 && _x <= 63 && _y  >= 0 && _y <= 47);
	}
	
	bool keyboardEvent(Keyboard* _keyboard)
	{
		if (_keyboard->keyWasPressed)
		{
			
			//pause = !pause;
			intro=0;
			
			_keyboard->clearAll();
			return true;
		}

		return false;
	}
	
	void introStep()
	{
		if ( intro < 11 )
		{
			// fill terminal with random glyphs.
			for (int i=0;i<3072;++i)
			{
				if (pGlyph[i] != ' ')
				{
				pGlyph[i] = Random::randomInt(127);
				if (pGlyph[i] == '\n' || Random::oneIn(4)) { pGlyph[i] = ' '; }
				}

			}
		}
		else
		{
			// fill terminal with random glyphs.
			for (int i=0;i<3072;++i)
			{
				pGlyph[i]= ' ';
			}
		}
		++intro;
	}
	
	// Return a random character, but not a newline.
	char getRandomChar()
	{
		char randChar = Random::randomInt(127);
		if (randChar == '\n') { return ' '; }
		return randChar;
	}
	
	void rainDemo()
	{
		//Add a rain
		if (Random::oneIn(3))
		{
			aGlyph[Random::randomInt(63)][0] = getRandomChar();
		}
		
		for (int _y=0;_y<48;++_y)
		{
			for (int _x=0;_x<64;++_x)
			{
				if(aGlyph[_y][_x]!= ' ')
				{
					//aGlyph[_x][_y] = ' ';
					if (_y < 47)
					{
						//aGlyph[_x][_y+1] = '#';
					}
				}
			}
		}
	}
};


#endif
