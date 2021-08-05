#include "UMat.h"
#include <vcclr.h>
using namespace System;
namespace LZH
{
	UMat::UMat()
	{
		_data = new lzh::Image();
	}
	UMat::UMat(int height, int width, int stride, System::IntPtr data)
	{
		if ((stride%width) == 0)
			_data = new lzh::Image(width, height, stride / width, data.ToPointer(), lzh::TP_8U, true);
		else
			_data = new lzh::Image(FromBitmap(height, width, stride, data.ToPointer(), lzh::TP_8U, true));
	}
	UMat::UMat(int height, int width, int stride, IntPtr data, DataType type, bool copy)
	{
		if ((stride%width) == 0)
			_data = new lzh::Image(width, height, stride / width, data.ToPointer(), lzh::TP_8U, copy);
		else
			_data = new lzh::Image(FromBitmap(height, width, stride, data.ToPointer(), lzh::TP_8U, copy));
	}
	UMat::UMat(int height, int width, int channel, int stride, IntPtr data, DataType type, bool copy)
	{
		if ((stride%width) == 0)
			_data = new lzh::Image(width, height, channel, data.ToPointer(), lzh::TP_8U, copy);
		else
			_data = new lzh::Image(FromBitmap(height, width, stride, data.ToPointer(), lzh::TP_8U, copy));
	}
	UMat::UMat(UMat ^ umat)
	{
		_data = new lzh::Image(umat->d_ptr());
	}
	UMat::UMat(System::String ^ path)
	{
		pin_ptr<const wchar_t> wcpath = PtrToStringChars(path);
		_data = new lzh::Image(lzh::imread(lzh::WStr2Str(wcpath)));
	}
	UMat::UMat(const lzh::Image image)
	{
		_data = new lzh::Image(image);
	}
	UMat::~UMat()
	{
		delete _data;
	}

	bool UMat::Empty()
	{
		return _data->empty();
	}

	DataType UMat::Type::get()
	{
		return (DataType)_data->type();
	}
	int UMat::Height::get()
	{
		return _data->rows;
	}
	int UMat::Width::get()
	{
		return _data->cols;
	}
	int UMat::Stride::get()
	{
		return _data->step;
	}
	int UMat::Channel::get()
	{
		return _data->channels();
	}
	bool UMat::DeepCopy::get()
	{
		return _data->isDeepCopy();
	}
	IntPtr UMat::Data::get()
	{
		return IntPtr(_data->Data());
	}
	void UMat::SetPixel(int x, int y, System::Drawing::Color^ color)
	{
		if (x < 0 || _data->cols >= x || y < 0 || _data->rows >= y)throw gcnew System::IndexOutOfRangeException();
		lzh::Color pixel(color->R, color->G, color->B, color->A);
		_data->setPixel(x, y, pixel);
	}
	System::Drawing::Color^ UMat::GetPixel(int x, int y)
	{
		if (x < 0 || _data->cols >= x || y < 0 || _data->rows >= y)throw gcnew System::IndexOutOfRangeException();
		lzh::Color pixel = _data->pixel(x, y);
		return System::Drawing::Color::FromArgb(pixel.alpha(), pixel.red(), pixel.green(), pixel.blue());
	}
	void UMat::Save(System::String ^ path)
	{
		pin_ptr<const wchar_t> wcpath = PtrToStringChars(path);
		lzh::imwrite(lzh::WStr2Str(wcpath), *_data);
	}
	lzh::Image& UMat::d_ptr()
	{
		return *_data;
	}
}