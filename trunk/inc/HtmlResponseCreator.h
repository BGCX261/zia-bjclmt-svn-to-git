#pragma once

#include    <iostream>
#include    <string>
#include    <map>

class HtmlResponseCreator
{
public:
    HtmlResponseCreator(void);
    ~HtmlResponseCreator(void);

    std::string createResponse();
    std::string getHeader(std::string name);

    void set_http_version(const std::string http_version);
    void set_status_code(const std::string status_code);
    void set_reason_phrase(const std::string reason_phrase);
    //general headers
    void set_cache_control(const std::string cache_control);
    void set_connexion(const std::string connexion);
    void set_date(const std::string date);
    void set_pragma(const std::string pragma);
    void set_trailer(const std::string trailer);
    void set_transfer_encoding(const std::string transfer_encoding);
    void set_upgrade(const std::string upgrade);
    void set_via(const std::string via);
    void set_warning(const std::string warning);
    //response headers
    void set_accept_ranges(const std::string accept_ranges);
    void set_age(const std::string age);
    void set_etag(const std::string etag);
    void set_location(const std::string location);
    void set_proxy_authenticate(const std::string proxy_authenticate);
    void set_retry_after(const std::string retry_after);
    void set_server(const std::string server);
    void set_vary(const std::string vary);
    void set_www_authenticate(const std::string www_authenticate);
    //entity headers
    void set_allow(const std::string allow);
    void set_content_encoding(const std::string content_encoding);
    void set_content_language(const std::string content_language);
    void set_content_length(const std::string content_length);
    void set_content_location(const std::string content_location);
    void set_content_md5(const std::string content_md5);
    void set_content_range(const std::string content_range);
    void set_content_type(const std::string content_type);
    void set_expires(const std::string expires);
    void set_last_modified(const std::string last_modified);
    void set_extension_header(const std::string extension_header);

private:
    std::map<std::string, std::string>  status_line;
    std::map<std::string, std::string>  headers;
    std::string                         body;
};
