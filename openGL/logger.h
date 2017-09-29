#ifndef LOGGER_H
#define LOGGER_H

int logger_restart();
int logger_log(const char *message, ...);
int logger_err(const char *message, ...);

#endif //LOGGER_H
