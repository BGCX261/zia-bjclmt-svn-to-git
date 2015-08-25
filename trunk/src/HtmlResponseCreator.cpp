#include "HtmlResponseCreator.h"


HtmlResponseCreator::HtmlResponseCreator(void)
{
    //status line
    this->status_line["Http-Version"] = "";
    this->status_line["Status-Code"] = "";
    this->status_line["Reason-Phrase"] = "";
    //general headers
    this->headers["Cache-Control"] = "";
    this->headers["Connexion"] = "";
    this->headers["Date"] = "";
    this->headers["Pragma"] = "";
    this->headers["Trailer"] = "";
    this->headers["Transfer-Encoding"] = "";
    this->headers["Upgrade"] = "";
    this->headers["Via"] = "";
    this->headers["Warning"] = "";
    //response headers
    this->headers["Accept-Ranges"] = "";
    this->headers["Age"] = "";
    this->headers["ETag"] = "";
    this->headers["Location"] = "";
    this->headers["Proxy-Authenticate"] = "";
    this->headers["Retry-After"] = "";
    this->headers["Server"] = "";
    this->headers["Vary"] = "";
    this->headers["WWW-Authenticate"] = "";
    //entity headers
    this->headers["Allow"] = "";
    this->headers["Content-Encoding"] = "";
    this->headers["Content-Language"] = "";
    this->headers["Content-Length"] = "";
    this->headers["Content-Location"] = "";
    this->headers["Content-MD5"] = "";
    this->headers["Content-Range"] = "";
    this->headers["Content-Type"] = "";
    this->headers["Expires"] = "";
    this->headers["Last-Modified"] = "";
    this->headers["Extension-Header"] = "";
    this->body = "";
}

HtmlResponseCreator::~HtmlResponseCreator(void){}

std::string HtmlResponseCreator::createResponse()
{
    std::string         response;

    if (this->status_line["Http-Version"] != ""
        && this->status_line["Status-Code"] != ""
        && this->status_line["Reason-Phrase"] != "")
    {
        response += this->status_line["Http-Version"];
        response += ' ';
        response += this->status_line["Status-Code"];
        response += ' ';
        response += this->status_line["Reason-Phrase"];
        response += "\r\n";
    }
    
    std::map<std::string, std::string>::iterator    cur = this->headers.begin();
    std::map<std::string, std::string>::iterator    end = this->headers.end();

    for (;cur != end; ++cur)
    {
        if ((*cur).second != "")
        {
            response += (*cur).first;
            response += ": ";
            response += (*cur).second;
            response += "\r\n";
        }
    }
    response += "\r\n";
    return (response);
}

std::string HtmlResponseCreator::getHeader(std::string name)
{
    return (this->headers[name]);
}

void HtmlResponseCreator::set_http_version(const std::string http_version)
{
	this->status_line["Http-Version"] = http_version;
}

void HtmlResponseCreator::set_status_code(const std::string status_code)
{
	this->status_line["Status-Code"] = status_code;
}

void HtmlResponseCreator::set_reason_phrase(const std::string reason_phrase)
{
	this->status_line["Reason-Phrase"] = reason_phrase;
}

//general headers
void HtmlResponseCreator::set_cache_control(const std::string cache_control)
{
	this->headers["Cache-Control"] = cache_control;
}

void HtmlResponseCreator::set_connexion(const std::string connexion)
{
	this->headers["Connexion"] = connexion;
}

void HtmlResponseCreator::set_date(const std::string date)
{
	this->headers["Date"] = date;
}

void HtmlResponseCreator::set_pragma(const std::string pragma)
{
	this->headers["Pragma"] = pragma;
}

void HtmlResponseCreator::set_trailer(const std::string trailer)
{
	this->headers["Trailer"] = trailer;
}

void HtmlResponseCreator::set_transfer_encoding(const std::string transfer_encoding)
{
	this->headers["Transfer-Encoding"] = transfer_encoding;
}

void HtmlResponseCreator::set_upgrade(const std::string upgrade)
{
	this->headers["Upgrade"] = upgrade;
}

void HtmlResponseCreator::set_via(const std::string via)
{
	this->headers["Via"] = via;
}

void HtmlResponseCreator::set_warning(const std::string warning)
{
	this->headers["Warning"] = warning;
}

//response headers
void HtmlResponseCreator::set_accept_ranges(const std::string accept_ranges)
{
	this->headers["Accept-Ranges"] = accept_ranges;
}

void HtmlResponseCreator::set_age(const std::string age)
{
	this->headers["Age"] = age;
}

void HtmlResponseCreator::set_etag(const std::string etag)
{
	this->headers["ETag"] = etag;
}

void HtmlResponseCreator::set_location(const std::string location)
{
	this->headers["Location"] = location;
}

void HtmlResponseCreator::set_proxy_authenticate(const std::string proxy_authenticate)
{
	this->headers["Proxy-Authenticate"] = proxy_authenticate;
}

void HtmlResponseCreator::set_retry_after(const std::string retry_after)
{
	this->headers["Retry-After"] = retry_after;
}

void HtmlResponseCreator::set_server(const std::string server)
{
	this->headers["Server"] = server;
}

void HtmlResponseCreator::set_vary(const std::string vary)
{
	this->headers["Vary"] = vary;
}

void HtmlResponseCreator::set_www_authenticate(const std::string www_authenticate)
{
	this->headers["WWW-Authenticate"] = www_authenticate;
}

//entity headers
void HtmlResponseCreator::set_allow(const std::string allow)
{
	this->headers["Allow"] = allow;
}

void HtmlResponseCreator::set_content_encoding(const std::string content_encoding)
{
	this->headers["Content-Encoding"] = content_encoding;
}

void HtmlResponseCreator::set_content_language(const std::string content_language)
{
	this->headers["Content-Language"] = content_language;
}

void HtmlResponseCreator::set_content_length(const std::string content_length)
{
	this->headers["Content-Length"] = content_length;
}

void HtmlResponseCreator::set_content_location(const std::string content_location)
{
	this->headers["Content-Location"] = content_location;
}

void HtmlResponseCreator::set_content_md5(const std::string content_md5)
{
	this->headers["Content-MD5"] = content_md5;
}

void HtmlResponseCreator::set_content_range(const std::string content_range)
{
	this->headers["Content-Range"] = content_range;
}

void HtmlResponseCreator::set_content_type(const std::string content_type)
{
	this->headers["Content-Type"] = content_type;
}

void HtmlResponseCreator::set_expires(const std::string expires)
{
	this->headers["Expires"] = expires;
}

void HtmlResponseCreator::set_last_modified(const std::string last_modified)
{
	this->headers["Last-Modified"] = last_modified;
}

void HtmlResponseCreator::set_extension_header(const std::string extension_header)
{
	this->headers["Extension-Header"] = extension_header;
}
