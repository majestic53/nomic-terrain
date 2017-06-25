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

#include "../include/exception.h"

// TODO: DEBUG
#include <fstream>
#include "../include/terrain/generator.h"
// ---

int 
main(void)
{
	int result = EXIT_SUCCESS;

	try {

		// TODO
		#define COUNT 10
		#define COLORIZE true
		#define BLOCK_HEIGHT_GRASS 66
		#define BLOCK_HEIGHT_STONE 100
		#define BLOCK_HEIGHT_WATER 64

		nomic::terrain::generator gen;
		nomic::terrain::chunk ch[COUNT][COUNT];

		for(int32_t z = 0; z < COUNT; ++z) {

			for(int32_t x = 0; x < COUNT; ++x) {
				gen.chunk(glm::ivec2(x, z), ch[x][z]);
			}
		}

		std::stringstream path;
		path << "./" << gen.seed() << ".ppm";

		std::ofstream file(path.str(), std::ios::out | std::ios::trunc);
		if(!file) {
			THROW_EXCEPTION("Failed to create ppm!");
		}

		file << "P3" << std::endl << (COUNT * CHUNK_WIDTH) << " " << (COUNT * CHUNK_WIDTH) << std::endl << "255" << std::endl;

		for(int32_t z = 0; z < (COUNT * CHUNK_WIDTH); ++z) {

			for(int32_t x = 0; x < (COUNT * CHUNK_WIDTH); ++x) {
				uint16_t scale;
				uint8_t height = ch[x / CHUNK_WIDTH][z / CHUNK_WIDTH].height(glm::uvec2(x % CHUNK_WIDTH, z % CHUNK_WIDTH));

				if(COLORIZE) {

					if(height <= BLOCK_HEIGHT_WATER) {
						scale = (19 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (89 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (226 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
					} else if((height > BLOCK_HEIGHT_WATER) && (height <= BLOCK_HEIGHT_GRASS)) {
						scale = (239 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (235 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (122 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
					} else if((height > BLOCK_HEIGHT_GRASS) && (height <= BLOCK_HEIGHT_STONE)) {
						scale = (48 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (104 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (20 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
					} else {
						scale = (82 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (82 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
						scale = (82 * (height	/ (double) BLOCK_HEIGHT_WATER));
						file << scale << " ";
					}
				} else {
					scale = (255 * (height	/ (double) CHUNK_HEIGHT));
					file << scale << " " << scale << " " << scale << " ";
				}
			}

			file << std::endl;
		}

		file.close();
		// ---

	} catch(nomic::exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = EXIT_FAILURE;
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = EXIT_FAILURE;
	}

	return result;
}
