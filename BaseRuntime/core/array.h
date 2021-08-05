#ifndef __ARRAY_H__
#define __ARRAY_H__
#include <vector>
#include "mat.h"

namespace lzh
{
	class _InputArray;
	class LZHAPI Array
	{
	public:
		explicit Array();
		/**
		����w_������
		@param w		��������
		*/
		Array(int32 w, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����w_*h_�ľ���
		@param w		��������
		@param h		��������
		*/
		Array(int32 w, int32 h, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����h_*w_*depth�ľ���
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		Array(int32 w, int32 h, int32 c, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����size*1�ľ���
		@param size	����ߴ�
		*/
		Array(Size size, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����size*c�ľ���
		@param size	����ߴ�
		*/
		Array(Size size, int32 c, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����size�ľ���
		@param size	����ߴ�
		*/
		Array(Size3 size, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����ָ��ɫ��, ��СΪsize�ľ���
		@param colorGamut	ָ��ɫ��
		*/
		Array(ColorGamut colorGamut, Size size, int32 flags);
		/**
		��������
		*/
		explicit Array(const Vector &src, bool copyData = false);
		/**
		��������
		*/
		Array(const Vector &src, Size3 sz, bool copyData = false);
		/**
		�������
		*/
		Array(const void * p, Size3 sz, int32 flags);
		/**
		��������
		@param src		��������
		*/
		Array(const Array &src);
		/**
		���ö���
		@param inputarray	�ӿ�����
		*/
		Array(const _InputArray & inputarray);
		/**
		������a��b�ϲ�(COLΪ���кϲ�|ROWΪ���кϲ�)
		@param a		�������1
		@param b		�������2
		@param merge	�ϲ���ʽ
		*/
		Array(const lzh::Array &a, const lzh::Array &b, RANK merge);
		/**
		���캯��
		���m
		@param m ����
		*/
		template<typename _Tp> Array(const MatCommaInitializer_<_Tp> &m);
		/**
		����1*w*1������,Ԫ��Ϊdata
		@param w		����
		@param data		����
		*/
		Array(int32 w, const void *data, int32 flags, bool copyData = false);
		/**
		����h*w*1�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param data		����Ԫ��
		*/
		Array(int32 w, int32 h, const void *data, int32 flags, bool copyData = false);
		/**
		����h*w*c�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		@param data		����Ԫ��
		*/
		Array(int32 w, int32 h, int32 c, const void *data, int32 flags, bool copyData = false);
		/**
		����size�ľ���,Ԫ��Ϊdata
		@param size		�ߴ�
		@param data		����Ԫ��
		@param refcount	������
		*/
		Array(Size3 size, MatStep step, const void *data, const int *refcount, int32 flags);
		/**
		����h*w*c�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		@param data		����Ԫ��
		@param refcount	������
		*/
		Array(int32 w, int32 h, int32 c, MatStep step, const void *data, const int *refcount, int32 flags);
		/**
		����size������,Ԫ��Ϊdata
		@param size		�ߴ�
		@param data		����
		*/
		Array(Size size, const void *data, int32 flags, bool copyData = false);
		/**
		����size������,Ԫ��Ϊdata
		@param size		�ߴ�
		@param data		����
		*/
		Array(Size size, int32 c, const void *data, int32 flags, bool copyData = false);
		/**
		����size������,Ԫ��Ϊdata
		@param size		�ߴ�
		@param data		����
		*/
		Array(Size3 size, const void *data, int32 flags, bool copyData = false);
		/**
		����h*w*c�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		@param step		����
		@param data		����Ԫ��
		*/
		Array(int32 w, int32 h, int32 c, MatStep step, const void *data, int32 flags);
		/**
		����h*w*c�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		@param step		����
		@param data		����Ԫ��
		*/
		Array(int32 w, int32 h, int32 step, const uint8 *data, ColorGamut colorgaumt);
		/**
		����size�ľ���
		Mat m(Size(10), 1,2,3,4,5,6,7,8,9,10);
		@param size		�����С
		@param v		��һ������(ȷ������)
		@param ...		��������
		*/
		template<typename _Tp> Array(Size3 size, int32 flags, _Tp v, ...);
		template<typename _Ty> Array(std::initializer_list<_Ty> _Ilist);
		template<typename _Ty> Array(std::initializer_list<std::initializer_list<_Ty>> _Ilist);
		template<typename _Ty> Array(std::initializer_list<std::initializer_list<std::initializer_list<_Ty>>> _Ilist);
		//template<class _Tp, int32 N>
		//Array(const Vec<_Tp, N> &vec)
		//{
		//	init();
		//	data.set(&vec.data[0], N, MAT_TYPE(_Tp));
		//}
		//template<class _Tp>
		//Array(const Point2_<_Tp> &p)
		//{
		//	init();
		//	data.set(&p.x, 2, MAT_TYPE(_Tp));
		//}
		//template<class _Tp>
		//Array(const Point3_<_Tp> &p)
		//{
		//	init();
		//	data.set(&p.x, 3, MAT_TYPE(_Tp));
		//}
		//Array(const Mat & src, int32 flags = MAT_T);
		~Array();
		/**
		����w������
		@param w		����
		*/
		void create(int32 w, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����w*h�ľ���
		@param w		����
		@param h		����
		*/
		void create(int32 w, int32 h, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����w*h*c������
		@param w		����
		@param h		����
		@param c		ͨ����
		*/
		void create(int32 w, int32 h, int32 c, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����size�ľ���
		@param size		�����С
		*/
		void create(Size size, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		����size������
		@param size		�����С
		*/
		void create(Size3 size, int32 flags, ColorGamut cg = ColorGamutCustom);
		void createColorGamut(ColorGamut colorGamut, Size sz, int32 flags);
		mat_t First()const;
		mat_t Last()const;
		template<typename _Tp> _Tp & first()const;
		template<typename _Tp> _Tp & last()const;
		template<typename _Tp> _Tp* begin();
		template<typename _Tp> const _Tp* begin()const;
		template<typename _Tp> _Tp* end();
		template<typename _Tp> const _Tp* end()const;
		int32 * memCount()const;
		Vector& ptr();
		const Vector& ptr()const;
		void* Data();
		const void* Data()const;
		const Mat toMat()const;
		template<typename _Tp> constexpr std::vector<_Tp> toVec()const noexcept;
		bool isDeepCopy() const;
		void addref();
		/**
		@brief �ڴ泤��
		*/
		uint32 memsize()const;
		/**
		@brief �ڴ����
		*/
		uint32 memdepth()const;
		/**
		@brief ���ά��
		*/
		void DimCheck()const;
		/**
		@brief ���ؾ���ߴ�(h_,w_,c_)
		*/
		Size3 size3()const;
		/**
		@brief ���ؾ���ƫ��
		*/
		int32 total()const;
		/**
		@brief ���ؾ���ƫ��
		*/
		int32 total_()const;
		/**
		@brief ����ά��
		*/
		int32 dims()const;
		/**
		@brief ���ز���
		*/
		MatStep stride()const;
		/**
		@brief ����
		*/
		Rect rect()const;
		/**
		@brief ����
		*/
		Point center()const;
		/**
		@brief ��������
		*/
		int32 depth()const;
		/**
		@brief ��������
		*/
		int32 type()const;
		/**
		@brief ����ͨ����
		*/
		int32 channels()const;
		/**
		@brief ��������
		*/
		int32 rank()const;
		/**
		@brief ����ɫ��
		*/
		ColorGamut colorGamut()const;
		/**
		@brief ����ɫ��
		*/
		ColorGamut format()const;
		/**
		@brief ���ؾ����СSize(w_,h_)
		*/
		Size size()const;
		int32 toidx(int32 i)const;
		int32 toidx(int32 i, int32 j)const;
		int32 toidx(int32 i, int32 j, int32 k)const;

		int32 toOneIndex(int32 i, int32 j)const;
		int32 toOneIndex(int32 i, int32 j, int32 k)const;

		void set(const _InputArray & src);
		template<typename _Tp> void set(int32 i, const _Tp & v);
		template<typename _Tp> void set(int32 i0, int32 i1, const _Tp & v);
		template<typename _Tp> void set(int32 i0, int32 i1, int32 i2, const _Tp & v);
		template<typename _Tp> void set(Point p, const _Tp & v);
		template<typename _Tp> void set(Point3 p, const _Tp & v);
		template<typename _Tp, int32 N> void set(int32 i0, int32 i1, const Vec<_Tp, N> &v);
		void set(int32 i0, int32 i1, const Color &v);
		void set(int32 i, const void * v, int32 addr_offset);
		void set(int32 i0, int32 i1, const void * v, int32 addr_offset);
		void set(int32 i0, int32 i1, int32 i2, const void * v, int32 addr_offset);
		void assign(int32 i, int32 j);
		void swap(int32 i, int32 j);
		template<typename _Tp> void add(int32 i, _Tp v);
		template<typename _Tp> void add(int32 i, int32 j, _Tp v);
		template<typename _Tp> void add(int32 i, int32 j, int32 k, _Tp v);
		template<typename _Tp> void reduce(int32 i, _Tp v);
		template<typename _Tp> void reduce(int32 i, int32 j, _Tp v);
		template<typename _Tp> void reduce(int32 i, int32 j, int32 k, _Tp v);
		template<typename _Tp> void mul(int32 i, _Tp v);
		template<typename _Tp> void mul(int32 i, int32 j, _Tp v);
		template<typename _Tp> void mul(int32 i, int32 j, int32 k, _Tp v);
		template<typename _Tp> void div(int32 i, _Tp v);
		template<typename _Tp> void div(int32 i, int32 j, _Tp v);
		template<typename _Tp> void div(int32 i, int32 j, int32 k, _Tp v);
		Array mul(const _InputArray & m)const;
		mat_t get_(int32 i)const;
		mat_t get_(int32 i0, int32 i1)const;
		mat_t get_(int32 i0, int32 i1, int32 i2)const;
		template<typename _Tp> _Tp get(int32 i)const;
		template<typename _Tp> _Tp get(int32 i0, int32 i1)const;
		template<typename _Tp> _Tp get(int32 i0, int32 i1, int32 i2)const;
		Color pixel(int32 i0, int32 i1)const;
		Color pixel(int32 i0, int32 i1, Color::Spec spec)const;
		void setPixel(int32 i0, int32 i1, Color color);
		void fill(Val value);
		template<typename _Tp, int N> void fill(const Vec<_Tp, N> &value);
		Array safediv(const Array & a, Val def = 0) const;
		/**
		�������
		@param file		�����ļ���
		@param binary	ѡ���ı����Ƕ�����
		binary = false	ѡ���ı�
		binary = true	ѡ�������
		*/
		void save(std::string file, bool binary = true)const;
		/**
		�����Ʊ������
		@param file		�����ļ�ָ��
		*/
		void save(FILE *file)const;
		/**
		��ȡ����
		@param file		��ȡ�ļ���
		ֻ֧�ֶ����ƶ�ȡ
		*/
		void load(std::string file);
		/**
		��ȡ����
		@param file		��ȡ�ļ�ָ��
		ֻ֧�ֶ����ƶ�ȡ
		*/
		void load(FILE *file);
		/**
		@brief ���ؾ����С(h_*w_*c_)
		*/
		int32 len()const;
		/**
		@brief ���ؾ����С(h_*w_*c_)
		*/
		int32 elementlen()const;
		/**
		@brief ���ؾ���״̬
		0Ϊ����
		-1Ϊ�վ���
		-2Ϊ�Ƿ���
		*/
		int32 enable()const;
		/**
		@brief ���ؾ����Ƿ�Ϊ��
		*/
		bool empty()const;
		/**
		@brief ���ؾ����Ƿ�Ϊ����
		*/
		bool isSquare()const;
		/**
		���������mat
		@param mat		����
		*/
		void copyTo(Array& mat)const;
		/**
		���ݸ��ǵ�mat
		@param mat		����
		*/
		void setTo(Array mat)const;
		/**
		����ת��
		@param m		���
		@param type		��������
		*/
		void convertTo(Array &m, int32 type)const;
		/**
		����ת��
		@brief ����type�������;���
		@param type		��������
		*/
		const Array convert(int32 type)const;
		/**
		�ھ�������߻����ұ����һ��1
		@param dire		ѡ����ӷ�ʽ
		*/
		void setAddOnes(Dire dire = RIGHT);
		/**
		�ھ�������߻����ұ����һ��0
		@param dire		ѡ����ӷ�ʽ
		*/
		void setAddZeros(Dire dire = RIGHT);
		/**
		�ͷ��ڴ�
		*/
		void release();
		/**
		@brief ���������ؾ���Ԫ��
		@param w		����
		*/
		template<typename _Tp> _Tp& at(int32 w);
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		*/
		template<typename _Tp> _Tp& at(int32 h, int32 w);
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		@param c		ͨ������
		*/
		template<typename _Tp> _Tp& at(int32 h, int32 w, int32 c);
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		template<typename _Tp> _Tp& at(Point p);
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		template<typename _Tp> _Tp& at(Point3i p);
		/**
		@brief ���������ؾ���Ԫ��
		@param w		����
		*/
		template<typename _Tp> const _Tp& at(int32 w)const;
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		*/
		template<typename _Tp> const _Tp& at(int32 h, int32 w)const;
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		@param c		ͨ������
		*/
		template<typename _Tp> const _Tp& at(int32 h, int32 w, int32 c)const;
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		template<typename _Tp> const _Tp& at(Point p)const;
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		template<typename _Tp> const _Tp& at(Point3i p)const;
		/**
		@brief ������ת��Ϊ��Ӧ����������
		@param index	����
		*/
		int32 toX(int32 index)const;
		/**
		@brief ������ת��Ϊ��Ӧ����������
		@param index	����
		*/
		int32 toY(int32 index)const;
		/**
		@brief ������ת��Ϊ��Ӧ����ͨ������
		@param index	����
		*/
		int32 toZ(int32 index)const;

		/**
		@brief ���ؾ�����value��ȵĵ�һ��Ԫ������
		@param value	Ԫ��
		*/
		template<typename _Tp>
		int32 find(_Tp value)const;
		/**
		@brief ���ؾ���Ԫ�����ֵ������
		*/
		int32 maxAt(mat_t *v = nullptr)const;
		/**
		@brief ���ؾ���Ԫ����Сֵ������
		*/
		int32 minAt(mat_t *v = nullptr)const;
		/**
		@brief ���ؾ����Ƿ����value
		@param value	Ԫ��
		*/
		template<typename _Tp> bool contains(_Tp value)const;
		template<typename _Tp> bool isValue(int32 i, _Tp v)const;
		template<typename _Tp> bool isValue(Point p, _Tp v)const;
		bool isZero(int32 i)const;
		bool isZero(Point p)const;
		bool isOne(int32 i)const;
		bool isOne(Point p)const;
		bool compare(int32 i1, int32 i2, int32 op)const;
		template<typename _Tp> bool compare(int32 i, _Tp v, int32 op)const;
		template<typename _Tp> bool compare(int32 i0, int32 i1, _Tp v, int32 op)const;
		template<typename _Tp> bool compare(int32 i0, int32 i1, int32 i2, _Tp v, int32 op)const;
		void BitNot(int32 idx);
		void BitAnd(int32 idx, Val val);
		void BitOr(int32 idx, Val val);
		const Array BitNot()const;
		const Array BitAnd(Val val)const;
		const Array BitAnd(const Array & a)const;
		const Array BitOr(Val val)const;
		const Array BitOr(const Array & a)const;
		Point toPoint()const;
		Point2d toPoint2d()const;
		int32 rankSize(RANK r) const;
		/**
		@brief ���ؾ�������ֵ
		@param is_abs	�Ƿ����ֵ
		*/
		mat_t max(Array mask = Array())const;
		/**
		@brief ���ؾ�������ֵ
		@param is_abs	�Ƿ����ֵ
		*/
		mat_t max(bool is_abs, Array mask = Array())const;
		/**
		@brief ���ؾ�������ֵ
		@param is_abs	�Ƿ����ֵ
		*/
		mat_t min(Array mask = Array())const;
		/**
		@brief ���ؾ������Сֵ
		@param is_abs	�Ƿ����ֵ
		*/
		mat_t min(bool is_abs, Array mask = Array())const;
		/**
		@brief ���ؾ���Ԫ�����ֵ
		*/
		template<typename _Tp> _Tp& findmax();
		/**
		@brief ���ؾ���Ԫ����Сֵ
		*/
		template<typename _Tp> _Tp& findmin();
		/**
		@brief �������������򿽱�Ԫ�ص�src������
		@param src			����������
		@param Row_Start	��ȡ�г�ʼ����ֵ
		@param Row_End		��ȡ�н�������ֵ
		@param Col_Start	��ȡ�г�ʼ����ֵ
		@param Col_End		��ȡ�н�������ֵ
		*/
		void copy(Array &src, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End)const;
		void copy(Array &src, Rect iou)const;
		/**
		@brief �����󿽱���src
		@param src ����������
		*/
		void swap(Array &src);
		/**
		@brief mChannel ��src���ǵ���c_ͨ��
		@param src		����
		@param c_	ͨ����
		*/
		void mChannel(const lzh::Array &src, int32 c);
		/**
		@brief mChannel ��src���ǵ���c_ͨ��
		@param src		����
		@param c_	ͨ����
		*/
		void mChannel(const lzh::Array &src, int32 w, int32 h);
		/**
		@brief ���þ���ά��
		������ı���󳤶�
		@param size		�����С
		*/
		Array reshape(Size3 size);
		/**
		@brief ���þ���ά��
		������ı���󳤶�
		@param h_		��������
		@param w_		��������
		@param c_	����ͨ��
		*/
		Array reshape(int32 w, int32 h, int32 c);
		/**
		@brief ���þ����С
		�������ԭ��С������h_*w_*1��Ԫ��ȫ������Ϊ0
		@param h_	��������
		@param w_	��������
		*/
		bool setSize(int32 w, int32 h, int32 c);
		/**
		@brief ��������src
		@param src	��������
		*/
		void setvalue(const lzh::Array &src);
		/**
		@brief ���������
		*/
		void setInv();
		/**
		@brief ���þ����num����
		@param num	����
		*/
		void setPow(mat_t num);
		/**
		@brief ���õ�λ����
		*/
		void setIden();
		/**
		@brief ���ð������
		*/
		void setAdj();
		/**
		@brief ����ת�þ���
		*/
		void setTran();
		/**
		����ɫ��
		@param cg	ָ��ɫ��
		*/
		void setColorGamut(ColorGamut cg);


		std::string infomation()const;
		/**
		@brief �������������
		*/
		void showInfomation()const;
		/**
		@brief �������������
		*/
		void show(bool showInfo = false)const;
		/**
		@brief �������
		*/
		std::ostream & show(std::ostream & out, bool showInfo)const;

		/**
		@brief ����h�о���
		@param ����
		*/
		Array row(int32 h);
		/**
		@brief ����h�о���
		@param ����
		*/
		const Array row(int32 h)const;
		/**
		@brief ����w�о���
		@param ����
		*/
		Array col(int32 w);
		/**
		@brief ����w�о���
		@param ����
		*/
		const Array col(int32 w)const;
		/**
		@brief ����cͨ������
		@param ͨ������
		*/
		Array channel(int32 c);
		/**
		@brief ����cͨ������
		@param ͨ������
		*/
		const Array channel(int32 c)const;
		template<typename _Tp> const Array addvalue(Dire dire, _Tp v) const;
		/**
		@brief �ھ�������߻����ұ����һ��1
		@param dire		ѡ����ӷ�ʽ
		*/
		const Array addones(Dire dire = RIGHT)const;
		/**
		@brief �ھ�������߻����ұ����һ��0
		@param dire		ѡ����ӷ�ʽ
		*/
		const Array addzeros(Dire dire = RIGHT)const;
		/**
		@brief ����start��end����
		@param start	��ʼ����
		@param end		��������
		*/
		const Array range(int32 start, int32 end)const;
		/**
		@brief ����roi��Χ�ľ���
		@param roi	��ʼ����
		*/
		const Array range(Rect roi)const;
		/**
		@brief ����roi��Χ�ľ���
		@param roi	��ʼ����
		*/
		const Array range(int32 h_start, int32 h_end, int32 w_start, int32 w_end)const;
		/**
		@brief ����h_start��h_end�о���
		@param h_start	�п�ʼ����
		@param h_end	�н�������
		*/
		const Array rowRange(int32 h_start, int32 h_end)const;
		/**
		@brief ����w_start��w_end�о���
		@param w_start	�п�ʼ����
		@param w_end	�н�������
		*/
		const Array colRange(int32 w_start, int32 w_end)const;
		/**
		@brief ����c_start��c_endͨ������
		@param c_start	ͨ����ʼ����
		@param c_end	ͨ����������
		*/
		const Array channelRange(int32 c_start, int32 c_end)const;
		/**
		@brief �����������
		*/
		const Array clone()const;
		/**
		@brief ����ȡ������
		*/
		const Array opp()const;
		/**
		@brief ���ؾ���ֵ����
		*/
		const Array abs()const;
		/**
		@brief ���ذ�num���ݾ���
		@param num ����
		*/
		const Array mPow(int32 num)const;
		/**
		@brief ���ذ�num���ݾ���
		@param num ����
		*/
		const Array pow(mat_t num)const;
		/**
		@brief ���ذ�Ԫ��ȡָ������
		*/
		const Array exp()const;
		/**
		@brief ���ذ�Ԫ��ȡ��������
		*/
		const Array log()const;
		/**
		@brief ���ذ�Ԫ��ȡ��������
		*/
		const Array sqrt()const;
		/**
		@brief ���ذ������
		*/
		const Array adj()const;
		/**
		@brief ����ת�þ���
		*/
		const Array t()const;
		/**
		@brief ���������
		*/
		const Array inv()const;
		/**
		@brief ���������
		*/
		const Array diag(int32 k = 0)const;
		/**
		@brief �����������
		�������������
		*/
		const Array reverse()const;
		/**
		@brief ������������(δ���)
		*/
		const Array EigenvectorsMax(mat_t offset = 1e-8)const;
		/**
		@brief sigmoid����
		*/
		const Array sigmoid()const;
		/**
		@brief tanh����
		*/
		const Array tanh()const;
		/**
		@brief relu����
		*/
		const Array relu()const;
		/**
		@brief elu����
		*/
		const Array elu()const;
		/**
		@brief selu����
		*/
		const Array selu()const;
		/**
		@brief leaky_relu����
		*/
		const Array leaky_relu()const;
		/**
		@brief softmax����
		*/
		const Array softmax()const;
		/**
		@brief ��������ʽ
		*/
		mat_t Det();
		/**
		@brief ����num����
		@param num ������
		*/
		mat_t norm(int32 num = 1)const;
		/**
		@brief ���ض�Ӧ����������ʽ
		@param x ������
		@param y ������
		*/
		mat_t Cof(int32 x, int32 y);
		mat_t EigenvalueMax(mat_t offset = 1e-8)const;
		/**
		@brief ���ؾ���Ԫ�غ�
		@param num	���ô���
		@param _abs �Ƿ�ȡ����ֵ
		*/
		mat_t sum(int32 num = 1, bool _abs = false)const;
		/**
		@brief ����ƽ��ֵ
		*/
		mat_t mean()const;
		/**
		@brief ���ر�׼��
		*/
		mat_t std()const;
		/**
		@brief ���ر�׼��
		*/
		mat_t D()const;
		/**
		@brief ���������=
		����
		*/
		Array & operator = (const Array &temp);
		/**
		@brief ���������=
		����
		*/
		//Array & operator = (const Mat &temp);
		/**
		@brief ���������=
		����
		*/
		template<class _Tp> Array & operator=(const MatCommaInitializer_<_Tp>& temp);
		/**
		@brief ���������==
		�жϾ����Ƿ����
		*/
		bool operator == (const lzh::Array &a)const;
		/**
		@brief ���������!=
		�жϾ����Ƿ����
		*/
		bool operator != (const lzh::Array &a)const;
		mat_t operator () (int32 i)const;
		mat_t operator () (int32 i0, int32 i1)const;
		mat_t operator () (int32 i0, int32 i1, int32 i2)const;
		mat_t operator () (Point p)const;
		mat_t operator () (Point3 p)const;
		Array operator () (Rect roi)const;
		Array operator () (Range row, Range col)const;
		/**
		@brief ���ؾ����Ӧ�������л���
		@param index	����
		@param rc		������ʽ
		*/
		Array operator () (int32 index, RANK rc)const;
		/**
		@brief ���ؾ����Ӧ�������л���
		@param index	����
		@param rc		������ʽ
		*/
		Array operator () (int32 v1, int32 v2, RANK rc)const;
		const Array operator [] (int32 idx) const;

		bool operator > (const Array &b) const;
		bool operator < (const Array &b) const;

		template<typename _Tp> operator _Tp *();
		template<typename _Tp> operator const _Tp *() const;

		/**
		@brief ����Ԫ��Ϊ0��w����
		@param w		��������
		*/
		static const Array zeros(int32 w, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��h*w����
		@param w		��������
		@param h		��������
		*/
		static const Array zeros(int32 w, int32 h, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��h*w*c����
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static const Array zeros(int32 w, int32 h, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��size����
		@param size �����С
		*/
		static const Array zeros(Size size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��size����
		@param size �����С
		*/
		static const Array zeros(Size size, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��size����
		@param size �����С
		*/
		static const Array zeros(Size3 size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��w����
		@param v		���Ԫ��
		@param w		��������
		*/
		static const Array value(Val v, int32 w, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��h*w����
		@param v		���Ԫ��
		@param w		��������
		@param h		��������
		*/
		static const Array value(Val v, int32 w, int32 h, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��h_*w_*c_����
		@param v		���Ԫ��
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static const Array value(Val v, int32 w, int32 h, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��h_*w_*c_����
		@param v		���Ԫ��
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static const Array value(Val v, Size3 size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��w����
		@param w		��������
		*/
		static const Array ones(int32 w, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��h*w����
		@param w	��������
		@param h	��������
		*/
		static const Array ones(int32 w, int32 h, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��h*w*c����
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static const Array ones(int32 w, int32 h, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��size����
		@param size �����С
		*/
		static const Array ones(Size size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��size����
		@param size �����С
		*/
		static const Array ones(Size size, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��size����
		@param size �����С
		*/
		static const Array ones(Size3 size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief �������ɵ�n*n*1��λ����
		@param n �����С
		*/
		static const Array eye(int32 n, int32 flag);


		/**
		���ô�ӡ���
		@param w ���
		*/
		static void setPrintW(uint32 w);
		/**
		���ô�ӡ��Ч����
		@param n λ��
		*/
		static void setPrintSignificantDigits(uint32 n);
		/**
		���ô�ӡ����
		@param t ��ӡ����
		*/
		static void setPrintType(lzh::PrintType t);

		int32 rows;
		int32 cols;
		MatStep step;
		Vector data;
	protected:
		void setp(int32 i, const void * v, int32 addr_offset);

		static lzh::uint32 print_width;
		static lzh::uint32 print_precision;
		static lzh::PrintType print_type;

		void init();
		void checkSquare();
#ifdef LZH_DEBUG
		void checkindex(int32 index);
		void checkindex(int32 index_x, int32 index_y);
#endif // LZH_DEBUG
		void setsize(int32 w, int32 h = 1, int32 c = 1);

	private:
		int32 c_;
		int32 dim;
		ColorGamut _type;
		bool square;
	};
	typedef const Array& IArray;
	typedef Array& OArray;

	/**
	@brief Array_ ������
	�̳�Mat�࣬����ʵ��
	Array mat = (Array_<_Tp>(3, 3) <<
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1);
	*/
	template<typename _Tp> class TEMPLATE_API Array_ : public Array
	{
	public:
		using value_type = typename _Tp;

		explicit Array_() : Array() {}
		/**
		@brief ����w������
		@param w		����
		*/
		Array_(int32 w) : Array(w, DataType<_Tp>::type) {}
		/**
		@brief ����h*w�ľ���
		@param w		����
		@param h		����
		*/
		Array_(int32 w, int32 h) : Array(w, h, DataType<_Tp>::type) {}
		/**
		@brief ����h*w*c������
		@param w		����
		@param h		����
		@param depth	ͨ����
		*/
		Array_(int32 w, int32 h, int32 c) : Array(w, h, c, DataType<_Tp>::type) {}
		/**
		@brief ����size�ľ���
		@param size		�ߴ�
		*/
		Array_(Size size) : Array(size, DataType<_Tp>::type) {}
		/**
		@brief ����size�ľ���
		@param size		�ߴ�
		*/
		Array_(Size size, int32 c) : Array(size, c, DataType<_Tp>::type) {}
		/**
		@brief ����size�ľ���
		@param size		�ߴ�
		*/
		Array_(Size3 size) : Array(size) {}
		Array_(std::initializer_list<_Tp> _Ilist) : Array((int32)_Ilist.size(), _Ilist.begin(), DataType<_Tp>::type) {}
		operator _Tp *() {
			return data;
		}
		operator const _Tp *() const {
			return data;
		}
		//_Tp& operator () (int32 i)const;
		//_Tp& operator () (int32 i0, int32 i1)const;
		//_Tp& operator () (int32 i0, int32 i1, int32 i2)const;
	};
	typedef Array_<mat_t> Mat_;


	/**
	@brief MatCommaInitializer_ ������
	��Ϊ������������ʵ��
	Mat mat = (Array_(3, 3) <<
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1);
	*/
	template<class _Tp> class TEMPLATE_API MatCommaInitializer_
	{
	public:
		using value_type = typename _Tp;

		explicit MatCommaInitializer_() {}
		MatCommaInitializer_(const Array_<_Tp>& m) {
			head = (_Tp*)m.data();
			it = head;
			h = m.rows;
			w = m.cols;
			c = m.channels();
		}
		template<typename _T2> MatCommaInitializer_<_Tp>& operator , (_T2 v);
		int32 rows()const { return h; }
		int32 cols()const { return w; }
		int32 channels()const { return c; }
		_Tp * data()const { return head; }
		int32 size()const { return h * w*c; }
	private:
		int32 h;
		int32 w;
		int32 c;
		_Tp *it;
		_Tp *head;
	};
	template<typename _Tp, typename _T2> inline
		MatCommaInitializer_<_Tp> operator << (const Array_<_Tp> & m, _T2 val);

	typedef std::vector<lzh::Array>									vec_array;
	typedef std::vector<std::vector<lzh::Array>>					vecs_array;
}

#include "array_inl.hpp"
#endif //__ARRAY_H__