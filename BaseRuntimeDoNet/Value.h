#pragma once
#include "lzh.h"

namespace LZH
{
	public enum class BaseType
	{
		INT8 = 1,
		UINT8 = 2,
		UINT16 = 3,
		INT16 = 4,
		UINT32 = 5,
		INT32 = 6,
		UINT64 = 7,
		INT64 = 8,
		FLOAT32 = 9,
		FLOAT64 = 10,
		FLOAT128 = 11,
	};
	public ref class Value
	{
	public:
		Value(char v);
		Value(unsigned char v);
		Value(short v);
		Value(unsigned short v);
		Value(int v);
		Value(unsigned int v);
		Value(float v);
		Value(double v);
		Value(long double v);
		Value(long long v);
		Value(unsigned long long v);
		Value(const lzh::Value *val);
		~Value();

		BaseType type();

		bool isChar();
		bool isUChar();
		bool isInt16();
		bool isUInt16();
		bool isInt32();
		bool isUInt32();
		bool isInt64();
		bool isUInt64();
		bool isFloat();
		bool isDouble();
		bool isLongDouble();

		property char Char
		{
			char get();
		}
		property unsigned char UChar
		{
			unsigned char get();
		}
		property short Short 
		{
			short get();
		}
		property unsigned short UShort
		{
			unsigned short get();
		}
		property int Int32
		{
			int get();
		}
		property unsigned int UInt32
		{
			unsigned int get();
		}
		property long long Int64
		{
			long long get();
		}
		property unsigned long long UInt64
		{
			unsigned long long get();
		}
		property float Float
		{
			float get();
		}
		property double Double
		{
			double get();
		}
		property long double LongDouble
		{
			long double get();
		}

		System::String^ toString();

		lzh::Value *d_ptr();
	private:
		lzh::Value *data;
	};
}

