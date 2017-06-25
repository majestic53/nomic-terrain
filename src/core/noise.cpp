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

/**
 * Code derived from: http://blog.kazade.co.uk/2014/05/a-public-domain-c11-1d2d3d-perlin-noise.html
 */

#include <algorithm>
#include "../../include/core/noise.h"
#include "../../include/core/random.h"
#include "./noise_type.h"

namespace nomic {

	namespace core {

		noise::noise(
			__in_opt uint32_t seed,
			__in_opt uint32_t octaves,
			__in_opt double amplitude,
			__in_opt double persistence
			) :
				m_amplitude(0),
				m_octaves(0),
				m_persistence(0),
				m_seed(0)
		{
			setup(seed, octaves, amplitude, persistence);
		}

		noise::noise(
			__in const noise &other
			) :
				m_amplitude(other.m_amplitude),
				m_octaves(other.m_octaves),
				m_persistence(other.m_persistence),
				m_seed(other.m_seed),
				m_vector(other.m_vector)
		{
			return;
		}

		noise::~noise(void)
		{
			return;
		}

		noise &
		noise::operator=(
			__in const noise &other
			)
		{

			if(this != &other) {
				m_amplitude = other.m_amplitude;
				m_octaves = other.m_octaves;
				m_persistence = other.m_persistence;
				m_seed = other.m_seed;
				m_vector = other.m_vector;
			}

			return *this;
		}

		double 
		noise::amplitude(void) const
		{
			return m_amplitude;
		}

		double 
		noise::fade(
			__in double t
			)
		{
			return (t * t * t * ((t * ((t * 6.0) - 15.0)) + 10.0));
		}

		double 
		noise::generate(
			__in double x
			)
		{
			return generate(x, 0.0, 0.0);
		}

		double 
		noise::generate(
			__in double x,
			__in double y
			)
		{
			return generate(x, y, 0.0);
		}

		double 
		noise::generate(
			__in double x,
			__in double y,
			__in double z
			)
		{
			double amplitude = m_amplitude, persistence = 1.0, result = 0;

			for(uint32_t iter = 0; iter < m_octaves; ++iter) {
				result += (generate_octave(x, y, z) * amplitude * persistence);
				amplitude *= 0.5;
				persistence *= m_persistence;
				x *= 2.0;
				y *= 2.0;
				z *= 2.0;
			}

			return result;
		}

		double 
		noise::generate_octave(
			__in double x,
			__in double y,
			__in double z
			)
		{			
			double a, b, u, v, w;
			int32_t xi, yi, zi;
			uint32_t i[6] = { 0 }, j[8] = { 0 };

			xi = ((int32_t) std::floor(x)) & 255;
			yi = ((int32_t) std::floor(y)) & 255;
			zi = ((int32_t) std::floor(z)) & 255;

			x -= std::floor(x);
			y -= std::floor(y);
			z -= std::floor(z);

			u = fade(x);
			v = fade(y);
			w = fade(z);

			i[0] = m_vector[xi] + yi;
			i[1] = m_vector[i[0]] + zi;
			i[2] = m_vector[i[0] + 1] + zi;
			i[3] = m_vector[xi + 1] + yi;
			i[4] = m_vector[i[3]] + zi;
			i[5] = m_vector[i[3] + 1] + zi;

			j[0] = m_vector[i[1]];
			j[1] = m_vector[i[4]];
			j[2] = m_vector[i[2]];
			j[3] = m_vector[i[5]];
			j[4] = m_vector[i[1] + 1];
			j[5] = m_vector[i[4] + 1];
			j[6] = m_vector[i[2] + 1];
			j[7] = m_vector[i[5] + 1];

			a = interpolate(v, interpolate(u, gradient(j[0], x, y, z), gradient(j[1], x - 1, y, z)),
				interpolate(u, gradient(j[2], x, y - 1, z), gradient(j[3], x - 1, y - 1, z)));
			b = interpolate(v, interpolate(u, gradient(j[4], x, y, z - 1), gradient(j[5], x - 1, y, z - 1)),
				interpolate(u, gradient(j[6], x, y - 1, z - 1), gradient(j[7], x - 1, y - 1, z - 1)));

			return interpolate(w, a, b);
		}

		double 
		noise::gradient(
			__in uint32_t hash,
			__in double x,
			__in double y,
			__in double z
			)
		{
			uint32_t h;
			double u, v;

			h = (hash & 15);
			u = ((h < 8) ? x : y);
			v = ((h < 4) ? y : ((h == 12) || (h == 14) ? x : z));

			return ((!(h & 1) ? u : -u) + (!(h & 2) ? v : -v));
		}

		double 
		noise::interpolate(
			__in double t,
			__in double a,
			__in double b
			)
		{
			return ((t * (b - a)) + a);
		}

		uint32_t 
		noise::octaves(void) const
		{
			return m_octaves;
		}

		double 
		noise::persistence(void) const
		{
			return m_persistence;
		}

		uint32_t 
		noise::seed(void) const
		{
			return m_seed;
		}

		void 
		noise::setup(
			__in uint32_t seed,
			__in_opt uint32_t octaves,
			__in_opt double amplitude,
			__in_opt double persistence
			)
		{
			nomic::core::random<uint32_t> rand;
			std::vector<uint32_t>::iterator middle;

			if(amplitude < NOISE_AMPLITUDE_MIN) {
				THROW_NOMIC_CORE_NOISE_EXCEPTION_FORMAT(NOMIC_CORE_NOISE_EXCEPTION_AMPLITUDE_INVALID,
					"Amplitude=%g", amplitude);
			}

			m_amplitude = amplitude;

			if(!octaves) {
				THROW_NOMIC_CORE_NOISE_EXCEPTION_FORMAT(NOMIC_CORE_NOISE_EXCEPTION_OCTAVES_INVALID,
					"Octaves=%u", octaves);
			}

			m_octaves = octaves;

			if(persistence < NOISE_PERSISTENCE_MIN) {
				THROW_NOMIC_CORE_NOISE_EXCEPTION_FORMAT(NOMIC_CORE_NOISE_EXCEPTION_PERSISTENCE_INVALID,
					"Persistence=%g", persistence);
			}

			m_persistence = persistence;

			rand.setup(seed);
			m_seed = rand.seed();
			m_vector.resize(512);
			middle = (m_vector.begin() + 256);
			std::iota(m_vector.begin(), middle, 0);
			std::shuffle(m_vector.begin(), middle, rand.generator());
			std::copy(m_vector.begin(), middle, middle);
		}

		std::string 
		noise::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NOMIC_CORE_NOISE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << ", Amplitude=" << m_amplitude
					<< ", Octaves=" << m_octaves
					<< ", Persistence=" << m_persistence
					<< ", Seed=" << SCALAR_AS_HEX(uint32_t, m_seed);
			}

			return result.str();
		}
	}
}
