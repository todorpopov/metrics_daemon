#ifndef METRICS_H
#define METRICS_H

enum metrics_format {
    FORMAT_JSON = 1,
};

char *get_metrics(enum metrics_format format);

#endif