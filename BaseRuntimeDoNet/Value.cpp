#include "Value.h"

namespace LZH
{
	Value::Value(char v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(unsigned char v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(short v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(unsigned short v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(int v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(unsigned int v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(float v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(double v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(long double v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(long long v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(unsigned long long v)
	{
		data = new lzh::Value(v);
	}
	Value::Value(const lzh::Value * val)
	{
		data = new lzh::Value(*val);
	}
	Value::~Value()
	{
		delete data;
	}
	BaseType Value::type()
	{
		return (BaseType)data->type();
	}
	bool Value::isChar()
	{
		return data->isi8();
	}
	bool Value::isUChar()
	{
		return data->isu8();
	}
	bool Value::isInt16()
	{
		return data->isi16();
	}
	bool Value::isUInt16()
	{
		return data->isu16();
	}
	bool Value::isInt32()
	{
		return data->isi32();
	}
	bool Value::isUInt32()
	{
		return data->isu32();
	}
	bool Value::isInt64()
	{
		return data->isi64();
	}
	bool Value::isUInt64()
	{
		return data->isu64();
	}
	bool Value::isFloat()
	{
		return data->isf32();
	}
	bool Value::isDouble()
	{
		return data->isf64();
	}
	bool Value::isLongDouble()
	{
		return data->isf128();
	}
	char Value::Char::get()
	{
		return data->Char();
	}
	unsigned char Value::UChar::get()
	{
		return data->UChar();
	}
	short Value::Short::get()
	{
		return data->Int16();
	}
	unsigned short Value::UShort::get()
	{
		return data->UInt16();
	}
	int Value::Int32::get()
	{
		return data->Int32();
	}
	unsigned int Value::UInt32::get()
	{
		return data->UInt32();
	}
	long long Value::Int64::get()
	{
		return data->Int64();
	}
	unsigned long long Value::UInt64::get()
	{
		return data->UInt64();
	}
	float Value::Float::get()
	{
		return data->Float();
	}
	double Value::Double::get()
	{
		return data->Double();
	}
	long double Value::LongDouble::get()
	{
		return data->LongDouble();
	}
	System::String ^ Value::toString()
	{
		return gcnew System::String(data->toString().c_str());
	}
	lzh::Value * Value::d_ptr()
	{
		return data;
	}
}
