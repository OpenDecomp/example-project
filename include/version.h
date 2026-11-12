#ifndef VERSION_H
#define VERSION_H

/* configure.py passes -DVERSION=<n> for the version being built. */
#define VERSION_US 0
#define VERSION_EU 1

#ifndef VERSION
#error "VERSION is not defined: build through configure.py and ninja"
#endif

#if VERSION == VERSION_EU
#define FRAME_RATE 50
#else
#define FRAME_RATE 60
#endif

#endif
