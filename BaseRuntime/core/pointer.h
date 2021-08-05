#ifndef __POINTER_H__
#define __POINTER_H__

#include "core.h"

namespace lzh
{
	class LZHAPI RefCountPointer
	{
	public:
		RefCountPointer();
		RefCountPointer(const RefCountPointer &p);
		RefCountPointer& operator = (const RefCountPointer &p);
		~RefCountPointer();

		void create();
		void addref();
		void release();
		void assign(int *refcount);

		bool last()const;
	protected:
		int *refcount;
	};

	template<typename _Tp> struct TEMPLATE_API PointerInterFace
	{
		using value_type = typename _Tp;
		bool Custom() { return false; }
		_Tp* Create() { return new _Tp(); }
		template<typename ...Args> _Tp* Create(const Args & ... argv) { return new _Tp(argv...); }
		void Release(_Tp* ptr) { FREE_PTR(ptr); }
	};

	template<typename _Tp> class TEMPLATE_API Pointer : public RefCountPointer
	{
	public:
		Pointer(bool realization = true);
		Pointer(_Tp &val);
		Pointer(const _Tp &val);
		Pointer(_Tp *val);
		Pointer(_Tp* val, bool realization);
		Pointer(const Pointer &p);
		Pointer& operator = (const Pointer &p);
		template<typename ..._Ty> Pointer(const _Ty & ...param);
		~Pointer();

		bool empty()const;

		void over(const _Tp &val);
		void create()noexcept;
		void create(_Tp &val)noexcept;
		void create(const _Tp &val)noexcept;
		//template<typename ..._Ty> void create(const _Ty & ...param);
		void release()noexcept;

		_Tp *Ptr();
		const _Tp *Ptr()const;
		_Tp &Data();
		const _Tp &Data()const;
		operator _Tp &();
		operator const _Tp &()const;
		operator _Tp *();
		operator const _Tp *()const;
		
		_Tp & operator *();
		const _Tp & operator *()const;

		_Tp * operator ->();
		const _Tp * operator ->()const;

		_Tp *& obj();

		static Pointer<_Tp> Quote(_Tp* ptr);
	protected:
		_Tp *data;
	};
	template<typename _Tp> TEMPLATE_API Pointer<_Tp> AllocNone();
	template<typename _Tp, typename ..._Ty> TEMPLATE_API Pointer<_Tp> AllocArgv(const _Ty & ...param);

	//template<typename _Tp> inline Pointer<_Tp>::Pointer() : data(nullptr), RefCountPointer() {}
	//template<typename _Tp> inline Pointer<_Tp>::Pointer(const _Tp & val) : data(nullptr), RefCountPointer() { create(val); }
	template<typename _Tp> inline Pointer<_Tp>::Pointer(bool realization) : data(nullptr), RefCountPointer() { if (realization)create(); }
	template<typename _Tp> inline Pointer<_Tp>::Pointer(_Tp & val) : data(nullptr), RefCountPointer() { create(val); }
	template<typename _Tp> inline Pointer<_Tp>::Pointer(const _Tp & val) : data(nullptr), RefCountPointer() { create(val); }
	template<typename _Tp> inline Pointer<_Tp>::Pointer(_Tp * val) : data(val), RefCountPointer() { RefCountPointer::create(); }
	template<typename _Tp> inline Pointer<_Tp>::Pointer(_Tp * val, bool realization) : data(val), RefCountPointer() { if (realization)RefCountPointer::create(); }
	template<typename _Tp> inline Pointer<_Tp>::Pointer(const Pointer<_Tp> & p) : data(nullptr), RefCountPointer() { *this = p; }
	template<typename _Tp> inline Pointer<_Tp> & Pointer<_Tp>::operator=(const Pointer<_Tp> & p)
	{
		if (this == &p)return *this;
		if (last())
			delete data;
		assign(p.refcount);
		data = p.data;
		return *this;
	}
	template<typename _Tp> template<typename ..._Ty> inline Pointer<_Tp>::Pointer(const _Ty & ...param)
		: data(nullptr), RefCountPointer() 
	{
		release(); 
		data = PointerInterFace<_Tp>().Create(param...);
		RefCountPointer::create();
	}
	template<typename _Tp> inline Pointer<_Tp>::~Pointer() { release(); }
	template<typename _Tp> inline bool Pointer<_Tp>::empty() const {
		return data == nullptr;
	}
	template<typename _Tp> inline void Pointer<_Tp>::over(const _Tp &val) {
		if (data)*data = val;
	}
	template<typename _Tp> inline void Pointer<_Tp>::create() noexcept {
		release(); data = new _Tp; RefCountPointer::create();
	}
	template<typename _Tp> inline void Pointer<_Tp>::create(_Tp & val) noexcept {
		release(); data = new _Tp(val); RefCountPointer::create();
	}
	template<typename _Tp> inline void Pointer<_Tp>::create(const _Tp & val) noexcept {
		release(); data = new _Tp(val); RefCountPointer::create();
	}
	//template<typename _Tp> template<typename ..._Ty> inline void Pointer<_Tp>::create(const _Ty & ...param) {
	//	release(); data = new _Tp(param...); RefCountPointer::create();
	//}
	template<typename _Tp> inline void Pointer<_Tp>::release() noexcept {
		if (last())PointerInterFace<_Tp>().Release(data); RefCountPointer::release(); data = nullptr;
	}
	template<typename _Tp> inline _Tp * Pointer<_Tp>::Ptr() {
		return data;
	}
	template<typename _Tp> inline const _Tp * Pointer<_Tp>::Ptr() const {
		return data;
	}
	template<typename _Tp> inline _Tp & Pointer<_Tp>::Data() {
		return *data;
	}
	template<typename _Tp> inline const _Tp & Pointer<_Tp>::Data() const {
		return *data;
	}
	template<typename _Tp> inline Pointer<_Tp>::operator _Tp&() {
		return Data();
	}
	template<typename _Tp> inline Pointer<_Tp>::operator const _Tp&() const {
		return Data();
	}
	template<typename _Tp> inline Pointer<_Tp>::operator _Tp*() {
		return data;
	}
	template<typename _Tp> inline Pointer<_Tp>::operator const _Tp*() const {
		return data;
	}
	template<typename _Tp> inline _Tp & Pointer<_Tp>::operator *() {
		return *data;
	}
	template<typename _Tp> inline const _Tp & Pointer<_Tp>::operator *()const {
		return *data;
	}
	template<typename _Tp> inline _Tp * Pointer<_Tp>::operator ->() {
		return data;
	}
	template<typename _Tp> inline const _Tp * Pointer<_Tp>::operator ->()const {
		return data;
	}
	template<typename _Tp> inline _Tp *& Pointer<_Tp>::obj() {
		return data;
	}
	template<typename _Tp> inline Pointer<_Tp> Pointer<_Tp>::Quote(_Tp* ptr) {
		return Pointer<_Tp>(ptr, false);
	}

	template<typename _Tp> class TEMPLATE_API PtrArray : public RefCountPointer
	{
	public:
		using iterator = typename _Tp*;
		using const_iterator = typename const _Tp*;
		PtrArray() : RefCountPointer(), arr(nullptr), len(0) { }
		PtrArray(size_t size) : RefCountPointer() { create(size); }
		PtrArray(const PtrArray &parr) : RefCountPointer() { *this = parr; }
		PtrArray& operator = (const PtrArray &p) {
			if (this == &p)return *this;
			if (last())
				fastFree(arr);
			RefCountPointer::assign(p.refcount);
			arr = p.arr;
			len = p.len;
			return *this;
		}
		~PtrArray() { release(); }
		void create(size_t size) { release(); arr = new _Tp[size]; len = size; RefCountPointer::create(); }
		void release() { if (last())delete[] (arr); RefCountPointer::release(); arr = nullptr; }
		void assign(size_t size, const _Tp * val) { release(); arr = (_Tp *)val; len = size; }
		void assign(const _Tp & val, size_t index) { arr[index] = val; }
		void memcopy(const _Tp * ptr) { memcpy(arr, ptr, sizeof(_Tp)*len); }
		void swap(PtrArray & arr) { std::swap(*this, arr); }

		bool empty()const noexcept { return arr == nullptr; }
		size_t size()const noexcept { return len; }

		PtrArray add() {
#ifdef LZH_DEBUG
			CHECK_INDEX(1, len);
#endif
			PtrArray parr;
			parr.arr = arr + 1;
			parr.len = len - 1;
			return parr;
		}
		PtrArray offset(size_t off) { 
#ifdef LZH_DEBUG
			CHECK_INDEX(off, len);
#endif
			PtrArray parr;
			parr.arr = arr + off;
			parr.len = len - off; 
			return parr;
		}

		iterator begin() { return arr; }
		const_iterator begin()const { return arr; }
		iterator end() { return arr + len; }
		const_iterator end()const { return arr + len; }
		iterator find(const _Tp & val) { return std::find(begin(), end(), val); }
		const_iterator find(const _Tp & val)const { return std::find(begin(), end(), val); }

		_Tp &at(size_t index) { return arr[index]; }
		const _Tp &at(size_t index)const { return arr[index]; }
		_Tp &operator()(size_t index) { return at(index); }
		const _Tp &operator()(size_t index)const { return at(index); }
		_Tp *ptr(size_t index) { return arr + index; }
		const _Tp *ptr(size_t index)const { return arr + index; }

		operator _Tp * () { return arr; }
		operator const _Tp * ()const { return arr; }

		_Tp * operator ->() { return arr; }
		const _Tp * operator ->()const { return arr; }
	protected:
		size_t len;
		_Tp *arr;
	};

	template<typename _Tp> inline Pointer<_Tp> AllocNone()
	{
		return PointerInterFace<_Tp>().Create();
	}

	template<typename _Tp, typename ..._Ty> inline Pointer<_Tp> AllocArgv(const _Ty & ...param)
	{
		return PointerInterFace<_Tp>().Create(param...);
	}

	/*template<> class TEMPLATE_API Pointer<void> : public RefCountPointer
	{
	public:
		Pointer(bool realization = true);
		Pointer(_Tp& val);
		Pointer(const _Tp& val);
		Pointer(_Tp* val);
		Pointer(_Tp* val, bool realization);
		Pointer(const Pointer& p);
		Pointer& operator = (const Pointer& p);
		template<typename ..._Ty> Pointer(const _Ty & ...param);
		~Pointer();

		bool empty()const;

		void over(const _Tp& val);
		void create()noexcept;
		template<typename ..._Ty> void create(const _Ty & ...param);
		void release()noexcept;

		void* Ptr();
		const void* Ptr()const;
		operator void* ();
		operator const void* ()const;

		void*& obj();

		static Pointer<void> Quote(void* ptr);
	protected:
		void* data;
	};*/
}
#endif // !
