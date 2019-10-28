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

//class Terminal: public GUI_Interface, public LogicTickInterface
class Terminal
{
	public:
	
	Terminal()
	{
	}
	
	void setGlyph ( int x, int y, char val)
	{
	}
	
	void randomFill()
	{
		// fill terminal with random glyphs.
	}
	
	
};


#endif
