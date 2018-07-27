#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include <map>
#include <sys/epoll.h>
#include "config_store.h"
#include "connection.h"
#include "event.h"

namespace ASYNC_SERVER {

    class EventHandler {
    public:
        int fd;
        uint32_t eventTypes;
        EventHandler(ConfigStore *, int);
        ~EventHandler();
        void handle(Event &);
    private:
        void setupEvent(int, uint32_t, int, int);
        void closeConnection(Event &, Connection *);
        void handleConnectionEvent(Event &);
        void handleReadEvent(Event &);
        void handleWriteEvent(Event &);
        void handleCloseEvent(Event &);
        ConfigStore * config;
        std::map<int, Connection *> * connectionsMap;
    };

}

#endif /* __EVENT_HANDLER_H__ */
