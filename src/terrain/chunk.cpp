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

#include <cstring>
#include "../../include/terrain/chunk.h"
#include "./chunk_type.h"

namespace nomic {

	namespace terrain {

		enum {
			BLOCK_POSITION = 0,
			BLOCK_TYPE,
			BLOCK_ATTRIBUTES,
		};

		chunk::chunk(
			__in_opt const glm::ivec2 &position
			) :
				m_active(false),
				m_position(position)
		{
			clear();
		}

		chunk::chunk(
			__in const chunk &other
			) :
				m_active(other.m_active),
				m_position(other.m_position),
				m_queue(other.m_queue)
		{
			copy(other);
		}

		chunk::~chunk(void)
		{
			return;
		}

		chunk &
		chunk::operator=(
			__in const chunk &other
			)
		{

			if(this != &other) {
				m_active = other.m_active;
				m_position = other.m_position;
				m_queue = other.m_queue;
				copy(other);
			}

			return *this;
		}

		uint8_t 
		chunk::attributes(
			__in const glm::uvec3 &position
			) const
		{

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT) || (position.z >= CHUNK_WIDTH)) {
				THROW_NOMIC_TERRAIN_CHUNK_EXCEPTION_FORMAT(NOMIC_TERRAIN_CHUNK_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u, %u}", position.x, position.y, position.z);
			}

			return m_block_attributes[position.x][position.y][position.z];
		}

		void 
		chunk::clear(void)
		{
			memset(&m_block, BLOCK_AIR, CHUNK_BLOCK_COUNT);
			memset(&m_block_attributes, BLOCK_ATTRIBUTES_DEFAULT, CHUNK_BLOCK_COUNT);
			memset(&m_height, 0, CHUNK_BLOCK_SURFACE_COUNT);
		}

		void 
		chunk::copy(
			__in const chunk &other
			)
		{
			memcpy(&m_block, &other.m_block, CHUNK_BLOCK_COUNT);
			memcpy(&m_block_attributes, &other.m_block_attributes, CHUNK_BLOCK_COUNT);
			memcpy(&m_height, &other.m_height, CHUNK_BLOCK_SURFACE_COUNT);
		}

		uint8_t 
		chunk::height(
			__in const glm::uvec2 &position
			) const
		{

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT)) {
				THROW_NOMIC_TERRAIN_CHUNK_EXCEPTION_FORMAT(NOMIC_TERRAIN_CHUNK_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u}", position.x, position.y);
			}

			return m_height[position.x][position.y];
		}

		glm::ivec2 
		chunk::position(void) const
		{
			return m_position;
		}

		void 
		chunk::set(
			__in const glm::uvec3 &position,
			__in uint8_t type,
			__in_opt uint8_t attributes
			)
		{
			m_queue.push(std::make_tuple(position, type, attributes));
			m_active = true;
		}

		void 
		chunk::set_block(
			__in const glm::uvec3 &position,
			__in uint8_t type,
			__in_opt uint8_t attributes
			)
		{
			int8_t height;

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT) || (position.z >= CHUNK_WIDTH)) {
				THROW_NOMIC_TERRAIN_CHUNK_EXCEPTION_FORMAT(NOMIC_TERRAIN_CHUNK_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u, %u}", position.x, position.y, position.z);
			}

			m_block[position.x][position.y][position.z] = type;
			m_block_attributes[position.x][position.y][position.z] = attributes;

			height = (CHUNK_HEIGHT - 1);
			while(m_block[position.x][height][position.z] == BLOCK_AIR) {
				--height;
			}

			m_height[position.x][position.z] = height;
		}

		void 
		chunk::set_position(
			__in const glm::ivec2 &position
			)
		{
			m_position = position;
		}

		std::string 
		chunk::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << NOMIC_TERRAIN_CHUNK_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Position={" << m_position.x << ", " << m_position.y << "}"
					<< ", State=" << (m_active ? "Active" : "Inactive");
			}

			return result.str();
		}

		uint8_t 
		chunk::type(
			__in const glm::uvec3 &position
			) const
		{

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT) || (position.z >= CHUNK_WIDTH)) {
				THROW_NOMIC_TERRAIN_CHUNK_EXCEPTION_FORMAT(NOMIC_TERRAIN_CHUNK_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u, %u}", position.x, position.y, position.z);
			}

			return m_block[position.x][position.y][position.z];
		}

		void 
		chunk::update(void)
		{

			if(m_active) {
				m_active = false;

				while(!m_queue.empty()) {
					std::tuple<glm::uvec3, uint8_t, uint8_t> &action = m_queue.front();
					set_block(std::get<BLOCK_POSITION>(action), std::get<BLOCK_TYPE>(action),
						std::get<BLOCK_ATTRIBUTES>(action));
					m_queue.pop();
				}
			}
		}
	}
}
