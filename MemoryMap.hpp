#pragma once
#ifndef TERMINAL_MEMORYMAP_HPP
#define TERMINAL_MEMORYMAP_HPP

/* Terminal: MemoryMap
   #include "MemoryMap.hpp"
   
   Stores the Terminal memory map. Traditionally accessed linearly,
   however I add 2D access functions.
   
   C64 addresses memory from 0 to 65535. This covers all of the C64's memory,
   and therefore it's possible to directly modify the OS code with a program.
   
   When a program is run, or a device is attached, it's RAM will be overlaid
   on specific RAM addresses. For example I/O devices use 53248-57343.
   
   Screen memory is at 1024-2023. Poking these addresses will draw a char on
   the screen. The screen is 25 columns and 40 rows, so 1,000 characters.
   
   Using this screen memory you can make ASCII/PETSCII applications.
*/

class MemoryMap
{
   unsigned char ram [65536] = {0}; //0 to 65535
   public:

   
   MemoryMap()
   {
      
   }
   void poke(int address, char value)
   {
      ram[address]=value;
   }
   unsigned char peek(int address)
   {
      return ram[address];
   }
};

#endif