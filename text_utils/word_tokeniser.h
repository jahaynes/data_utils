#ifndef WORD_TOKENISER
#define WORD_TOKENISER

#include <stddef.h>
#include <stdint.h>

/* Read some UTF-8 'data' of size 'sz' and  *
 * apply the given function to each token   *
 * discovered.                              *
 *                                          *
 * Note: tokens are in-place (not copied),  *
 * and their length is given rather than    *
 * being 0-terminated.                      */
void apply_to_words(const size_t sz,
                    const uint8_t *data,
                    void *state,
                    void (*word_func)(void *state,
                                      const size_t,
                                      const uint8_t*));

#endif

