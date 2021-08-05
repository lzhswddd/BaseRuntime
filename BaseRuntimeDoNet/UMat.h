#pragma once
#include "lzh.h"
namespace LZH
{
	//public enum class ColorGamut
	//{
	//	ColorGamutShift = (1 << 5),
	//	ColorGamutMask = 0x1F,

	//	ColorGamutCustom = 0,
	//	ColorGamutGray,
	//	ColorGamutRGB,
	//	ColorGamutBGR,
	//	ColorGamutRGBA,
	//	ColorGamutBGRA,
	//	ColorGamutCMYK,
	//	ColorGamutHSV,
	//	ColorGamutHSL,
	//	ColorGamutXYZ,
	//	ColorGamutYXY,
	//	ColorGamutLab,
	//	ColorGamutLuv,
	//	ColorGamutMean,
	//	ColorGamutMaxMin,
	//	ColorGamutR,
	//	ColorGamutB,
	//	ColorGamutG
	//};
	public enum class DataType
	{
		UINT8 = 1,
		INT8 = 2,
		UINT16 = 3,
		INT16 = 4,
		UINT32 = 5,
		INT32 = 6,
		UINT64 = 7,
		INT64 = 8,
		FLOAT = 9,
		DOUBLE = 10,
		LDOUBLE = 11,
	};
	public ref class UMat
	{
	public:
		UMat();
		//默认DataType为UINT8, 内存深拷贝
		UMat(int height, int width, int stride, System::IntPtr data);
		UMat(int height, int width, int stride, System::IntPtr data, DataType type, bool copy);
		UMat(int height, int width, int channel, int stride, System::IntPtr data, DataType type, bool copy);
		UMat(UMat^ umat);
		UMat(System::String ^ path);
		UMat(const lzh::Image image);
		~UMat();

		bool Empty();

		property DataType Type
		{
			DataType get();
		}
		property int Height
		{
			int get();
		}
		property int Width
		{
			int get();
		}
		property int Stride
		{
			int get();
		}
		property int Channel
		{
			int get();
		}
		property bool DeepCopy
		{
			bool get();
		}
		property System::IntPtr Data
		{
			System::IntPtr get();
		}
		void SetPixel(int x, int y, System::Drawing::Color^ color);
		System::Drawing::Color^ GetPixel(int x, int y);
		void Save(System::String ^ path);
		lzh::Image& d_ptr();
	private:
		lzh::Image* _data;
	};
};