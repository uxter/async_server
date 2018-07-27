#include "http_parser.h"

namespace HTTP {

    HttpParser::HttpParser(HttpType type): type(type) {
        switch (this->type) {
            case HTTP_REQUEST:
                this->currentState = HTTP_PARSER_STATE_METHOD_START;
                break;
            case HTTP_RESPONSE:
                this->currentState = HTTP_PARSER_STATE_VERSION_START;
                break;
            default:
                this->currentState = HTTP_PARSER_STATE_ERROR;
        }
        this->headers = new std::map<std::string, std::string>;
        this->method = HTTP_METHOD_NONE;
        this->version = HTTP_VERSION_NONE;
        this->status = 0;
        this->reason = "";
        this->uri = "";
        this->stringData = "";
    }

    HttpParser::~HttpParser() {
        delete this->headers;
    }

    void HttpParser::parseString(std::string source) {
        for (char charItem : source) {
            this->handleCharItem(charItem);
            if (this->currentState == HTTP_PARSER_STATE_ERROR) {
                return;
            }
        }
    }

    std::string HttpParser::getHeader(std::string key) {
        auto it = this->headers->find(key);
        return it != this->headers->end() ? it->second : "";
    }

    HttpMethod HttpParser::getMethod() {
        return this->method;
    }

    HttpVersion HttpParser::getVersion() {
        return this->version;
    }

    std::string HttpParser::getReason() {
        return this->reason;
    }

    std::string HttpParser::getUri() {
        return this->uri;
    }

    void HttpParser::handleCharItem(char charItem) {
        switch (this->currentState) {
            case HTTP_PARSER_STATE_METHOD_START:
                this->handleMethodCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_METHOD_END:
                if (isspace(charItem)) {
                    this->currentState = HTTP_PARSER_STATE_URI_START;
                } else {
                    this->currentState = HTTP_PARSER_STATE_ERROR;
                }
                break;
            case HTTP_PARSER_STATE_STATUS_START:
                this->handleStatusCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_STATUS_END:
                if (isspace(charItem)) {
                    this->currentState = HTTP_PARSER_STATE_REASON_START;
                } else {
                    this->currentState = HTTP_PARSER_STATE_ERROR;
                }
                break;
            case HTTP_PARSER_STATE_REASON_START:
                this->handleReasonCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_REASON_END:
                this->currentState = HTTP_PARSER_STATE_KEY_START;
                this->handleHeaderKeyCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_URI_START:
                this->handleUriCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_URI_END:
                this->currentState = HTTP_PARSER_STATE_VERSION_START;
                this->handleVersionCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_VERSION_START:
                this->handleVersionCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_VERSION_END:
                if (this->type == HTTP_REQUEST) {
                    this->currentState = HTTP_PARSER_STATE_KEY_START;
                    this->handleHeaderKeyCharItem(charItem);
                } else {
                    this->currentState = HTTP_PARSER_STATE_STATUS_START;
                    this->handleStatusCharItem(charItem);
                }
                break;
            case HTTP_PARSER_STATE_KEY_START:
                this->handleHeaderKeyCharItem(charItem);
                break;
            case HTTP_PARSER_STATE_KEY_END:
                break;
            case HTTP_PARSER_STATE_VALUE_START:
                break;
            case HTTP_PARSER_STATE_VALUE_END:
                break;
            case HTTP_PARSER_STATE_END:
                break;
            case HTTP_PARSER_STATE_ERROR:
                break;
        }
    }

    void HttpParser::handleMethodCharItem(char charItem) {
        this->stringData += charItem;
        if (this->stringData == "OPTIONS") {
            this->method = HTTP_METHOD_OPTIONS;
        } else if (this->stringData == "GET") {
            this->method = HTTP_METHOD_GET;
        } else if (this->stringData == "HEAD") {
            this->method = HTTP_METHOD_HEAD;
        } else if (this->stringData == "POST") {
            this->method = HTTP_METHOD_POST;
        } else if (this->stringData == "PUT") {
            this->method = HTTP_METHOD_PUT;
        } else if (this->stringData == "PATCH") {
            this->method = HTTP_METHOD_PATCH;
        } else if (this->stringData == "DELETE") {
            this->method = HTTP_METHOD_DELETE;
        } else if (this->stringData == "TRACE") {
            this->method = HTTP_METHOD_TRACE;
        } else if (this->stringData == "CONNECT") {
            this->method = HTTP_METHOD_CONNECT;
        } else if (isspace(charItem) || this->stringData.size() > 7) {
            this->currentState = HTTP_PARSER_STATE_ERROR;
            this->stringData = "";
        }
        if (this->method != HTTP_METHOD_NONE) {
            this->currentState = HTTP_PARSER_STATE_METHOD_END;
            this->stringData = "";
        }
    }

    void HttpParser::handleUriCharItem(char charItem) {
        if (isspace(charItem) || charItem == '\n') {
            this->uri = this->stringData;
            this->stringData = "";
            if (isspace(charItem)) {
                this->currentState = HTTP_PARSER_STATE_URI_END;
            } else {
                if (this->method == HTTP_METHOD_GET) {
                    this->currentState = HTTP_PARSER_STATE_VERSION_END;
                    this->version = HTTP_VERSION_09;
                } else {
                    this->currentState = HTTP_PARSER_STATE_ERROR;
                }
            }
        } else {
            this->stringData += charItem;
        }
    }

    void HttpParser::handleVersionCharItem(char charItem) {
        this->stringData += charItem;
        if (this->stringData == "HTTP/1.0") {
            this->version= HTTP_VERSION_10;
        } else if (this->stringData == "HTTP/1.1") {
            this->version = HTTP_VERSION_11;
        } else if (this->stringData == "HTTP/2") {
            this->version = HTTP_VERSION_20;
        } else if (isspace(charItem) || this->stringData.size() > 8) {
            this->currentState = HTTP_PARSER_STATE_ERROR;
            this->stringData = "";
        }
        if (this->version != HTTP_VERSION_NONE) {
            this->currentState = HTTP_PARSER_STATE_VERSION_END;
            this->stringData = "";
        }
    }

    void HttpParser::handleHeaderKeyCharItem(char charItem) {

    }

    void HttpParser::handleStatusCharItem(char charItem) {
        switch(charItem) {
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
                if (this->stringData.size() == 3) {
                    std::string::size_type stringTypeSize;
                    this->status = std::stoi(this->stringData, &stringTypeSize);
                    this->currentState = HTTP_PARSER_STATE_STATUS_END;
                    this->stringData = "";
                }
            default:
                this->currentState = HTTP_PARSER_STATE_ERROR;
                this->stringData = "";
        }
    }

    void HttpParser::handleReasonCharItem(char charItem) {
        if (charItem == '\n') {
            this->reason = this->stringData;
            this->currentState = HTTP_PARSER_STATE_REASON_END;
            this->stringData = "";
        } else {
            this->stringData += charItem;
        }
    }

}
