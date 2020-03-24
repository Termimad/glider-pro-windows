#include "ByteIO.h"
#include "WinAPI.h"
#include <limits.h>
#include <stdlib.h>

typedef int ASSERT_CHAR_BIT_IS_EIGHT[(CHAR_BIT == 8) ? 1 : -1];

//--------------------------------------------------------------

static int minimal_read(byteio *stream, void *buffer, size_t size);
static int minimal_write(byteio *stream, const void *buffer, size_t size);

typedef struct handle_reader {
	HANDLE hFile;
	size_t size;
	const unsigned char *bufptr;
	unsigned char buffer[8 * 1024];
} handle_reader;

static handle_reader *handle_reader_init(HANDLE hFile);
static int handle_reader_read(byteio *stream, void *buffer, size_t size);
static void handle_reader_close(byteio *stream);

typedef struct handle_writer {
	HANDLE hFile;
	size_t size;
	unsigned char *bufptr;
	unsigned char buffer[8 * 1024];
} handle_writer;

static handle_writer *handle_writer_init(HANDLE hFile);
static int handle_writer_write(byteio *stream, const void *buffer, size_t size);
static void handle_writer_close(byteio *stream);

typedef struct memory_reader {
	size_t size;
	size_t pos;
	const unsigned char *buffer;
} memory_reader;

static memory_reader *memory_reader_init(const void *buffer, size_t size);
static int memory_reader_read(byteio *stream, void *buffer, size_t size);
static int memory_reader_close(byteio *stream);

typedef struct memory_writer {
	size_t size;
	size_t pos;
	unsigned char *buffer;
} memory_writer;

static memory_writer *memory_writer_init(void *buffer, size_t size);
static int memory_writer_write(byteio *stream, const void *buffer, size_t size);
static int memory_writer_close(byteio *stream);

//--------------------------------------------------------------

int byteio_read(byteio *stream, void *buffer, size_t size)
{
	if (stream == NULL || buffer == NULL)
		return 0;
	return stream->fn_read(stream, buffer, size);
}

int byteio_skip(byteio *stream, size_t size)
{
	if (stream == NULL)
		return 0;
	return stream->fn_read(stream, NULL, size);
}

int byteio_write(byteio *stream, const void *buffer, size_t size)
{
	if (stream == NULL || buffer == NULL)
		return 0;
	return stream->fn_write(stream, buffer, size);
}

void byteio_close(byteio *stream)
{
	if (stream != NULL)
		stream->fn_close(stream);
}

//--------------------------------------------------------------

int byteio_read_be_u8(byteio *stream, uint8_t *num)
{
	unsigned char buf[sizeof(*num)];
	if (num == NULL || byteio_read(stream, buf, sizeof(buf)) == 0)
		return 0;
	*num = (uint8_t)buf[0];
	return 1;
}

int byteio_read_le_u8(byteio *stream, uint8_t *num)
{
	unsigned char buf[sizeof(*num)];
	if (num == NULL || byteio_read(stream, buf, sizeof(buf)) == 0)
		return 0;
	*num = (uint8_t)buf[0];
	return 1;
}

int byteio_read_be_i8(byteio *stream, int8_t *num)
{
	return byteio_read_be_u8(stream, (uint8_t *)num);
}

int byteio_read_le_i8(byteio *stream, int8_t *num)
{
	return byteio_read_le_u8(stream, (uint8_t *)num);
}

int byteio_read_be_u16(byteio *stream, uint16_t *num)
{
	unsigned char buf[sizeof(*num)];
	if (num == NULL || byteio_read(stream, buf, sizeof(buf)) == 0)
		return 0;
	*num = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];
	return 1;
}

int byteio_read_le_u16(byteio *stream, uint16_t *num)
{
	unsigned char buf[sizeof(*num)];
	if (num == NULL || byteio_read(stream, buf, sizeof(buf)) == 0)
		return 0;
	*num = (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
	return 1;
}

int byteio_read_be_i16(byteio *stream, int16_t *num)
{
	return byteio_read_be_u16(stream, (uint16_t *)num);
}

int byteio_read_le_i16(byteio *stream, int16_t *num)
{
	return byteio_read_le_u16(stream, (uint16_t *)num);
}

int byteio_read_be_u32(byteio *stream, uint32_t *num)
{
	unsigned char buf[sizeof(*num)];
	if (num == NULL || byteio_read(stream, buf, sizeof(buf)) == 0)
		return 0;
	*num = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
			((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
	return 1;
}

int byteio_read_le_u32(byteio *stream, uint32_t *num)
{
	unsigned char buf[sizeof(*num)];
	if (num == NULL || byteio_read(stream, buf, sizeof(buf)) == 0)
		return 0;
	*num = (uint32_t)buf[0] | ((uint32_t)buf[1] << 8) |
			((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
	return 1;
}

int byteio_read_be_i32(byteio *stream, int32_t *num)
{
	return byteio_read_be_u32(stream, (uint32_t *)num);
}

int byteio_read_le_i32(byteio *stream, int32_t *num)
{
	return byteio_read_le_u32(stream, (uint32_t *)num);
}

//--------------------------------------------------------------

int byteio_write_be_u8(byteio *stream, uint8_t num)
{
	unsigned char buf[sizeof(num)];
	buf[0] = (unsigned char)(num & 0xff);
	return byteio_write(stream, buf, sizeof(buf));
}

int byteio_write_le_u8(byteio *stream, uint8_t num)
{
	unsigned char buf[sizeof(num)];
	buf[0] = (unsigned char)(num & 0xff);
	return byteio_write(stream, buf, sizeof(buf));
}

int byteio_write_be_i8(byteio *stream, int8_t num)
{
	return byteio_write_be_u8(stream, (uint8_t)num);
}

int byteio_write_le_i8(byteio *stream, int8_t num)
{
	return byteio_write_le_u8(stream, (uint8_t)num);
}

int byteio_write_be_u16(byteio *stream, uint16_t num)
{
	unsigned char buf[sizeof(num)];
	buf[0] = (unsigned char)((num >> 8) & 0xff);
	buf[1] = (unsigned char)(num & 0xff);
	return byteio_write(stream, buf, sizeof(buf));
}

int byteio_write_le_u16(byteio *stream, uint16_t num)
{
	unsigned char buf[sizeof(num)];
	buf[0] = (unsigned char)(num & 0xff);
	buf[1] = (unsigned char)((num >> 8) & 0xff);
	return byteio_write(stream, buf, sizeof(buf));
}

int byteio_write_be_i16(byteio *stream, int16_t num)
{
	return byteio_write_be_u16(stream, (uint16_t)num);
}

int byteio_write_le_i16(byteio *stream, int16_t num)
{
	return byteio_write_le_u16(stream, (uint16_t)num);
}

int byteio_write_be_u32(byteio *stream, uint32_t num)
{
	unsigned char buf[sizeof(num)];
	buf[0] = (unsigned char)((num >> 24) & 0xff);
	buf[1] = (unsigned char)((num >> 16) & 0xff);
	buf[2] = (unsigned char)((num >> 8) & 0xff);
	buf[3] = (unsigned char)(num & 0xff);
	return byteio_write(stream, buf, sizeof(buf));
}

int byteio_write_le_u32(byteio *stream, uint32_t num)
{
	unsigned char buf[sizeof(num)];
	buf[0] = (unsigned char)(num & 0xff);
	buf[1] = (unsigned char)((num >> 8) & 0xff);
	buf[2] = (unsigned char)((num >> 16) & 0xff);
	buf[3] = (unsigned char)((num >> 24) & 0xff);
	return byteio_write(stream, buf, sizeof(buf));
}

int byteio_write_be_i32(byteio *stream, int32_t num)
{
	return byteio_write_be_u32(stream, (uint32_t)num);
}

int byteio_write_le_i32(byteio *stream, int32_t num)
{
	return byteio_write_le_u32(stream, (uint32_t)num);
}

//--------------------------------------------------------------

static int minimal_read(byteio *stream, void *buffer, size_t num)
{
	UNREFERENCED_PARAMETER(stream);
	UNREFERENCED_PARAMETER(buffer);
	UNREFERENCED_PARAMETER(num);
	return 0;
}

static int minimal_write(byteio *stream, const void *buffer, size_t num)
{
	UNREFERENCED_PARAMETER(stream);
	UNREFERENCED_PARAMETER(buffer);
	UNREFERENCED_PARAMETER(num);
	return 0;
}

//--------------------------------------------------------------

static handle_reader *handle_reader_init(HANDLE hFile)
{
	handle_reader *self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;
	self->hFile = hFile;
	self->bufptr = &self->buffer[0];
	self->size = 0;
	return self;
}

static int handle_reader_read(byteio *stream, void *buffer, size_t size)
{
	unsigned char *outptr = buffer;
	size_t readsize;
	handle_reader *self = stream->priv;

	if (self == NULL)
		return 0;
	while (size != 0)
	{
		// Fill up the buffer, if needed
		if (self->size == 0)
		{
			DWORD numRead;
			self->bufptr = &self->buffer[0];
			if (!ReadFile(self->hFile, self->buffer, sizeof(self->buffer), &numRead, NULL))
				return 0;
			self->size = numRead;
		}
		// Read as much as we can into the output
		readsize = (self->size <= size) ? self->size : size;
		if (readsize == 0)
			return 0;
		if (outptr != NULL)
		{
			memcpy(outptr, self->bufptr, readsize);
			outptr += readsize;
		}
		size -= readsize;
		self->bufptr += readsize;
		self->size -= readsize;
	}
	return 1;
}

static void handle_reader_close(byteio *stream)
{
	free(stream->priv);
	stream->priv = NULL;
}

int byteio_init_handle_reader(byteio *stream, void *hFile)
{
	if (stream == NULL)
		return 0;
	stream->fn_read = handle_reader_read;
	stream->fn_write = minimal_write;
	stream->fn_close = handle_reader_close;
	stream->priv = handle_reader_init(hFile);
	if (stream->priv == NULL)
		return 0;
	return 1;
}
