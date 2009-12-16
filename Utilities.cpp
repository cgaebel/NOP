#include "Utilities.h"

std::string ConvertToString(size_t num)
{
	std::ostringstream stream;
	stream << num;
	return stream.str();
}
