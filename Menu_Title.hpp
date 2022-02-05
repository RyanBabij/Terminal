#pragma once
#ifndef TERMINAL_MENU_TITLE_HPP
#define TERMINAL_MENU_TITLE_HPP

/* Terminal: Menu_Title
   #include "Menu_Title.hpp"
   
   The title menu for the game, incidentally the only menu in the game, as everything is done from the terminal.
   
   Current terminal is only allowing 64 character width because I'm using square 10x10 font.
   Traditional 80 character width would be nice, but I kinda prefer square font.
*/

#include "Terminal.cpp"
#include "Terminal_Program.cpp"

class Menu_Title: public GUI_Interface, public LogicTickInterface, public IdleTickInterface
{
   public:

   Terminal terminal;
   int terminalFlicker;
   
   /* Colours / theme. */
   ColourRGBA <unsigned char> cNormal;
   ColourRGBA <unsigned char> cSelected;
   ColourRGBA <unsigned char> cDropPanel;
   ColourRGBA <unsigned char> cHighlight;
   
   /* GUI manager. Manages all GUI controls for this menu. */
   GUI_Manager guiManager;
   
   /* Background image */
   Texture* backgroundTexture;

   Menu_Title()
   {
      active=false;
      panelX1=0; panelY1=0; panelX2=0; panelY2=0;
      font=0;
      
      terminalFlicker=255;
   }
   
   void setFont(Wildcat::Font* _font) override
   {
      font = _font;
      terminal.setFont(_font);
      //guiManager.setFont(_font);
   }

   void eventResize() override
   {
      const int centerX = panelX1 + (panelNX / 2);
      const int centerY = panelY1 + (panelNY / 2);
      // terminal.setPanel(centerX-320,centerY-200,centerX+320,centerY+200);
      
      double  multiplier = 1;
      
      int currentX = 640;
      int currentY = 400;
      
      while (true)
      {
         
         if (currentX * multiplier+80 < panelNX 
            && currentY * multiplier+80 < panelNY )
         {
            //okay.
         }
         else
         {
            multiplier-=0.5;
            break;
         }
         multiplier+=0.5;
      }
      
       currentX = (currentX * multiplier)/2;
       currentY = (currentY * multiplier)/2;
      

      terminal.setPanel(centerX-currentX,centerY-currentY,centerX+currentX,centerY+currentY);
   }
   
   void init() override
   {
      cNormal.set(200,200,200);
      cSelected.set(180,180,180);
      cDropPanel.set(170,170,170);
      cHighlight.set(170,170,170);
      
      /* Update GUI positions. */
      eventResize();
      
      setFont(font);
      
      terminal.init();

   }
   /* DisplayInterface:: */
   void render() override
   {
		/* Background image. Stretch to fit, preserve aspect ratio. */
		//Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,false);
		//Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,&TEX_TERMINAL_BKG,true);
   
      //flicker();
      //place terminal text indicator panel
      
      int centerX = panelX1 + (panelNX / 2);
      int centerY = panelY1 + (panelNY / 2);
      
      //Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,&TEX_TERMINAL_BKG,true);
      glColor4ub(terminalFlicker,terminalFlicker,terminalFlicker,255);
      
      //Renderer::placeTexture4(centerX-385,centerY+305,centerX+385,centerY-305,backgroundTexture,false);
      //Renderer::placeTexture4(centerX-384,centerY+210,centerX+384,centerY-290,backgroundTexture,false);
      //Renderer::placeTexture4(centerX-320,centerY+200,centerX+320,centerY-200,&TEX_TERMINAL_GRID,false);
   
		glColor4ub(255,255,255,255);
		// RENDER TERMINAL

		terminal.render();


		guiManager.render();
   }
  
   /* MouseInterface:: */
   bool mouseEvent (Mouse* _mouse) override
   {
      return false;
   }
   
   void logicTick() override
   {
		terminal.idleTick();
   }
   
   bool keyboardEvent(Keyboard* _keyboard) override
   {
      terminal.keyboardEvent(_keyboard);
      return false;
   }
   
   bool stealKeyboard() override
   {
      return guiManager.stealKeyboard();
   }
   
   
   //Flicker the terminal just for fun.
   void flicker()
   {
      if (Random::oneIn(3))
      {
         --terminalFlicker;
         --terminalFlicker;
         --terminalFlicker;
         if (terminalFlicker < 230 ) { terminalFlicker = 230; }
         //terminalFlicker = 255;
      }
      else if (Random::oneIn(3))
      {
         ++terminalFlicker;
         ++terminalFlicker;
         ++terminalFlicker;
         if (terminalFlicker > 255 ) { terminalFlicker = 255; }
      }
   }
   
   void idleTick() override
   {
      //terminal.idleTick();
   }
   
};

#endif