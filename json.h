#ifndef __JSON_H__
#define __JSON_H__

#include <string>
#include "json_value.h"
#include "json_parser.h"

namespace JSON {

    std::string stringify(JsonValue *);

    JsonValue * parse(std::string);

}

#endif /* __JSON_H__ */
