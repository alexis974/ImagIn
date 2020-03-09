#include <syslog.h>

#include "log.h"

void log_info(char *str)
{
    char *file = "test_log";

    openlog(file, LOG_CONS|LOG_PID, LOG_LOCAL7);
    syslog(LOG_DEBUG, "First try...");
    syslog(LOG_DEBUG, str);
    closelog();
}

