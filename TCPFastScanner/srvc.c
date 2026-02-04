#include "srvc.h"
#include <string.h>
const char *detect_service(int port, const char *banner){
 if(banner && banner[0]){
 if(strstr(banner,"SSH-"))
             return "SSH";
 if(strstr(banner,"HTTP/"))
             return "HTTP";
 if (strstr(banner,"FTP"))
              return "FTP";
 if(strstr(banner,"SMTP"))
              return "SMTP";
}
if (port == 21) return "FTP?";
if(port == 22) return "SSH?";
if(port == 25) return "SMTP?";
if(port == 80) return "HTTP";
if(port == 443) return "HTTPS(TLS)";
return "unknown";
}
