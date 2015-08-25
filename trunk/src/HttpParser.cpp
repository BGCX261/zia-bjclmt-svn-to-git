#include    "HttpParser.h"

HttpParser::HttpParser()
{
    typedef std::map<std::string, header_type>::iterator head_hash_it;

    this->types_hash["UNDEFINED"] = cmd_type::UNDEFINED;
    this->types_hash["GET"] = cmd_type::GET;
    this->types_hash["HEAD"] = cmd_type::HEAD;
    this->types_hash["POST"] = cmd_type::POST;
    this->types_hash["OPTIONS"] = cmd_type::OPTIONS;
    this->types_hash["CONNECT"] = cmd_type::CONNECT;
    this->types_hash["TRACE"] = cmd_type::TRACE;
    this->types_hash["PUT"] = cmd_type::PUT;
    this->types_hash["DELETE"] = cmd_type::DELETE;

    this->header_hash["UNKNOWN"] = header_type::UNKNOWN;
    this->header_hash["Accept"] = header_type::ACCEPT;
    this->header_hash["Accept-Charset"] = header_type::ACCEPT_CHARSET;
    this->header_hash["Accept-Encoding"] = header_type::ACCEPT_ENCODING;
    this->header_hash["Accept-Language"] = header_type::ACCEPT_LANGUAGE;
    this->header_hash["Accept-Ranges"] = header_type::ACCEPT_RANGES;
    this->header_hash["Age"] = header_type::AGE;
    this->header_hash["Allow"] = header_type::ALLOW;
    this->header_hash["Authorization"] = header_type::AUTHORIZATION;
    this->header_hash["Cache-Control"] = header_type::CACHE_CONTROL;
    this->header_hash["Connection"] = header_type::CONNECTION;
    this->header_hash["Content-Encoding"] = header_type::CONTENT_ENCODING;
    this->header_hash["Content-Language"] = header_type::CONTENT_LANGUAGE;
    this->header_hash["Content-Length"] = header_type::CONTENT_LENGTH;
    this->header_hash["Content-Location"] = header_type::CONTENT_LOCATION;
    this->header_hash["Content-MD5"] = header_type::CONTENT_MD5;
    this->header_hash["Content-Range"] = header_type::CONTENT_RANGE;
    this->header_hash["Content-Type"] = header_type::CONTENT_TYPE;
    this->header_hash["Cookie"] = header_type::COOKIE;
    this->header_hash["Date"] = header_type::DATE;
    this->header_hash["ETag"] = header_type::ETAG;
    this->header_hash["Expect"] = header_type::EXPECT;
    this->header_hash["Expires"] = header_type::EXPIRES;
    this->header_hash["Forwarded"] = header_type::FORWARDED;
    this->header_hash["From"] = header_type::FROM;
    this->header_hash["Host"] = header_type::HOST;
    this->header_hash["If-Match"] = header_type::IF_MATCH;
    this->header_hash["If-Modified-Since"] = header_type::IF_MODIFIED_SINCE;
    this->header_hash["If-None-Match"] = header_type::IF_NONE_MATCH;
    this->header_hash["If-Range"] = header_type::IF_RANGE;
    this->header_hash["If-Unmodified-Since"] = header_type::IF_UNMODIFIED_SINCE;
    this->header_hash["Keep-Alive"] = header_type::KEEP_ALIVE;
    this->header_hash["Last-Modified"] = header_type::LAST_MODIFIED;
    this->header_hash["Location"] = header_type::LOCATION;
    this->header_hash["Max-Forwards"] = header_type::MAX_FORWARDS;
    this->header_hash["Pragma"] = header_type::PRAGMA;
    this->header_hash["Proxy-Authenticate"] = header_type::PROXY_AUTHENTICATE;
    this->header_hash["Proxy-Authorization"] = header_type::PROXY_AUTHORIZATION;
    this->header_hash["Range"] = header_type::RANGE;
    this->header_hash["Referer"] = header_type::REFERER;
    this->header_hash["Retry-After"] = header_type::RETRY_AFTER;
    this->header_hash["Server"] = header_type::SERVER;
    this->header_hash["TE"] = header_type::TE;
    this->header_hash["Trailer"] = header_type::TRAILER;
    this->header_hash["Transfer-Encoding"] = header_type::TRANSFER_ENCODING;
    this->header_hash["Upgrade"] = header_type::UPGRADE;
    this->header_hash["User-Agent"] = header_type::USER_AGENT;
    this->header_hash["Vary"] = header_type::VARY;
    this->header_hash["Via"] = header_type::VIA;
    this->header_hash["Warning"] = header_type::WARNING;
    this->header_hash["WWW-Authenticate"] = header_type::WWW_AUTHENTICATE;

    //initialisations
    this->cmd_line.type = cmd_type::UNDEFINED;
    this->cmd_line.url = "";
    this->cmd_line.version = "";

    head_hash_it    cur1 = this->header_hash.begin();
    head_hash_it    end1 = this->header_hash.end();

    while (cur1 != end1)
    {
        if ((*cur1).second != header_type::UNKNOWN)
            this->header.insert(std::pair<header_type, std::string>((*cur1).second, ""));
        ++(cur1);
    }

    this->body = "";
}

HttpParser::~HttpParser(){}

HttpParser::t_cmd_line  HttpParser::getCmdLineData()
{
    return (this->cmd_line);
}

HttpParser::t_header_map HttpParser::getHeaderData()
{
    return (this->header);
}

//--- PARSING METHODS ---//

// TOP LEVEL METHODS
bool                    HttpParser::entryPoint()
{
    if (readCmdLine() && readHeader() && readBody())
        return (true);
    return (false);
}

HttpParser::cmd_type    HttpParser::readCmd()
{
    cmd_type    type = cmd_type::UNDEFINED;
    std::string cmd;

    this->parser->saveContext("read_cmd");
    this->parser->beginCapture("read_cmd");
    if (this->parser->readIdentifier())
    {
        this->parser->endCapture("read_cmd", cmd);
        type = this->types_hash[cmd];
    }
    this->cmd_line.type = type;
    return (type);
}

bool                    HttpParser::readFileName()
{
    bool    success = false;

    this->parser->saveContext("read_cmd");
    if (this->parser->readIdentifier()
        && this->parser->readChar('.')
        && this->parser->readIdentifier())
        success = true;
    if (!success)
        this->parser->restoreContext("read_cmd");
    return (success);
}

bool                    HttpParser::readPath()
{
    bool        success = false;
    std::string url;

    this->parser->saveContext("read_path");
    this->parser->beginCapture("read_path");
    if (this->parser->readChar('/'))
    {
        bool    path = true;
        while (path)
        {
            this->parser->saveContext("read_path_loop");
            if (!(this->parser->readIdentifier()
                && this->parser->readChar('/')))
            {
                this->parser->restoreContext("read_path_loop");
                path = false;
            }
        }
        readFileName();
        this->parser->endCapture("read_path", url);
        readPathOptions();
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_path");
    this->cmd_line.url = url;
    return (success);
}

bool                    HttpParser::readPathOptions()
{
    bool        success = true;
    bool        options_loop = true;

    if (this->parser->readChar('?'))
    {
        while (options_loop)
        {
            this->parser->saveContext("read_path_options");
            this->parser->beginCapture("read_path_options_name");
            if (this->parser->readIdentifier())
            {
                std::string name;

                this->parser->endCapture("read_path_options_name", name);
                if (this->parser->readChar('='))
                {
                    this->cmd_line.options[name] = "";
                    this->parser->beginCapture("read_path_options_value");
                    if (this->parser->readUntil('&'))
                    {
                        this->parser->endCapture("read_path_options_value", this->cmd_line.options[name]);
                        this->parser->readChar('&');
                    }
                    else
                    {
                        if (this->parser->readUntil(' '))
                        {
                            options_loop = false;
                            this->parser->endCapture("read_path_options_value", this->cmd_line.options[name]);
                        }
                        options_loop = false;
                    }
                }
            }
            else
            {
                options_loop = false;
                this->parser->restoreContext("read_path_options");
            }
        }
        this->parser->readUntil(' ');
    }
    return (success);
}

bool                    HttpParser::readOptionName()
{
    bool        success = false;
    std::string option;

    this->parser->saveContext("read_option_name");
    this->parser->beginCapture("read_option_name");
    if (this->parser->readUntil(':'))
    {
        this->parser->endCapture("read_option_name", option);
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_option_name");
    return (success);
}

bool                    HttpParser::readOptionValue()
{
    bool        success = false;

    this->parser->saveContext("read_option_value");
    if (this->parser->readUntil('\r'))
        success = true;
    if (!success)
        this->parser->restoreContext("read_option_value");
    return (success);
}

bool                    HttpParser::readVersion()
{
    bool        success = false;

    this->parser->saveContext("read_version");
    if (this->parser->readText("HTTP/")
        && this->parser->readInteger())
    {
        bool    version_point = true;

        while (version_point)
        {
            this->parser->saveContext("read_version_point");
            if (!(this->parser->readChar('.')
                && this->parser->readInteger()))
            {
                this->parser->restoreContext("read_version_point");
                version_point = false;
            }
        }
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_version");
    return (success);
}

bool                    HttpParser::readCmdLine()
{
    bool        success = false;
    std::string  cmd_line;

    this->parser->saveContext("read_cmd_line");
    if (readCmd() != UNKNOWN
        && this->parser->readChar(' ')
        && (readUrlHttp()
            || readPath()))
    {
        if (this->parser->readChar(' ')
            && readHttpVersion()
            && readCRLF())
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_cmd_line");
    return (success);
}

bool                    HttpParser::readHeader()
{
    bool        success = true;
    std::string header;

    this->parser->saveContext("read_header");

    bool    header_loop = true;

    while (header_loop)
    {
        std::string name;
        std::string value;
        bool        loop_success = false;

        this->parser->saveContext("read_header_loop");
        this->parser->beginCapture("header_name");
        if (readOptionName())
        {
            this->parser->endCapture("header_name", name);
            if (this->parser->readText(": "))
            {
                this->parser->beginCapture("header_value");
                if (readOptionValue())
                {
                    this->parser->endCapture("header_value", value);
                    if (readCRLF())
                        loop_success = true;
                }
            }
        }
        if (!loop_success)
        {
            header_loop = false;
            this->parser->restoreContext("read_header_loop");
        }
        else
            this->header[this->header_hash[name]] = value;
    }
    if (!(readCRLF()))
        this->parser->restoreContext("read_header");
    return (success);
}

bool                    HttpParser::readBody()
{
    this->parser->beginCapture("read_body");
    this->parser->readUntilEOF();
    this->parser->endCapture("read_body", this->body);
    std::cout << "BODY: " << this->body << std::endl;
    return (true);
}

// UTILITIES
bool        HttpParser::readSafe()
{
    bool    success = false;

    this->parser->saveContext("read_safe");
    if (this->parser->readChar('$')
        || this->parser->readChar('-')
        || this->parser->readChar('_')
        || this->parser->readChar('.')
        || this->parser->readChar('+'))
        success = true;
    else
        this->parser->restoreContext("read_safe");
    return (success);
}

bool        HttpParser::readExtra()
{
    bool    success = false;

    this->parser->saveContext("read_extra");
    if (this->parser->readChar('!')
        || this->parser->readChar('*')
        || this->parser->readChar('\'')
        || this->parser->readChar('(')
        || this->parser->readChar(')')
        || this->parser->readChar(','))
        success = true;
    else
        this->parser->restoreContext("read_extra");
    return (success);
}

bool        HttpParser::readNational()
{
    bool    success = false;

    this->parser->saveContext("read_national");
    if (this->parser->readChar('{')
        || this->parser->readChar('}')
        || this->parser->readChar('|')
        || this->parser->readChar('\\')
        || this->parser->readChar('^')
        || this->parser->readChar('~')
        || this->parser->readChar('[')
        || this->parser->readChar(']')
        || this->parser->readChar('`'))
        success = true;
    else
        this->parser->restoreContext("read_national");
    return (success);
}

bool        HttpParser::readPunctuation()
{
    bool    success = false;

    this->parser->saveContext("read_punctuation");
    if (this->parser->readChar('<')
        || this->parser->readChar('>')
        || this->parser->readChar('#')
        || this->parser->readChar('%')
        || this->parser->readChar('"'))
        success = true;
    else
        this->parser->restoreContext("read_punctuation");
    return (success);
}

bool        HttpParser::readEscape()
{
    bool    success = false;

    this->parser->saveContext("read_escape");
    if (this->parser->readChar('%')
        && readHex()
        && readHex())
        success = true;
    else
        this->parser->restoreContext("read_escape");
    return (success);
}

bool        HttpParser::readUchar()
{
    bool    success = false;

    this->parser->saveContext("read_uchar");
    if (readUnreserved()
        || readEscape())
        success = true;
    else
        this->parser->restoreContext("read_uchar");
    return (success);
}

bool        HttpParser::readXchar()
{
    bool    success = false;

    this->parser->saveContext("read_xchar");
    if (readUnreserved()
        || readReserved()
        || readEscape())
        success = true;
    else
        this->parser->restoreContext("read_xchar");
    return (success);
}

// BASIC RULES
bool        HttpParser::readOctet()
{
    bool    success = false;

    this->parser->saveContext("read_octet");
    if (this->parser->readRange(0, 255))
        success = true;
    else
        this->parser->restoreContext("read_octet");
    return (success);
}

bool        HttpParser::readLowAlpha()
{
    bool    success = false;

    this->parser->saveContext("read_low_alpha");
    if (this->parser->readRange('a', 'z'))
        success = true;
    else
        this->parser->restoreContext("read_low_alpha");
    return (success);
}

bool        HttpParser::readHighAlpha()
{
    bool    success = false;

    this->parser->saveContext("read_high_alpha");
    if (this->parser->readRange('A', 'Z'))
        success = true;
    else
        this->parser->restoreContext("read_high_alpha");
    return (success);
}

bool        HttpParser::readAlpha()
{
    bool    success = false;

    this->parser->saveContext("read_alpha");
    if (readLowAlpha()
        || readHighAlpha())
        success = true;
    else
        this->parser->restoreContext("read_alpha");
    return (success);
}

bool        HttpParser::readDigit()
{
    bool    success = false;

    this->parser->saveContext("read_digit");
    if (this->parser->readRange('0', '9'))
        success = true;
    else
        this->parser->restoreContext("read_digit");
    return (success);
}

bool        HttpParser::readAlphanum()
{
    bool    success = false;

    this->parser->saveContext("read_alphanum");
    if (readAlpha()
        || readDigit())
        success = true;
    else
        this->parser->restoreContext("read_alphanum");
    return (success);
}

bool        HttpParser::readCTL()
{
    bool    success = false;
    bool    token = true;

    this->parser->saveContext("read_ctl");
    if (this->parser->readRange(0, 31)
        || this->parser->readChar(127))
        success = true;
    if (!success)
        this->parser->restoreContext("read_ctl");
    return (success);
}

bool        HttpParser::readControl()
{
    bool    success = false;

    this->parser->saveContext("read_control");
    if (this->parser->readRange(0, 31)
        || this->parser->readChar(127))
        success = true;
    else
        this->parser->restoreContext("read_control");
    return (success);
}

bool        HttpParser::readCRLF()
{
    bool    success = false;

    this->parser->saveContext("read_crlf");
    if (this->parser->readChar(13)
        && this->parser->readChar(10))
        success = true;
    else
        this->parser->restoreContext("read_crlf");
    return (success);
}

bool        HttpParser::readLWS()
{
    bool    success = false;
    bool    blanks = true;

    this->parser->saveContext("read_lws");
    readCRLF();
    while (blanks)
    {
        this->parser->saveContext("read_lws_loop");
        if (this->parser->readChar(32)
            || this->parser->readChar(9))
            success = true;
        else
        {
            this->parser->restoreContext("read_lws_loop");
            blanks = false;
        }
    }
    if (!success)
        this->parser->restoreContext("read_lws");
    return (success);
}

bool        HttpParser::readHex()
{
    bool    success = false;

    this->parser->saveContext("read_hex");
    if (readDigit()
        || this->parser->readRange('A', 'F')
        || this->parser->readRange('a', 'f'))
        success = true;
    else
        this->parser->restoreContext("read_hex");
    return (success);
}

bool        HttpParser::readTEXT()
{
    bool    success = false;

    this->parser->saveContext("read_text");
    if (!readCTL()
        && readOctet())
        success = true;
    if (!success)
        this->parser->restoreContext("read_text");
    return (success);
}

bool        HttpParser::readSeparator()
{
    bool    success = false;

    this->parser->saveContext("read_separator");
    if (this->parser->readChar('(')
        || this->parser->readChar(')')
        || this->parser->readChar('<')
        || this->parser->readChar('>')
        || this->parser->readChar('@')
        || this->parser->readChar(',')
        || this->parser->readChar(';')
        || this->parser->readChar(':')
        || this->parser->readChar('\\')
        || this->parser->readChar('"')
        || this->parser->readChar('/')
        || this->parser->readChar('[')
        || this->parser->readChar(']')
        || this->parser->readChar('?')
        || this->parser->readChar('=')
        || this->parser->readChar('{')
        || this->parser->readChar('}')
        || this->parser->readChar(' ')
        || this->parser->readChar('\t'))
        success = true;
    else
        this->parser->restoreContext("read_separators");
    return (success);
}

bool        HttpParser::readToken()
{
    bool    success = false;
    bool    token = true;

    this->parser->saveContext("read_token");
    if (!readCTL()
        && !readSeparator()
        && this->parser->readRange(0, 127))
    if (!success)
        this->parser->restoreContext("read_token");
    return (success);
}

bool        HttpParser::readCtext()
{
    bool    success = false;

    this->parser->saveContext("read_ctext");
    if (!this->parser->readChar('(')
        && !this->parser->readChar(')')
        && readTEXT())
        success = true;
    if (!success)
        this->parser->restoreContext("read_ctext");
    return (success);
}

bool        HttpParser::readQuotedPair()
{
    bool    success = false;

    this->parser->saveContext("read_quoted_pair");
    if (this->parser->readChar('\\')
        && this->parser->readRange(0, 127))
        success = true;
    if (!success)
        this->parser->restoreContext("read_quoted_pair");
    return (success);
}

bool        HttpParser::readComment()
{
    bool    success = false;

    this->parser->saveContext("read_comment");
    if (this->parser->readChar('('))
    {
        bool    comment_loop = true;

        while (comment_loop)
        {
            this->parser->saveContext("read_comment_loop");
            if (!(readCtext()
                || readQuotedPair()
                || readComment()))
            {
                this->parser->restoreContext("read_comment_loop");
                comment_loop = false;
            }
        }
        if (this->parser->readChar(')'))
            success = true;
    }
    if (!success)
        this->parser->restoreContext("read_comment");
    return (success);
}

bool        HttpParser::readQdText()
{
    bool    success = false;

    this->parser->saveContext("read_qd_text");
    if (!this->parser->readChar('"')
        && readTEXT())
        success = true;
    if (!success)
        this->parser->restoreContext("read_qd_text");
    return (success);
}

bool        HttpParser::readQuotedString()
{
    bool    success = false;

    this->parser->saveContext("read_quoted_string");
    if (this->parser->readChar('"'))
    {
        bool    quoted_string_loop = true;

        while (quoted_string_loop)
        {
            this->parser->saveContext("read_quoted_string_loop");
            if (!(readQdText()
                || readQuotedPair()))
            {
                this->parser->restoreContext("read_quoted_string_loop");
                quoted_string_loop = false;
            }
        }
        if (this->parser->readChar('"'))
            success = true;
    }
    if (!success)
        this->parser->restoreContext("read_quoted_string");
    return (success);
}

bool        HttpParser::readUnreserved()
{
    bool    success = false;

    this->parser->saveContext("read_unreserved");
    if (readAlphanum()
        || readMark())
        success = true;
    else
        this->parser->restoreContext("read_unreserved");
    return (success);
}

bool        HttpParser::readReserved()
{
    bool    success = false;

    this->parser->saveContext("read_reserved");
    if (this->parser->readChar(';')
        || this->parser->readChar('/')
        || this->parser->readChar('?')
        || this->parser->readChar(':')
        || this->parser->readChar('@')
        || this->parser->readChar('&')
        || this->parser->readChar('=')
        || this->parser->readChar('+')
        || this->parser->readChar('$')
        || this->parser->readChar(','))
        success = true;
    else
        this->parser->restoreContext("read_reserved");
    return (success);
}

bool        HttpParser::readMark()
{
    bool    success = false;

    this->parser->saveContext("read_mark");
    if (this->parser->readChar('-')
        || this->parser->readChar('_')
        || this->parser->readChar('.')
        || this->parser->readChar('!')
        || this->parser->readChar('~')
        || this->parser->readChar('*')
        || this->parser->readChar('\'')
        || this->parser->readChar('(')
        || this->parser->readChar(')'))
        success = true;
    else
        this->parser->restoreContext("read_mark");
    return (success);
}


//PROTOCOL PARAMETERS
bool        HttpParser::readHttpVersion()
{
    bool        success = false;
    std::string version;

    this->parser->beginCapture("read_http_version");
    this->parser->saveContext("read_http_version");
    if (this->parser->readText("HTTP/")
        && this->parser->readInteger()
        && this->parser->readChar('.')
        && this->parser->readInteger())
        success = true;
    if (!success)
        this->parser->restoreContext("read_http_version");
    this->parser->endCapture("read_http_version", version);
    this->cmd_line.version = version;
    return (success);
}

bool        HttpParser::readUrlHttp()
{
    bool        success = false;
    std::string url;

    this->parser->beginCapture("read_url_http");
    this->parser->saveContext("read_url_http");
    if (this->parser->readText("http://")
        && readHost())
    {
        this->parser->saveContext("read_url_http_opt1");
        if (!(this->parser->readChar(':')
            && readPort()))
            this->parser->restoreContext("read_url_http_opt1");
        this->parser->saveContext("read_url_http_opt2");
        if (readAbsPath())
        {
            this->parser->saveContext("read_url_http_opt3");
            if (!(this->parser->readChar('?')
                && readQuery()))
                this->parser->restoreContext("read_url_http_opt3");
        }
        else
            this->parser->restoreContext("read_url_http_opt2");
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_url_http");
    this->parser->endCapture("read_url_http", url);
    this->cmd_line.url = url;
    return (success);
}

bool        HttpParser::readPort()
{
    bool    success = true;

    this->parser->readInteger();
    return (success);
}

bool        HttpParser::readIpv4Address()
{
    bool    success = false;

    this->parser->saveContext("read_ipv4_address");
    if (this->parser->readInteger()
        && this->parser->readChar('.')
        && this->parser->readInteger()
        && this->parser->readChar('.')
        && this->parser->readInteger()
        && this->parser->readChar('.')
        && this->parser->readInteger())
        success = true;
    if (!success)
        this->parser->restoreContext("read_ipv4_address");
    return (success);
}

bool        HttpParser::readTopLabel()
{
    bool    success = false;

    this->parser->saveContext("read_top_label");
    if (readAlpha())
    {
        bool    top_label_loop = true;
        bool    last_loop = false;

        while (top_label_loop)
        {
            this->parser->saveContext("read_top_label_loop_start");
            if ((readAlphanum()
                || this->parser->readChar('-'))
                    && !last_loop)
            {
                this->parser->saveContext("read_top_label_loop_control");
                if (!(readAlphanum()
                    || this->parser->readChar('-')))
                    last_loop = true;
                else
                    this->parser->restoreContext("read_top_label_loop_control");
            }
            else
            {
                top_label_loop = false;
                if (!last_loop)
                    this->parser->restoreContext("read_top_label_loop_start");
                else
                    this->parser->restoreContext("read_top_label_before_last_loop");
            }
            if (!last_loop)
                this->parser->saveContext("read_top_label_before_last_loop");
        }
        if (readAlphanum())
            success = true;
    }
    if (!success)
    {
        this->parser->restoreContext("read_top_label");
        if (readAlpha())
            success = true;
    }
    if (!success)
        this->parser->restoreContext("read_top_label");
    return (success);
}

bool        HttpParser::readDomainLabel()
{
    bool    success = false;

    this->parser->saveContext("read_domain_label");
    if (readAlphanum())
    {
        bool    domain_label_loop = true;
        bool    last_loop = false;

        while (domain_label_loop)
        {
            this->parser->saveContext("read_domain_label_loop_start");
            if ((readAlphanum()
                || this->parser->readChar('-'))
                    && !last_loop)
            {
                this->parser->saveContext("read_domain_label_loop_control");
                if (!(readAlphanum()
                    || this->parser->readChar('-')))
                    last_loop = true;
                else
                    this->parser->restoreContext("read_domain_label_loop_control");
            }
            else
            {
                domain_label_loop = false;
                if (!last_loop)
                    this->parser->restoreContext("read_domain_label_loop_start");
                else
                    this->parser->restoreContext("read_domain_label_before_last_loop");
            }
            if (!last_loop)
                this->parser->saveContext("read_domain_label_before_last_loop");
        }
        if (readAlphanum())
            success = true;
    }
    if (!success)
    {
        this->parser->restoreContext("read_domain_label");
        if (readAlphanum())
            success = true;
    }
    if (!success)
        this->parser->restoreContext("read_domain_label");
    return (success);
}

bool        HttpParser::readHostname()
{
    bool    success = false;
    bool    host_name_loop = true;

    this->parser->saveContext("read_host_name");
    while (host_name_loop)
    {
        this->parser->saveContext("read_host_name_loop");
        if (!(readDomainLabel()
            && this->parser->readChar('.')))
        {
            this->parser->restoreContext("read_host_name_loop");
            host_name_loop = false;
        }
    }
    if (readTopLabel())
    {
        this->parser->readChar('.');
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_host_name");
    return (success);
}

bool        HttpParser::readHost()
{
    bool    success = false;

    this->parser->saveContext("read_host");
    if (readHostname()
        || readIpv4Address())
        success = true;
    if (!success)
        this->parser->restoreContext("read_host");
    return (success);
}

bool        HttpParser::readHostPort()
{
    bool    success = false;

    this->parser->saveContext("read_host_port");
    if (readHost())
    {
        this->parser->saveContext("read_host_port_opt");
        if (!(this->parser->readChar(':')
            && readPort()))
            this->parser->restoreContext("read_host_port_opt");
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_host_port");
    return (success);
}

bool        HttpParser::readPchar()
{
    bool    success = false;

    this->parser->saveContext("read_pchar");
    if (readUnreserved()
        || readEscape()
        || this->parser->readChar(':')
        || this->parser->readChar('@')
        || this->parser->readChar('&')
        || this->parser->readChar('=')
        || this->parser->readChar('+')
        || this->parser->readChar('$')
        || this->parser->readChar(','))
        success = true;
    if (!success)
        this->parser->restoreContext("read_pchar");
    return (success);
}

bool        HttpParser::readUric()
{
    bool    success = false;

    this->parser->saveContext("read_uric");
    if (readUnreserved()
        || readReserved()
        || readEscape())
        success = true;
    if (!success)
        this->parser->restoreContext("read_uric");
    return (success);
}

bool        HttpParser::readQuery()
{
    bool    success = true;
    bool    query_loop = true;

    while (query_loop)
    {
        this->parser->saveContext("read_query_loop");
        if (!(readUric()))
        {
            this->parser->restoreContext("read_query_loop");
            query_loop = false;
        }
    }
    return (success);
}

bool        HttpParser::readParam()
{
    bool    success = true;
    bool    param_loop = true;

    while (param_loop)
    {
        this->parser->saveContext("read_param_loop");
        if (!(readPchar()))
        {
            this->parser->restoreContext("read_param_loop");
            param_loop = false;
        }
    }
    return (success);
}

bool        HttpParser::readSegment()
{
    bool    success = true;
    bool    segment_loop1 = true;
    bool    segment_loop2 = true;

    while (segment_loop1)
    {
        this->parser->saveContext("read_segment_loop1");
        if (!(readPchar()))
        {
            this->parser->restoreContext("read_segment_loop1");
            segment_loop1 = false;
        }
    }
    while (segment_loop2)
    {
        this->parser->saveContext("read_segment_loop2");
        if (!(this->parser->readChar(';')
            && readParam()))
        {
            this->parser->restoreContext("read_segment_loop2");
            segment_loop2 = false;
        }
    }
    return (success);
}

bool        HttpParser::readPathSegment()
{
    bool    success = false;

    this->parser->saveContext("read_path_segment");
    if (readSegment())
    {
        bool    path_segment_loop = true;
        while (path_segment_loop)
        {
            this->parser->saveContext("read_path_segment_loop");
            if (!(this->parser->readChar('/')
                && readSegment()))
            {
                this->parser->restoreContext("read_path_segment_loop");
                path_segment_loop = false;
            }
        }
        success = true;
    }
    if (!success)
        this->parser->restoreContext("read_path_segment");
    return (success);
}

bool        HttpParser::readAbsPath()
{
    bool    success = false;

    this->parser->saveContext("read_abs_path");
    if (this->parser->readChar('/')
        && readPathSegment())
        success = true;
    if (!success)
        this->parser->restoreContext("read_abs_path");
    return (success);
}