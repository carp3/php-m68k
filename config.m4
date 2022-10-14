dnl $Id$
dnl config.m4 for extension m68k

sinclude(./autoconf/pecl.m4)
sinclude(./autoconf/php-executable.m4)

PECL_INIT([m68k])

PHP_ARG_ENABLE(m68k, whether to enable m68k, [ --enable-m68k   Enable m68k])

if test "$PHP_m68k" != "no"; then
  AC_DEFINE(HAVE_m68k, 1, [whether m68k is enabled])
  PHP_NEW_EXTENSION(m68k, m68k.c musashi/m68kcpu.c musashi/m68kops.c musashi/softfloat/softfloat.c, $ext_shared,, "-g0 -O2 -Wall -Wextra -pedantic -c -Im68k -I. --std=c99 -fPIC")

  M68M_LDFLAGS = -shared

  PHP_ADD_MAKEFILE_FRAGMENT

  PHP_SUBST(M68M_LDFLAGS)
  PHP_INSTALL_HEADERS([ext/m68k], [php_m68k.h])
fi
