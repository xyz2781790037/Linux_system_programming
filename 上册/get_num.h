#ifndef GET_ _NUM. H

#define GET_ NUM_ H

#define GN_ NONNEG 01

/* Value must be >= 0 */

#define GN_ GT_0 02

/*Valuemustbe>0*/

/* By default, integers are decimal */

#define GN ANY_ BASE 0100

/* Can use any base - like strtol(3) */

#define GN_ BASE_8 0200

/* Value is expressed in octal */

#define GN_ BASE_16 0400

/* Value is expressed in hexadecimal */

long getLong(const char *arg, int flags, const char *name);

int getInt(const char *arg, int flags, const char *name);

#endif