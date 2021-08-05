#include "Mat.h"
#include "UMat.h"

namespace LZH
{
	Matrix::Matrix()
	{
		_data = new lzh::Mat();
	}
	Matrix::Matrix(const lzh::Mat & mat)
	{
		_data = new lzh::Mat(mat);
	}
	Matrix::Matrix(array<double, 1>^ data)
	{
		_data = new lzh::Mat(data->Length);
		for (int i = 0; i < _data->len(); i++) {
			_data->at(i) = _T((double)data->GetValue(i));
		}
	}
	Matrix::Matrix(array<double, 2>^ data)
	{
		_data = new lzh::Mat(data->GetLength(1), data->GetLength(0));
		for (int h = 0; h < _data->rows(); h++) {
			for (int w = 0; w < _data->cols(); w++) {
				_data->at(h, w) = _T((double)data->GetValue(h, w));
			}
		}
	}
	Matrix::Matrix(array<double, 3>^ data)
	{
		_data = new lzh::Mat(data->GetLength(1), data->GetLength(0), data->GetLength(2));
		for (int h = 0; h < _data->rows(); h++) {
			for (int w = 0; w < _data->cols(); w++) {
				for (int c = 0; c < _data->channels(); c++) {
					_data->at(h, w, c) = _T((double)data->GetValue(h, w, c));
				}
			}
		}
	}
	Matrix::Matrix(int height, int width)
	{
		_data = new lzh::Mat(width, height);
	}
	Matrix::Matrix(int height, int width, int channels)
	{
		_data = new lzh::Mat(width, height, channels);
	}
	Matrix::Matrix(int height, int width, int channels, System::IntPtr data)
	{
		_data = new lzh::Mat(width, height, channels, data.ToPointer(), true);
	}
	Matrix::Matrix(int height, int width, int channels, System::IntPtr data, bool copy)
	{
		_data = new lzh::Mat(width, height, channels, data.ToPointer(), copy);
	}
	Matrix::~Matrix()
	{
		delete _data;
	}

	bool Matrix::Empty()
	{
		return _data->empty();
	}

	void Matrix::InitializeZero()
	{
		_data->fill(0);
	}

	array<double, 1>^ Matrix::Vec()
	{
		array<double, 1>^ data = gcnew array<double, 1>(_data->len());
		for (int i = 0; i < _data->len(); i++) {
			data->SetValue(_data->at(i), i);
		}
		return data;
	}

	array<double, 2>^ Matrix::Mat()
	{
		array<double, 2>^ data = gcnew array<double, 2>(_data->rows(), _data->cols());
		for (int h = 0; h < _data->rows(); h++) {
			for (int w = 0; w < _data->cols(); w++) {
				data->SetValue(_data->at(h, w), h, w);
			}
		}
		return data;
	}

	array<double, 3>^ Matrix::Tensor()
	{
		array<double, 3>^ data = gcnew array<double, 3>(_data->rows(), _data->cols(), _data->channels());
		for (int h = 0; h < _data->rows(); h++) {
			for (int w = 0; w < _data->cols(); w++) {
				for (int c = 0; c < _data->channels(); c++) {
					data->SetValue(_data->at(h, w, c), h, w, c);
				}
			}
		}
		return data;
	}

	double Matrix::GetValue(int index_1)
	{
		return _data->at(index_1);
	}

	double Matrix::GetValue(int index_1, int index_2)
	{
		return _data->at(index_1, index_2);
	}

	double Matrix::GetValue(int index_1, int index_2, int index_3)
	{
		return _data->at(index_1, index_2, index_3);
	}

	void Matrix::SetValue(int index_1, double value)
	{
		_data->at(index_1) = value;
	}

	void Matrix::SetValue(int index_1, int index_2, double value)
	{
		_data->at(index_1, index_2) = value;
	}

	void Matrix::SetValue(int index_1, int index_2, int index_3, double value)
	{
		_data->at(index_1, index_2, index_3) = value;
	}

	Matrix ^ Matrix::Normalize(Matrix ^ matrix, double alpha, double bias)
	{
		lzh::Mat dst;
		lzh::normalize(matrix->d_ptr(), dst, alpha, bias, lzh::NORM_MINMAX);
		return gcnew Matrix(dst);
	}
	UMat ^ Matrix::ToImage()
	{
		lzh::Mat dst;
		lzh::normalize(*_data, dst, 0, 255, lzh::NORM_MINMAX);
		UMat^ image = gcnew UMat();
		image->d_ptr() = dst.toArray(lzh::TP_8U);
		return image;
	}

	int Matrix::Height::get()
	{
		return _data->rows();
	}
	int Matrix::Width::get()
	{
		return _data->cols();
	}
	int Matrix::Stride::get()
	{
		return _data->step();
	}
	int Matrix::Channel::get()
	{
		return _data->channels();
	}
	System::IntPtr Matrix::Data::get()
	{
		return System::IntPtr(_data->data());
	}
	lzh::Mat & Matrix::d_ptr()
	{
		return *_data;
	}
}