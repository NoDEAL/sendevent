#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define EVENTS_NUMBER 9
#define DELAY_CUTLINE 1
#define COMMAND_LENGTH 1024

char *values[EVENTS_NUMBER][3] = {
    {"3", "57", "1152"},
    {"3", "50", "0"},
    {"3", "53", ""},
    {"3", "54", ""},
    {"3", "48", "8"},
    {"3", "60", "4294967046"},
    {"0", "0", "0"},
    {"3", "57", "4294967295"},
    {"0", "0", "0"},
};

int main(int argc, char *argv[]) {
    int fd;
    ssize_t ret;
    int version;
    struct input_event events[EVENTS_NUMBER];
    
    if(argc != 4) {
        fprintf(stderr, "use: %s X Y DURATION\n", argv[0]);
        return 1;
    }

    fd = open("/dev/input/event1", O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "could not open %s, %s\n", argv[optind], strerror(errno));
        return 1;
    }

    values[2][2] = argv[1];
    values[3][2] = argv[2];

    if (ioctl(fd, EVIOCGVERSION, &version)) {
        fprintf(stderr, "could not get driver version for %s, %s\n", argv[optind], strerror(errno));
        return 1;
    }
    
    char *sleep = (char*) malloc(COMMAND_LENGTH);
    strcat(sleep, "sleep ");
    strcat(sleep, argv[3]);
    
    for (int i = 0; i < EVENTS_NUMBER; i++) {
        if (i == (EVENTS_NUMBER - DELAY_CUTLINE)) {
            system(sleep);
            free(sleep);
        }
        int ret = work(fd, events[i], values[i]);
        if (ret == -1) {
            fprintf(stderr, "return value: %d\n", ret);
            return ret;
        }
    }

    return 0;
}

int work(int fd, struct input_event eventIn, char *data[]) {
    memset(&eventIn, 0, sizeof(eventIn));
    fprintf(stderr, "%s, %s, %s\n", data[0], data[1], data[2]);
    eventIn.type = atoi(data[0]);
    eventIn.code = atoi(data[1]);
    eventIn.value = atoi(data[2]);
    ssize_t ret = write(fd, &eventIn, sizeof(eventIn));

    if(ret < (ssize_t) sizeof(eventIn)) {
        fprintf(stderr, "write event failed, %s\n", strerror(errno));
        return -1;
    }
    
    return 0;
}
