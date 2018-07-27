#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string>
#include "config_store.h"
#include "event.h"

namespace ASYNC_SERVER {

    enum ConnectionState {
        CONNECTION_READ_NEXT,
        CONNECTION_READ_DONE,
        CONNECTION_READ_ERROR,
        CONNECTION_WRITE_DONE,
        CONNECTION_WRITE_ERROR
    };

    class Connection {
        public:
            int fd;
            Connection(ConfigStore *, Event &);
            ~Connection();
            ConnectionState readRequest();
            ConnectionState writeResponse();
        private:
            int loopFd;
            ConfigStore * config;
            size_t bufferSize;
            std::string * requestRawData;
    };

}

#endif /* __CONNECTION_H__ */
