#include "Vector.h"

namespace LZH
{
	Vector::Vector(int size, BaseType type) {
		vec = new lzh::Vector(size, (int)type);
	}
	Vector::Vector(int n, System::IntPtr p, BaseType type) {
		vec = new lzh::Vector(n, p.ToPointer(), (int)type, true);
	}
	Vector::Vector(int n, System::IntPtr p, BaseType type, bool deepcopy) {
		vec = new lzh::Vector(n, p.ToPointer(), (int)type, deepcopy);
	}
	Vector::Vector(System::Collections::Generic::List<int>^ list)
	{
		vec = new lzh::Vector(list->Count, (int)BaseType::INT32);
		for (int i = 0; i < list->Count; i++) {
			vec->setv(i, (*list)[i]);
		}
	}
	Vector::Vector(System::Collections::Generic::List<double>^ list)
	{
		vec = new lzh::Vector(list->Count, (int)BaseType::FLOAT64);
		for (int i = 0; i < list->Count; i++) {
			vec->setv(i, (*list)[i]);
		}
	}
	Vector::Vector(System::Collections::Generic::List<Value^>^ list, BaseType type)
	{
		vec = new lzh::Vector(list->Count, (int)type);
		for (int i = 0; i < list->Count; i++) {
			vec->setv(i, (*list)[i]->Double);
		}
	}
	Vector::~Vector() {
		if (vec != nullptr)delete vec;
	}
	bool Vector::Empty()
	{
		return vec->empty();
	}
	int Vector::Length::get() {
		return (int)vec->size();
	}
	BaseType Vector::Type::get() {
		return (BaseType)vec->depth();
	}
	Value ^ Vector::at(int index)
	{
		return gcnew Value(&vec->val(index));
	}
	void Vector::set(int index, Value ^ val)
	{
		vec->setv(index, val->Double);
	}
	System::Collections::Generic::List<Value^>^ Vector::toList()
	{
		System::Collections::Generic::List<Value^>^ list = gcnew System::Collections::Generic::List<Value^>();
		for (size_t i = 0; i < vec->size(); i++) {
			list->Add(gcnew Value(vec->get<double>(i)));
		}
		return list;
	}
}
