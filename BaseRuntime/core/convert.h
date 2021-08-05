#ifndef __CONVERT_H__
#define __CONVERT_H__
#include "core.h"
#include <string>

namespace lzh
{
	class LZHAPI Convert
	{
	public:
		static int toInt32(const std::string &value);
		static int toInt32(const Value &value);
		static unsigned int toUInt32(const std::string &value);
		static unsigned int toUInt32(const Value &value);
		static long long toInt64(const std::string &value);
		static long long toInt64(const Value &value);
		static unsigned long long toUInt64(const std::string &value);
		static unsigned long long toUInt64(const Value &value);
		static float toSingle(const std::string &value);
		static float toSingle(const Value &value);
		static double toDouble(const std::string &value);
		static double toDouble(const Value &value);
		static std::string toString(const Value &value);
	};
}
#endif // !__CONVERT_H__
