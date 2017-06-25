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
				m_max(max),
				m_random(seed)
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

					height = (((BLOCK_HEIGHT_MAX - BLOCK_HEIGHT_MIN) * scale) + BLOCK_HEIGHT_MIN);
					if(height < BLOCK_HEIGHT_MIN) {
						height = BLOCK_HEIGHT_MIN;
					}

					if(height > BLOCK_HEIGHT_MAX) {
						height = BLOCK_HEIGHT_MAX;
					}

					for(int32_t y = (CHUNK_HEIGHT - 1); y >= 0; --y) {

						if(!y) {
							chunk.set_block(glm::uvec3(x, y, z), BLOCK_BOUNDARY,
								BLOCK_ATTRIBUTE_STATIC | ~BLOCK_ATTRIBUTE_BREAKABLE);
						} else if(y > height) {
							chunk.set_block(glm::uvec3(x, y, z), BLOCK_AIR,
								BLOCK_ATTRIBUTE_STATIC | ~BLOCK_ATTRIBUTE_BREAKABLE);
						} else {
							chunk_column(glm::vec3(x, y, z), chunk);
							break;
						}
					}
				}
			}
		}

		uint32_t 
		generator::chunk_block_blend(
			__in uint32_t position,
			__in uint32_t min,
			__in uint32_t max,
			__in uint32_t a,
			__in uint32_t b
			)
		{
			uint32_t result;

			if(min > max) {
				THROW_NOMIC_TERRAIN_GENERATOR_EXCEPTION_FORMAT(NOMIC_TERRAIN_GENERATOR_EXCEPTION_RANGE_INVALID,
					"Range={%u, %u}", min, max);
			}

			if(position > max) {
				result = b;
			} else if(position < min) {
				result = a;
			} else {
				uint32_t first, second;
				std::vector<double> interval, weight;

				if(a <= b) {
					first = a;
					second = b;
				} else {
					first = b;
					second = a;
				}

				for(uint32_t iter = first; iter <= second; ++iter) {
					interval.push_back(iter);

					if(iter == first) {
						weight.push_back((min < max) ? ((position - min) / (double) (max - min)) : 0.5);
					} else if(iter == second) {
						weight.push_back((min < max) ? (1.0 - ((position - min) / (double) (max - min))) : 0.5);
					} else {
						weight.push_back(0.0);
					}
				}

				result = (uint32_t) std::piecewise_linear_distribution<>(interval.begin(), interval.end(), weight.begin())(
					m_random.generator());
			}

			return result;
		}

		void 
		generator::chunk_column(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool top = true;

			for(int32_t y = BLOCK_HEIGHT_WATER; y >= 0; --y) {

				if(!y) { // boundary
					chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_BOUNDARY,
						BLOCK_ATTRIBUTE_STATIC | ~BLOCK_ATTRIBUTE_BREAKABLE);
				} else if(position.y >= BLOCK_HEIGHT_DIRT) { // above-water

					if(position.y >= BLOCK_HEIGHT_STONE) { // snow/stone

						if(top && (position.y >= BLOCK_HEIGHT_SNOW)) {
							top = false;
							chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_SNOW);
						} else {
							chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_STONE);
						}
					} else {

						if((y <= position.y) && (y > (position.y - BLOCK_DEPTH_DIRT))) { // dirt/stone

							if(top) { // grass
								top = false;
								chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_GRASS);
							} else { // dirt/stone
								chunk.set_block(glm::uvec3(position.x, y, position.z), chunk_block_blend(y,
									(position.y - BLOCK_DEPTH_DIRT), position.y, BLOCK_DIRT, BLOCK_STONE));
							}
						} else { // stone
							chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_STONE);
						}
					}

				} else { // underwater

					if((y <= position.y) && (y > (position.y - BLOCK_DEPTH_SAND))) { // sand/stone

						if(top) { // sand
							top = false;
							chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_SAND);
						} else { // sand/stone
							chunk.set_block(glm::uvec3(position.x, y, position.z), chunk_block_blend(y,
								(position.y - BLOCK_DEPTH_SAND), position.y, BLOCK_SAND, BLOCK_STONE));
						}
					} else if(y < position.y) { // stone
						chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_STONE);
					} else { // water
						chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_WATER,
							BLOCK_ATTRIBUTE_STATIC | ~BLOCK_ATTRIBUTE_BREAKABLE);
					}
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
			m_random.setup(seed);
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
