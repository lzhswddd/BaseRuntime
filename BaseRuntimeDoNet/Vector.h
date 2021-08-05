#pragma once
#include "Value.h"

namespace LZH
{
	public ref class Vector
	{
	public:
		Vector(int size, BaseType type);
		Vector(int n, System::IntPtr p, BaseType type);
		Vector(int n, System::IntPtr p, BaseType type, bool deepcopy);
		Vector(System::Collections::Generic::List<int>^ list);
		Vector(System::Collections::Generic::List<double>^ list);
		Vector(System::Collections::Generic::List<Value^>^ list, BaseType type);
		~Vector();

		bool Empty();

		property int Length
		{
			int get();
		}
		property BaseType Type
		{
			BaseType get();
		}

		Value^ at(int index);
		void set(int index, Value^ val);

		System::Collections::Generic::List<Value^>^ toList();
	private:
		lzh::Vector *vec;
	};
} 
