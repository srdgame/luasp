
#include <syslog.h>

#ifdef DEBUG
#define log(level, format, ...) syslog(level, "%s:%d"#format, __FILE__, __LINE__, __VA_ARGS__)
#else
#define log(...)
#endif

