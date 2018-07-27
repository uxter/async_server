#include "json_parser.h"

namespace JSON {

    JsonParser::JsonParser() {
        this->currentState = JSON_PARSER_STATE_INIT_VALUE;
        this->stringData = "";
        this->isData = false;
        this->valuesStack = new std::stack<JsonValue *>();
        this->rootValue = new JsonUndefined();
    }

    JsonParser::~JsonParser() {
        delete this->valuesStack;
    }

    void JsonParser::parseFile(std::string filePath) {
        std::ifstream readFile;
        readFile.open(filePath.c_str());
        char charItem;
        while (readFile.get(charItem)) {
            this->handleCharItem(charItem);
            if (this->currentState == JSON_PARSER_STATE_ERROR) {
                return;
            }
        }
        readFile.close();
    }

    void JsonParser::parseString(std::string source) {
        for (char charItem : source) {
            this->handleCharItem(charItem);
            if (this->currentState == JSON_PARSER_STATE_ERROR) {
                return;
            }
        }
    }

    JsonValue * JsonParser::getValue() {
        return this->rootValue;
    }

    void JsonParser::handleCharItem(char charItem) {
        JsonObject * object;
        std::string key;
        JsonValue * value;
        if (!this->isData && isspace(charItem)) {
            return;
        }
        switch(this->currentState) {
            case JSON_PARSER_STATE_INIT_VALUE:
                delete this->rootValue;
                this->rootValue = this->initValue(charItem);
                this->valuesStack->push(this->rootValue);
                break;
            case JSON_PARSER_STATE_OBJECT_START:
                this->handleAfterStartObjectCharItem(charItem);
                break;
            case JSON_PARSER_STATE_OBJECT_PROPERTY_KEY_START:
                this->handleStringCharItem(charItem, JSON_PARSER_STATE_OBJECT_PROPERTY_KEY_END);
                break;
            case JSON_PARSER_STATE_OBJECT_PROPERTY_KEY_END:
                if (charItem == ':') {
                    this->currentState = JSON_PARSER_STATE_OBJECT_PROPERTY_COLON;
                } else {
                    this->initError();
                }
                break;
            case JSON_PARSER_STATE_OBJECT_PROPERTY_COLON:
                key = this->stringData;
                value = this->initValue(charItem);
                object = dynamic_cast<JsonObject *>(this->valuesStack->top());
                object->setProperty(key, value);
                this->valuesStack->push(value);
                break;
            case JSON_PARSER_STATE_ARRAY_START:
                this->handleAfterStartArrayCharItem(charItem);
                break;
            case JSON_PARSER_STATE_NUMBER_DIGIT:
                this->handleNumberCharItem(charItem);
                break;
            case JSON_PARSER_STATE_STRING_START:
                this->handleStringCharItem(charItem, JSON_PARSER_STATE_VALUE_END);
                if (this->currentState == JSON_PARSER_STATE_VALUE_END) {
                    dynamic_cast<JsonString *>(this->valuesStack->top())->value = this->stringData;
                    this->valuesStack->pop();
                }
                break;
            case JSON_PARSER_STATE_VALUE_END:
                this->handleAfterEndValueCharItem(charItem);
                break;
            case JSON_PARSER_STATE_STRING_ESCAPE:break;
            case JSON_PARSER_STATE_TRUE:
                this->handleLiteralNameCharItem("true", charItem);
                break;
            case JSON_PARSER_STATE_FALSE:
                this->handleLiteralNameCharItem("false", charItem);
                break;
            case JSON_PARSER_STATE_NULL:
                this->handleLiteralNameCharItem("null", charItem);
                break;
        }
    }

    void JsonParser::handleNumberCharItem(char charItem) {
        switch(charItem) {
            case '-':
            case '.':
            case 'e':
            case '+':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                this->stringData += charItem;
                break;
            default:
                auto * number = dynamic_cast<JsonNumber *>(this->valuesStack->top());
                std::string::size_type stringTypeSize;
                number->value = std::stod(this->stringData, &stringTypeSize);
                this->isData = false;
                this->currentState = JSON_PARSER_STATE_VALUE_END;
                this->valuesStack->pop();
                this->handleAfterEndValueCharItem(charItem);
        }
    }

    void JsonParser::handleStringCharItem(char charItem, JsonParserState nextState) {
        if (this->stringData.back() == '\\') {
            switch(charItem) {
                case '"':
                    this->stringData.back() = '\u0022';
                    break;
                case '\\':
                    this->stringData.back() = '\u005c';
                    break;
                case '/':
                    this->stringData.back() = '\u002f';
                    break;
                case 'b':
                    this->stringData.back() = '\u0008';
                    break;
                case 'f':
                    this->stringData.back() = '\u000c';
                    break;
                case 'n':
                    this->stringData.back() = '\u000a';
                    break;
                case 'r':
                    this->stringData.back() = '\u000d';
                    break;
                case 't':
                    this->stringData.back() = '\u0009';
                    break;
                default:
                    this->initError();
            }
        } else if (charItem == '"') {
            this->isData = false;
            this->currentState = nextState;
        } else {
            this->stringData += charItem;
        }
    }

    void JsonParser::handleLiteralNameCharItem(std::string literalName, char charItem) {
        this->stringData += charItem;
        if (literalName.find(this->stringData) == std::string::npos) {
            this->initError();
            return;
        }
        if (literalName == this->stringData) {
            this->isData = false;
            this->currentState = JSON_PARSER_STATE_VALUE_END;
            this->valuesStack->pop();
        }
    }

    JsonValue * JsonParser::initValue(char charItem) {
        switch(charItem) {
            case '{':
                this->currentState = JSON_PARSER_STATE_OBJECT_START;
                return new JsonObject();
            case '[':
                this->currentState = JSON_PARSER_STATE_ARRAY_START;
                return new JsonArray();
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                this->currentState = JSON_PARSER_STATE_NUMBER_DIGIT;
                this->isData = true;
                this->stringData = charItem;
                return new JsonNumber(0);
            case '"':
                this->currentState = JSON_PARSER_STATE_STRING_START;
                this->isData = true;
                this->stringData = "";
                return new JsonString("");
            case 't':
                this->currentState = JSON_PARSER_STATE_TRUE;
                this->stringData = "t";
                this->isData = true;
                return new JsonBoolean(true);
            case 'f':
                this->currentState = JSON_PARSER_STATE_FALSE;
                this->stringData = "f";
                this->isData = true;
                return new JsonBoolean(false);
            case 'n':
                this->currentState = JSON_PARSER_STATE_NULL;
                this->stringData = "n";
                this->isData = true;
                return new JsonNull();
            default:
                return this->initError();
        }
    }

    void JsonParser::handleAfterEndValueCharItem(char charItem) {
        if (!this->isData && isspace(charItem)) {
            return;
        }
        if (this->valuesStack->empty()) {
            this->initError();
            return;
        }
        switch(this->valuesStack->top()->getType()) {
            case JSON_VALUE_TYPE_OBJECT:
                switch(charItem) {
                    case ',':
                        this->currentState = JSON_PARSER_STATE_OBJECT_START;
                        break;
                    case '}':
                        this->currentState = JSON_PARSER_STATE_VALUE_END;
                        this->valuesStack->pop();
                        break;
                    default:
                        this->initError();
                }
                break;
            case JSON_VALUE_TYPE_ARRAY:
                switch(charItem) {
                    case ',':
                        this->currentState = JSON_PARSER_STATE_ARRAY_START;
                        break;
                    case ']':
                        this->currentState = JSON_PARSER_STATE_VALUE_END;
                        this->valuesStack->pop();
                        break;
                    default:
                        this->initError();
                }
                break;
            default:
                this->initError();
        }
    }

    void JsonParser::handleAfterStartObjectCharItem(char charItem) {
        switch (charItem) {
            case '"':
                this->currentState = JSON_PARSER_STATE_OBJECT_PROPERTY_KEY_START;
                this->stringData = "";
                this->isData = true;
                break;
            case '}':
                this->currentState = JSON_PARSER_STATE_VALUE_END;
                this->valuesStack->pop();
                break;
            default:
                this->initError();
        }
    }

    void JsonParser::handleAfterStartArrayCharItem(char charItem) {
        if (charItem == ']') {
            this->currentState = JSON_PARSER_STATE_VALUE_END;
            this->valuesStack->pop();
            return;
        }
        JsonValue * arrayItem = this->initValue(charItem);
        dynamic_cast<JsonArray *>(this->valuesStack->top())->push(arrayItem);
        this->valuesStack->push(arrayItem);
    }

    JsonValue * JsonParser::initError() {
        this->currentState = JSON_PARSER_STATE_ERROR;
        delete this->rootValue;
        this->rootValue = new JsonUndefined();
        return this->rootValue;
    }

}
