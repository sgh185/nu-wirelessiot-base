/*
 * Simple ring buffer implementation for byte values
 */ 
#pragma once

#include "config.h"


/*
 * ---------- Setup, definitions ----------
 */ 
#define DEFAULT_RING_BUF_SIZE 16

typedef struct ring_buffer_t {

    /*
     * Buffer 
     */ 
    uint8_t buf[DEFAULT_RING_BUF_SIZE] ;

    /*
     * State
     */ 
    uint8_t next_index_to_fill /*, capacity, size */ ;


} ring_buffer ;


/*
 * ---------- Operations ----------
 */ 

/*
 * NOTE --- This only supports the operations needed
 * in the protocol/implementation of the system
 *
 * NOTE --- Why are these statement expressions? b/c
 * I didn't think this through.
 */

/*
 * _incr_rb_next
 *
 * Increments the ring buffer's .next_index_to_fill field
 *
 * @rb
 * - Ring buffer to push to
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
#define _incr_rb_next(rb) (rb.next_index_to_fill = ((rb.next_index_to_fill + 1) % DEFAULT_RING_BUF_SIZE))


/*
 * rb_get
 *
 * Gets the value stored at a particular index. Performs
 * no bounds checking.
 *
 * @idx:
 * - Index of value to fetch
 * - typeof(@idx) = uint8_t
 *
 * @rb
 * - Ring buffer to work with
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
#define rb_get(idx, rb) (rb.buf[idx])


/*
 * rb_push
 *
 * Pushes a value to the ring buffer's next index to fill
 *
 * @val:
 * - Value to push
 * - typeof(@val) = uint8_t
 *
 * @rb
 * - Ring buffer to push to
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
#define rb_push(val, rb) \
({ \
    rb.buf[rb.next_index_to_fill] = val; \
    _incr_rb_next(rb); \
})


/*
 * rb_find_once
 *
 * Finds a value and returns the index of its first 
 * occurrence, return -1 otherwise 
 *
 * @val:
 * - Value to find
 * - typeof(@val) = uint8_t
 *
 * @rb
 * - Ring buffer to look in
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
#define rb_find_once(val, rb) \
({ \
    /*
     * Setup
     */ \
    uint8_t idx; \
    \
    \
    /*
     * Iterate
     */ \
    for (idx = 0 ; idx < DEFAULT_RING_BUF_SIZE ; idx++) \
	if (rb.buf[idx] == val) \
	    break; \
    \
    \
    /*
     * If @val not found, set return to -1
     */ \
    if (idx == DEFAULT_RING_BUF_SIZE) idx = -1; \
    \
    \
    /*
     * Return the index
     */ \
    idx; \
})


/*
 * rb_find_all
 *
 * Finds a value and fills out an array of all 
 * matching indices and returns the number of 
 * occurrences total
 *
 * @val:
 * - Value to find 
 * - typeof(@val) = uint8_t
 *
 * @rb
 * - Ring buffer to look in
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 *
 * @arr
 * - Array to populate with matching indices
 * - typeof(@arr) = uint8_t * (size = 16 entries)
 *
 * @num_occ
 * - Variable to set with number of indices found
 * - typeof(@num_occ) = uint8_t * (ptr to variable) 
 */ 
#define rb_find_all(val, rb, arr, num_occ) \
({ \
    /*
     * Setup
     */ \
    uint8_t idx, local_num_occ = 0; \
    \
    \
    /*
     * Iterate
     */ \
    for (idx = 0 ; idx < DEFAULT_RING_BUF_SIZE ; idx++) \
	if (rb.buf[idx] == val) \
	    arr[local_num_occ++] = idx; \
    \
    \
    /*
     * @arr is populated, set @num_occ, and we're done 
     */ \
    *num_occ = local_num_occ; \
})















