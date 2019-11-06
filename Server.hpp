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
	DIS - Disconnect connection.
	PGD - Page data. Data to render requested page. Entire page is passed at once for simplicity.
	
	RDR - Redraw. Client should request new page data.
	
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
	virtual std::string servePage()
	{
		std::string mainPkt = "[PGD][data]";
		return "";
	}
	
	virtual std::string recievePacket(std::string _packet)
	{
		return "";
	}
	virtual void sendPacket(std::string _packet)
	{
	}
};

Server testServer;




class Server_RedThread: public Server
{
	public:
	
	//std::string number;
	std::string userHandle;
	
   enum ActiveMenu
   {
      MAIN,
      BBS
   };
	 ActiveMenu activeMenu;
	
	Server_RedThread()
	{
		number = "0011111111";
		userHandle="";
		
		activeMenu=MAIN;
	}
	
	virtual std::string servePage()
	{
		std::string strMain = "";
		if (activeMenu==MAIN)
		{
			strMain+="REDTHREAD\n\033[1;31m________________________________________________________________\033[0m";
			strMain+= "\n\nPlease enter a handle:";
		}
		else
		{
			strMain+="BBS STUFF GOES HERE";
		}


		return strMain;
	}
	

	std::string recievePacket(std::string _command)
	{
		if ( activeMenu==MAIN )
		{
			activeMenu=BBS;
			// send redraw command
			return "[RDR]";
		}
		else
		{
			activeMenu=MAIN;
			// send redraw command
			return "[RDR]";
		}
		return "";
	}
};
Server_RedThread serverRedThread;

class Server_Mail: public Server
{
	public:
	
	Server_Mail()
	{
		number = "0011234567";
	}
};

class Server_LumaBank: public Server
{
	public:
	
	
};


#endif
