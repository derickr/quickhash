/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010 StumbleUpon Inc.                                  |
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

#include "hash-algorithms.h"

/**
 * Hashes the key
 *
 * The algorithm is from: http://burtleburtle.net/bob/hash/integer.html
 *
 * Parameters:
 * - key, the key to be hashsed
 *
 * Returns:
 * - the hash key
 */
inline uint32_t qha_jenkins1(uint32_t key)
{
	key = (key ^ 61) ^ (key >> 16);
	key = key + (key << 3);
	key = key ^ (key >> 4);
	key = key * 0x27d4eb2d;
	key = key ^ (key >> 15);
	return key;
}

/**
 * Hashes the key
 *
 * The algorithm is from: http://www.concentric.net/~Ttwang/tech/inthash.htm
 *
 * Parameters:
 * - key, the key to be hashsed
 *
 * Returns:
 * - the hash key
 */
inline uint32_t qha_jenkins2(uint32_t key)
{
	key = (key+0x7ed55d16) + (key<<12);
	key = (key^0xc761c23c) ^ (key>>19);
	key = (key+0x165667b1) + (key<<5);
	key = (key+0xd3a2646c) ^ (key<<9);
	key = (key+0xfd7046c5) + (key<<3);
	key = (key^0xb55a4f09) ^ (key>>16);
	return key;
}

/**
 * 'Hashes' the key by passing it straight through
 *
 * Parameters:
 * - key, the key to be hashsed
 *
 * Returns:
 * - the hash key
 */
inline uint32_t qha_no_hash(uint32_t key)
{
	return key;
}
