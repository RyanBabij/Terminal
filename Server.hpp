#pragma once
#ifndef TERMINAL_SERVER_HPP
#define TERMINAL_SERVER_HPP

/* Terminal: Server
	#include "Server.hpp"
	
	The server tracks its internal state, and sends and recieves packets.
	Mostly used for text-only websites and also databases.
	
*/

#include <string>

class Server
{
	public:
	
	Server()
	{
	}
	
	void recievePacket(std::string _packet)
	{
	}
	void sendPacket(std::string _packet)
	{
	}
};