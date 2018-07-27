#include "event_loop.h"

namespace ASYNC_SERVER {

    EventLoop::EventLoop(ConfigStore * config) {
        this->maxEvents = config->getIntProperty("maxEvents", maxEvents);
        this->fd = epoll_create(1);
        this->handlersList = new std::vector<EventHandler *>;
    }

    EventLoop::~EventLoop() {
        delete this->handlersList;
    }

    void EventLoop::addHandler(EventHandler * handler) {
        struct epoll_event event{};
        memset(&event, 0, sizeof(event));
        event.events = handler->eventTypes;
        event.data.fd = handler->fd;
        epoll_ctl(this->fd, EPOLL_CTL_ADD, handler->fd, &event);
        this->handlersList->push_back(handler);
    }

    void EventLoop::run() {
        auto *events = new epoll_event[this->maxEvents];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        for (;;) {
            int length = epoll_wait(this->fd, events, this->maxEvents, -1);
            for (int i = 0; i < length; i++) {
                auto handler = this->handlersList->begin();
                for (; handler != this->handlersList->end(); handler++) {
                    struct Event event(events[i].data.fd, this->fd, events[i].events);
                    (*handler)->handle(event);
                }
            }
        }
#pragma clang diagnostic pop
    }

}
