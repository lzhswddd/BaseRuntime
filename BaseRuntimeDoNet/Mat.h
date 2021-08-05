#pragma once
#include "Value.h"

namespace LZH
{
	ref class UMat;
	public ref class Matrix
	{
	public:
		Matrix();
		//默认DataType为UINT8, 内存深拷贝
		Matrix(array<double, 1>^ data);
		Matrix(array<double, 2>^ data);
		Matrix(array<double, 3>^ data);
		Matrix(int height, int width);
		Matrix(int height, int width, int channels);
		Matrix(int height, int width, int channels, System::IntPtr data);
		Matrix(int height, int width, int channels, System::IntPtr data, bool copy);
		~Matrix();

		bool Empty();
		void InitializeZero();

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
		property System::IntPtr Data
		{
			System::IntPtr get();
		}
		
		array<double, 1>^ Vec();
		array<double, 2>^ Mat();
		array<double, 3>^ Tensor();

		double GetValue(int index_1);
		double GetValue(int index_1, int index_2);
		double GetValue(int index_1, int index_2, int index_3);

		void SetValue(int index_1, double value);
		void SetValue(int index_1, int index_2, double value);
		void SetValue(int index_1, int index_2, int index_3, double value);

		UMat^ ToImage();

		static Matrix^ Normalize(Matrix ^ matrix, double alpha, double bias);
		

		lzh::Mat& d_ptr();
	private:
		Matrix(const lzh::Mat & mat);

		lzh::Mat* _data;
	};
};
