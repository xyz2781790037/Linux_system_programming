#include <stdarg.h>
#include "error_functions.h"
#include "tlpi_hdr.h"
#include "ename.c.inc"

/* Defines ename and MAX_ ENAME */

#ifdef __GNUC__

__attribute__((__noreturn__))

#endif

static void

terminate(Boolean useExit3)
{
    char *s;

    /* Dump core if EF_ DUMPCORE environment variable is defined andis a nonempty string; otherwise call exit(3) or_ exit(2),

    depending on the value of 'useExit3'. */

    s = getenv("EF_ DUMPCORE");

    if (s != NULL && *s != '\0')

        abort();

    else if (useExit3)

        exit(EXIT_FAILURE);

    else

        _exit(EXIT_FAILURE);
}
static void outputError(Boolean useErr, int err, Boolean flushStdout, const char *format, va_list ap)

#define BUF_SIZE 500

    char buf[BUF_ _SIZE],
    userMsg[BUF_ SIZE], errText[BUF_ SIZE];

vsnprintf(userMsg, BUF_ SIZE, format, ap);
if (useErr)

(err > 0 && err <= MAX_ ENAME)

ename[err] : "?UNKNOWN?", strerror(err));

else

    snprintf(errText, BUF_ SIZE, ":");

snprintf(buf, BUF_ SIZE, "ERROR%s %s\n", errText, userMsg);

if (flushStdout)

    /* Flush any pending stdout */

    fputs(buf, stderr);

第3章系统编程概念

异步社区会员flyman150(2410757683 @qq.com) 专享尊重版权

    fflush(stderr);

/* In case stderr is not line-buffered */

void

errMsg(const char *format, ...)

    va_ list argList;
int savedErrno;

savedErrno = errno;
.

    /* In case we change it here */

    va_ start(argList, format);

outputError(TRUE,

            no, TRUE, format, argList);

va_ _end(argList);
