
/*
 * Copyright (c) 2019
 * Copyright (C) 2023 raf <raf@raf.org>, Tavian Barnes <tavianator@tavianator.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _MACPORTS_DIRENT_H_
#define _MACPORTS_DIRENT_H_

/* MP support header */
#include "MacportsLegacySupport.h"

/* Include the primary system dirent.h */
#include_next <dirent.h>

/* fdopendir */
#if __MP_LEGACY_SUPPORT_FDOPENDIR__

/* Wrapper struct for DIR */
typedef struct __MPLS_DIR __MPLS_DIR;
struct __MPLS_DIR {
    DIR *__mpls_dir;
    int __mpls_dirfd;
};
#define DIR __MPLS_DIR

__MP__BEGIN_DECLS

#ifndef __DARWIN_ALIAS_I
extern DIR *fdopendir(int fd) __DARWIN_ALIAS(fdopendir);
#else
extern DIR *fdopendir(int fd) __DARWIN_ALIAS_I(fdopendir);
#endif

/* Wrapper functions/macros to support fdopendir */
extern DIR *__mpls_opendir(const char *name);
extern int __mpls_closedir(DIR *dir);
extern struct dirent *__mpls_readdir(DIR *dir);
extern int __mpls_readdir_r(DIR *dir, struct dirent *entry, struct dirent **result);
extern void __mpls_rewinddir(DIR *dir);
extern void __mpls_seekdir(DIR *dir, long loc);
extern long __mpls_telldir(DIR *dir);
extern int __mpls_dirfd(DIR *dir);

#define opendir __mpls_opendir
/*
** Add params to closedir to avoid apparently triggering a mysterious
** problem encountered when testing capnproto. But only do this in C++
** for capnproto's benefit. It is unknown why this makes any difference.
** It really shouldn't. It makes no sense. It's probably unrelated.
** WARNING: This means that it's not possible to take the address of
** closedir in C++ (which has also been seen in the wild (dyld)).
** But it is possible in C code. This is pragmatic(?), but not ideal.
*/
#if defined(__cplusplus)
#define closedir(dir) __mpls_closedir(dir)
#else
#define closedir __mpls_closedir
#endif
#define readdir __mpls_readdir
#define readdir_r __mpls_readdir_r
#define rewinddir __mpls_rewinddir
/* Add params to seekdir in C++ to not clash with type std::ios_base::seekdir */
#if defined(__cplusplus)
#define seekdir(dir, loc) __mpls_seekdir((dir), (loc))
#else
#define seekdir __mpls_seekdir
#endif
#define telldir __mpls_telldir

#ifndef __MP_LEGACY_SUPPORT_NO_DIRFD_MACRO
#undef dirfd
#define dirfd(dir) __mpls_dirfd(dir)
#endif

__MP__END_DECLS

#endif /* __MP_LEGACY_SUPPORT_FDOPENDIR__ */

#endif /* _MACPORTS_DIRENT_H_ */
