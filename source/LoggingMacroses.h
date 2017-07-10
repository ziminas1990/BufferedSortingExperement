#ifndef LOGGING_MACROSES_H
#define LOGGING_MACROSES_H

#define WITH_LOGS

#ifdef WITH_LOGS
#include <stdio.h>

#define LOG(format) \
  printf(format); fflush(stdout);                                             \

#define LOG_1(format, value_1)  \
  printf(format, value_1);  fflush(stdout);

#define LOG_2(format, value_1, value_2)  \
  printf(format, value_1, value_2);  fflush(stdout);

#define LOG_3(format, value_1, value_2, value_3)  \
  printf(format, value_1, value_2, value_3);  fflush(stdout);

#define LOG_4(format, value_1, value_2, value_3, value_4)  \
  printf(format, value_1, value_2, value_3, value_4);  fflush(stdout);

#define LOG_5(format, value_1, value_2, value_3, value_4, value_5)  \
  printf(format, value_1, value_2, value_3, value_4, value_5);  fflush(stdout);

#define LOG_PERSENT(prev, curr, total, format)     \
{                                                  \
  uint8_t nCurPersent  = (100 * (curr)) / (total); \
  uint8_t nPrevPersent = (100 * (prev)) / (total); \
  if(nCurPersent > nPrevPersent) {                 \
    LOG_1(format, nCurPersent);                    \
  }                                                \
}

#else

#define LOG(format)

#define LOG_1(format, value_1)

#define LOG_2(format, value_1, value_2)

#define LOG_3(format, value_1, value_2, value_3)

#define LOG_4(format, value_1, value_2, value_3, value_4)

#define LOG_5(format, value_1, value_2, value_3, value_4, value_5)

#endif

#endif //LOGGING_MACROSES_H
