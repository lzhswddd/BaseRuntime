#include "Elem.h"
#include "lzh.h"
#include <vcclr.h>
using namespace System;
using namespace Drawing;
using namespace Drawing::Imaging;

namespace LZH
{
	Elem::Elem()
	{
		elem = new lzh::Element();
	}
	Elem::Elem(System::Object ^ value)
	{
		System::String^ type = value->GetType()->ToString();
		Elem^ e = nullptr;
		if (type == gcnew System::String("System.Boolean"))
			e = ToElem((bool)value);
		else if (type == gcnew System::String("System.Int32"))
			e = ToElem((int)value);
		else if (type == gcnew System::String("System.Double"))
			e = ToElem((double)value);
		else if (type == gcnew System::String("System.String"))
			e = ToElem((System::String^)value);
		else if (type == gcnew System::String("LZH.UMat"))
			e = ToElem((UMat^)value);
		else if (type == gcnew System::String("System.Drawing.Bitmap"))
			e = ToElem((Drawing::Bitmap^)value);
		else if (type == gcnew System::String("LZH.Matrix"))
			e = ToElem((Matrix^)value);
		else if (type == gcnew System::String("System.Boolean[]"))
			e = ToElem((array<bool, 1>^)value);
		else if (type == gcnew System::String("System.Int32[]"))
			e = ToElem((array<int, 1>^)value);
		else if (type == gcnew System::String("System.Double[]"))
			e = ToElem((array<double, 1>^)value);
		else if (type == gcnew System::String("System.Char[]"))
			e = ToElem((array<char, 1>^)value);
		else if (type == gcnew System::String("System.String[]"))
			e = ToElem((array<System::String^, 1>^)value);
		else if (type == gcnew System::String("System.Collections.Generic.List`1[System.Int32]"))
			e = ToElem((System::Collections::Generic::List<int>^)value);
		else if (type == gcnew System::String("System.Collections.Generic.List`1[System.Double]"))
			e = ToElem((System::Collections::Generic::List<double>^)value);
		else if (type == gcnew System::String("System.Collections.Generic.List`1[System.String]"))
			e = ToElem((System::Collections::Generic::List<System::String^>^)value);
		else if (type == gcnew System::String("System.Collections.Generic.List`1[LZH.Elem]"))
			e = ToElem((System::Collections::Generic::List<Elem^>^)value);
		if (e == nullptr)throw gcnew System::NotSupportedException(type + ":不支持的类型");
		this->elem = new lzh::Element(*e->d_ptr());
		e->Release();
	}
	Elem::Elem(Elem ^ value)
	{
		if (value->Empty())throw gcnew System::ObjectDisposedException("Elem数据为空");
		elem = new lzh::Element(*value->d_ptr());
	}
	Elem::Elem(const lzh::Element & elem)
	{
		this->elem = new lzh::Element(elem);
	}
	Elem::Elem(const lzh::ElemList & elem)
	{
		this->elem = new lzh::Element(elem);
	}
	Elem::~Elem()
	{
		Release();
	}

	bool Elem::Empty()
	{
		return elem != nullptr ? elem->empty() : false;
	}

	bool Elem::IsIntPtr()
	{
		return elem->isIntPtr();
	}

	bool Elem::IsBool()
	{
		return elem->isbool();
	}

	bool Elem::IsInt()
	{
		return elem->isInt();
	}
	bool Elem::IsFloat()
	{
		return elem->isFloat();
	}
	bool Elem::IsDouble()
	{
		return elem->isDouble();
	}
	bool Elem::IsNumber()
	{
		return elem->isNumber();
	}
	bool Elem::IsMatrix()
	{
		return elem->isMat();
	}
	bool Elem::IsImage()
	{
		return elem->isImage();
	}
	bool Elem::IsString()
	{
		return elem->isString();
	}
	bool Elem::IsArray()
	{
		return elem->isStdVector();
	}
	bool Elem::IsList()
	{
		return elem->isList();
	}
	System::Object ^ Elem::toObject()
	{
		if (Empty())return nullptr;
		else if (IsIntPtr())return toIntPtr();
		else if (IsBool())return toBool();
		else if (IsInt())return toInt();
		else if (IsFloat())return toFloat();
		else if (IsDouble())return toDouble();
		else if (IsNumber())return toNumber();
		else if (IsMatrix())return toMatrix();
		else if (IsImage())return toBitmap();
		else if (IsString())return toString();
		else if (IsList())return toList();
		else gcnew System::FormatException("不支持的类型");
		return nullptr;
	}
	System::IntPtr Elem::toIntPtr()
	{
		EXCEPTION_TRANSFORM(return System::IntPtr(elem->toIntPtr()), FormatException);
	}
	bool Elem::toBool()
	{
		EXCEPTION_TRANSFORM(return elem->tobool(), FormatException);
	}
	int Elem::toInt()
	{
		EXCEPTION_TRANSFORM(return elem->toInt(), FormatException);
	}
	float Elem::toFloat()
	{
		EXCEPTION_TRANSFORM(return elem->toFloat(), FormatException);
	}
	double Elem::toDouble()
	{
		EXCEPTION_TRANSFORM(return elem->toDouble(), FormatException);
	}
	double Elem::toNumber()
	{
		EXCEPTION_TRANSFORM(return elem->toNumber(), FormatException);
	}
	Matrix ^ Elem::toMatrix()
	{
		EXCEPTION_TRANSFORM(
		lzh::Mat mat = elem->toMat();
		return gcnew Matrix(mat.rows(), mat.cols(), mat.channels(), System::IntPtr(mat.data()));
		, FormatException);
	}
	UMat^ Elem::toImage()
	{
		EXCEPTION_TRANSFORM(
		return gcnew UMat(elem->toImage());
		, FormatException);
	}
	Drawing::Bitmap ^ Elem::toBitmap()
	{
		if (!IsImage()) return nullptr;
		PixelFormat pixelFormat = PixelFormat::Format32bppArgb;
		UMat^ mat = toImage();
		switch (mat->Channel)
		{
		case 1:
			pixelFormat = PixelFormat::Format8bppIndexed;
			break;
		case 3:
			pixelFormat = PixelFormat::Format24bppRgb;
			break;
		case 4:
			pixelFormat = PixelFormat::Format32bppArgb;
			break;
		default: return nullptr;
		}
		Bitmap^ bitmap = nullptr;
		if (!mat->DeepCopy || mat->Stride % 4 != 0)
		{
			bitmap = gcnew Bitmap(mat->Width, mat->Height, pixelFormat);
			BitmapData^ bitmapData = bitmap->LockBits(Rectangle(0, 0, bitmap->Width, bitmap->Height), ImageLockMode::ReadWrite, bitmap->PixelFormat);
			for (int i = 0; i < mat->Height; i++)
			{
				memcpy((bitmapData->Scan0 + i * bitmapData->Stride).ToPointer(), (mat->Data + i * mat->Stride).ToPointer(), mat->Width*mat->Channel);
			}
			bitmap->UnlockBits(bitmapData);
		}
		else
		{
			bitmap = gcnew Bitmap(mat->Width, mat->Height, mat->Stride, pixelFormat, mat->Data);
		}
		if (bitmap != nullptr && pixelFormat == PixelFormat::Format8bppIndexed)
		{
			ColorPalette^ GrayColorPalette = bitmap->Palette;
			for (int Index = 0; Index <= Byte::MaxValue; Index++)
			{
				GrayColorPalette->Entries->SetValue(Color::FromArgb(Byte::MaxValue, Index, Index, Index), Index);
			}
			bitmap->Palette = GrayColorPalette;
		}
		return bitmap;
	}
	String^ Elem::toString()
	{
		EXCEPTION_TRANSFORM(return gcnew String(elem->toCString()), FormatException);
	}
	array<Elem^, 1>^ Elem::toArray()
	{
		try
		{
			lzh::ElemList& elemArray = elem->toList();
			array<Elem^, 1>^ elemlist = gcnew array<Elem^, 1>((int)elemArray.size());
			for (int i = 0; i < (int)elemArray.size(); i++) elemlist->SetValue(gcnew Elem(elemArray[i]), i);
			return elemlist;
		}
		_EXCEPTION_TRANSFORM(FormatException);
	}
	System::Collections::Generic::List<Elem^>^ Elem::toList()
	{
		EXCEPTION_TRANSFORM(
		lzh::ElemList& elemArray = elem->toList();
		System::Collections::Generic::List<Elem^>^ elemlist = gcnew System::Collections::Generic::List<Elem^>();
		for (size_t i = 0; i < elemArray.size(); i++) elemlist->Add(gcnew Elem(elemArray[i]));
		return elemlist;
		, FormatException);
	}
	Elem ^ Elem::at(int index)
	{
		if (!elem->isList())throw gcnew System::TypeAccessException();
		if (elem->toList().size() <= index || index < 0)throw gcnew System::IndexOutOfRangeException();
		return gcnew Elem(elem->toList()[index]);
	}
	void Elem::Release()
	{
		if (elem != nullptr) { delete elem; elem = nullptr; }
	}
	lzh::Element * Elem::d_ptr()
	{
		return elem;
	}
	System::IntPtr Elem::ptr()
	{
		return System::IntPtr(elem);
	}
	Elem ^ Elem::Null()
	{ 
		return gcnew Elem(lzh::Element());
	}
	Elem^ Elem::ToElem(bool value){
		Elem^ elem = gcnew Elem();
		*elem->d_ptr() = value;
		return elem;
	}
	Elem^ Elem::ToElem(int value){
		Elem^ elem = gcnew Elem();
		*elem->d_ptr() = value;
		return elem;
	}
	Elem^ Elem::ToElem(double value){
		Elem^ elem = gcnew Elem();
		*elem->d_ptr() = value;
		return elem;
	}
	Elem^ Elem::ToElem(System::String^ str){
		Elem^ elem = gcnew Elem();
		pin_ptr<const wchar_t> wcstr = PtrToStringChars(str);
		*elem->d_ptr() = lzh::WStr2Str(wcstr);
		return elem;
	}
	Elem^ Elem::ToElem(UMat^ image){
		Elem^ elem = gcnew Elem();
		*elem->d_ptr() = image->d_ptr();
		return elem;
	}
	Elem ^ Elem::ToElem(System::Drawing::Bitmap ^ bitmap)
	{
		BitmapData^ bitmapData = bitmap->LockBits(Rectangle(0, 0, bitmap->Width, bitmap->Height), ImageLockMode::ReadWrite, bitmap->PixelFormat);
		UMat^ mat = gcnew UMat(bitmapData->Height, bitmapData->Width, bitmapData->Stride, bitmapData->Scan0);
		bitmap->UnlockBits(bitmapData);
		return ToElem(mat);
	}
	Elem^ Elem::ToElem(Matrix^ matrix){
		Elem^ elem = gcnew Elem();
		*elem->d_ptr() = matrix->d_ptr();
		return elem;
	}

	Elem^ Elem::ToElem(array<bool, 1>^ value){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(value->Length);
		for (int i = 0; i < value->Length; i++) {
			bool val = (bool)value->GetValue(i);
			elemArray[i] = val;
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}
	Elem^ Elem::ToElem(array<int, 1>^ value){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(value->Length);
		for (int i = 0; i < value->Length; i++) {
			int val = (int)value->GetValue(i);
			elemArray[i] = val;
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}
	Elem^ Elem::ToElem(array<double, 1>^ value){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(value->Length);
		for (int i = 0; i < value->Length; i++) {
			double val = (double)value->GetValue(i);
			elemArray[i] = val;
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}
	Elem^ Elem::ToElem(array<char, 1>^ value){
		Elem^ elem = gcnew Elem();
		std::string str;
		for (int i = 0; i < value->Length; i++) {
			char val = (char)value->GetValue(i);
			str.push_back(val);
		}
		*elem->d_ptr() = str;
		return elem;
	}
	Elem^ Elem::ToElem(array<System::String^, 1>^ value){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(value->Length);
		for (int i = 0; i < value->Length; i++) {
			System::String^ val = (System::String^)value->GetValue(i);
			pin_ptr<const wchar_t> wcvalue = PtrToStringChars(val);
			elemArray[i] = lzh::WStr2Str(wcvalue);
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}

	Elem^ Elem::ToElem(System::Collections::Generic::List<int>^ elemList){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(elemList->Count);
		for (int i = 0; i < elemList->Count; i++) {
			elemArray[i] = (*elemList)[i];
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}
	Elem^ Elem::ToElem(System::Collections::Generic::List<double>^ elemList){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(elemList->Count);
		for (int i = 0; i < elemList->Count; i++) {
			elemArray[i] = (*elemList)[i];
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}
	Elem^ Elem::ToElem(System::Collections::Generic::List<System::String^>^ elemList){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(elemList->Count);
		for (int i = 0; i < elemList->Count; i++) {
			pin_ptr<const wchar_t> wcvalue = PtrToStringChars((*elemList)[i]);
			elemArray[i] = lzh::WStr2Str(wcvalue);
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}
	Elem^ Elem::ToElem(System::Collections::Generic::List<Elem^>^ elemList){
		Elem^ elem = gcnew Elem();
		lzh::ElemList elemArray(elemList->Count);
		for (int i = 0; i < elemList->Count; i++) {
			elemArray[i] = *(*elemList)[i]->d_ptr();
		}
		*elem->d_ptr() = elemArray;
		return elem;
	}
	Elem^ Elem::CppElem(System::IntPtr intptr)
	{
		Elem^ elem = gcnew Elem();
		*elem->d_ptr() = *(lzh::Element*)intptr.ToPointer();
		return elem;
	}
	ElemType Elem::Type::get()
	{
		int type = elem->type();
		return (ElemType)type;
	}
}
