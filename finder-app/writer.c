#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include <errno.h>

int main(int argc, char **argv){
    openlog("writer", LOG_PID, LOG_USER);
    if (argc != 3){
        printf("Error: Please specify writefile and writestr");
        return 1;
    }
    FILE *fp = fopen(argv[1], "w");
    if (fp == NULL){
        printf("Error: Could not open file %s", argv[1]);
        syslog(LOG_ERR, "Error: Could not open file %s", argv[1]);
        return 1;
    }
    fprintf(fp, "%s", argv[2]);
    fclose(fp);
    syslog(LOG_DEBUG, "Writing %s to %s", argv[2], argv[1]);

    if (errno){
        syslog(LOG_ERR, "Error: %s", strerror(errno));
        return 1;
    }
}

