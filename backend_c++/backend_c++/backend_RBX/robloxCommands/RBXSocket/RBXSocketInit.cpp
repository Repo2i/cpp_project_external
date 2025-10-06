//
//  RBXSocketInit.cpp
//  backend_RBX
//
//  Created by aibolat on 28.10.2025.
//

#include "RBXSocketInit.hpp"

std::function<int(int domain,int type, int protocol)> RBXSocket::MakeSocket = [](int domain,int type, int protocol) -> int {
        sockaddr_in adrress{};
        adrress.sin_family = AF_INET;
        adrress.sin_port = htons(8000);
        adrress.sin_addr.s_addr = htonl(INADDR_ANY);
        
        int RBXSock = socket(domain, type, protocol);
        
        if (RBXSock < 0)
                return 1;
        
        if (bind(RBXSock, (struct sockaddr*)&adrress, sizeof(adrress)) < 0)
                close(RBXSock);
                return -1;
        
        return RBXSock;
};

std::function<int(int port)> RBXSocket::StartServer = [](int port) -> int {
        int sock = RBXSocket::MakeSocket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) return -1;
        
        if (listen(sock, SOMAXCONN) < 0)
                return -1;
        
        for (;;) {
                int client = accept(sock, nullptr, nullptr);
                
                if (client >= 0) {
                        char buf[1024];
                        int n = static_cast<int>(recv(client, buf, sizeof(buf), 0));
                        
                        if (n > 0) {
                                send(client, "pong", 4, 0);
                        }
                        close(client);
                }
        }
        
        return 0;
};
