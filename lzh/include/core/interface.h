#pragma once
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "mat.h"
#include "array.h"
#include "structure.h"

namespace lzh
{

	template<typename T> struct TEMPLATE_API is_pointer { static const bool value = false; };
	template<typename T> struct TEMPLATE_API is_pointer<T*> { static const bool value = true; };
	template<typename T> TEMPLATE_API void* Ptr(const T &v) {
		return (void*)&v;
	}
	class LZHAPI Object
	{
	public:
		explicit Object() : pre(nullptr), refcount(nullptr) {}
		template<typename _Tp> Object(_Tp v)
		{
			pre = new _Tp(v);
			refcount = new int(1);
		}
		template<typename _Tp> Object(_Tp *v)
		{
			pre = v;
			refcount = nullptr;
		}
		Object(const void* p) : pre((void*)p), refcount(nullptr) {}
		Object(const Object & obj) : pre(nullptr), refcount(nullptr) {
			*this = obj;
		}
		~Object() {
			release();
		}
		void addref() {
			if (refcount)
				(*refcount)++;
		}
		void release() {
			if (refcount && --(*refcount) == 0) {
				FREE_PTR(pre); FREE_PTR(refcount);
			}
			else {
				pre = nullptr; refcount = nullptr;
			}
		}
		bool empty()const { return pre == nullptr; }
		Object& operator = (const Object& obj)
		{
			if (obj.pre == pre)return *this;
			if (obj.refcount)
				++*(obj.refcount);
			release();
			refcount = obj.refcount;
			pre = obj.pre;
			return *this;
		}
		template<typename _Tp> _Tp& as() {
#ifdef LZH_DEBUG
			CHECK_PTR(pre);
#endif // LZH_DEBUG
			return *(_Tp*)pre;
		}
		template<typename _Tp> const _Tp& as() const {
#ifdef LZH_DEBUG
			CHECK_PTR(pre);
#endif // LZH_DEBUG
			return *(_Tp*)pre;
		}
		void** address() { return &pre; }
		const void** address() const { return (const void**)&pre; }
		void* data() { return pre; }
		const void* data()const { return pre; }
		template<typename _Tp> _Tp* ptr() {
			return pre == nullptr ? nullptr : (_Tp*)pre;
		}
		template<typename _Tp> const _Tp* ptr() const {
			return pre == nullptr ? nullptr : (const _Tp*)pre;
		}
		template<typename _Tp> operator _Tp&() {
			return as<_Tp>();
		}
		template<typename _Tp> operator const _Tp&()const {
			return as<_Tp>();
		}
		template<typename _Tp> operator _Tp*() {
			return ptr<_Tp>();
		}
		template<typename _Tp> operator const _Tp*()const {
			return ptr<_Tp>();
		}

		template<typename _Tp> static const void* tran(const _Tp* v)
		{
			return (const void*)v;
		}
		template<typename _Tp> static TEMPLATE_API void addObjList(std::vector<Object> *objectArray, const _Tp & arg) {
			objectArray->push_back(Object(arg));
		}
		template <typename _Tp, typename ... Types> static TEMPLATE_API void addObjList(std::vector<Object> *objectArray, const _Tp & arg1, const Types & ... args) {
			objectArray->push_back(Object(arg1));
			addObjList(&objectArray, args...);
		}
	protected:
		void* pre;
		int* refcount;
	};
	typedef std::vector<Object> ObjectArray;
	template<typename _Tp> TEMPLATE_API ObjectArray Objs(const _Tp & args) {
		ObjectArray objectArray; Object::addObjList(&objectArray, args); return objectArray;
	}
	template<typename ... _Tps> TEMPLATE_API ObjectArray Objs(const _Tps & ... args) {
		ObjectArray objectArray; Object::addObjList(&objectArray, args...); return objectArray;
	}
	class LZHAPI _InputArray
	{
	public:
		enum
		{
			NONE = 0,
			MAT = 1,
			CVMAT = 2,
			STD_VECTOR = 3,
			STD_VECTOR_MAT = 4,
			STD_VECTOR_CVMAT = 5,
			STD_VECTOR_VECTOR = 6,
			VALUE = 7,
			MAT_ITER = 8,
			VEC = 9,
			VECTOR = 10,
		};
		_InputArray() : obj(0) {}
		_InputArray(IMat m);
		_InputArray(const Array &v);
		_InputArray(const Vector &v);
		_InputArray(const vec_mat &v);
		_InputArray(const vec_array&v);
		template<typename _Tp>_InputArray(MatCommaInitializer_<_Tp> &m) {
			init(&m, MAT_ITER, DataType<_Tp>::type);
		}
		//template<typename _Tp> _InputArray(const _Tp &v) {
		//	init(&v, VALUE, DataType<_Tp>::type, Size(1, 1));
		//}
		template<typename _Tp> _InputArray(const std::vector<_Tp> &v) {
			init(&v, STD_VECTOR, DataType<_Tp>::type, Size(1, (int32)v.size()));
		}
		template<typename _Tp, int32 N> _InputArray(const Vec<_Tp, N> &v) {
			init(&v, VEC, DataType<_Tp>::type, Size(1, N));
		}
		template<typename _Tp> _InputArray(const std::vector<std::vector<_Tp>> &v) {
			init(&v, STD_VECTOR_VECTOR, DataType<_Tp>::type, Size(1, (int32)v.size()));
		}

		Array getArray(int32 i = -1)const;
		Mat getMat(int32 i = -1)const;
		Mat getMat_(int32 i = -1)const;
		void release();
		void swap(Mat & dst);
		bool empty()const;
		bool isImage()const;
		bool isMat()const;
		bool isStdVector()const;
		int32 value_type()const;
		int32 kind()const;
		int32 depth()const;
		int32 getTp()const;
		int32 size()const;
		int32 rows()const;
		int32 cols()const;
		int32 channels()const;
		int32 len()const;
		Size getSz() const;
		Size getSz_() const;
		template<typename _Tp> operator _Tp*() {
			return obj;
		}
		template<typename _Tp> operator const _Tp*()const {
			return obj;
		}
		const void * object()const { return obj; }

		bool operator > (const _InputArray &a)const;
		bool operator < (const _InputArray &a)const;
		bool operator == (const _InputArray &a)const;
		bool operator != (const _InputArray &a)const;
	protected:
		void init(const void *obj, int32 flags, int32 type);
		void init(const void *obj, int32 flags, int32 type, Size sz);

		int32 flags;
		int32 type;
		void *obj;
		Size sz;
	};
	typedef const _InputArray& InputArray;
	class LZHAPI _OutputArray : public _InputArray
	{
	public:
		_OutputArray() : _InputArray() {}
		_OutputArray(IMat m) : _InputArray(m) {}
		_OutputArray(const Vector &v) : _InputArray(v) {}
		_OutputArray(const vec_mat &v) : _InputArray(v) {}
		_OutputArray(const Array &m) : _InputArray(m) {}
		_OutputArray(const vec_array &v) : _InputArray(v) {}
		template<typename _Tp> _OutputArray(const std::vector<_Tp> &v) : _InputArray(v) {}
		template<typename _Tp, int32 N> _OutputArray(const Vec<_Tp, N> &v) : _InputArray(v) {}


		void create(Size sz, int32 mtype) const;
		void create(Size3 sz, int32 mtype) const;
		void create(int32 rows, int32 mtype) const;
		void create(int32 rows, int32 cols, int32 mtype) const;
		void create(int32 rows, int32 cols, int32 channels, int32 mtype) const;
		void createSameSize(InputArray arr, int32 mtype = 0) const;
		void release() const;
		void clear() const;
		Mat & getMatRef(int32 i = -1)const;
		Array & getArrayRef(int32 i = -1)const;
		void setMatRef(InputArray input, int32 i = -1)const;
		void set(InputArray input)const;
		void setTo(InputArray input)const;
		void copyTo(InputArray input)const;

		void operator = (InputArray input)const;

	private:
	};
	class LZHAPI _InputOutputArray : public _OutputArray
	{
	public:
		_InputOutputArray() : _OutputArray() {}
		_InputOutputArray(IMat m) : _OutputArray(m) {}
		_InputOutputArray(const Vector &v) : _OutputArray(v) {}
		_InputOutputArray(const vec_mat &v) : _OutputArray(v) {}
		_InputOutputArray(const Array &m) : _OutputArray(m) {}
		_InputOutputArray(const vec_array &v) : _OutputArray(v) {}
		template<typename _Tp> _InputOutputArray(const _Tp &v) : _OutputArray(v) {}
		template<typename _Tp> _InputOutputArray(const std::vector<_Tp> &v) : _OutputArray(v) {}
		template<typename _Tp, int32 N> _InputOutputArray(const Vec<_Tp, N> &v) : _OutputArray(v) {}
	};
	typedef InputArray InputArrayOfArrays;
	typedef const _OutputArray& OutputArray;
	typedef OutputArray OutputArrayOfArrays;
	typedef const _InputOutputArray& InputOutputArray;
	typedef InputOutputArray InputOutputArrayOfArrays; 
	extern LZHAPI InputOutputArray noArray();

	template<class _Tp> class TEMPLATE_API STLApi
	{
	public:
		using _Ty = typename _Tp::value_type;
		using size_type = typename _Tp::size_type;
		using difference_type = typename _Tp::difference_type;
		using iterator = typename _Tp::iterator;
		using const_iterator = typename _Tp::const_iterator;
		using reverse_iterator = typename _Tp::reverse_iterator;
		using const_reverse_iterator = typename _Tp::reverse_iterator;
		using allocator_type = typename _Tp::allocator_type;

		explicit STLApi() : stldata(new _Tp) {}
		STLApi(const _Ty *elem) : stldata(new _Tp(elem)) {}
		STLApi(std::initializer_list<_Ty> _Ilist) : stldata(new _Tp(_Ilist)) {}
		STLApi(const _Tp &api) : stldata(new _Tp(api)) {}
		STLApi(const STLApi& api) : stldata(new _Tp(api)) { *this = api; }
		STLApi(const size_type _Count) : stldata(new _Tp(_Count)) {}
		STLApi(const size_type _Count, const _Ty& _Val) : stldata(new _Tp(_Count, _Val)) {}
		~STLApi() { FREE_PTR(stldata); }
		iterator begin()noexcept { return stldata->begin(); }
		const_iterator begin()const noexcept { return stldata->begin(); }
		iterator end()noexcept { return stldata->end(); }
		const_iterator end()const noexcept { return stldata->end(); }
		iterator erase(const_iterator _Where) { return stldata->erase(_Where); }
		iterator erase(const_iterator _First, const_iterator _Last) { return stldata->erase(_First, _Last); }
		iterator insert(const_iterator _Where, _Ty&& _Val) { return stldata->insert(_Where, _Val); }
		iterator insert(const_iterator _Where, const _Ty& _Val) { return stldata->insert(_Where, _Val); }
		iterator insert(const_iterator _Where, const size_type _Count, const _Ty& _Val) { return stldata->insert(_Where, _Count, _Val); }
		iterator insert(const_iterator _Where, std::initializer_list<_Ty> _Ilist) { return stldata->insert(_Where, _Ilist); }

		void assign(std::initializer_list<_Ty> _Ilist) { stldata->assign(_Ilist); }
		void assign(const size_type _Newsize, const _Ty& _Val) { stldata->assign(_Newsize, _Val); }
		template<class _Iter, class = std::enable_if_t<std::_Is_iterator_v<_Iter>>> void assign(_Iter _First, _Iter _Last) { stldata->assign(_First, _Last); }
		_Ty& at(const size_type _Pos) { return stldata->at(_Pos); }
		const _Ty& at(const size_type _Pos)const { return stldata->at(_Pos); }
		_Ty& back() { return stldata->back(); }
		const _Ty& back() const { return stldata->back(); }
		size_type capacity() const noexcept { return stldata->capacity(); }
		const_iterator cbegin() const noexcept { return stldata->cbegin(); }
		const_iterator crend() const noexcept { return stldata->cbegin(); }
		void clear() noexcept { stldata->clear(); }
		_Ty * data() noexcept { return (_Ty*)stldata->data(); }
		const _Ty * data()const noexcept { return (_Ty*)stldata->data(); }
		bool empty()const noexcept { return stldata->empty(); }
		template<class... _Valty> iterator emplace(const_iterator _Where, _Valty&&... _Val) { return stldata->emplace(_Where, _Val); }
		template<class... _Valty> decltype(auto) emplace_back(_Valty&&... _Val) { return stldata->emplace_back(_Val); }
		_Ty& front() { return stldata->front(); }
		const _Ty& front() const { return stldata->front(); }
		allocator_type get_allocator() const noexcept { return stldata->get_allocator(); }
		size_type max_size() const noexcept { return stldata->max_size(); }
		void pop_back() { stldata->pop_back(); }
		void push_back(_Ty && val) { stldata->push_back(val); }
		void push_back(const _Ty & val) { stldata->push_back(val); }
		reverse_iterator rbegin() noexcept { return stldata->rbegin(); }
		const_reverse_iterator rbegin() const noexcept { return stldata->rbegin(); }
		reverse_iterator rend() noexcept { return stldata->rend(); }
		const_reverse_iterator rend() const noexcept { return stldata->rend(); }
		void resize(const size_type len)noexcept { stldata->resize(len); }
		void reserve(const size_type _Newcapacity) { stldata->reserve(_Newcapacity); }
		void shrink_to_fit() { stldata->shrink_to_fit(); }
		size_type size() const noexcept { return stldata->size(); }
		void swap(_Tp &api)noexcept { stldata->swap(api); }
		void swap(STLApi &api)noexcept { stldata->swap(*(api.stldata)); }

		_Ty& operator [](const size_type idx) { return (*stldata)[idx]; }
		const _Ty& operator [](const size_type idx)const { return (*stldata)[idx]; }

		_Tp& operator *() noexcept { return *stldata; }
		const _Tp& operator * ()const noexcept { return *stldata; }
		_Tp* d_ptr() noexcept { return stldata; }
		const _Tp* d_ptr()const noexcept { return stldata; }
		_Tp& obj() noexcept { return *stldata; }
		const _Tp& obj()const noexcept { return *stldata; }
		operator _Tp& () { return *stldata; }
		operator const _Tp& ()const { return *stldata; }

		_Tp* operator -> ()noexcept { return stldata; }
		const _Tp* operator -> ()const noexcept { return stldata; }

		constexpr bool operator == (const _Tp& api) const noexcept { return *this == api; }
		constexpr bool operator != (const _Tp& api) const noexcept { return *this != api; }
		constexpr bool operator > (const _Tp& api) const noexcept { return *this > api; }
		constexpr bool operator < (const _Tp& api) const noexcept { return *this < api; }
		constexpr bool operator >= (const _Tp& api) const noexcept { return *this >= api; }
		constexpr bool operator <= (const _Tp& api) const noexcept { return *this <= api; }
		constexpr bool operator + (const _Tp& api) const noexcept { return *this + api; }
		STLApi& operator = (const STLApi& api)noexcept {
			if (this == &api)return *this;
			if (stldata != nullptr && api.stldata != nullptr)
				*stldata = *api.stldata;
			return *this;
		}
		STLApi& operator = (const _Tp& api)noexcept {
			if (stldata == &api)return *this;
			if (stldata != nullptr)
				*stldata = api;
			return *this;
		}
	private:
		_Tp *stldata;
	};
}

#endif //__INTERFACE_H__