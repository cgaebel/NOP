#include "Utilities.h"

namespace Utilities
{
	std::string ConvertUIntToString(const unsigned int var)
	{
		std::ostringstream stream;
		stream << var;
		return stream.str();
	}

	std::string ConvertSizeTToString(const size_t var)
	{
		std::ostringstream stream;
		stream << var;
		return stream.str();
	}
}
