#ifndef ERROR_ FUNCTIONS H#define ERROR_ FUNCTIONS_ H

void errMsg(const char *format, ...);
#ifdef _GNUC_

/* This macro stops 'gcc -Wall' complaining that " control reaches

end of non-void function" if we use the following functions toterminate main() or some other non-void function. */

#define NORETURN_ _attribute_ ((_ noreturn_ _))
#else
#define NORETURN
#endif

void errExit(const char *format, ...) NORETURN ;

void err_exit(const char *format, ...) NORETURN ;

void errExitEN(int errnum, const char *format, ...) NORETURN ;void fatal(const char *format, ...) NORETURN ;

void usageErr(const char *format, ...) NORETURN ;

void cmdLineErr(const char *format, ...) NORETURN ;

#endif