#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <vector>
#include "core.h"
#include "point.h"

namespace lzh
{
	template<class _Tp, int32 N>
	class TEMPLATE_API Vec
	{
	public:
        using value_type = _Tp;

		explicit Vec();
		Vec(const _Tp *v);
		Vec(const MatPtr<_Tp> & m);
		template<typename T2_, int32 N1> Vec(const Vec<T2_, N1> & m);
		template<typename ... Types> Vec(const Types & ... args);
		template<typename _Ty> Vec(std::initializer_list<_Ty> & _List);
		_Tp & at(int32 i);
		const _Tp & at(int32 i)const;
		_Tp & operator [](int32 i);
		const _Tp & operator [](int32 i)const;
		_Tp & operator ()(int32 i);
		const _Tp & operator ()(int32 i)const;
		int32 size()const;
		_Tp *begin();
		const _Tp *begin() const;
		_Tp *end();
		const _Tp *end() const;

		Vec<_Tp, N> t()const;
		//Vec<_Tp, N> operator = (const Vec<_Tp, N> &vec);
		//Vec<_Tp, N>& operator = (int32 tab);
		template<class _T2, int32 N1> Vec<_Tp, N>& operator = (const Vec<_T2, N1> &vec);
		template <typename T> void addvalue(int32 i, const T & arg);
		template <typename T, typename ... Types> void addvalue(int32 i, const T & arg1, const Types & ... args);
		_Tp data[N];
		int32 h = 1;
		int32 w = N;
	};
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator+(Val_ v, const Vec<_Tp, N> &vector);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator-(Val_ v, const Vec<_Tp, N> &vector);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator*(Val_ v, const Vec<_Tp, N> &vector);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator/(Val_ v, const Vec<_Tp, N> &vector);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator+(const Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator-(const Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator*(const Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator/(const Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator+(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator-(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator*(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator/(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator+=(Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator-=(Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator*=(Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator/=(Vec<_Tp, N> &vector, Val_ v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator+=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator-=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator*=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);
	template<class _Tp, int32 N> TEMPLATE_API Vec<_Tp, N> operator/=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v);


	/**
	内存管理类
	*/
	class LZHAPI Vector
	{
	public:
		Vector();
		Vector(msize size, int32 type);
		Vector(const Vector &m);
		Vector(msize n, const void *p, int32 type, bool deepcopy = false);
		Vector(msize n, const void *p, int32 type, int32* refcount);
		template<typename _Ty> Vector(std::initializer_list<_Ty> _Ilist);
		~Vector();
		void create(msize len, int32 type);
		void addref();
		void release();
		Vector clone()const;
		Vector copy()const;

		void save(FILE* file)const;
		void load(FILE* file);

		template<typename _Tp> _Tp* begin();
		template<typename _Tp> const _Tp* begin()const;
		template<typename _Tp> _Tp* end();
		template<typename _Tp> const _Tp* end()const;
		template<typename _Tp> _Tp& at(msize i);
		template<typename _Tp> const _Tp& at(msize i)const;
		bool operator == (void *m) const;
		bool operator != (void *m) const;
		template<typename _Tp> _Tp& operator [] (msize i) const;
		template<typename _Tp> operator _Tp* ();
		template<typename _Tp> operator const _Tp* ()const;
		//void* operator + (int32 i)const;
		//void* operator - (int32 i)const;
		template<typename _Tp> _Tp get(msize i) const;
		Value val(msize i) const;
		mat_t operator ()(msize i)const;

		template<typename _Tp> _Tp ptr();
		template<typename _Tp> const _Tp ptr()const;
		template<typename _Tp> void setv(msize i, _Tp v);
		template<typename _Tp> void add(msize i, _Tp v);
		template<typename _Tp> void sub(msize i, _Tp v);
		template<typename _Tp> void mul(msize i, _Tp v);
		template<typename _Tp> void div(msize i, _Tp v);

		bool empty()const;
		int32 depth()const;
		int32 getTp()const;
		msize size()const;
		int32 &T();
		msize &L();
		void* &P();
		void* obj()const;
		void* index(msize i)const;
		void* add(msize i)const;
		void* reduce(msize i)const;
		void* offset(msize i)const;
		int32* memCount()const;

		void set(const Vector &ptr);
		void set(const void* p, msize length, int32 data_type);
		void set(const void* p, msize length, int32 data_type, const int32 *count);
		Vector & operator=(const Vector & m);
		void* operator ()();
		const void* operator ()()const;
		void show()const;
		std::ostream& show(std::ostream & out)const;
	protected:
		msize len;
		void *data;
		int32 type;
		void init(const void* p, int32 type, msize len, const int32 *count);
	private:
		int32 *refcount = 0;
	};
	Vector operator + (Val v, const Vector &vec);
	Vector operator + (const Vector &vec, Val v);
	Vector operator + (const Vector &v1, const Vector &v2);
	Vector operator - (Val v, const Vector &vec);
	Vector operator - (const Vector &vec, Val v);
	Vector operator - (const Vector &v1, const Vector &v2);
	Vector operator * (Val v, const Vector &vec);
	Vector operator * (const Vector &vec, Val v);
	Vector operator * (const Vector &v1, const Vector &v2);
	Vector operator / (Val v, const Vector &vec);
	Vector operator / (const Vector &vec, Val v);
	Vector operator / (const Vector &v1, const Vector &v2);

	Vector& operator += (Vector &v1, Val v);
	Vector& operator += (Vector &v1, const Vector &v2);
	Vector& operator -= (Vector &v1, Val v);
	Vector& operator -= (Vector &v1, const Vector &v2);
	Vector& operator *= (Vector &v1, Val v);
	Vector& operator *= (Vector &v1, const Vector &v2);
	Vector& operator /= (Vector &v1, Val v);
	Vector& operator /= (Vector &v1, const Vector &v2);

	template<typename _Tp, int32 N> class DataType<Vec<_Tp, N>>
	{
	public:
		typedef Vec<_Tp, N> value_type;
		enum { type = MAT_CREATETYPE(N) + DataType<_Tp>::type };
		const int8 * type_name = ("Vec<" + std::string(DataType<_Tp>().type_name) + std::to_string(N) + ">").c_str();
	};

	//extern std::string DataTypeName(int32 flags);

	typedef Vec<int8, 2>		Vec2c;
	typedef Vec<uint8, 2>		Vec2b;
	typedef Vec<int32, 2>		Vec2i;
	typedef Vec<float32, 2>		Vec2f;
	typedef Vec<float64, 2>		Vec2d;
	typedef Vec<mat_t, 2>		Vec2m;
	typedef Vec<float128, 2>	Vec2ld;
	typedef Vec<int8, 3>		Vec3c;
	typedef Vec<uint8, 3>		Vec3b;
	typedef Vec<int32, 3>		Vec3i;
	typedef Vec<float32, 3>		Vec3f;
	typedef Vec<float64, 3>		Vec3d;
	typedef Vec<mat_t, 3>		Vec3m;
	typedef Vec<float128, 3>	Vec3ld;
	typedef Vec<int8, 4>		Vec4c;
	typedef Vec<uint8, 4>		Vec4b;
	typedef Vec<int32, 4>		Vec4i;
	typedef Vec<float32, 4>		Vec4f;
	typedef Vec<float64, 4>		Vec4d;
	typedef Vec<mat_t, 4>		Vec4m;
	typedef Vec<float128, 4>	Vec4ld;

	typedef std::vector<lzh::int8>									vec_8s;
	typedef std::vector<lzh::uint8>									vec_8b;
	typedef std::vector<lzh::int16>									vec_16s;
	typedef std::vector<lzh::uint16>								vec_16u;
	typedef std::vector<lzh::int32>									vec_32s;
	typedef std::vector<lzh::uint32>								vec_32u;
	typedef std::vector<lzh::float32>								vec_32f;
	typedef std::vector<lzh::int64>									vec_64s;
	typedef std::vector<lzh::uint64>								vec_64u;
	typedef std::vector<lzh::float64>								vec_64f;
	typedef std::vector<lzh::float128>								vec_64d;
	typedef std::vector<lzh::mat_t>									vec_m;
	typedef std::vector<std::vector<lzh::int8>>						vecs_8s;
	typedef std::vector<std::vector<lzh::uint8>>					vecs_8b;
	typedef std::vector<std::vector<lzh::int16>>					vecs_16s;
	typedef std::vector<std::vector<lzh::uint16>>					vecs_16u;
	typedef std::vector<std::vector<lzh::int32>>					vecs_32s;
	typedef std::vector<std::vector<lzh::uint32>>					vecs_32u;
	typedef std::vector<std::vector<lzh::float32>>					vecs_32f;
	typedef std::vector<std::vector<lzh::int64>>					vecs_64s;
	typedef std::vector<std::vector<lzh::uint64>>					vecs_64u;
	typedef std::vector<std::vector<lzh::float64>>					vecs_64f;
	typedef std::vector<std::vector<lzh::float128>>					vecs_64d;
	typedef std::vector<std::vector<lzh::mat_t>>					vecs_m;
	typedef std::vector<lzh::Point2_<lzh::int8>>					vec_p2_8s;
	typedef std::vector<lzh::Point2_<lzh::uint8>>					vec_p2_8b;
	typedef std::vector<lzh::Point2_<lzh::int16>>					vec_p2_16s;
	typedef std::vector<lzh::Point2_<lzh::uint16>>					vec_p2_16u;
	typedef std::vector<lzh::Point2_<lzh::int32>>					vec_p2_32s;
	typedef std::vector<lzh::Point2_<lzh::uint32>>					vec_p2_32u;
	typedef std::vector<lzh::Point2_<lzh::int64>>					vec_p2_64s;
	typedef std::vector<lzh::Point2_<lzh::uint64>>					vec_p2_64u;
	typedef std::vector<lzh::Point2_<lzh::float32>>					vec_p2_32f;
	typedef std::vector<lzh::Point2_<lzh::float64>>					vec_p2_64f;
	typedef std::vector<lzh::Point2_<lzh::float128>>				vec_p2_64d;
	typedef std::vector<lzh::Point2_<lzh::mat_t>>					vec_p2_m;
	typedef std::vector<std::vector<lzh::Point2_<lzh::int8>>>		vecs_p2_8s;
	typedef std::vector<std::vector<lzh::Point2_<lzh::uint8>>>		vecs_p2_8b;
	typedef std::vector<std::vector<lzh::Point2_<lzh::int16>>>		vecs_p2_16s;
	typedef std::vector<std::vector<lzh::Point2_<lzh::uint16>>>		vecs_p2_16u;
	typedef std::vector<std::vector<lzh::Point2_<lzh::int32>>>		vecs_p2_32s;
	typedef std::vector<std::vector<lzh::Point2_<lzh::uint32>>>		vecs_p2_32u;
	typedef std::vector<std::vector<lzh::Point2_<lzh::int64>>>		vecs_p2_64s;
	typedef std::vector<std::vector<lzh::Point2_<lzh::uint64>>>		vecs_p2_64u;
	typedef std::vector<std::vector<lzh::Point2_<lzh::float32>>>	vecs_p2_32f;
	typedef std::vector<std::vector<lzh::Point2_<lzh::float64>>>	vecs_p2_64f;
	typedef std::vector<std::vector<lzh::Point2_<lzh::float128>>>	vecs_p2_64d;
	typedef std::vector<std::vector<lzh::Point2_<lzh::mat_t>>>		vecs_p2_m;
	typedef std::vector<lzh::Point3_<lzh::int8>>					vec_p3_8s;
	typedef std::vector<lzh::Point3_<lzh::uint8>>					vec_p3_8b;
	typedef std::vector<lzh::Point3_<lzh::int16>>					vec_p3_16s;
	typedef std::vector<lzh::Point3_<lzh::uint16>>					vec_p3_16u;
	typedef std::vector<lzh::Point3_<lzh::int32>>					vec_p3_32s;
	typedef std::vector<lzh::Point3_<lzh::uint32>>					vec_p3_32u;
	typedef std::vector<lzh::Point3_<lzh::int64>>					vec_p3_64s;
	typedef std::vector<lzh::Point3_<lzh::uint64>>					vec_p3_64u;
	typedef std::vector<lzh::Point3_<lzh::float32>>					vec_p3_32f;
	typedef std::vector<lzh::Point3_<lzh::float64>>					vec_p3_64f;
	typedef std::vector<lzh::Point3_<lzh::float128>>				vec_p3_64d;
	typedef std::vector<lzh::Point3_<lzh::mat_t>>					vec_p3_m;
	typedef std::vector<std::vector<lzh::Point3_<lzh::int8>>>		vecs_p3_8s;
	typedef std::vector<std::vector<lzh::Point3_<lzh::uint8>>>		vecs_p3_8b;
	typedef std::vector<std::vector<lzh::Point3_<lzh::int16>>>		vecs_p3_16s;
	typedef std::vector<std::vector<lzh::Point3_<lzh::uint16>>>		vecs_p3_16u;
	typedef std::vector<std::vector<lzh::Point3_<lzh::int32>>>		vecs_p3_32s;
	typedef std::vector<std::vector<lzh::Point3_<lzh::uint32>>>		vecs_p3_32u;
	typedef std::vector<std::vector<lzh::Point3_<lzh::int64>>>		vecs_p3_64s;
	typedef std::vector<std::vector<lzh::Point3_<lzh::uint64>>>		vecs_p3_64u;
	typedef std::vector<std::vector<lzh::Point3_<lzh::float32>>>	vecs_p3_32f;
	typedef std::vector<std::vector<lzh::Point3_<lzh::float64>>>	vecs_p3_64f;
	typedef std::vector<std::vector<lzh::Point3_<lzh::float128>>>	vecs_p3_64d;
	typedef std::vector<std::vector<lzh::Point3_<lzh::mat_t>>>		vecs_p3_m;
}

#include "vector_inl.hpp"
#endif //__VECTOR_H__
