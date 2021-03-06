/*
 * dynbuf.h
 *
 * Copyright (C) 2009 Aerospike, Inc.
 *
 * Portions may be licensed to Aerospike, Inc. under one or more contributor
 * license agreements.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 */

/*
 * A simple dynamic buffer implementation
 * Allows the first, simpler part of the buffer to be on the stack
 * which is usually all that's needed
 *
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct cf_dyn_buf_s {
	uint8_t		*buf;
	bool		is_stack;
	size_t		alloc_sz;
	size_t		used_sz;
} cf_dyn_buf;

#define cf_dyn_buf_define(__x)  uint8_t dyn_buf##__x[1024]; cf_dyn_buf __x = { dyn_buf##__x, true, 1024, 0 }
#define cf_dyn_buf_define_size(__x, __sz)  uint8_t dyn_buf##__x[__sz]; cf_dyn_buf __x = { dyn_buf##__x, true, __sz, 0 }

extern int cf_dyn_buf_append_string(cf_dyn_buf *db, const char *s);
extern int cf_dyn_buf_append_char(cf_dyn_buf *db, char c);
extern int cf_dyn_buf_append_buf(cf_dyn_buf *db, uint8_t *buf, size_t sz);
extern int cf_dyn_buf_append_int(cf_dyn_buf *db, int i);
extern int cf_dyn_buf_append_uint64_x(cf_dyn_buf *db, uint64_t i); // HEX FORMAT!
extern int cf_dyn_buf_append_uint64(cf_dyn_buf *db, uint64_t i);
extern int cf_dyn_buf_append_uint32(cf_dyn_buf *db, uint32_t i);
extern int cf_dyn_buf_chomp(cf_dyn_buf *db);
extern char *cf_dyn_buf_strdup(cf_dyn_buf *db);
extern void cf_dyn_buf_free(cf_dyn_buf *db);

typedef struct cf_buf_builder_s {
	size_t	alloc_sz;
	size_t	used_sz;
	uint8_t buf[];
} cf_buf_builder;

extern cf_buf_builder *cf_buf_builder_create();
extern cf_buf_builder *cf_buf_builder_create_size(size_t sz);
extern void cf_buf_builder_free(cf_buf_builder *bb);
extern void cf_buf_builder_reset(cf_buf_builder *bb);
extern int cf_buf_builder_chomp(cf_buf_builder *bb_r);
// If you use any binary components, this strdup thing is a bad idea:
extern char *cf_buf_builder_strdup(cf_buf_builder *bb_r);

extern int cf_buf_builder_append_string(cf_buf_builder **bb_r, const char *s);
extern int cf_buf_builder_append_char(cf_buf_builder **bb_r, char c);
extern int cf_buf_builder_append_buf(cf_buf_builder **bb_r, uint8_t *buf, size_t sz);
// These append ASCII versions:
extern int cf_buf_builder_append_ascii_uint64_x(cf_buf_builder **bb_r, uint64_t i); // HEX FORMAT!
extern int cf_buf_builder_append_ascii_uint64(cf_buf_builder **bb_r, uint64_t i);
extern int cf_buf_builder_append_ascii_uint32(cf_buf_builder **bb_r, uint32_t i);
extern int cf_buf_builder_append_ascii_int(cf_buf_builder **bb_r, int i);
// These append network-order bytes:
extern int cf_buf_builder_append_uint64(cf_buf_builder **bb_r, uint64_t i);
extern int cf_buf_builder_append_uint32(cf_buf_builder **bb_r, uint32_t i);
extern int cf_buf_builder_append_uint16(cf_buf_builder **bb_r, uint16_t i);
extern int cf_buf_builder_append_uint8(cf_buf_builder **bb_r, uint8_t i);
// Reserve the bytes and give me the handle to the spot reserved:
extern int cf_buf_builder_reserve(cf_buf_builder **bb_r, int sz, uint8_t **buf);
extern int cf_buf_builder_size(cf_buf_builder *bb);
extern size_t cf_dyn_buf_get_newsize(int alloc, int used, int requested);
