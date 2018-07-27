#include <sstream>
#include <iomanip>
#include <limits>
#include "json_value.h"

namespace JSON {

    JsonNumber::JsonNumber(double value): value(value) {}

    std::string JsonNumber::toString() {
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<double>::digits10+2);
        ss << this->value;
        return ss.str();
    }

    JsonValueType JsonNumber::getType() {
        return JSON_VALUE_TYPE_NUMBER;
    };

    JsonString::JsonString(std::string value): value(std::move(value)) {}

    std::string JsonString::toString() {
        return "\"" + escape(this->value) + "\"";
    }

    JsonValueType JsonString::getType() { return JSON_VALUE_TYPE_STRING; }

    JsonBoolean::JsonBoolean(bool value): value(value) {}

    std::string JsonBoolean::toString() {
        std::ostringstream result;
        result << (this->value ? "true" : "false");
        return result.str();
    }

    JsonValueType JsonBoolean::getType() { return JSON_VALUE_TYPE_BOOLEAN; }

    JsonUndefined::JsonUndefined() = default;

    std::string JsonUndefined::toString() {
        return "undefined";
    }

    JsonValueType JsonUndefined::getType() { return JSON_VALUE_TYPE_UNDEFINED; }

    JsonNull::JsonNull() = default;

    std::string JsonNull::toString() {
        return "null";
    }

    JsonValueType JsonNull::getType() { return JSON_VALUE_TYPE_NULL; }

    JsonObject::JsonObject() {
        this->value = new std::map<std::string, JsonValue *>;
    }

    JsonObject::~JsonObject() {
        auto it = this->value->begin();
        auto itEnd = this->value->end();
        for (; it != itEnd; it++) {
            delete it->second;
        }
        delete this->value;
    }

    std::string JsonObject::toString() {
        std::ostringstream result;
        result << "{";
        auto it = this->value->begin();
        auto itEnd = this->value->end();
        if (it != itEnd) {
            auto itPreEnd = --this->value->end();
            for (; it != itEnd; it++) {
                result << "\"" << escape(it->first) << "\":" << it->second->toString();
                if (it != itPreEnd) {
                    result << ",";
                }
            }
        }
        result << "}";
        return result.str();
    }

    JsonValue * JsonObject::getProperty(std::string propertyKey) {
        auto it = this->value->find(propertyKey);
        return static_cast<JsonValue *>(it != this->value->end() ? it->second : new JsonUndefined());
    }

    void JsonObject::setProperty(std::string propertyKey, JsonValue * propertyValue) {
        this->value->insert(std::pair<std::string, JsonValue *>(propertyKey, propertyValue));
    }

    void JsonObject::removeProperty(std::string propertyKey) {
        this->value->erase(propertyKey);
    }

    JsonValueType JsonObject::getType() { return JSON_VALUE_TYPE_OBJECT; }

    JsonArray::JsonArray() {
        this->value = new std::vector<JsonValue *>;
    }

    JsonArray::~JsonArray() {
        auto it = this->value->begin();
        auto itEnd = this->value->end();
        for (; it != itEnd; it++) {
            delete *it;
        }
        delete this->value;
    }

    std::string JsonArray::toString() {
        std::ostringstream result;
        result << "[";
        auto it = this->value->begin();
        auto itEnd = this->value->end();
        if (it != itEnd) {
            auto itPreEnd = --this->value->end();
            for (; it != itEnd; it++) {
                result << (*it)->toString();
                if (it != itPreEnd) {
                    result << ",";
                }
            }
        }
        result << "]";
        return result.str();
    }

    void JsonArray::push(JsonValue * item) {
        this->value->push_back(item);
    }

    JsonValueType JsonArray::getType() { return JSON_VALUE_TYPE_ARRAY; }

    std::string escape(std::string source) {
        std::string result;
        for (char charItem : source) {
            switch(charItem) {
                case '\u0022':
                    result += "\\\"";
                    break;
                case '\u005c':
                    result += "\\\\";
                    break;
                case '\u0008':
                    result += "\\b";
                    break;
                case '\u000c':
                    result += "\\f";
                    break;
                case '\u000a':
                    result += "\\n";
                    break;
                case '\u000d':
                    result += "\\r";
                    break;
                case '\u0009':
                    result += "\\t";
                    break;
                default:
                    result += charItem;
            }
        }
        return result;
    }

}
