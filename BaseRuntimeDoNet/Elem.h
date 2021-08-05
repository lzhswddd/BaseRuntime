#ifndef __ELEM_H__
#define __ELEM_H__
#include "UMat.h"
#include "Mat.h"

#define EXCEPTION_TRANSFORM(f, type) do{try{f;}catch(std::exception ex){ throw gcnew type(gcnew System::String(ex.what())); }}while(0)

#define _EXCEPTION_TRANSFORM(type) catch(std::exception ex){ throw gcnew type(gcnew System::String(ex.what())); }

namespace LZH
{
	public enum class ElemType
	{
		NONE = 0,
		BOOL = 1,
		INT32 = 6,
		FLOAT = 10,
		DOUBLE = 11,
		STRING = 55,
		MATRIX = 56,
		IMAGE = 57,
		LIST = 58,
		INTPTR = 59
	};
	public ref class Elem
	{
	public:
		Elem();
		Elem(System::Object^ value);
		Elem(Elem^ value);
		//Elem::Elem(bool value);
		//Elem::Elem(int value);
		//Elem::Elem(double value);
		//Elem::Elem(System::String^ str);
		//Elem::Elem(UMat^ image);
		//Elem::Elem(Matrix^ matrix);

		//Elem::Elem(array<bool, 1>^ value);
		//Elem::Elem(array<int, 1>^ value);
		//Elem::Elem(array<double, 1>^ value);
		//Elem::Elem(array<char, 1>^ value);
		//Elem::Elem(array<System::String^, 1>^ value);

		//Elem::Elem(System::Collections::Generic::List<int>^ elemList);
		//Elem::Elem(System::Collections::Generic::List<double>^ elemList);
		//Elem::Elem(System::Collections::Generic::List<System::String^>^ elemList);
		//Elem::Elem(System::Collections::Generic::List<Elem^>^ elemList);
		Elem(const lzh::Element & elem);
		Elem(const lzh::ElemList & elem);
		~Elem();

		bool Empty();
		bool IsIntPtr();
		bool IsBool();
		bool IsInt();
		bool IsFloat();
		bool IsDouble();
		bool IsNumber();
		bool IsMatrix();
		bool IsImage();
		bool IsString();
		bool IsArray();
		bool IsList();

		System::Object^ toObject();
		System::IntPtr toIntPtr();
		bool toBool();
		int toInt();
		float toFloat();
		double toDouble();
		double toNumber();
		Matrix^ toMatrix();
		UMat^ toImage();
		System::Drawing::Bitmap^ toBitmap();
		System::String^ toString();
		array<Elem^, 1>^ toArray();
		System::Collections::Generic::List<Elem^>^ toList();
		Elem^ at(int index);
		void Release();

		property ElemType Type
		{
			ElemType get();
		}
		lzh::Element *d_ptr();
		System::IntPtr ptr();

		static Elem^ Null();
		static Elem^ ToElem(bool value);
		static Elem^ ToElem(int value);
		static Elem^ ToElem(double value);
		static Elem^ ToElem(System::String^ str);
		static Elem^ ToElem(UMat^ image);
		static Elem^ ToElem(System::Drawing::Bitmap^ image);
		static Elem^ ToElem(Matrix^ matrix);

		static Elem^ ToElem(array<bool, 1>^ value);
		static Elem^ ToElem(array<int, 1>^ value);
		static Elem^ ToElem(array<double, 1>^ value);
		static Elem^ ToElem(array<char, 1>^ value);
		static Elem^ ToElem(array<System::String^, 1>^ value);

		static Elem^ ToElem(System::Collections::Generic::List<int>^ elemList);
		static Elem^ ToElem(System::Collections::Generic::List<double>^ elemList);
		static Elem^ ToElem(System::Collections::Generic::List<System::String^>^ elemList);
		static Elem^ ToElem(System::Collections::Generic::List<Elem^>^ elemList);

		static Elem^ CppElem(System::IntPtr intptr);
	private:
		lzh::Element *elem;
	};
};
#endif // !__ELEM_H__