#include "word_tokeniser.h"

#include <unicode/ubrk.h>
#include <stdbool.h>

typedef struct {

    //File-Level
    uint32_t max_chunk_size;
    const char *locale;
    UErrorCode status;
    const uint8_t *text;
    UText *utf;

    //Outer-loop
    uint64_t remaining;
    uint64_t chunk_offset;
    UBreakIterator *it;

    //Inner-loop
    int32_t start;
    uint32_t end;

    void (*word_func)(const size_t, const uint8_t*);

} machine_t;

static inline void inner_loop(machine_t *m) {

    while (m->end < m->max_chunk_size) {

        m->start = m->end;                
        m->end = ubrk_next(m->it);

        if(m->end == (uint32_t)UBRK_DONE) {
            return;
        }

        uint64_t position_in_file = m->chunk_offset + m->start;
        (*m->word_func)(m->end - m->start, m->text + position_in_file);
    }
}

#define MIN(a,b) a < b ? a : b

static void outer_loop(machine_t *m) {

    while (m->end != (uint32_t)UBRK_DONE) {

        //(Re)position iterator
        m->utf = utext_openUTF8(m->utf,
                                (const char*)m->text + m->chunk_offset,
                                MIN(m->remaining, m->max_chunk_size * 2),
                                &m->status);

        m->it = ubrk_open(UBRK_WORD, m->locale, NULL, -1, &m->status);
        ubrk_setUText(m->it, m->utf, &m->status);
        m->end = ubrk_first(m->it);

        //Iterate
        inner_loop(m);
        m->chunk_offset += m->end;
        m->remaining -= m->end;
        ubrk_close(m->it);
    }
}

void apply_to_words(const size_t sz,
                    const uint8_t *data,
                    void (*word_func)(const size_t, const uint8_t*)) {
    machine_t m;
    m.max_chunk_size = 2 * 1024 * 1024;
    m.locale = "";
    m.status = U_ZERO_ERROR;
    m.text = data;
    m.utf = NULL;
    m.remaining = sz;
    m.chunk_offset = 0;
    m.start = 0;
    m.end = 0;
    m.word_func = word_func;
    outer_loop(&m);
    utext_close(m.utf);
    u_cleanup();
}

