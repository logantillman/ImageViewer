/* libtiff/tiffconf.h.  Generated from tiffconf.h.in by configure.  */
/*
  Configuration defines for installed libtiff.
  This file maintained for backward compatibility. Do not use definitions
  from this file in your programs.
*/

#ifndef _TIFFCONF_
#define _TIFFCONF_

#if defined (TIFF_PREFIX)
#include "tiffprefix.h"
#endif

#include <qglobal.h>

#if !defined (Q_OS_HAIKU)

/* Signed 16-bit type */
#define TIFF_INT16_T qint16

/* Signed 32-bit type */
#define TIFF_INT32_T qint32

/* Signed 64-bit type */
#define TIFF_INT64_T qint64

/* Signed 8-bit type */
#define TIFF_INT8_T qint8

/* Unsigned 16-bit type */
#define TIFF_UINT16_T quint16

/* Unsigned 32-bit type */
#define TIFF_UINT32_T quint32

/* Unsigned 64-bit type */
#define TIFF_UINT64_T quint64

/* Unsigned 8-bit type */
#define TIFF_UINT8_T quint8

#else

#include <SupportDefs.h>

#define TIFF_INT8_T     __haiku_int8
#define TIFF_UINT8_T    __haiku_uint8
#define TIFF_INT16_T    __haiku_int16
#define TIFF_UINT16_T   __haiku_uint16
#define TIFF_INT32_T    __haiku_int32
#define TIFF_UINT32_T   __haiku_uint32
#define TIFF_INT64_T    __haiku_int64
#define TIFF_UINT64_T   __haiku_uint64

#endif

/* Unsigned size type */
#if defined(QT_POINTER_SIZE) && (QT_POINTER_SIZE == 4)
#define TIFF_SIZE_T quint32
#else
#define TIFF_SIZE_T quint64
#endif

/* Signed size type */
#if defined(QT_POINTER_SIZE) && (QT_POINTER_SIZE == 4)
#define TIFF_SSIZE_T qint32
#else
#define TIFF_SSIZE_T qint64
#endif

/* Pointer difference type */
#define TIFF_PTRDIFF_T ptrdiff_t

/* Compatibility stuff. */

/* Define as 0 or 1 according to the floating point format suported by the
   machine */
#define HAVE_IEEEFP 1

/* Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB) */
#define HOST_FILLORDER FILLORDER_LSB2MSB

/* Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
   (Intel) */
#if defined(Q_BYTE_ORDER) && (Q_BYTE_ORDER == Q_BIG_ENDIAN)
#define HOST_BIGENDIAN 1
#else
#define HOST_BIGENDIAN 0
#endif

/* Support CCITT Group 3 & 4 algorithms */
#define CCITT_SUPPORT 1

/* Support JPEG compression (requires IJG JPEG library) */
#if defined(HAS_LIBJPEG)
#define JPEG_SUPPORT 1
#endif

/* Support JBIG compression (requires JBIG-KIT library) */
#if defined(HAS_JBIGKIT)
#define JBIG_SUPPORT 1
#endif

/* Support LogLuv high dynamic range encoding */
#define LOGLUV_SUPPORT 1

/* Support LZW algorithm */
#define LZW_SUPPORT 1

/* Support NeXT 2-bit RLE algorithm */
#define NEXT_SUPPORT 1

/* Support Old JPEG compresson (read contrib/ojpeg/README first! Compilation
   fails with unpatched IJG JPEG library) */
#if defined(HAS_LIBJPEG)
#define OJPEG_SUPPORT 1
#endif

/* Support Macintosh PackBits algorithm */
#define PACKBITS_SUPPORT 1

/* Support Pixar log-format algorithm (requires Zlib) */
#if defined(HAS_ZLIB)
#define PIXARLOG_SUPPORT 1
#endif

/* Support ThunderScan 4-bit RLE algorithm */
#define THUNDER_SUPPORT 1

/* Support Deflate compression */
#if defined(HAS_ZLIB)
#define ZIP_SUPPORT 1
#endif

/* Support libdeflate enhanced compression */
/* #undef LIBDEFLATE_SUPPORT */

/* Support strip chopping (whether or not to convert single-strip uncompressed
   images to mutiple strips of ~8Kb to reduce memory usage) */
#define STRIPCHOP_DEFAULT TIFF_STRIPCHOP

/* Enable SubIFD tag (330) support */
#define SUBIFD_SUPPORT 1

/* Treat extra sample as alpha (default enabled). The RGBA interface will
   treat a fourth sample with no EXTRASAMPLE_ value as being ASSOCALPHA. Many
   packages produce RGBA files but don't mark the alpha properly. */
#define DEFAULT_EXTRASAMPLE_AS_ALPHA 1

/* Pick up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */
#define CHECK_JPEG_YCBCR_SUBSAMPLING 1

/* Support MS MDI magic number files as TIFF */
#define MDI_SUPPORT 1

/*
 * Feature support definitions.
 * XXX: These macros are obsoleted. Don't use them in your apps!
 * Macros stays here for backward compatibility and should be always defined.
 */
#define COLORIMETRY_SUPPORT
#define YCBCR_SUPPORT
#define CMYK_SUPPORT
#define ICC_SUPPORT
#define PHOTOSHOP_SUPPORT
#define IPTC_SUPPORT

#endif /* _TIFFCONF_ */
