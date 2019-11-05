#pragma once
#ifndef TERMINAL_SERVER_HPP
#define TERMINAL_SERVER_HPP

/* Terminal: Server
	#include "Server.hpp"
	
	The server tracks its internal state, and sends and recieves packets.
	Mostly used for text-only websites and also databases.
	
	PACKET TYPES:
	
	PIN - Ping. Ping target.
	CON - Connect. Request connection.
	ACK - Acknowledge. Acknowledge connection.
	REJ - Reject. Reject connection.
	PGD - Page data. Data to render requested page. Entire page is passed at once for simplicity.
	
*/

#include <string>

class Server
{
	public:
	
	std::string number;
	
	Server()
	{
	}
	
	// Server will return the main page upon successful connection.
	std::string serveMain()
	{
		std::string mainPkt = "[PGD][data]";
		return "";
	}
	
	void recievePacket(std::string _packet)
	{
	}
	void sendPacket(std::string _packet)
	{
	}
};

Server testServer;

void initServers()
{
	testServer.number = "0011234567";
}


// class Server_RedThread: public Server
// {
	// public:
	
// };

#endif
