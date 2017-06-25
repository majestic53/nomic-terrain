# Nomic
# Copyright (C) 2017 David Jolly
#
# Nomic is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Nomic is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -Wall -Werror
CC_FLAGS_EXT=
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_INC_CORE=./../include/core/
DIR_INC_TERRAIN=./../include/terrain/
DIR_SRC=./
DIR_SRC_CORE=./core/
DIR_SRC_TERRAIN=./terrain/
LIB=libnomic-terrain.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)base_exception.o \
		$(DIR_BUILD)core_noise.o \
		$(DIR_BUILD)terrain_chunk.o $(DIR_BUILD)terrain_generator.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_base build_core build_terrain

### BASE ###

build_base: base_exception.o

base_exception.o: $(DIR_SRC)exception.cpp $(DIR_INC)exception.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)exception.cpp -o $(DIR_BUILD)base_exception.o

### CORE ###

build_core: core_noise.o

core_noise.o: $(DIR_SRC_CORE)noise.cpp $(DIR_INC_CORE)noise.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)noise.cpp -o $(DIR_BUILD)core_noise.o

### TERRAIN ###

build_terrain: terrain_chunk.o terrain_generator.o

terrain_chunk.o: $(DIR_SRC_TERRAIN)chunk.cpp $(DIR_INC_TERRAIN)chunk.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_TERRAIN)chunk.cpp -o $(DIR_BUILD)terrain_chunk.o

terrain_generator.o: $(DIR_SRC_TERRAIN)generator.cpp $(DIR_INC_TERRAIN)generator.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_TERRAIN)generator.cpp -o $(DIR_BUILD)terrain_generator.o
