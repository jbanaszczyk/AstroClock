#pragma once
#include <Arduino.h>

#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

extern uint32_t initFreeHeapSize;

const char *getLastRestartReason_P();

char *strcpy_s(char *destination, const char *source, size_t max_len);

char *strcpy_P_s(char *destination, const char *source, size_t max_len);
