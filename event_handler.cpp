#include "event_handler.h"
#include <unistd.h>

namespace ASYNC_SERVER {

    EventHandler::EventHandler(ConfigStore * config, int fd): config(config), fd(fd) {
        this->eventTypes = EPOLLIN | EPOLLHUP;
        this->connectionsMap = new std::map<int, Connection *>;
    }

    EventHandler::~EventHandler() {
        delete this->connectionsMap;
    }

    void EventHandler::handle(Event &event) {
        if (this->fd == event.fd) {
            this->handleConnectionEvent(event);
        } else if (event.events & EPOLLIN) {
            this->handleReadEvent(event);
        } else if (event.events & EPOLLOUT) {
            this->handleWriteEvent(event);
        } else if (event.events & EPOLLHUP) {
            this->handleCloseEvent(event);
        }
    }

    void EventHandler::handleConnectionEvent(Event &event) {
        auto * connection = new Connection(this->config, event);
        this->connectionsMap->insert(std::pair<int, Connection *>(connection->fd, connection));
        this->setupEvent(EPOLL_CTL_ADD, EPOLLIN | EPOLLONESHOT, event.loopFd, connection->fd);
    }

    void EventHandler::handleReadEvent(Event &event) {
        auto it = this->connectionsMap->find(event.fd);
        if (it != this->connectionsMap->end()) {
            int readStatus = it->second->readRequest();
            switch(readStatus) {
                case CONNECTION_READ_DONE:
                    this->setupEvent(EPOLL_CTL_MOD, EPOLLOUT | EPOLLONESHOT, event.loopFd, event.fd);
                    break;
                case CONNECTION_READ_ERROR:
                    this->closeConnection(event, it->second);
                    break;
                case CONNECTION_READ_NEXT:
                    this->setupEvent(EPOLL_CTL_MOD, EPOLLIN | EPOLLONESHOT, event.loopFd, event.fd);
                    break;
            }
        }
    }

    void EventHandler::handleWriteEvent(Event &event) {
        auto it = this->connectionsMap->find(event.fd);
        if (it != this->connectionsMap->end()) {
            it->second->writeResponse();
            this->closeConnection(event, it->second);
        }
    }

    void EventHandler::handleCloseEvent(Event &event) {
        auto it = this->connectionsMap->find(event.fd);
        if (it != this->connectionsMap->end()) {
            this->closeConnection(event, it->second);
        }
    }

    void EventHandler::closeConnection(Event &event, Connection * connection) {
        this->connectionsMap->erase(event.fd);
        this->setupEvent(EPOLL_CTL_DEL, EPOLLOUT | EPOLLONESHOT, event.loopFd, event.fd);
        close(event.fd);
        delete connection;
    }

    void EventHandler::setupEvent(int action, uint32_t type, int loopFd, int sockFd) {
        struct epoll_event epollEvent{};
        epollEvent.events = type;
        epollEvent.data.fd = sockFd;
        epoll_ctl(loopFd, action, sockFd, &epollEvent);
    }

}
