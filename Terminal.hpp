#pragma once
#ifndef TERMINAL_TERMINAL_HPP
#define TERMINAL_TERMINAL_HPP

/* Terminal: Terminal
	#include "Terminal.hpp"
	
	The terminal menu. Keeps track of internal terminal state, and also handles all input/output.

	Will be expanded to include all kinds of fun stuff.
	
	Terminal will currently be hardcoded with 8x8 font, and 80 x  60 character space.
	
	We'll probably stick with an ASCII table of 256 characters.
	
	
	Intro idea: set all glyphs to random value, and cycle them randomly until they hit space.
	
*/

class Terminal: public GUI_Interface, public LogicTickInterface
//class Terminal
{
	
	char aGlyph [64][60];
	char* pGlyph;
	
	public:
	
	Terminal()
	{
		for (int _y=0;_y<60;++_y)
		{
			for (int _x=0;_x<64;++_x)
			{
				aGlyph[_x][_y] = '#';
			}
		}
		pGlyph = &aGlyph[0][0];
	}
	
	void init()
	{
		
	}
	
	void setGlyph ( int x, int y, char val)
	{
	}
	
	void randomFill()
	{
		// fill terminal with random glyphs.
		for (int i=0;i<3840;++i)
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
	

	randomFill();
	      //Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
   //int linesDrawn = font8x8.drawText(randomText,panelX1,panelY1,panelX2,panelY2,false,false,true,180,180,180);
  int linesDrawn = font8x8.drawText(&aGlyph[0][0],3840,panelX1,panelY1,panelX2,panelY2,false,false,true,180,180,180);
	}
	
	bool keyboardEvent(Keyboard* _keyboard)
	{
		return false;
	}
};


#endif
