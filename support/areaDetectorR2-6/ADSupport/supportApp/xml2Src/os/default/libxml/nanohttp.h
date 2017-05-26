/*
 * Summary: minimal HTTP implementation
 * Description: minimal HTTP implementation allowing to fetch resources
 *              like external subset.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __NANO_HTTP_H__
#define __NANO_HTTP_H__

#include <libxml/xmlversion.h>

#ifdef LIBXML_HTTP_ENABLED

#ifdef HAVE_ZLIB_H
#include <zlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xmlNanoHTTPCtxt {
    char *protocol;	/* the protocol name */
    char *hostname;	/* the host name */
    int port;		/* the port */
    char *path;		/* the path within the URL */
    char *query;	/* the query string */
    int fd;		/* the file descriptor for the socket */
    int state;		/* WRITE / READ / CLOSED */
    char *out;		/* buffer sent (zero terminated) */
    char *outptr;	/* index within the buffer sent */
    char *in;		/* the receiving buffer */
    char *content;	/* the start of the content */
    char *inptr;	/* the next byte to read from network */
    char *inrptr;	/* the next byte to give back to the client */
    int inlen;		/* len of the input buffer */
    int last;		/* return code for last operation */
    int returnValue;	/* the protocol return value */
    int version;        /* the protocol version */
    int ContentLength;  /* specified content length from HTTP header */
    char *contentType;	/* the MIME type for the input */
    char *location;	/* the new URL in case of redirect */
    char *authHeader;	/* contents of {WWW,Proxy}-Authenticate header */
    char *encoding;	/* encoding extracted from the contentType */
    char *mimeType;	/* Mime-Type extracted from the contentType */
#ifdef HAVE_ZLIB_H
    z_stream *strm;	/* Zlib stream object */
    int usesGzip;	/* "Content-Encoding: gzip" was detected */
#endif
} xmlNanoHTTPCtxt, *xmlNanoHTTPCtxtPtr;

XMLPUBFUN void XMLCALL
	xmlNanoHTTPInit		(void);
XMLPUBFUN void XMLCALL
	xmlNanoHTTPCleanup	(void);
XMLPUBFUN void XMLCALL
	xmlNanoHTTPScanProxy	(const char *URL);
XMLPUBFUN int XMLCALL
	xmlNanoHTTPFetch	(const char *URL,
				 const char *filename,
				 char **contentType);
XMLPUBFUN void * XMLCALL
	xmlNanoHTTPMethod	(const char *URL,
				 const char *method,
				 const char *input,
				 char **contentType,
				 const char *headers,
				 int   ilen);
XMLPUBFUN xmlNanoHTTPCtxtPtr XMLCALL
	xmlNanoHTTPMethodRedir	(const char *URL,
				 const char *method,
				 const char *input,
				 char **contentType,
				 char **redir,
				 const char *headers,
				 int   ilen);
XMLPUBFUN xmlNanoHTTPCtxtPtr XMLCALL
	xmlNanoHTTPOpen		(const char *URL,
				 char **contentType);
XMLPUBFUN void * XMLCALL
	xmlNanoHTTPOpenRedir	(const char *URL,
				 char **contentType,
				 char **redir);
XMLPUBFUN int XMLCALL
	xmlNanoHTTPReturnCode	(void *ctx);
XMLPUBFUN const char * XMLCALL
	xmlNanoHTTPAuthHeader	(void *ctx);
XMLPUBFUN const char * XMLCALL
	xmlNanoHTTPRedir	(void *ctx);
XMLPUBFUN int XMLCALL
	xmlNanoHTTPContentLength( void * ctx );
XMLPUBFUN const char * XMLCALL
	xmlNanoHTTPEncoding	(void *ctx);
XMLPUBFUN const char * XMLCALL
	xmlNanoHTTPMimeType	(void *ctx);
XMLPUBFUN int XMLCALL
	xmlNanoHTTPRead		(void *ctx,
				 void *dest,
				 int len);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN int XMLCALL
	xmlNanoHTTPSave		(void *ctxt,
				 const char *filename);
#endif /* LIBXML_OUTPUT_ENABLED */
XMLPUBFUN void XMLCALL
	xmlNanoHTTPClose	(void *ctx);
#ifdef __cplusplus
}
#endif

#endif /* LIBXML_HTTP_ENABLED */
#endif /* __NANO_HTTP_H__ */
