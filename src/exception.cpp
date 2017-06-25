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

#include <cstdarg>
#include "../include/exception.h"

namespace nomic {

	exception::exception(
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line
		) :
			std::runtime_error(message),
			m_file(file),
			m_function(function),
			m_line(line)
	{
		return;
	}

	exception::exception(
		__in const exception &other
		) :
			std::runtime_error(other),
			m_file(other.m_file),
			m_function(other.m_function),
			m_line(other.m_line)
	{
		return;
	}

	exception::~exception(void)
	{
		return;
	}

	exception &
	exception::operator=(
		__in const exception &other
		)
	{

		if(this != &other) {
			std::runtime_error::operator=(std::runtime_error(other));
			m_file = other.m_file;
			m_function = other.m_function;
			m_line = other.m_line;
		}

		return *this;
	}

	void 
	exception::generate(
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line,
		__in const char *format,
		...
		)
	{
		std::stringstream result;

		result << message;

		if(format) {
			int length;
			va_list arguments;
			std::string buffer;

			va_start(arguments, format);
			length = std::vsnprintf(nullptr, 0, format, arguments);
			va_end(arguments);

			if(length > 0) {
				buffer.resize(++length);
				va_start(arguments, format);
				length = std::vsnprintf(&buffer[0], buffer.size(), format, arguments);
				va_end(arguments);
			}

			if(length < 0) {
				buffer = EXCEPTION_MALFORMED;
			}

			if(!buffer.empty()) {

				if(!result.str().empty()) {
					result << ": ";
				}

				result << buffer;
			}
		}

		throw nomic::exception(result.str(), file, function, line);
	}

	std::string 
	exception::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		result << std::runtime_error::what();

		if(verbose) {

			if(!result.str().empty()) {
				result << " ";
			}

			result << "(";

			if(!m_function.empty()) {
				result << m_function << ":";
			}

			if(!m_file.empty()) {
				result << m_file << ":";
			}

			result << m_line << ")";
		}

		return result.str();
	}
}
