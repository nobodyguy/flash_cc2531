
#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <time.h>

static uint64_t epochMilli, epochMicro;

static void time_init_epoch(void)
{
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  epochMilli = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L);
  epochMicro = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000L);
}

unsigned int time_micros(void)
{
  uint64_t now;
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  now = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000);

  return (uint32_t)(now - epochMicro);
}

unsigned int time_millis(void)
{
  uint64_t now;
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  now = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L);

  return (uint32_t)(now - epochMilli);
}

int time_init()
{
  time_init_epoch();
  return 1;
}

#endif
