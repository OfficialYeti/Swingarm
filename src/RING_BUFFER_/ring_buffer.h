#ifndef RING_BUFFER__RING_BUFFER_H
#define RING_BUFFER__RING_BUFFER_H

#include "stdint.h"
#include "string.h"

/**
  * @brief  This structure contains the user defined attributes of the buffer,
  *         which will be passed to initialization routine.
  * @note   The void* buffer pointer means that user needs to provide memory for data.
  */
typedef struct ring_buffer_attr_t
{
  size_t item_size; /* Size of each buffer element */
  size_t length;    /* Count of elements */
  void *data;       /* Pointer to where buffer holds data */
} ring_buffer_attr_t;

/**
  * @brief  This structure is the buffer main type.
  * @note   Besides fields from ring_buffer_attr_t it has a head and a tail,
  *         that specify offset for first and last element.
  * @note   The head and the tail are defined as volatile, becouse they will be
  *         accessed from diffrent places in application.
  */
struct ring_buffer
{
  size_t item_size;
  size_t length;
  uint8_t *data;
  volatile size_t head; /* Defines next not in use memory block */
  volatile size_t tail; /* Defines last in use memory block */
};

/**
  * @brief  Descriptor is used to access specific buffer.
  */
typedef uint16_t ring_buffer_descriptor_t;

/**
  * @brief  Maximum count of buffers that can be initialized.
  */
#define MAX_BUFFER_COUNT 2

int8_t ring_buffer_init(ring_buffer_descriptor_t *bd, ring_buffer_attr_t *attr);
int8_t ring_buffer_put(ring_buffer_descriptor_t bd, const void *item);
int8_t ring_buffer_pop(ring_buffer_descriptor_t bd, void *item);
int8_t ring_buffer_skip(ring_buffer_descriptor_t bd);
void* ring_buffer_get_tail_ptr(ring_buffer_descriptor_t bd);


#endif /* RING_BUFFER__RING_BUFFER_H */
