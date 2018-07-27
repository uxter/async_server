#ifndef __EVENT_H__
#define __EVENT_H__

#include <sys/epoll.h>

namespace ASYNC_SERVER {

    struct Event {
        Event(int fd, int loopFd, int events): fd(fd), loopFd(loopFd), events(events) {};
        int fd;
        int loopFd;
        int events;
    };

}

#endif /* __EVENT_H__ */
