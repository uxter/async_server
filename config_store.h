#ifndef __CONFIG_STORE_H__
#define __CONFIG_STORE_H__

#include <string>
#include "json.h"

namespace ASYNC_SERVER {

    class ConfigStore {
    public:
        ConfigStore(int, char **);
        ~ConfigStore();
        int getIntProperty(const std::string &, int);
    private:
        int argc;
        char ** argv;
        JSON::JsonObject * data;
        bool loadConfigFile(std::string);
        template <class PT, class RT> RT getConfigProperty(const std::string &, RT);
        std::string getArgumentValue(const std::string &);
    };

}

#endif /* __CONFIG_STORE_H__ */
