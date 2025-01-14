/*
 * Dynamic data buffer
 * Copyright (c) 2007-2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPABUF_H
#define WPABUF_H

/* wpabuf::buf is a pointer to external data */
#define WPABUF_FLAG_EXT_DATA BIT(0)

/*
 * Internal data structure for wpabuf. Please do not touch this directly from
 * elsewhere. This is only defined in header file to allow inline functions
 * from this file to access data.
 */
struct wpabuf {
	size_t size; 		/* total size of the allocated buffer (i.e allocated buffer size)*/
	size_t used; 		/* length of data in the buffer (i.e data length) */
	uint8_t *buf; 		/* pointer to the head of the buffer (i.e buffer address) */ 
	unsigned int flags;
				/* optionally followed by the allocated buffer */
};


int wpabuf_resize(struct wpabuf **buf, size_t add_len);
struct wpabuf * wpabuf_alloc(size_t len);
struct wpabuf * wpabuf_alloc_ext_data(uint8_t *data, size_t len);
struct wpabuf * wpabuf_alloc_copy(const void *data, size_t len);
struct wpabuf * wpabuf_dup(const struct wpabuf *src);
void wpabuf_free(struct wpabuf *buf);
void * wpabuf_put(struct wpabuf *buf, size_t len);
struct wpabuf * wpabuf_concat(struct wpabuf *a, struct wpabuf *b);
struct wpabuf * wpabuf_zeropad(struct wpabuf *buf, size_t len);
void wpabuf_printf(struct wpabuf *buf, char *fmt, ...) PRINTF_FORMAT(2, 3);


/**
 * wpabuf_size - Get the currently allocated size of a wpabuf buffer
 * @buf: wpabuf buffer
 * Returns: Currently allocated size of the buffer
 */
static inline size_t wpabuf_size(const struct wpabuf *buf)
{
	return buf->size;
}

/**
 * wpabuf_len - Get the current length of a wpabuf buffer data
 * @buf: wpabuf buffer
 * Returns: Currently used length of the buffer
 */
static inline size_t wpabuf_len(const struct wpabuf *buf)
{
	return buf->used;
}

/**
 * wpabuf_tailroom - Get size of available tail room in the end of the buffer
 * @buf: wpabuf buffer
 * Returns: Tail room (in bytes) of available space in the end of the buffer
 */
static inline size_t wpabuf_tailroom(const struct wpabuf *buf)
{
	return buf->size - buf->used;
}

/**
 * wpabuf_head - Get pointer to the head of the buffer data
 * @buf: wpabuf buffer
 * Returns: Pointer to the head of the buffer data
 */
static inline const void *wpabuf_head(const struct wpabuf *buf)
{
	return buf->buf;
}

static inline const uint8_t *wpabuf_head_u8(const struct wpabuf *buf)
{
	return wpabuf_head(buf);
}

/**
 * wpabuf_mhead - Get modifiable pointer to the head of the buffer data
 * @buf: wpabuf buffer
 * Returns: Pointer to the head of the buffer data
 */
static inline void *wpabuf_mhead(struct wpabuf *buf)
{
	return buf->buf;
}

static inline uint8_t *wpabuf_mhead_u8(struct wpabuf *buf)
{
	return wpabuf_mhead(buf);
}

static inline void wpabuf_put_u8(struct wpabuf *buf, uint8_t data)
{
	uint8_t *pos = NULL;
	if ( buf == NULL)
		return;
	pos = wpabuf_put(buf, 1);
	*pos = data;
}

static inline void wpabuf_put_le16(struct wpabuf *buf, uint16_t data)
{
	uint8_t *pos = NULL;
	if ( buf == NULL)
		return;
	pos = wpabuf_put(buf, 2);
	WPA_PUT_LE16(pos, data);
}

static inline void wpabuf_put_le32(struct wpabuf *buf, uint32_t data)
{
	uint8_t *pos = NULL;
	if ( buf == NULL)
		return;
	pos = wpabuf_put(buf, 4);
	WPA_PUT_LE32(pos, data);
}

static inline void wpabuf_put_be16(struct wpabuf *buf, uint16_t data)
{
	uint8_t *pos = NULL;
	if ( buf == NULL)
		return;
	pos = wpabuf_put(buf, 2);
	WPA_PUT_BE16(pos, data);
}

static inline void wpabuf_put_be24(struct wpabuf *buf, uint32_t data)
{
	uint8_t *pos = NULL;
	if ( buf == NULL)
		return;
	pos = wpabuf_put(buf, 3);
	WPA_PUT_BE24(pos, data);
}

static inline void wpabuf_put_be32(struct wpabuf *buf, uint32_t data)
{
	uint8_t *pos = NULL;
	if ( buf == NULL)
		return;
	pos = wpabuf_put(buf, 4);
	WPA_PUT_BE32(pos, data);
}

// 							encr >> 16 B	64 - 16 = 48
static inline void wpabuf_put_data(struct wpabuf *buf, const void *data, size_t len)
{
	if ( buf == NULL)
		return;
	if (data)
		os_memcpy(wpabuf_put(buf, len), data, len);
}

static inline void wpabuf_put_buf(struct wpabuf *dst,
				  const struct wpabuf *src)
{
	if ( dst == NULL)
		return;
	wpabuf_put_data(dst, wpabuf_head(src), wpabuf_len(src));
}

static inline void wpabuf_set(struct wpabuf *buf, const void *data, size_t len)
{
	if ( buf == NULL)
		return;
	buf->buf = (uint8_t *) data;
	buf->flags = WPABUF_FLAG_EXT_DATA;
	buf->size = buf->used = len;
}

static inline void wpabuf_put_str(struct wpabuf *dst, const char *str)
{
	if ( dst == NULL)
		return;
	wpabuf_put_data(dst, str, os_strlen(str));
}

#endif /* WPABUF_H */
