/*
 * wpa_supplicant/hostapd / common helper functions, etc.
 * Copyright (c) 2002-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef COMMON_H
#define COMMON_H

#include "utils/os.h"
#define TO_TEST_WPS 0
#define ETH_ALEN 6

#if defined(__linux__) || defined(__GLIBC__)
	#include <endian.h>
	#include <byteswap.h>
#endif /* __linux__ */

#if defined(PLATFORM_FREERTOS) 
	//#include "little_endian.h"
	//#include "basic_types.h"
#endif /* PLATFORM_FREERTOS */


#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__) || \
    defined(__OpenBSD__)
	#include <sys/types.h>
	#include <sys/endian.h>
	#define __BYTE_ORDER	_BYTE_ORDER
	#define	__LITTLE_ENDIAN	_LITTLE_ENDIAN
	#define	__BIG_ENDIAN	_BIG_ENDIAN

	#ifdef __OpenBSD__
		#define bswap_16 swap16
		#define bswap_32 swap32
		#define bswap_64 swap64
	#else /* __OpenBSD__ */
		#define bswap_16 bswap16
		#define bswap_32 bswap32
		#define bswap_64 bswap64
	#endif /* __OpenBSD__ */
#endif /* defined(__FreeBSD__) || defined(__NetBSD__) || * defined(__DragonFly__) || defined(__OpenBSD__) */

#ifdef __APPLE__
	#include <sys/types.h>
	#include <machine/endian.h>
	#define __BYTE_ORDER	_BYTE_ORDER
	#define __LITTLE_ENDIAN	_LITTLE_ENDIAN
	#define __BIG_ENDIAN	_BIG_ENDIAN
static inline unsigned short bswap_16(unsigned short v)
{
	return ((v & 0xff) << 8) | (v >> 8);
}

static inline unsigned int bswap_32(unsigned int v)
{
	return ((v & 0xff) << 24) | ((v & 0xff00) << 8) |
		((v & 0xff0000) >> 8) | (v >> 24);
}
#endif /* __APPLE__ */

#ifdef CONFIG_TI_COMPILER
	#define __BIG_ENDIAN 4321
	#define __LITTLE_ENDIAN 1234
	#ifdef __big_endian__
		#define __BYTE_ORDER __BIG_ENDIAN
	#else
		#define __BYTE_ORDER __LITTLE_ENDIAN
	#endif
#endif /* CONFIG_TI_COMPILER */

#ifdef CONFIG_NATIVE_WINDOWS
	#include <winsock.h>
	typedef int socklen_t;
	#ifndef MSG_DONTWAIT
		#define MSG_DONTWAIT 0 /* not supported */
	#endif
#endif /* CONFIG_NATIVE_WINDOWS */

#ifdef _MSC_VER
#define inline __inline

#undef vsnprintf
#define vsnprintf _vsnprintf
#undef close
#define close closesocket
#endif /* _MSC_VER */


/* Define platform specific integer types */

#ifdef _MSC_VER
typedef UINT64 uint64_t;
typedef UINT32 uint32_t;
typedef UINT16 uint16_t;
typedef UINT8 uint8_t;
typedef INT64 int64_t;
typedef INT32 int32_t;
typedef INT16 int16_t;
typedef INT8 int8_t;
#define WPA_TYPES_DEFINED
#endif /* _MSC_VER */

#ifdef __vxworks
typedef unsigned long long uint64_t;
typedef UINT32 uint32_t;
typedef UINT16 uint16_t;
typedef UINT8 uint8_t;
typedef long long int64_t;
typedef INT32 int32_t;
typedef INT16 int16_t;
typedef INT8 int8_t;
#define WPA_TYPES_DEFINED
#endif /* __vxworks */

#ifdef CONFIG_TI_COMPILER
#ifdef _LLONG_AVAILABLE
typedef unsigned long long uint64_t;
#else
/*
 * TODO: 64-bit variable not available. Using long as a workaround to test the
 * build, but this will likely not work for all operations.
 */
typedef unsigned long uint64_t;
#endif
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#define WPA_TYPES_DEFINED
#endif /* CONFIG_TI_COMPILER */

#ifndef WPA_TYPES_DEFINED
#ifdef CONFIG_USE_INTTYPES_H
#include <inttypes.h>
#else
//#include <stdint.h>
#endif
#if 0
typedef uint64_t uint64_t;
typedef uint32_t uint32_t;
typedef uint16_t uint16_t;
typedef uint8_t uint8_t;
typedef int64_t int64_t;
typedef int32_t int32_t;
typedef int16_t int16_t;
typedef int8_t int8_t;
#endif
#define WPA_TYPES_DEFINED
#endif /* !WPA_TYPES_DEFINED */


/* Define platform specific byte swapping macros */

#if defined(__CYGWIN__) || defined(CONFIG_NATIVE_WINDOWS)

static inline unsigned short wpa_swap_16(unsigned short v)
{
	return ((v & 0xff) << 8) | (v >> 8);
}

static inline unsigned int wpa_swap_32(unsigned int v)
{
	return ((v & 0xff) << 24) | ((v & 0xff00) << 8) |
		((v & 0xff0000) >> 8) | (v >> 24);
}

#define le_to_host16(n) (n)
#define host_to_le16(n) (n)
#define be_to_host16(n) wpa_swap_16(n)
#define host_to_be16(n) wpa_swap_16(n)
#define le_to_host32(n) (n)
#define be_to_host32(n) wpa_swap_32(n)
#define host_to_be32(n) wpa_swap_32(n)

#define WPA_BYTE_SWAP_DEFINED

#endif /* __CYGWIN__ || CONFIG_NATIVE_WINDOWS */

#ifndef WPA_BYTE_SWAP_DEFINED
#if 0
	#ifndef __BYTE_ORDER
		#ifndef __LITTLE_ENDIAN
			#ifndef __BIG_ENDIAN
				#define __LITTLE_ENDIAN 1234
				#define __BIG_ENDIAN 4321

				#if defined(sparc)
					#define __BYTE_ORDER __BIG_ENDIAN
				#endif
			#endif /* __BIG_ENDIAN */
		#endif /* __LITTLE_ENDIAN */
	#endif /* __BYTE_ORDER */
#else
	#ifndef __LITTLE_ENDIAN
		#define __LITTLE_ENDIAN 1234
	#endif
	#ifndef __BIG_ENDIAN
		#define __BIG_ENDIAN 4321
	#endif
	#ifndef __BYTE_ORDER
		#define __BYTE_ORDER __LITTLE_ENDIAN
	#endif
#endif

	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define le_to_host16(n) ((__force uint16_t) (le16) (n))
		#define host_to_le16(n) ((__force le16) (uint16_t) (n))
		#define be_to_host16(n) bswap_16((__force uint16_t) (be16) (n))
		#define host_to_be16(n) ((__force be16) bswap_16((n)))
		#define le_to_host32(n) ((__force uint32_t) (le32) (n))
		#define host_to_le32(n) ((__force le32) (uint32_t) (n))
		#define be_to_host32(n) bswap_32((__force uint32_t) (be32) (n))
		#define host_to_be32(n) ((__force be32) bswap_32((n)))
		#define le_to_host64(n) ((__force uint64_t) (le64) (n))
		#define host_to_le64(n) ((__force le64) (uint64_t) (n))
		#define be_to_host64(n) bswap_64((__force uint64_t) (be64) (n))
		#define host_to_be64(n) ((__force be64) bswap_64((n)))
	#elif __BYTE_ORDER == __BIG_ENDIAN
		#define le_to_host16(n) bswap_16(n)
		#define host_to_le16(n) bswap_16(n)
		#define be_to_host16(n) (n)
		#define host_to_be16(n) (n)
		#define le_to_host32(n) bswap_32(n)
		#define be_to_host32(n) (n)
		#define host_to_be32(n) (n)
		#define le_to_host64(n) bswap_64(n)
		#define host_to_le64(n) bswap_64(n)
		#define be_to_host64(n) (n)
		#define host_to_be64(n) (n)	
	
		#ifndef WORDS_BIGENDIAN
			#define WORDS_BIGENDIAN
		#endif
#else
	//#error Could not determine CPU byte order
#endif

	#define WPA_BYTE_SWAP_DEFINED
#endif /* !WPA_BYTE_SWAP_DEFINED */


/* Macros for handling unaligned memory accesses */

#define WPA_GET_BE16(a) ((uint16_t) (((a)[0] << 8) | (a)[1]))
#define WPA_PUT_BE16(a, val)			\
	do {					\
		(a)[0] = ((uint16_t) (val)) >> 8;	\
		(a)[1] = ((uint16_t) (val)) & 0xff;	\
	} while (0)

#define WPA_GET_LE16(a) ((uint16_t) (((a)[1] << 8) | (a)[0]))
#define WPA_PUT_LE16(a, val)			\
	do {					\
		(a)[1] = ((uint16_t) (val)) >> 8;	\
		(a)[0] = ((uint16_t) (val)) & 0xff;	\
	} while (0)

#define WPA_GET_BE24(a) ((((uint32_t) (a)[0]) << 16) | (((uint32_t) (a)[1]) << 8) | \
			 ((uint32_t) (a)[2]))
#define WPA_PUT_BE24(a, val)					\
	do {							\
		(a)[0] = (uint8_t) ((((uint32_t) (val)) >> 16) & 0xff);	\
		(a)[1] = (uint8_t) ((((uint32_t) (val)) >> 8) & 0xff);	\
		(a)[2] = (uint8_t) (((uint32_t) (val)) & 0xff);		\
	} while (0)

#define WPA_GET_BE32(a) ((((uint32_t) (a)[0]) << 24) | (((uint32_t) (a)[1]) << 16) | \
			 (((uint32_t) (a)[2]) << 8) | ((uint32_t) (a)[3]))
#define WPA_PUT_BE32(a, val)					\
	do {							\
		(a)[0] = (uint8_t) ((((uint32_t) (val)) >> 24) & 0xff);	\
		(a)[1] = (uint8_t) ((((uint32_t) (val)) >> 16) & 0xff);	\
		(a)[2] = (uint8_t) ((((uint32_t) (val)) >> 8) & 0xff);	\
		(a)[3] = (uint8_t) (((uint32_t) (val)) & 0xff);		\
	} while (0)

#define WPA_GET_LE32(a) ((((uint32_t) (a)[3]) << 24) | (((uint32_t) (a)[2]) << 16) | \
			 (((uint32_t) (a)[1]) << 8) | ((uint32_t) (a)[0]))
#define WPA_PUT_LE32(a, val)					\
	do {							\
		(a)[3] = (uint8_t) ((((uint32_t) (val)) >> 24) & 0xff);	\
		(a)[2] = (uint8_t) ((((uint32_t) (val)) >> 16) & 0xff);	\
		(a)[1] = (uint8_t) ((((uint32_t) (val)) >> 8) & 0xff);	\
		(a)[0] = (uint8_t) (((uint32_t) (val)) & 0xff);		\
	} while (0)

#define WPA_GET_BE64(a) ((((uint64_t) (a)[0]) << 56) | (((uint64_t) (a)[1]) << 48) | \
			 (((uint64_t) (a)[2]) << 40) | (((uint64_t) (a)[3]) << 32) | \
			 (((uint64_t) (a)[4]) << 24) | (((uint64_t) (a)[5]) << 16) | \
			 (((uint64_t) (a)[6]) << 8) | ((uint64_t) (a)[7]))
#define WPA_PUT_BE64(a, val)				\
	do {						\
		(a)[0] = (uint8_t) (((uint64_t) (val)) >> 56);	\
		(a)[1] = (uint8_t) (((uint64_t) (val)) >> 48);	\
		(a)[2] = (uint8_t) (((uint64_t) (val)) >> 40);	\
		(a)[3] = (uint8_t) (((uint64_t) (val)) >> 32);	\
		(a)[4] = (uint8_t) (((uint64_t) (val)) >> 24);	\
		(a)[5] = (uint8_t) (((uint64_t) (val)) >> 16);	\
		(a)[6] = (uint8_t) (((uint64_t) (val)) >> 8);	\
		(a)[7] = (uint8_t) (((uint64_t) (val)) & 0xff);	\
	} while (0)

#define WPA_GET_LE64(a) ((((uint64_t) (a)[7]) << 56) | (((uint64_t) (a)[6]) << 48) | \
			 (((uint64_t) (a)[5]) << 40) | (((uint64_t) (a)[4]) << 32) | \
			 (((uint64_t) (a)[3]) << 24) | (((uint64_t) (a)[2]) << 16) | \
			 (((uint64_t) (a)[1]) << 8) | ((uint64_t) (a)[0]))


#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif
#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif
#ifndef ETH_P_ALL
#define ETH_P_ALL 0x0003
#endif
#ifndef ETH_P_80211_ENCAP
#define ETH_P_80211_ENCAP 0x890d /* TDLS comes under this category */
#endif
#ifndef ETH_P_PAE
#define ETH_P_PAE 0x888E /* Port Access Entity (IEEE 802.1X) */
#endif /* ETH_P_PAE */
#ifndef ETH_P_EAPOL
#define ETH_P_EAPOL ETH_P_PAE
#endif /* ETH_P_EAPOL */
#ifndef ETH_P_RSN_PREAUTH
#define ETH_P_RSN_PREAUTH 0x88c7
#endif /* ETH_P_RSN_PREAUTH */
#ifndef ETH_P_RRB
#define ETH_P_RRB 0x890D
#endif /* ETH_P_RRB */


#if 0	//#ifdef __GNUC__
#define PRINTF_FORMAT(a,b) __attribute__ ((format (printf, (a), (b))))
#define STRUCT_PACKED __attribute__ ((packed))
#else
#define PRINTF_FORMAT(a,b)
#define STRUCT_PACKED
#endif


#ifdef CONFIG_ANSI_C_EXTRA

#if !defined(_MSC_VER) || _MSC_VER < 1400
/* snprintf - used in number of places; sprintf() is _not_ a good replacement
 * due to possible buffer overflow; see, e.g.,
 * http://www.ijs.si/software/snprintf/ for portable implementation of
 * snprintf. */
int snprintf(char *str, size_t size, const char *format, ...);

/* vsnprintf - only used for wpa_msg() in wpa_supplicant.c */
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
#endif /* !defined(_MSC_VER) || _MSC_VER < 1400 */

/* getopt - only used in main.c */
int getopt(int argc, char *const argv[], const char *optstring);
extern char *optarg;
extern int optind;

#ifndef CONFIG_NO_SOCKLEN_T_TYPEDEF
#ifndef __socklen_t_defined
typedef int socklen_t;
#endif
#endif

/* inline - define as __inline or just define it to be empty, if needed */
#ifdef CONFIG_NO_INLINE
#define inline
#else
#define inline __inline
#endif

#ifndef __func__
#define __func__ "__func__ not defined"
#endif

#ifndef bswap_16
#define bswap_16(a) ((((uint16_t) (a) << 8) & 0xff00) | (((uint16_t) (a) >> 8) & 0xff))
#endif

#ifndef bswap_32
#define bswap_32(a) ((((uint32_t) (a) << 24) & 0xff000000) | \
		     (((uint32_t) (a) << 8) & 0xff0000) | \
     		     (((uint32_t) (a) >> 8) & 0xff00) | \
     		     (((uint32_t) (a) >> 24) & 0xff))
#endif

#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

#ifdef _WIN32_WCE
void perror(const char *s);
#endif /* _WIN32_WCE */

#endif /* CONFIG_ANSI_C_EXTRA */

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

/*
 * Compact form for string representation of MAC address
 * To be used, e.g., for constructing dbus paths for P2P Devices
 */
#define COMPACT_MACSTR "%02x%02x%02x%02x%02x%02x"
#endif

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

/*
 * Definitions for sparse validation
 * (http://kernel.org/pub/linux/kernel/people/josh/sparse/)
 */
#ifdef __CHECKER__
#define __force __attribute__((force))
#define __bitwise __attribute__((bitwise))
#else
#define __force
#define __bitwise
#endif

typedef uint16_t __bitwise be16;
typedef uint16_t __bitwise le16;
typedef uint32_t __bitwise be32;
typedef uint32_t __bitwise le32;
typedef uint64_t __bitwise be64;
typedef uint64_t __bitwise le64;

#ifndef __must_check
#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#define __must_check __attribute__((__warn_unused_result__))
#else
#define __must_check
#endif /* __GNUC__ */
#endif /* __must_check */

//int hwaddr_aton(const char *txt, uint8_t *addr);
int hwaddr_compact_aton(const char *txt, uint8_t *addr);
int hwaddr_aton2(const char *txt, uint8_t *addr);
int hex2byte(const char *hex);
int hexstr2bin(const char *hex, uint8_t *buf, size_t len);
void inc_byte_array(uint8_t *counter, size_t len);
void wpa_get_ntp_timestamp(uint8_t *buf);
//int wpa_snprintf_hex(char *buf, size_t buf_size, const uint8_t *data, size_t len);
int wpa_snprintf_hex_uppercase(char *buf, size_t buf_size, const uint8_t *data,
			       size_t len);

#ifdef CONFIG_NATIVE_WINDOWS
void wpa_unicode2ascii_inplace(TCHAR *str);
TCHAR * wpa_strdup_tchar(const char *str);
#else /* CONFIG_NATIVE_WINDOWS */
#define wpa_unicode2ascii_inplace(s) do { } while (0)
#define wpa_strdup_tchar(s) strdup((s))
#endif /* CONFIG_NATIVE_WINDOWS */

void printf_encode(char *txt, size_t maxlen, const uint8_t *data, size_t len);
size_t printf_decode(uint8_t *buf, size_t maxlen, const char *str);

const char * wpa_ssid_txt(const uint8_t *ssid, size_t ssid_len);

char * wpa_config_parse_string(const char *value, size_t *len);
int is_hex(const uint8_t *data, size_t len);
size_t merge_byte_arrays(uint8_t *res, size_t res_len,
			 const uint8_t *src1, size_t src1_len,
			 const uint8_t *src2, size_t src2_len);

static inline int is_zero_ether_addr(const uint8_t *a)
{
	return !(a[0] | a[1] | a[2] | a[3] | a[4] | a[5]);
}

static inline int is_broadcast_ether_addr(const uint8_t *a)
{
	return (a[0] & a[1] & a[2] & a[3] & a[4] & a[5]) == 0xff;
}

#define broadcast_ether_addr (const uint8_t *) "\xff\xff\xff\xff\xff\xff"

#include "wpa_debug.h"


char * dup_binstr(const void *src, size_t len);
struct wpa_freq_range_list {
	struct wpa_freq_range {
		unsigned int min;
		unsigned int max;
	} *range;
	unsigned int num;
};

int freq_range_list_parse(struct wpa_freq_range_list *res, const char *value);
int freq_range_list_includes(const struct wpa_freq_range_list *list,
			     unsigned int freq);
char * freq_range_list_str(const struct wpa_freq_range_list *list);

int int_array_len(const int *a);
void int_array_concat(int **res, const int *a);
void int_array_sort_unique(int *a);
void int_array_add_unique(int **res, int a);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

void str_clear_free(char *str);
void bin_clear_free(void *bin, size_t len);

int random_mac_addr(uint8_t *addr);
int random_mac_addr_keep_oui(uint8_t *addr);

const char * cstr_token(const char *str, const char *delim, const char **last);
char * str_token(char *str, const char *delim, char **context);
size_t utf8_escape(const char *inp, size_t in_size,
		   char *outp, size_t out_size);
size_t utf8_unescape(const char *inp, size_t in_size,
		     char *outp, size_t out_size);
int is_ctrl_char(char c);

#ifndef bswap_16
#define bswap_16(a) ((((uint16_t) (a) << 8) & 0xff00) | (((uint16_t) (a) >> 8) & 0xff))
#endif


/*
 * gcc 4.4 ends up generating strict-aliasing warnings about some very common
 * networking socket uses that do not really result in a real problem and
 * cannot be easily avoided with union-based type-punning due to struct
 * definitions including another struct in system header files. To avoid having
 * to fully disable strict-aliasing warnings, provide a mechanism to hide the
 * typecast from aliasing for now. A cleaner solution will hopefully be found
 * in the future to handle these cases.
 */
void * __hide_aliasing_typecast(void *foo);
#define aliasing_hide_typecast(a,t) (t *) __hide_aliasing_typecast((a))

#ifdef CONFIG_VALGRIND
#include <valgrind/memcheck.h>
#define WPA_MEM_DEFINED(ptr, len) VALGRIND_MAKE_MEM_DEFINED((ptr), (len))
#else /* CONFIG_VALGRIND */
#define WPA_MEM_DEFINED(ptr, len) do { } while (0)
#endif /* CONFIG_VALGRIND */

#endif /* COMMON_H */
