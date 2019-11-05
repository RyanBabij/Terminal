#pragma once
#ifndef TERMINAL_TERMINAL_HPP
#define TERMINAL_TERMINAL_HPP

/* Terminal: Terminal
	#include "Terminal.hpp"
	
	The terminal menu. Keeps track of internal terminal state, and also handles all input/output.

	Will be expanded to include all kinds of fun stuff.
	
	Terminal is currently hardcoded with 10x10 font and 64x48 character area, in a 640x480 resolution.
	I'll probably need to add the ability to scale it up otherwise it'll look tiny on huge monitors.
	
	We'll probably stick with an ASCII table of 256 characters. 128 standard and 128 extended.
	Not sure about colour and other modifiers.

	There are plans for multiple versions, and 2 or 3 different vendors. Basically to introduce different
	specs/cosmetics/programs/vulns etc.
	
	CONNECT.  There should be 7, 10, 12, 19, or 22 digits, depending on whether using city extension number,
	and whether including a calling code. CONNECT also returns 0 for failed connection, and 1 for
	successful connection.
	
	AUTODIAL. Dials random numbers until a connection is made.
	
	
			ANSI ESCAPE NOTES
			
	The ANSI escape sequences are very important for terminals. They allow colours, and also things like
	loading indicators and inputs.
	
	ANSI codes are stripped before being pushed to Terminal otherwise corruption might screw things up.
	
*/

#include <string>

/* Audio stuffs. */
#include <Audio/Sound.hpp> // Generic sound data handler
#include <Audio/Wav.hpp>
#include <Audio/AudioPlayer.hpp> // Generic audio player
#include <Audio/AudioPlayer_OpenAL.hpp>
/* Global OpenAL audio player. */
AudioPlayer_OpenAL globalAudioPlayer;
/* Dial tones. */
Sound * dialTone[10];
Sound* sRing;

#include <Time/Timer.hpp>
Timer toneTimer; /* Keep track of when last tone was played */

#include <Data/Tokenize.hpp> // Tokenize console commands
#include <Data/DataTools.hpp> // Check commands

#include "Server.hpp"
#include "Operator.hpp"
Operator op;
	
#include <String/ANSI.hpp>
	
class Terminal: public GUI_Interface, public LogicTickInterface
{
	
	char aGlyph [48][64]; /* Beware. The x and y are  flipped here because C++ stores arrays in row major. */
	char* pGlyph;
	
	char aGlyphBacklog[48][64]; /* Character to be loaded onto the screen*/
	char* pGlyphBacklog;
	int loadX, loadY; /* current position the loader is at */
	
	char aCorrupt[48][64]; /* corrupted characters go here */
	char* pCorrupt;
	
	Colour foregroundColour [48][64]; /* Keep track of colour for this glyph */
	
	int cursorX,cursorY;
	int cursorBlink; /* counts up from zero */
	
	bool pause;
	
	int intro;
	
	bool bootScreen; /* Main boot screen */
	bool helpScreen; /* Shows list of commands */
	bool debugConsole; /* Access special features of browser */
	
	Vector <std::string> vPackets; /* Log of packets sent/recieved */
	Vector <std::string> vFile; /* Files can just be text strings */
	
	std::string dialTones; /* Dialtones to play */
	std::string command; /* command the user has typed */
	
	public:
	
	Terminal()
	{
		pGlyph = &aGlyph[0][0];
		pGlyphBacklog = &aGlyphBacklog[0][0];
		pCorrupt = &aCorrupt[0][0];
		init();
		bootSystem1();
		
		dialTones="";
	}
	~Terminal()
	{
		globalAudioPlayer.close();
	}
	
	void init()
	{
		int i=0;
		for (int _y=0;_y<48;++_y)
		{
			for (int _x=0;_x<64;++_x)
			{
					aGlyph[_y][_x] = ' ';
					aGlyphBacklog[_y][_x] = ' ';
					aCorrupt[_y][_x] = ' ';
			}
		}

		cursorX = -1; cursorY = -1;
		cursorBlink = 0;
		pause=false;
		intro=0;
		debugConsole=false;
		bootScreen=true;
		helpScreen=false;
		vPackets.clear();
		command = "";
		
		loadX=0; loadY=0;
		
		putCursor(0,0);
		//bootSystem1();
		
		frameRateTimer.init();
		frameRateTimer.start();
		
		loadAudio();
	}
	
	void loadAudio()
	{
		globalAudioPlayer.init();
		globalAudioPlayer.globalVolume=25;
			
		Wav w1;
		w1.readFile("wav/1.wav");
		
		Wav w2;
		w2.readFile("wav/2.wav");
		
		Wav w3;
		w3.readFile("wav/3.wav");

		Wav w4;
		w4.readFile("wav/4.wav");

		Wav w5;
		w5.readFile("wav/5.wav");

		Wav w6;
		w6.readFile("wav/6.wav");

		Wav w7;
		w7.readFile("wav/7.wav");

		Wav w8;
		w8.readFile("wav/8.wav");

		Wav w9;
		w9.readFile("wav/9.wav");

		Wav w0;
		w0.readFile("wav/0.wav");
		
		Wav wRing;
		wRing.readFile("wav/ringtone.wav");
		sRing = wRing.toSound();

		dialTone[0] = w0.toSound();
		dialTone[1] = w1.toSound();
		dialTone[2] = w2.toSound();
		dialTone[3] = w3.toSound();
		dialTone[4] = w4.toSound();
		dialTone[5] = w5.toSound();
		dialTone[6] = w6.toSound();
		dialTone[7] = w7.toSound();
		dialTone[8] = w8.toSound();
		dialTone[9] = w9.toSound();
	}
	
	void clearScreen()
	{
		int i=0;
		for (int _y=0;_y<48;++_y)
		{
			for (int _x=0;_x<64;++_x)
			{
					aGlyphBacklog[_y][_x] = ' ';
			}
		}
		command="";
	}
	
	void setGlyph ( int x, int y, char val)
	{
	}
	
	void writeString(int _x, int _y, std::string _str)
	{
		ANSI ansi;
		ansi.read(_str);
		
		
		for (unsigned int i=0;i<ansi.size();++i)
		{
			if ( isSafe(_x,_y) )
			{
				aGlyphBacklog[_y][_x] = ansi.ansiString[i];
				foregroundColour[_y][_x].set(ansi.vForegroundColour(i));
				++_x;
			}
		}
	}
	
	// Normal "screen wiping" method of loading up a page, typical of old computers
	void loadChar()
	{
		int maxSkip = 10;
		while (maxSkip-- > 0)
		{
			
			if ( isSafe(loadX,loadY) )
			{
				if ( aGlyph[loadY][loadX] != aGlyphBacklog[loadY][loadX] )
				{
					aGlyph[loadY][loadX] = aGlyphBacklog[loadY][loadX];
					maxSkip=0;
				}
				++loadX;
				if (loadX > 63)
				{
					if ( loadY < 47 )
					{
						loadX=0; ++loadY;
					}
					else
					{
						loadX=0; loadY=0;
					}
				}
			}
		}
	}
	
	void corrupt()
	{
		int rY = Random::randomInt(47);
		int rX = Random::randomInt(63);
		
		aGlyph[rY][rX]-= Random::randomInt(10);
		if (aGlyph[rY][rX] < 0)
		{
			aGlyph[rY][rX] = 0;
		}
	}
	
	//Variant which loads all of screen at once, but cycles each character through the
	// character table until it hits the right one.
	void loadChar2()
	{
		// fill terminal with random glyphs.
		for (int i=0;i<3072;++i)
		{
			if (pGlyph[i] != pGlyphBacklog[i])
			{
				pGlyph[i]++;
				if (pGlyph[i] < 0) { pGlyph[i]=0; }
				if (pGlyph[i] == '\n') { pGlyph[i]++; }
			}
		}
	}
	
	/* Shows a loading bar if the computer is in a loading state */
	void loadingBar()
	{
	//	writeString(63,47,"@");
	}
	
	void randomFill()
	{
		// fill terminal with random glyphs.
		for (int i=0;i<3072;++i)
		{
			pGlyph[i] = Random::randomInt(127);
			if (pGlyph[i] == '\n' ) { pGlyph[i] = ' '; }
			pGlyphBacklog[i] = ' ';
		}
		
	}
	
	// Terminal only renders text, not any decoration.
	void render()
	{

		//loadChar();
		//loadChar();	
		loadChar2();
		loadChar2();
		loadChar2();
		loadChar2();
		loadingBar();
		
		while (Random::oneIn(1000))
		{
			corrupt();
		}

		int centerX = panelX1 + (panelNX / 2);
		int centerY = panelY1 + (panelNY / 2);
	
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

	if (dialTones.size() > 0)
	{
		toneTimer.update();
		
		if ( toneTimer.fullSeconds > 0.12)
		{ globalAudioPlayer.stopAllSounds();
		}
		
		if (toneTimer.fullSeconds > 0.20)
		{
			int nextTone = (int)dialTones[0]-48;

			if ( nextTone >= 0 && nextTone <= 9)
			{ globalAudioPlayer.playSoundOnce(dialTone[nextTone]);
			}
			else if (dialTones[0] == 'R')
			{
				globalAudioPlayer.playSoundOnce(sRing);
			}
			
			toneTimer.start();
			dialTones.erase(0,1);
		}
	}

		//Update: Glyphs now render on a grid instead of using error-prone string.
		// Also foreground colour support added.
		int i=0;
		for (int _y=0;_y<48;++_y)
		{
			for (int _x=0;_x<64;++_x)
			{
					font8x8.putChar(aGlyph[_y][_x],panelX1+(10*_x),panelY2-(10*_y),foregroundColour[_y][_x]);
			}
		}
	}
	
	void putCursor(int _x, int _y)
	{
		if (isSafe(_x,_y))
		{
			if ( isSafe(cursorX,cursorY) )
			{
				aGlyph[cursorY][cursorX] = ' ';
			}
		
			cursorX = _x;
			cursorY = _y;
			aGlyph[cursorY][cursorX] = 1;
		}
		else
		{
			if ( isSafe(cursorX,cursorY) )
			{
				aGlyph[cursorY][cursorX] = ' ';
			}
		
			cursorX = -1;
			cursorY = -1;
		}
	}
	
	// User has pressed enter and console either goes down by 1, or shuffles everything up by 1.
	void newLine()
	{
		if (isSafe(cursorX,cursorY))
		{
			if (cursorY < 47)
			{
				putCursor(0,cursorY+1);
			}
		}
	}
	
	void blinkCursor()
	{
		if (isSafe(cursorX,cursorY))
		{
			if (++cursorBlink > 40)
			{
				cursorBlink = 0;
			}
			if ( cursorBlink > 20)
			{
				aGlyph[cursorY][cursorX] = ' ';
			}
			else
			{
				aGlyph[cursorY][cursorX] = 1;
			}
		}
	}
	
	void typeChar (char c)
	{
		// Make sure we're on an input space before we type.
		if (isSafe(cursorX,cursorY) && isSafe(cursorX+1,cursorY))
		{
				putCursor(cursorX+1,cursorY);
				aGlyph[cursorY][cursorX-1] = c;
				aGlyphBacklog[cursorY][cursorX-1] = c;
				command+=c;
		}
	}
	
	void backspace()
	{
		if (isSafe(cursorX-1,cursorY))
		{
			putCursor(cursorX-1,cursorY);
			aGlyph[cursorY][cursorX+1] = ' ';
			aGlyphBacklog[cursorY][cursorX+1] = ' ';
			
			if ( command.size () > 0 )
			{ command = command.substr(0, command.size()-1);
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
			std::cout<<"Keypress: "<<(int) _keyboard->lastKey<<".\n";
			
			if ( _keyboard->isAlphaNumeric(_keyboard->lastKey) || _keyboard->lastKey == Keyboard::SPACE)
			{ typeChar(_keyboard->lastKey); }
			else if (_keyboard->lastKey == Keyboard::SPACEBAR )
			{
				//bbsDemo();
			}
			else if (_keyboard->lastKey == 96)
			{
				std::cout<<"ild\n";
				debugConsole=!debugConsole;
				
				if ( debugConsole )
				{
					loadDebugConsole();
				}
			}
			else if (_keyboard->lastKey == 18)
			{
				init();
				bootSystem1();
			}
			else if (_keyboard->lastKey == 19)
			{
				std::cout<<"Save\n";
			}
			// Get whatever the user typed.
			else if (_keyboard->lastKey == Keyboard::ENTER )
			{
				newLine();
				
				//command = command.toUpper();
				
				// Convert string to upper case
				for (auto & c: command) c = toupper(c);
				std::cout<<"Entered command: "<<command<<".\n";
				
				//Tokenise
				Vector <std::string> * vToken = Tokenize::tokenize(command,' ');
			
				for (int i=0;i<vToken->size();++i)
				{
					std::cout<<"Token: "<<(*vToken)(i)<<"\n";
				}
				
				//First token is command.
				if (vToken->size() != 0)
				{
					command = (*vToken)(0);
				}
				else
				{
					command = "";
				}
				
				
				if (command == "BBS" && ( bootScreen == true || helpScreen == true ) )
				{
					bbsDemo();
					command = "";
				}
				else if (command == "HELP" && bootScreen == true)
				{
					helpScreen=true;
					bootScreen=false;
					debugConsole=false;
					command = "";
					loadHelpScreen();
				}
				if (command == "REBOOT" || command == "RESET")
				{
					init();
					bootSystem1();
				}
				if (command == "WRITE")
				{
					command="";
					clearScreen();
					writeScreen();
				}
				if (command == "MAIL")
				{
					command="";
					clearScreen();
					mailScreen();
				}
				if (command == "GAME")
				{
					command="";
					clearScreen();
					game1();
				}
				
				if (command == "CONNECT")
				{
					// strip everything except numbers. There should be 7, 10, 12, 19, or 22 digits.
					// phonecards will be 12 digits.
					
					
					if (vToken->size() == 1)
					{
						std::cout<<"Connect must have argument.\n";
					}
					else if (vToken->size() == 2)
					{
						std::string targetDial = (*vToken)(1);
						std::cout<<"Dial arg: "<<targetDial<<".\n";
						
						// dial must be 10 or 7 digits. (3 digits are city code)
						if ( DataTools::isNumber(targetDial) )
						{
							if (targetDial.size() == 10)
							{
							//	std::cout<<"Dialling: "<<targetDial<<".\n";
								dialTones = targetDial+"R";
							}
							else if (targetDial.size()==7)
							{
								targetDial = "001" + targetDial;
								//std::cout<<"Dialling: "<<targetDial<<".\n";
								dialTones = targetDial+"R";
							}
							else
							{
								std::cout<<"ERROR: Dial number must be 7 or 10 digits, no spaces.\n";
							}
						}
						else
						{
							std::cout<<"Error: Arg must be number.\n";
						}
					}
					else
					{
						std::cout<<"Bad args\n";
					}
					//screenConnect("","");	
					//command = "";
				}
				
				if (command == "AUTODIAL")
				{
					std::cout<<"Autodial.\n";
				}
				command = "";
			}
			else if (_keyboard->lastKey == 8 )
			{
				backspace();
			}
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
	
	void bootSystem1()
	{
		randomFill();
		writeString(0,0,"                    *** SUDACHI SYSTEM 1 ***                    ");
		writeString(0,1,"WHITE TEXT \033[1;31mbold red text\033[0m\033[1;31mbold red text\033[0m\033[1;31mbold red text\033[0m WHITE TEXT");
		putCursor(0,5);
		setInputSpace(0,1,64,1);
	}
	
	void loadHelpScreen()
	{
		clearScreen();
		//randomFill();
		writeString(0,0,"                    *** SUDACHI SYSTEM 1 ***                    ");
		writeString(0,2,"CATALOG - LIST PROGRAMS");
		writeString(0,3,"LIST - LIST FILES");
		writeString(0,4,"RUN - RUN PROGRAM");
		writeString(0,5,"REBOOT - REBOOT COMPUTER");
		writeString(0,6,"POWEROFF - POWER OFF COMPUTER");
		putCursor(0,10);
		setInputSpace(0,10,64,1);
		
	}
	
	/* Show a menu of all the programs available on this computer.
	It'll be cool if it's possible for a user to make their own programs */
	void loadCatalog()
	{
		writeString(0,0,"                    *** SUDACHI SYSTEM 1 ***                    ");
		writeString(0,2,"CONNECT"); /* Allows connection to servers with WireNet protocol */
		writeString(0,3,"GAME1"); /* Some game probably breakout. */
		writeString(0,4,"GAME2"); /* Maybe tetris or smth idk */
		writeString(0,5,"DEBUG"); /* Snowcrash console, contains set of network programs */
		writeString(0,6,"SONG"); /* A realistic elephant. */
		writeString(0,7,"WRITE"); /* Take notes or write program */
		writeString(0,8,"MAIL"); /* Check/send mail */
	}
	
	/* List all files on computer. No fancy directory stuff. */
	void loadList()
	{
		
	}
	
	void loadDebugConsole()
	{
		randomFill();
		writeString(0,0,"                    *** SNOWCRASH CONSOLE ***                    ");
		putCursor(0,40);
		setInputSpace(0,1,64,1);
		
		for (int i=0;i<vPackets.size() && i < 20;++i)
		{
			writeString(0,i+2,vPackets(i));
		}
	}
	
	void bbsDemo()
	{
		dialTones = "1234567890";
		
		vPackets.push("SENT 111 111 1111 CONNECT");
		vPackets.push("RECV 111 111 1111 ACK");
		vPackets.push("RECV 111 111 1111 [SITE DATA]");
		
		clearScreen();
		//init();
		//randomFill();
		
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
		
		setInputSpace(7,8,12,1);
		
	}
	
	// Allow this area to be written on by user.
	void setInputSpace(int _x, int _y, int length, int ID)
	{
		// for (int i=0;i<length;++i)
		// {
			// if (isSafe(_x,_y))
			// {
				// //inputSpace[_y][_x] = ID;
				// ++_x;
			// }
			// else { return; }

		// }
	}
	
	/* Runs the text editor. Basically a blank screen to type on. CTRL+S to save */
	void writeScreen()
	{
		//init();
		clearScreen(); 
		writeString(0,0,"*** WRITE (CTRL+S to save. CTRL+R to exit.)***");
		putCursor(0,1);
		//setInputSpace(0,1,64,1);
	}
	
	/* Shows any mail addressed to you */
	void mailScreen()
	{
		char r = 1;
		std::string redacted  = std::string() + r + "" + r + "/" + r + r + "/" +r +r +r +r;
		clearScreen(); 
		writeString(0,0,"*** MAIL. Type number to read. ***");
		writeString(0,1,"1. "+redacted+" - WELCOME TO MAIL");
		writeString(0,2,"2. "+redacted+" - SUP");
		putCursor(0,10);
		setInputSpace(0,10,64,1);
	}
	
	/* Breakout */
	void game1()
	{
		putCursor(-1,-1);
		char block = 1;
		std::string blocks = "   ";
		for (int i=0;i<58;++i)
		{
			blocks+=block;
		}
		blocks+="   ";
		
		writeString(0,1,blocks);
		writeString(0,2,blocks);
		writeString(0,3,blocks);
		
		std::string paddle = "";
		paddle+=block; paddle+=block; paddle+=block; paddle+=block;
		
		writeString(30,47,paddle);
		
		std::string ball = "";
		ball += 1;
		
		writeString(32,24,ball);
		
	}
	
	void screenConnect(std::string _number1="", std::string _number2="")
	{
		clearScreen();
		writeString(0,0,"DIALING...");
	}
	
};


#endif
