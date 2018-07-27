#ifndef __JSON_VALUE_H__
#define __JSON_VALUE_H__

#include <string>
#include <map>
#include <vector>

/**
 *  ECMA-404
 *  http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf
 */

namespace JSON {

    enum JsonValueType {
        JSON_VALUE_TYPE_OBJECT,
        JSON_VALUE_TYPE_ARRAY,
        JSON_VALUE_TYPE_NUMBER,
        JSON_VALUE_TYPE_STRING,
        JSON_VALUE_TYPE_BOOLEAN,
        JSON_VALUE_TYPE_NULL,
        JSON_VALUE_TYPE_UNDEFINED
    };

    class JsonValue {
    public:
        virtual std::string toString() = 0;
        virtual ~JsonValue() = default;
        virtual JsonValueType getType() = 0;
    };

    class JsonNumber: public JsonValue {
    public:
        explicit JsonNumber(double);
        std::string toString() override;
        double value;
        JsonValueType getType() override;
    };

    class JsonString: public JsonValue {
    public:
        explicit JsonString(std::string);
        std::string toString() override;
        std::string value;
        JsonValueType getType() override;
    };

    class JsonBoolean: public JsonValue {
    public:
        explicit JsonBoolean(bool value);
        std::string toString() override;
        bool value;
        JsonValueType getType() override;
    };

    class JsonUndefined: public JsonValue {
    public:
        JsonUndefined();
        std::string toString() override;
        JsonValueType getType() override;
    };

    class JsonNull: public JsonValue {
    public:
        JsonNull();
        std::string toString() override;
        JsonValueType getType() override;
    };

    class JsonObject: public JsonValue {
    public:
        JsonObject();
        ~JsonObject() override;
        JsonValue * getProperty(std::string);
        void setProperty(std::string, JsonValue *);
        void removeProperty(std::string);
        std::string toString() override;
        std::map<std::string, JsonValue *> * value;
        JsonValueType getType() override;
    };

    class JsonArray: public JsonValue {
    public:
        JsonArray();
        ~JsonArray() override;
        void push(JsonValue *);
        std::string toString() override;
        std::vector<JsonValue *> * value;
        JsonValueType getType() override;
    };

    std::string escape(std::string);

}

#endif /* __JSON_VALUE_H__ */
