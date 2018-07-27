#ifndef __SERVER_H__
#define __SERVER_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include "config_store.h"

namespace ASYNC_SERVER {

    class Server {
    public:
        explicit Server(ConfigStore *);
        int run();
    private:
        ConfigStore * config;
        int fd;
        int port;
        int backlog;
        sockaddr_in makeAddress();
    };

}

#endif /* __SERVER_H__ */
