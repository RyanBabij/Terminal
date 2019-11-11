#pragma once
#ifndef TERMINAL_DRIVER_INIT_SERVERS_HPP
#define TERMINAL_DRIVER_INIT_SERVERS_HPP

/* Terminal: Driver_InitServers,hpp
  #include "Driver_InitServers.hpp"

Create default servers and put them into Operator.

*/

#include "Server.hpp"
#include "Operator.hpp"

void initServers()
{
	op.addServer(&serverRedThread);
	op.addServer(&serverMail);
	op.addServer(&serverLumaBank);
}

#endif
