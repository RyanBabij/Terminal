#pragma once
#ifndef TERMINAL_OPERATOR_HPP
#define TERMINAL_OPERATOR_HPP

/* Terminal: Operator
	#include "Operator.hpp"
	
	The operator routes calls/packets to the correct destination,
	or provides notification of failure to deliver.
	
*/

#include <string>

class Operator
{
	public:
	
	Operator()
	{
	}
	
	void recievePacket(std::string _packet)
	{
	}
	void sendPacket(std::string _packet)
	{
	}
	
	void addServer(std::string _address)
	{
	}
};