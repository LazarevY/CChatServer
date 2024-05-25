//
// Created by yaroslav on 5/25/24.
//
#include <stddef.h>
#include "utils.h"

long long utils_time_in_milliseconds() {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (((long long) tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}
