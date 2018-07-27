#include "config_store.h"
#include "server.h"
#include "event_loop.h"

int main(int argc, char ** argv) {
    auto * config = new ASYNC_SERVER::ConfigStore(argc, argv);
    ASYNC_SERVER::Server server(config);
    ASYNC_SERVER::EventLoop loop(config);
    auto listenerFd = server.run();
    ASYNC_SERVER::EventHandler handler(config, listenerFd);
    loop.addHandler(&handler);
    loop.run();
    return 0;
}
