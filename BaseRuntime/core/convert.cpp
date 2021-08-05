#define LZH_EXPORTS
#include "convert.h"

int lzh::Convert::toInt32(const std::string & value)
{
	return std::stoi(value);
}

int lzh::Convert::toInt32(const Value & value)
{
	return value.Int32();
}

unsigned int lzh::Convert::toUInt32(const std::string & value)
{
	return std::stoul(value);
}

unsigned int lzh::Convert::toUInt32(const Value & value)
{
	return value.UInt32();
}

long long lzh::Convert::toInt64(const std::string & value)
{
	return std::stoll(value);
}

long long lzh::Convert::toInt64(const Value & value)
{
	return value.Int64();
}

unsigned long long lzh::Convert::toUInt64(const std::string & value)
{
	return std::stoull(value);
}

unsigned long long lzh::Convert::toUInt64(const Value & value)
{
	return value.UInt64();
}

float lzh::Convert::toSingle(const std::string & value)
{
	return std::stof(value);
}

float lzh::Convert::toSingle(const Value & value)
{
	return value.Float();
}

double lzh::Convert::toDouble(const std::string & value)
{
	return std::stod(value);
}

double lzh::Convert::toDouble(const Value & value)
{
	return value.Double();
}

std::string lzh::Convert::toString(const Value & value)
{
	return value.toString();
}
