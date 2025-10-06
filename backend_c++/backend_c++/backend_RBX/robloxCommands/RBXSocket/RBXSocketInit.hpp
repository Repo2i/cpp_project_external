//
//  RBXSocketInit.hpp
//  backend_RBX
//
//  Created by aibolat on 28.10.2025.
//

#pragma once

#include "../includes/includes.hpp"

namespace RBXSocket {
extern std::function<int(int domain, int type, int protocol)> MakeSocket;
extern std::function<int(int port)> StartServer;
}

