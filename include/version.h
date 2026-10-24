#ifndef VERSION_H
#define VERSION_H

/* Game versions. BUILD_VERSION is the index of the version in configure.py VERSIONS. */
#define VERSION_EUR 0 /* RM7P4F, Europe (En,Fr,De,Es,It) */
#define VERSION_USA 1 /* RM7E4F, USA (En,Fr,Es) */

#define REGION_PAL (BUILD_VERSION == VERSION_EUR)
#define REGION_USA (BUILD_VERSION == VERSION_USA)

#endif
