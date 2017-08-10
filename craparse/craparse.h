#ifndef CRAPARSE
#define CRAPARSE

#include <stdint.h>

typedef struct {
    const uint8_t *input;
    const void *state;
    uint64_t remaining;
} parse_t;

const parse_t line(const parse_t in,
                   void (*line_f)(const void *state,
                                  const uint8_t*,
                                  const uint64_t));

const parse_t bytes(const parse_t in,
                    const uint8_t *bs,
                    const uint64_t len,
                    void (*bytes_f)(const void *state,
                                    const uint8_t*,
                                    const uint64_t));

const parse_t take(const parse_t in,
                   const uint64_t n,
                   void (*taken_f)(const void *state,
                                   const uint8_t*,
                                   const uint64_t));

#endif 
