#pragma once
#ifndef MESSMER_FSPP_FUSE_PARAMS_H_
#define MESSMER_FSPP_FUSE_PARAMS_H_

#define FUSE_USE_VERSION 26
#if defined(__linux__) || defined(__FreeBSD__)
#include <fuse.h>
#elif __APPLE__
#include <osxfuse/fuse.h>
#elif defined(_MSC_VER)
#include <fuse.h> // Dokany fuse
// Define stuff that's missing in Dokany
#define	S_ISREG(m)	(((m)&S_IFMT) == S_IFREG)
#define	S_ISDIR(m)	(((m)&S_IFMT) == S_IFDIR)
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100
#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010
#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
#else
#error System not supported
#endif

#endif
