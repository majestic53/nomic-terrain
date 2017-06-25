/**
 * Nomic
 * Copyright (C) 2017 David Jolly
 *
 * Nomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NOMIC_DEFINE_H_
#define NOMIC_DEFINE_H_

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>

namespace nomic {

#ifndef __in
	#define __in
#endif // __in
#ifndef __in_opt
	#define __in_opt
#endif // __in_opt
#ifndef __inout
	#define __inout
#endif // __inout
#ifndef __inout_opt
	#define __inout_opt
#endif // __inout_opt

	#define BLOCK_ATTRIBUTES_DEFAULT (BLOCK_ATTRIBUTE_STATIC | BLOCK_ATTRIBUTE_BREAKABLE)

	#define CHUNK_BLOCK_COUNT (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH)
	#define CHUNK_BLOCK_SURFACE_COUNT (CHUNK_WIDTH * CHUNK_WIDTH)
	#define CHUNK_MAX_DEFAULT 64
	#define CHUNK_HEIGHT 128
	#define CHUNK_HEIGHT_MAX 120
	#define CHUNK_HEIGHT_MIN 20
	#define CHUNK_WIDTH 16

	#define EXCEPTION_UNKNOWN "Unknown exception"
	#define EXCEPTION_MALFORMED "Malformed exception"

	#define NOISE_AMPLITUDE_DEFAULT 1.0
	#define NOISE_AMPLITUDE_MIN 0.0
	#define NOISE_OCTAVES_DEFAULT 7
	#define NOISE_PERSISTENCE_DEFAULT 1.0
	#define NOISE_PERSISTENCE_MIN 0.0
	#define NOISE_SCALE 256.0
	#define NOISE_SEED_DEFAULT 0

	#define RANDOM_SEED_DEFAULT 0

	#define SCALAR_AS_HEX(_TYPE_, _VALUE_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex << (uintmax_t) ((_TYPE_) (_VALUE_)) \
			<< std::dec

	#define STRING_EMPTY "EMPTY"

	#define STRING_CHECK(_STRING_) (_STRING_.empty() ? STRING_EMPTY : _STRING_.c_str())

	#define THROW_EXCEPTION(_EXCEPT_) THROW_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		nomic::exception::generate(_EXCEPT_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)

	enum {
		BLOCK_AIR = 0,
		BLOCK_BOUNDARY,
		BLOCK_WATER,
		BLOCK_STONE,
		BLOCK_SAND,
		BLOCK_DIRT,
	};

	enum {
		BLOCK_ATTRIBUTE_STATIC = 1,
		BLOCK_ATTRIBUTE_BREAKABLE = 2,
	};
}

#endif // NOMIC_DEFINE_H_
