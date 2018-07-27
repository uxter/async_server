#include "server.h"

namespace ASYNC_SERVER {

    Server::Server(ConfigStore * config): config(config) {
        this->fd = socket(AF_INET, SOCK_STREAM, 0);
        this->port = config->getIntProperty("port", 3000);
        this->backlog = config->getIntProperty("backlog", 1024);
    }

    sockaddr_in Server::makeAddress() {
        struct sockaddr_in address{};
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons((uint16_t)this->port);
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        return address;
    }

    int Server::run() {
        sockaddr_in address = this->makeAddress();
        fcntl(this->fd, F_SETFL, O_NONBLOCK);
        int soValue = 1;
        setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &soValue, sizeof(soValue));
        bind(this->fd, (struct sockaddr *) &address, sizeof(address));
        listen(this->fd, this->backlog);
        return this->fd;
    }

}
