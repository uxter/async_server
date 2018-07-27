#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include "connection.h"
#include "json.h"

namespace ASYNC_SERVER {

    Connection::Connection(ConfigStore * config, Event &event): config(config), loopFd(event.loopFd) {
        this->bufferSize = (size_t)config->getIntProperty("bufferSize", 64);
        this->requestRawData = new std::string;
        struct sockaddr_in address{};
        socklen_t socklen = sizeof(struct sockaddr_in);
        this->fd = accept(event.fd, (struct sockaddr *) &address, &socklen);
        fcntl(this->fd, F_SETFL, O_NONBLOCK);
    }

    Connection::~Connection() {
        delete this->requestRawData;
    }

    ConnectionState Connection::readRequest() {
        char buffer[this->bufferSize];
        memset(buffer, 0, this->bufferSize);
        ssize_t bytesReceived = recv(this->fd, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0) {
            this->requestRawData->append(buffer, (unsigned long)bytesReceived);
        }

        if (bytesReceived == -1) {
            return CONNECTION_READ_ERROR;
        } else if (bytesReceived < this->bufferSize) {
            return CONNECTION_READ_DONE;
        }

        return CONNECTION_READ_NEXT;
    }

    ConnectionState Connection::writeResponse() {
        JSON::JsonObject data;
        data.setProperty("status", new JSON::JsonBoolean(true));
        std::string body = JSON::stringify(&data);

        std::ostringstream result;
        result << "HTTP/1.1 200 Ok\n";
        result << "Content-Type: application/json; charset=UTF-8\n";
        result << "Content-Length: " << body.length() << "\n";
        result << "\n";
        result << body;

        ssize_t sendResult = send(this->fd, result.str().c_str(), strlen(result.str().c_str()), 0);
        if (sendResult == -1) {
            return CONNECTION_WRITE_ERROR;
        }
        return CONNECTION_WRITE_DONE;
    }

}
