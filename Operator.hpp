#pragma once
#ifndef TERMINAL_OPERATOR_HPP
#define TERMINAL_OPERATOR_HPP

/* Terminal: Operator
	#include "Operator.hpp"
	
	The operator routes calls/packets to the correct destination,
	or provides notification of failure to deliver. However for now
	it's basically just a container for Server objects.
	
*/

#include <string>


#include "Server.hpp"

class Operator
{
	public:
	
	Vector <Server*> vServer;
	
	Operator()
	{
	}
	
	// 0 is no Server for that number.
	Server* getServer(std::string _number)
	{
		for (int i=0;i<vServer.size();++i)
		{
			if ( vServer(i)->number == _number)
			{
				return vServer(i);
			}
		}
		return 0;
	}
	
	void recievePacket(std::string _packet)
	{
	}
	
	// return value is any potential rebound packet. Empty string for no response.
	std::string sendPacket(std::string address, std::string _packet)
	{
		std::string _rebound = "";
		Server* target = getServer(address);
		
		if (target)
		{
			_rebound = target->recievePacket(_packet);
		}
		return _rebound;
	}
	bool dial(std::string _number)
	{
		std::cout<<"OPERATOR RECIEVED CALL\n";
		for (int i=0;i<vServer.size();++i)
		{
			std::cout<<"Checking numbers: "<<_number<<" "<<vServer(i)->number<<".\n";
			if ( vServer(i)->number == _number)
			{
				return true;
			}
		}
		return false;
	}
	
	std::string servePage(std::string _number)
	{
		for (int i=0;i<vServer.size();++i)
		{
			if ( vServer(i)->number == _number)
			{
				return vServer(i)->servePage();
			}
		}
		return "404";
	}
	
	void addServer(Server* server)
	{
		vServer.push(server);
	}
};

Operator op;

#endif
