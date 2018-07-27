#include "json.h"

namespace JSON {

    std::string stringify(JsonValue *value) {
        return value->toString();
    }

    JsonValue * parse(std::string source) {
        JsonParser reader;
        reader.parseString(std::move(source));
        return reader.getValue();
    }

}
