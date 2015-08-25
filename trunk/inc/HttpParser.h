#ifndef     HTTPPARSER_H_
# define    HTTPPARSER_H_

# include   <map>

# include   "constants.hpp"
# include   "ParserBase.h"

class   HttpParser  : public ParserBase
{
public :

    enum    header_type
    {
        UNKNOWN           = 0,
        ACCEPT,
        ACCEPT_CHARSET,
        ACCEPT_ENCODING,
        ACCEPT_LANGUAGE,
        ACCEPT_RANGES,
        AGE,
        ALLOW,
        AUTHORIZATION,
        CACHE_CONTROL,
        CONNECTION,
        CONTENT_ENCODING,
        CONTENT_LANGUAGE,
        CONTENT_LENGTH,
        CONTENT_LOCATION,
        CONTENT_MD5,
        CONTENT_RANGE,
        CONTENT_TYPE,
        COOKIE,
        DATE,
        ETAG,
        EXPECT,
        EXPIRES,
        FORWARDED,
        FROM,
        HOST,
        IF_MATCH,
        IF_MODIFIED_SINCE,
        IF_NONE_MATCH,
        IF_RANGE,
        IF_UNMODIFIED_SINCE,
        KEEP_ALIVE,
        LAST_MODIFIED,
        LOCATION,
        MAX_FORWARDS,
        PRAGMA,
        PROXY_AUTHENTICATE,
        PROXY_AUTHORIZATION,
        RANGE,
        REFERER,
        RETRY_AFTER,
        SERVER,
        TE,
        TRAILER,
        TRANSFER_ENCODING,
        UPGRADE,
        USER_AGENT,
        VARY,
        VIA,
        WARNING,
        WWW_AUTHENTICATE
    };

    typedef ZIA::API::HTTP_METHOD::Type                     cmd_type;
    typedef std::map<HttpParser::header_type, std::string>  t_header_map;
    typedef struct  s_cmd_line
    {
        cmd_type                            type;
        std::string                         url;
        std::map<std::string, std::string>  options;
        std::string                         version;
    }                     t_cmd_line;

    HttpParser();
    ~HttpParser();
    t_cmd_line      getCmdLineData();
    t_header_map    getHeaderData();

private:
    bool            entryPoint();
    cmd_type        readCmd();
    bool            readFileName();
    bool            readPath();
    bool            readPathOptions();
    bool            readOptionName();
    bool            readOptionValue();
    bool            readVersion();
    bool            readCmdLine();
    bool            readHeader();
    bool            readBody();
    //------
    bool            readSafe();
    bool            readExtra();
    bool            readNational();
    bool            readPunctuation();
    bool            readEscape();
    bool            readUchar();
    bool            readXchar();
    //-------
    bool            readOctet();
    bool            readLowAlpha();
    bool            readHighAlpha();
    bool            readAlpha();
    bool            readDigit();
    bool            readAlphanum();
    bool            readCTL();
    bool            readControl();
    bool            readCRLF();
    bool            readLWS();
    bool            readTEXT();
    bool            readHex();
    bool            readSeparator();
    bool            readToken();
    bool            readCtext();
    bool            readQuotedPair();
    bool            readComment();
    bool            readQdText();
    bool            readQuotedString();
    bool            readUnreserved();
    bool            readReserved();
    bool            readMark();
    //-------
    bool            readHttpVersion();
    bool            readUrlHttp();
    bool            readPort();
    bool            readIpv4Address();
    bool            readTopLabel();
    bool            readDomainLabel();
    bool            readHostname();
    bool            readHost();
    bool            readHostPort();
    bool            readPchar();
    bool            readUric();
    bool            readQuery();
    bool            readParam();
    bool            readSegment();
    bool            readPathSegment();
    bool            readAbsPath();

    std::map<std::string, cmd_type>     types_hash;
    std::map<std::string, header_type>  header_hash;
    
    t_cmd_line      cmd_line;
    t_header_map    header;
    std::string     body;
};

#endif      //HTTPPARSER_H_