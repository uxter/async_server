#include <stdexcept>
#include <fstream>
#include "config_store.h"

namespace ASYNC_SERVER {

    ConfigStore::ConfigStore(int argc, char ** argv) {
        this->argc = argc;
        this->argv = argv;
        std::string configPath = this->getArgumentValue("config");
        if (configPath.empty()) {
            throw std::invalid_argument("Argument --config is required!");
        }
        bool isLoaded = this->loadConfigFile(configPath);
        if (!isLoaded) {
            throw std::invalid_argument("Argument --config is invalid!");
        }
    }

    ConfigStore::~ConfigStore() {
        delete this->data;
    }

    std::string ConfigStore::getArgumentValue(const std::string &key) {
        for (int i = 0; i < this->argc; i++) {
            if ((std::string)this->argv[i] == "--" + key && i + 1 < this->argc) {
                return this->argv[i + 1];
            }
        }
        return "";
    }

    bool ConfigStore::loadConfigFile(std::string path) {
        std::ifstream file(path);
        if (!file.good()) {
            return false;
        }
        JSON::JsonParser parser;
        parser.parseFile(std::move(path));
        this->data = dynamic_cast<JSON::JsonObject *>(parser.getValue());
        return true;
    }

    template<class PT, class RT>
    RT ConfigStore::getConfigProperty(const std::string &key, RT defaultValue) {
        PT property = (PT)this->data->getProperty(key);
        if (property->getType() == JSON::JSON_VALUE_TYPE_UNDEFINED) {
            return defaultValue;
        }
        return (RT)property->value;
    }

    int ConfigStore::getIntProperty(const std::string &key, int defaultValue) {
        return this->getConfigProperty<JSON::JsonNumber *, int>(key, defaultValue);
    }

}
