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
	
	void recievePacket(std::string _packet)
	{
	}
	void sendPacket(std::string _packet)
	{
	}
	bool dial(std::string _number)
	{
		for (int i=0;i<vServer.size();++i)
		{
		//if ( vServer(i)->
		}
		return false;
	}
	
	// void addServer(std::string _address)
	// {
	// }
};

#endif
