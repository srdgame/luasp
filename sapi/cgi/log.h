
#include <syslog.h>

#define log_open() \
	openlog("luasp_cgi", LOG_CONS|LOG_NDELAY, LOG_USER);

#ifdef DEBUG
#define log(level, format, ...) syslog(level, "%s:%d"#format, __FILE__, __LINE__, __VA_ARGS__)
#else
#define log(...)
#endif

