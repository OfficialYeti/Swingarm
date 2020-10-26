#include "ring_buffer.h"

static struct ring_buffer _buffer[MAX_BUFFER_COUNT];

int8_t ring_buffer_init(ring_buffer_descriptor_t *bd, ring_buffer_attr_t *attr)
{
	static int index = 0;
	uint8_t err = -1;

	if (index > MAX_BUFFER_COUNT || bd == NULL || attr == NULL)
		return err;
	if (attr->data == NULL || attr->item_size <= 0)
		return err;
	/* Checks if buffer count is power of two. ex.: (1000 & 0111 = 0) */
	if ((attr->length & (attr->length - 1)) != 0)
		return err;

	_buffer[index].data = attr->data;
	_buffer[index].length = attr->length;
	_buffer[index].item_size = attr->item_size;
	_buffer[index].head = 0;
	_buffer[index].tail = 0;

	*bd = index++;

	return 0;
}

static uint8_t _is_buffer_full(struct ring_buffer *buff)
{
	return ((buff->head - buff->tail) == buff->length) ? 1 : 0;
}

static uint8_t _is_buffer_empty(struct ring_buffer *buff)
{
	return ((buff->head - buff->tail) == 0U) ? 1 : 0;
}

int8_t ring_buffer_put(ring_buffer_descriptor_t bd, const void *item)
{
	int8_t err = -1;

	if (bd >= MAX_BUFFER_COUNT)
		return err;
	if (_is_buffer_full(&_buffer[bd]) != 0)
		return err;

	/** Our head is constantly incrementing up to UINT32_MAX when it rolls to 0.
	 *	From other side buffer has limited length.
	 *	To define what is our offset for a new item, we are using a bitwise modulo.
	 *	as ex.: head = 23 (0b1 0111), length - 1 = 15 (0b1111) -> 10111 & 1111 = 0111 => offset = 7items
	 **/
	const size_t offset = (_buffer[bd].head & (_buffer[bd].length - 1)) * _buffer[bd].item_size;
	memcpy(&_buffer[bd].data[offset], item, _buffer[bd].item_size);
	_buffer[bd].head++;

	return 0;
}

int8_t ring_buffer_pop(ring_buffer_descriptor_t bd, void *item)
{
	int8_t err = -1;

	if (bd >= MAX_BUFFER_COUNT)
		return err;
	if (_is_buffer_empty(&_buffer[bd]) != 0)
		return err;

	const size_t offset = (_buffer[bd].tail & (_buffer[bd].length - 1)) * _buffer[bd].item_size;
	memcpy(item, &_buffer[bd].data[offset], _buffer[bd].item_size);
	_buffer[bd].tail++;

	return 0;
}

int8_t ring_buffer_skip(ring_buffer_descriptor_t bd)
{
	int8_t err = -1;

	if (bd >= MAX_BUFFER_COUNT)
		return err;
	if (_is_buffer_empty(&_buffer[bd]) != 0)
		return err;

	_buffer[bd].tail++;

	return 0;
}

void *ring_buffer_get_tail_ptr(ring_buffer_descriptor_t bd)
{
	if (bd >= MAX_BUFFER_COUNT)
		return NULL;
	if (_is_buffer_empty(&_buffer[bd]) != 0)
		return NULL;

	const size_t offset = (_buffer[bd].tail & (_buffer[bd].length - 1)) * _buffer[bd].item_size;
	void *tail = &_buffer[bd].data[offset];

	return tail;
}
