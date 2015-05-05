/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2011 StumbleUpon Inc.                             |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Derick Rethans <derick@derickrethans.nl>                    |
   +----------------------------------------------------------------------+
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif
#if HAVE_STDINT_H
# include <stdint.h>
#endif
#if HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifndef QH_HASH_ALGORITHMS_H
#define QH_HASH_ALGORITHMS_H

uint32_t qha_jenkins1(uint32_t key);
uint32_t qha_jenkins2(uint32_t key);
uint32_t qha_no_hash(uint32_t key);

uint32_t qha_djb2(char *key);
uint32_t qha_sdbm(char *key);

#endif
