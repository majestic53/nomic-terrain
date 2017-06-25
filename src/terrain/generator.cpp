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

#include "../../include/terrain/generator.h"
#include "./generator_type.h"

namespace nomic {

	namespace terrain {

		generator::generator(
			__in_opt uint32_t seed,
			__in_opt uint32_t octaves,
			__in_opt double amplitude,
			__in_opt uint32_t max
			) :
				nomic::core::noise(seed, octaves, amplitude),
				m_max(max)
		{
			return;
		}

		generator::generator(
			__in const generator &other
			) :
				nomic::core::noise(other),
				m_max(other.m_max)
		{
			return;
		}

		generator::~generator(void)
		{
			return;
		}

		generator &
		generator::operator=(
			__in const generator &other
			)
		{

			if(this != &other) {
				nomic::core::noise::operator=(other);
				m_max = other.m_max;
			}

			return *this;
		}

		void 
		generator::chunk(
			__in const glm::ivec2 &position,
			__inout nomic::terrain::chunk &chunk
			)
		{
			double scale;
			uint32_t height;

			if((position.x >= m_max) || (position.y >= m_max)) {
				THROW_NOMIC_TERRAIN_GENERATOR_EXCEPTION_FORMAT(NOMIC_TERRAIN_GENERATOR_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u}", position.x, position.y);
			}

			chunk.set_position(position);

			for(int32_t z = (CHUNK_WIDTH - 1); z >= 0; --z) {

				for(int32_t x = 0; x < CHUNK_WIDTH; ++x) {
					scale = std::abs(nomic::core::noise::generate(((position.x * CHUNK_WIDTH) + x) / NOISE_SCALE,
						((position.y * CHUNK_WIDTH) + z) / NOISE_SCALE));

					height = (((CHUNK_HEIGHT_MAX - CHUNK_HEIGHT_MIN) * scale) + CHUNK_HEIGHT_MIN);
					if(height < CHUNK_HEIGHT_MIN) {
						height = CHUNK_HEIGHT_MIN;
					}

					if(height > CHUNK_HEIGHT_MAX) {
						height = CHUNK_HEIGHT_MAX;
					}

					// TODO: generate blocks
					chunk.set_block(glm::uvec3(x, height, z), BLOCK_BOUNDARY);
					// ---
				}
			}
		}

		uint32_t 
		generator::max(void) const
		{
			return m_max;
		}

		uint32_t 
		generator::seed(void) const
		{
			return m_seed;
		}

		void 
		generator::setup(
			__in uint32_t seed,
			__in_opt uint32_t octaves,
			__in_opt double amplitude,
			__in_opt uint32_t max
			)
		{
			nomic::core::noise::setup(seed, octaves, amplitude);
			m_max = max;
		}

		std::string 
		generator::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NOMIC_TERRAIN_GENERATOR_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::noise::to_string(verbose)
					<< ", Max=" << m_max;
			}

			return result.str();
		}
	}
}
