#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include <sys/epoll.h>
#include <cstring>
#include <vector>
#include "config_store.h"
#include "event_handler.h"
#include "event.h"

namespace ASYNC_SERVER {

    class EventLoop {
    public:
        int fd;
        explicit EventLoop(ConfigStore *);
        ~EventLoop();
        void addHandler(EventHandler *);
        void run();

    private:
        int maxEvents;
        std::vector<EventHandler *> * handlersList;
    };

}

#endif /* __EVENT_LOOP_H__ */
