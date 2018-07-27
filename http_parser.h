#ifndef __HTTP_PARSER_H__
#define __HTTP_PARSER_H__

#include <string>
#include <map>

namespace HTTP {

    enum HttpParserState {
        HTTP_PARSER_STATE_METHOD_START,
        HTTP_PARSER_STATE_METHOD_END,
        HTTP_PARSER_STATE_STATUS_START,
        HTTP_PARSER_STATE_STATUS_END,
        HTTP_PARSER_STATE_REASON_START,
        HTTP_PARSER_STATE_REASON_END,
        HTTP_PARSER_STATE_URI_START,
        HTTP_PARSER_STATE_URI_END,
        HTTP_PARSER_STATE_VERSION_START,
        HTTP_PARSER_STATE_VERSION_END,
        HTTP_PARSER_STATE_KEY_START,
        HTTP_PARSER_STATE_KEY_END,
        HTTP_PARSER_STATE_VALUE_START,
        HTTP_PARSER_STATE_VALUE_END,
        HTTP_PARSER_STATE_END,
        HTTP_PARSER_STATE_ERROR
    };

    enum HttpMethod {
        HTTP_METHOD_NONE,
        HTTP_METHOD_OPTIONS,
        HTTP_METHOD_GET,
        HTTP_METHOD_HEAD,
        HTTP_METHOD_POST,
        HTTP_METHOD_PUT,
        HTTP_METHOD_PATCH,
        HTTP_METHOD_DELETE,
        HTTP_METHOD_TRACE,
        HTTP_METHOD_CONNECT
    };

    enum HttpVersion {
        HTTP_VERSION_NONE,
        HTTP_VERSION_09,
        HTTP_VERSION_10,
        HTTP_VERSION_11,
        HTTP_VERSION_20
    };

    enum HttpType {
        HTTP_REQUEST,
        HTTP_RESPONSE
    };

    class HttpParser {
    public:
        HttpParser(HttpType);
        ~HttpParser();
        void parseString(std::string);
        std::string getHeader(std::string);
        HttpMethod getMethod();
        HttpVersion getVersion();
        std::string getReason();
        std::string getUri();
    private:
        HttpType type;
        HttpParserState currentState;
        std::string stringData;
        HttpMethod method;
        int status;
        HttpVersion version;
        std::string reason;
        std::string uri;
        std::map<std::string, std::string> * headers;
        void handleCharItem(char);
        void handleMethodCharItem(char);
        void handleUriCharItem(char);
        void handleVersionCharItem(char);
        void handleHeaderKeyCharItem(char);
        void handleStatusCharItem(char);
        void handleReasonCharItem(char);
    };

}

#endif /* __HTTP_PARSER_H__ */
