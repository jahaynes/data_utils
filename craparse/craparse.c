#include "craparse.h"

#include <string.h>

const parse_t advance(const parse_t in,
                      const uint64_t amount) {
    const parse_t out = {in.input + amount, in.state, in.remaining - amount};
    return out;
}

const parse_t line(const parse_t in,
                   void (*line_f)(const void *state,
                                  const uint8_t*,
                                  const uint64_t)) {

    uint64_t i;
    int newline_bytes;

    for(i = 0, newline_bytes = 0;
        i < in.remaining;
        i++) {

            if(in.input[i] == 13) {

                if(i + 1 < in.remaining && in.input[i+1] == 10) {
                    newline_bytes++;
                    i++;
                }
                newline_bytes++;
                i++;
                break;
            }

            if(in.input[i] == 10) {
                newline_bytes++;
                i++;
                break;
            }
    }

    if(line_f) {
        line_f(in.state, in.input, i - newline_bytes);
    }

    return advance(in, i);
}

const parse_t bytes(const parse_t in,
                    const uint8_t *bs,
                    const uint64_t len,
                    void (*bytes_f)(const void *state,
                                    const uint8_t*,
                                    const uint64_t)) {

    if (in.remaining < len || memcmp(in.input, bs, len)) {
        return in;
    }

    if(bytes_f) {
        bytes_f(in.state, in.input, len);
    }

    return advance(in, len);
}

const parse_t take(const parse_t in,
                   const uint64_t n,
                   void (*taken_f)(const void *state,
                                   const uint8_t*,
                                   const uint64_t)) {

    if (in.remaining < n) {
        return in;
    }

    if(taken_f) {
        taken_f(in.state, in.input, n);
    }

    return advance(in, n);
}

