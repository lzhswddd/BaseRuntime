#pragma once
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "element.h"
#include "pointer.h"
#include <map>
#include <sstream>
#include <functional>

namespace lzh
{
	template<typename _Tp, typename _Item> using Dict = typename std::map<_Tp, _Item>;
	template<typename _Tp> class TEMPLATE_API DictItem
	{
	public:
		using iterator = typename Dict<_Tp, DictItem<_Tp>>::iterator;
		using const_iterator = typename Dict<_Tp, DictItem<_Tp>>::const_iterator;

		DictItem() : _obj(), node(nullptr) {}
		DictItem(const Element &item) : _obj(item), node(nullptr){}
		~DictItem() {}
		DictItem(const DictItem<_Tp> &item) : _obj(), node(nullptr) { *this = item; }
		//DictItem(DictItem<_Tp> &&item) : _obj(), node(nullptr) { *this = item; }
		DictItem& operator = (const DictItem<_Tp> &item);
		DictItem& operator = (const Element &item);
		DictItem& operator = (const ElemList &item);

		bool empty()const { return _obj.Data().empty(); }
		size_t size()const {
			if (isParent())return dict().size();
			else return (size_t)_obj.Data().len();
		}

		iterator begin();
		const_iterator begin()const;
		iterator end();
		const_iterator end()const;
		iterator find(const _Tp & key);
		const_iterator find(const _Tp & key)const;

		bool isParent()const noexcept { return node.empty() ? false : node.Data() != nullptr; }
		template<typename _Ty> _Ty& Data()const {
			return _obj.Data().toData<_Ty>();
		}
		DictItem & at(const _Tp & key);
		const DictItem & at(const _Tp & key)const;
		DictItem &operator [](const _Tp & key);
		template<typename _Ty> operator _Ty& () {
			return _obj.Data();
		}
		template<typename _Ty> operator const _Ty& () const {
			return _obj.Data();
		}
		
		template<typename _Ty> bool operator == (const _Ty & elem)const {
			return _obj.Data() == elem;
		}
		template<typename _Ty> bool operator != (const _Ty & elem)const {
			return _obj.Data() != elem;
		}
		template<typename _Ty> bool operator > (const _Ty & elem)const {
			return _obj.Data() > elem;
		}
		template<typename _Ty> bool operator >= (const _Ty & elem)const {
			return _obj.Data() >= elem;
		}
		template<typename _Ty> bool operator <= (const _Ty & elem)const {
			return _obj.Data() <= elem;
		}
		template<typename _Ty> bool operator < (const _Ty & elem)const {
			return _obj.Data() < elem;
		}

		void GenDict() {
			if (node.Data() == nullptr)
			{
				node.Data() = new Dict<_Tp, DictItem<_Tp>>();
				if (!_obj.empty())_obj.over(node.Data());
				else _obj.create(node.Data());
			}
		}

		Element& Elem() { return _obj.Data(); }
		const Element& Elem()const { return _obj.Data(); }
		bool Bool()const { return _obj.Data().tobool(); }
		int Int()const { return _obj.Data().toInt(); }
		float Float()const { return _obj.Data().toFloat(); }
		double Double()const { return _obj.Data().toDouble(); }
		Value Val()const { return _obj.Data().toValue(); }
		std::string String()const { return _obj.Data().toString(); }
		ElemList List()const { return _obj.Data().toList(); }

		Dict<_Tp, DictItem<_Tp>> & dict();
		const Dict<_Tp, DictItem<_Tp>> & dict()const;
		Dict<_Tp, DictItem<_Tp>> * d_ptr() { return node.Data(); }
		const Dict<_Tp, DictItem<_Tp>> * d_ptr()const { return node.Data(); }

		Element * operator ->() { return _obj.Ptr(); }
		const Element * operator ->()const { return _obj.Ptr();}
		Element & operator *() { return Elem(); }
		const Element & operator *()const { return Elem(); }
	private:
		Pointer<Dict<_Tp, DictItem<_Tp>>*> node;
		Pointer<Element> _obj;
	};
	template<typename _Tp> using Dir = Dict<_Tp, DictItem<_Tp>>;
	template<typename _Tp> class TEMPLATE_API Container
	{
	public:
		using item = typename DictItem<_Tp>;
		using const_item = typename const DictItem<_Tp>;
		using iterator = typename Dict<_Tp, DictItem<_Tp>>::iterator;
		using const_iterator = typename Dict<_Tp, DictItem<_Tp>>::const_iterator;
		using pair = typename std::pair<const _Tp, DictItem<_Tp>>;
		using const_pair = typename const std::pair<const _Tp, const DictItem<_Tp>>;
		Container() : data() {}
		~Container() {
			if (data.last()) {
				for (std::pair<const _Tp, lzh::DictItem<_Tp>> & iter : data.Data()) {
					if (iter.second.isParent()) {
						Release(iter.second.dict());
						delete iter.second.d_ptr();
					}
				}
			}
		}
		static void Release(lzh::Dict<_Tp, lzh::DictItem<_Tp>> &dict) {
			for (std::pair<const _Tp, DictItem<_Tp>> & iter : dict) {
				if (iter.second.isParent()) {
					Release(iter.second.dict());
					delete iter.second.d_ptr();
				}
			}
		}
		bool empty() const { return data.Data().empty(); }
		iterator find(const _Tp &key) { return data.Data().find(key); }
		const_iterator find(const _Tp &key) const { return data.Data().find(key); }
		iterator begin() { return data.Data().begin(); }
		const_iterator begin()const { return data.Data().begin(); }
		iterator end() { return data.Data().end(); }
		const_iterator end()const { return data.Data().end(); }
		DictItem<_Tp>& at(_Tp key) {
			return data.Data().at(key);
		}
		const DictItem<_Tp>& at(_Tp key)const {
			return data.Data().at(key);
		}
		DictItem<_Tp>& operator [](_Tp key) {
			return data.Data()[key];
		}
	protected:
		Pointer<Dict<_Tp, DictItem<_Tp>>> data;
	};
	//template<typename _Tp> class TEMPLATE_API Dict : public std::map<_Tp, DictItem<_Tp>>
	//{
	//public:
	//	using pair = typename std::pair<_Tp, DictItem<_Tp>>;
	//	using const_pair = typename const std::pair<const _Tp, DictItem<_Tp>>;
	////	DictItem<_Tp> &at(const _Tp &key) {
	////		return std::map<_Tp, DictItem<_Tp>>::at(key);
	////	}
	////	const DictItem<_Tp> &at(const _Tp &key)const {
	////		return std::map<_Tp, DictItem<_Tp>>::at(key);
	////	}
	////	DictItem<_Tp> &operator [](const _Tp &key) {
	////		return std::map<_Tp, DictItem<_Tp>>::operator[](key);
	////	}
	//public:
	//	static void ShowNode(Dict<_Tp>::const_pair& iter, Element obj) {
	//		static std::string prefix = "";
	//		if (obj.empty()) { prefix = ""; return; }
	//		if (iter.second.isParent()) {
	//			std::stringstream str;
	//			str << "[" << iter.first << "]";
	//			prefix += str.str();
	//		}
	//		else {
	//			obj.toData<std::ostream>() << prefix << "[" << iter.first << "] = " << iter.second.object() << std::endl;
	//		}
	//	}
	template<typename _Tp> static void Ergodic(const Dir<_Tp>& dict, Element obj, std::function<void(std::pair<_Tp, DictItem<_Tp>>&, Element)> && callback) {
		for (std::pair<_Tp, DictItem<_Tp>> & iter : dict) {
			callback(iter, obj);
			if (iter.second.isParent()) {
				Ergodic(iter.second.dict(), obj, callback);
				callback(std::pair<_Tp, DictItem<_Tp>>(), Element());
			}
		}
	}
	//};
	//template<typename _Tp> inline std::ostream & operator << (std::ostream & out, const Dict<_Tp> &dict) {
	//	Dict<_Tp>::Ergodic(dict, out, Dict<_Tp>::ShowNode); return out;
	//}
	template<typename _Tp> inline typename DictItem<_Tp>::iterator DictItem<_Tp>::begin() {
		if (isParent()) { return dict().begin(); }
		else return DictItem<_Tp>::iterator();
	}
	template<typename _Tp> inline typename DictItem<_Tp>::const_iterator DictItem<_Tp>::begin() const {
		if (isParent()) { return dict().begin(); }
		else return DictItem<_Tp>::iterator();
	}
	template<typename _Tp> inline typename DictItem<_Tp>::iterator DictItem<_Tp>::end() {
		if (isParent()) { return dict().end(); }
		else return DictItem<_Tp>::iterator();
	}
	template<typename _Tp> inline typename DictItem<_Tp>::const_iterator DictItem<_Tp>::end() const {
		if (isParent()) { return dict().end(); }
		else return DictItem<_Tp>::iterator();
	}
	template<typename _Tp> inline typename DictItem<_Tp>::iterator DictItem<_Tp>::find(const _Tp & key) {
		if (isParent()) { return dict().find(key); }
		else return DictItem<_Tp>::iterator();
	}
	template<typename _Tp> inline typename DictItem<_Tp>::const_iterator DictItem<_Tp>::find(const _Tp & key) const {
		if (isParent()) { return dict().find(key); }
		else return DictItem<_Tp>::iterator();
	}
	template<typename _Tp> inline DictItem<_Tp> & DictItem<_Tp>::operator=(const DictItem<_Tp>& item)
	{
		if (this == &item)return *this;
	    node = item.node;
		_obj = item._obj;
		return *this;
	}
	template<typename _Tp> inline DictItem<_Tp> & DictItem<_Tp>::operator=(const Element & item)
	{
		if ((void*)this == (void*)&item)return *this;
		if (!node.empty())node.release();
		if (!_obj.empty())_obj.over(item);
		else _obj.create(item);
		return *this;
	}
	template<typename _Tp> inline DictItem<_Tp> & DictItem<_Tp>::operator=(const ElemList & items)
	{
		if ((void*)this == (void*)&items)return *this;
		if (!node.empty())node.release();
		if (!_obj.empty())_obj.over(items);
		else _obj.create(items);
		return *this;
	}
	template<typename _Tp> inline DictItem<_Tp> & DictItem<_Tp>::at(const _Tp & key) {
		return _obj.Data().toData<Dict<_Tp, DictItem<_Tp>>>().at(key);
	}
	template<typename _Tp> inline const DictItem<_Tp> & DictItem<_Tp>::at(const _Tp & key)const {
		return _obj.Data().toData<Dict<_Tp, DictItem<_Tp>>>().at(key);
	}
	template<typename _Tp> inline DictItem<_Tp> & DictItem<_Tp>::operator[](const _Tp & key)
	{
		if (node.Data() == nullptr)
			GenDict();
		return _obj.Data().toData<Dict<_Tp, DictItem<_Tp>>>()[key];
	}
	template<typename _Tp> inline Dict<_Tp, DictItem<_Tp>>& DictItem<_Tp>::dict()
	{
		return (_obj.Data().toData<Dict<_Tp, DictItem<_Tp>>>());
	}
	template<typename _Tp> inline const Dict<_Tp, DictItem<_Tp>>& DictItem<_Tp>::dict() const
	{
		return (_obj.Data().toData<Dict<_Tp, DictItem<_Tp>>>());
	}
	template<typename _Tp> std::ostream & operator << (std::ostream &out, const DictItem<_Tp> &item)
	{
		out << item.Elem(); return out;
	}
}

#endif // !__DICTIONARY_H__