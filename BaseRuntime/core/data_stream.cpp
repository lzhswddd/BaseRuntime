#define LZH_EXPORTS
#include "data_stream.h"
using namespace lzh;

std::ostream & lzh::operator << (std::ostream &out, const Mat &ma)
{
	return ma.show(out, false);
}
std::ostream & lzh::operator << (std::ostream &out, const Array &ma)
{
	return ma.show(out, false);
}
std::ostream & lzh::operator << (std::ostream & out, const Val_ & t)
{
	out << "Val_(" << t.v << ", type=" << DataType<mat_t>().type_name << ")";
	return out;
}
std::ostream & lzh::operator << (std::ostream & out, const Val & t)
{
	switch (t.type)
	{
	case lzh::TP_8S:	out << "Val(" << saturate_cast<int16>(t.v) << ", type=int8)"; break;
	case lzh::TP_8U:	out << "Val(" << saturate_cast<uint16>(t.v) << ", type=uint8)"; break;
	case lzh::TP_16S:	out << "Val(" << saturate_cast<int16>(t.v) << ", type=int16)"; break;
	case lzh::TP_16U:	out << "Val(" << saturate_cast<uint16>(t.v) << ", type=uint16)"; break;
	case lzh::TP_32S:	out << "Val(" << saturate_cast<int32>(t.v) << ", type=int32)"; break;
	case lzh::TP_32U:	out << "Val(" << saturate_cast<uint32>(t.v) << ", type=uint32)"; break;
	case lzh::TP_32F:	out << "Val(" << saturate_cast<float32>(t.v) << ", type=float32)"; break;
	case lzh::TP_64S:	out << "Val(" << saturate_cast<float64>(t.v) << ", type=float64)"; break;
	case lzh::TP_64U:	out << "Val(" << saturate_cast<float128>(t.v) << ", type=float64)"; break;
	case lzh::TP_64F:	out << "Val(" << saturate_cast<int64>(t.v) << ", type=int64)"; break;
	case lzh::TP_64D:	out << "Val(" << saturate_cast<uint64>(t.v) << ", type=uint64)"; break;
	default:
		break;
	}
	return out;
}
std::ostream & lzh::operator << (std::ostream & out, const Complex & o)
{
	out << "Complex(" << o.r << "+" << o.i << "i" << ", type=" << DataType<mat_t>().type_name << ")";
	return out;
}
std::ostream & lzh::operator << (std::ostream &out, const LargeNumber &LN)
{
	LN.show(out);
	return out;
}

std::ostream & lzh::operator<<(std::ostream & out, const Moments & mu)
{
	out << "Moments(" 
		<< "m00:" << mu.m00 << " "
		<< "m10:" << mu.m10 << " "
		<< "m01:" << mu.m01 << " "
		<< "m20:" << mu.m20 << " "
		<< "m11:" << mu.m11 << " "
		<< "m02:" << mu.m02 << " "
		<< "m30:" << mu.m30 << " "
		<< "m21:" << mu.m21 << " "
		<< "m12:" << mu.m12 << " "
		<< "m03:" << mu.m03 << std::endl;
	out << "mu20:" << mu.mu20 << " "
		<< "mu11:" << mu.mu11 << " "
		<< "mu02:" << mu.mu02 << " "
		<< "mu30:" << mu.mu30 << " "
		<< "mu21:" << mu.mu21 << " "
		<< "mu12:" << mu.mu12 << " "
		<< "mu03:" << mu.mu03 << std::endl;
	out << "nu20:" << mu.nu20 << " "
		<< "nu11:" << mu.nu11 << " "
		<< "nu02:" << mu.nu02 << " "
		<< "nu30:" << mu.nu30 << " "
		<< "nu21:" << mu.nu21 << " "
		<< "nu12:" << mu.nu12 << " "
		<< "nu03:" << mu.nu03 <<", type=" << DataType<float64>().type_name << ")";
	return out;
}

std::ostream & lzh::operator<<(std::ostream & out, const Vector & vec)
{
	// TODO: 在此处插入 return 语句
	return vec.show(out);
}

std::ostream & lzh::operator<<(std::ostream & out, const Element & element)
{
	element.show(out);
	return out;
}

std::ostream & lzh::operator<<(std::ostream & out, const TimeData & timeData)
{
	out << timeData.toString();
	return out;
}

std::vector<std::string>& lzh::operator<<(std::vector<std::string>& vecs, const char * v)
{ 
	vecs.push_back(v); return vecs;
}
