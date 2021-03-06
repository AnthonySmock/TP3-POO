// Copyright © 2015 Lénaïc Bagnères, hnc@singularity.fr

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef HOPP_STREAM_IGNORE_LINE_HPP
#define HOPP_STREAM_IGNORE_LINE_HPP

#include <cctype>
#include <istream>


namespace hopp
{
	/// @brief Ignore one line
	/// @param[in,out] in A std::istream
	/// @ingroup hopp_stream
	inline void ignore_line(std::istream & in)
	{
		std::string line;
		std::getline(in, line);
	}
	
	/// @brief Ignore one line until a delimiter
	/// @param[in,out] in        A std::istream
	/// @param[in]     delimiter A char
	/// @ingroup hopp_stream
	inline void ignore_line(std::istream & in, char const delimiter)
	{
		std::string line;
		std::getline(in, line, delimiter);
	}
}

#endif
