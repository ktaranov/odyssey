
/*
 * SHAPITO.
 *
 * Protocol-level PostgreSQL client library.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <so_macro.h>
#include <so_stream.h>
#include <so_header.h>
#include <so_key.h>
#include <so_read.h>

int so_read_startup(uint32_t *len, char **data, uint32_t *size)
{
	if (*size < sizeof(uint32_t))
		return sizeof(uint32_t) - *size;
	/* len */
	uint32_t pos_size = *size;
	char *pos = *data;
	so_stream_read32(len, &pos, &pos_size);
	uint32_t len_to_read;
	len_to_read = *len - *size;
	if (len_to_read > 0)
		return len_to_read;
	/* advance data stream */
	*data += *len;
	*size -= *len;
	/* set actual data size */
	*len  -= sizeof(uint32_t);
	return 0;
}

int so_read(uint32_t *len, char **data, uint32_t *size)
{
	if (*size < sizeof(so_header_t))
		return sizeof(so_header_t) - *size;
	uint32_t pos_size = *size - sizeof(uint8_t);
	char *pos = *data + sizeof(uint8_t);
	/* type */
	so_stream_read32(len, &pos, &pos_size);
	uint32_t len_to_read;
	len_to_read = (*len + sizeof(char)) - *size;
	if (len_to_read > 0)
		return len_to_read;
	/* advance data stream */
	*data += sizeof(uint8_t) + *len;
	*size -= sizeof(uint8_t) + *len;
	/* set actual data size */
	*len  -= sizeof(uint32_t);
	return 0;
}
