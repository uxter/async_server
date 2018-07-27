#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#include <fstream>
#include <stack>
#include <map>
#include <utility>
#include "json_value.h"

namespace JSON {

    enum JsonParserState {
        JSON_PARSER_STATE_INIT_VALUE,
        JSON_PARSER_STATE_OBJECT_START,              // {
        JSON_PARSER_STATE_OBJECT_PROPERTY_KEY_START, // "
        JSON_PARSER_STATE_OBJECT_PROPERTY_KEY_END,   // (not \)"
        JSON_PARSER_STATE_OBJECT_PROPERTY_COLON,     // :
        JSON_PARSER_STATE_ARRAY_START,               // [
        JSON_PARSER_STATE_NUMBER_DIGIT,              // 0-9
        JSON_PARSER_STATE_STRING_START,              // "
        JSON_PARSER_STATE_VALUE_END,
        JSON_PARSER_STATE_STRING_ESCAPE,             // back slash
        JSON_PARSER_STATE_TRUE,                      // true
        JSON_PARSER_STATE_FALSE,                     // false
        JSON_PARSER_STATE_NULL,                      // null
        JSON_PARSER_STATE_ERROR
    };

    class JsonParser {
    public:
        JsonParser();
        ~JsonParser();
        void parseFile(std::string);
        void parseString(std::string);
        JsonValue * getValue();
    private:
        JsonValue * rootValue;
        JsonParserState currentState;
        std::stack<JsonValue *> * valuesStack;
        std::string stringData;
        bool isData;
        void handleCharItem(char);
        void handleNumberCharItem(char);
        void handleStringCharItem(char, JsonParserState);
        void handleLiteralNameCharItem(std::string, char);
        void handleAfterEndValueCharItem(char);
        void handleAfterStartObjectCharItem(char);
        void handleAfterStartArrayCharItem(char);
        JsonValue * initValue(char);
        JsonValue * initError();
    };

}

#endif /* __JSON_PARSER_H__ */
