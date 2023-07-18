#pragma once
#ifndef __MAT_H__
#define __MAT_H__

#include <vector>
#include <string>
#include <iostream>
#include <stdarg.h>
//#define MAT_EXPORTS


#include "core.h"
#include "point.h"
#include "vector.h"
#include "color.h"
//#include <windows.h>

#define PTR_COMPARE(p, op, i1, i2, out) \
		do{switch (op) {\
		case -2:	out = (p[i1] < p[i2]);break;\
		case -1:	out = (p[i1] <= p[i2]);break;\
		case 0:		out = (p[i1] == p[i2]);break;\
		case 1:		out = (p[i1] >= p[i2]);break;\
		case 2:		out = (p[i1] > p[i2]);break;\
		default:break;}}while(0)

namespace lzh {
	class MatIter; template<class _Tp> class MatCommaInitializer_; class Array; class _InputArray;
	class LZHAPI Mat
	{
	public:
		typedef MatIter iterator;
		typedef const MatIter const_iterator;
		explicit Mat();
		/**
		����w_*h_�ľ���
		@param w		��������
		@param h		��������
		*/
		Mat(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		����h_*w_*depth�ľ���
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		Mat(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		����size*1�ľ���
		@param size_	����ߴ�
		*/
		Mat(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		����size*1�ľ���
		@param size_	����ߴ�
		*/
		Mat(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		����size�ľ���
		@param size_	����ߴ�
		*/
		Mat(Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		����ָ��ɫ��, ��СΪsize�ľ���
		@param colorGamut	ָ��ɫ��
		*/
		Mat(ColorGamut colorGamut, Size sz);
		/**
		��������
		*/
		Mat(const MatPtr<mat_t> &src, Size3 sz, bool copyData = false);
		/**
		��������
		@param src		��������
		*/
		Mat(const Mat &src);
		/**
		Arrayת��Mat
		@param src		Array
		*/
		Mat(const Array &src);
		/**
		������a��b�ϲ�(COLΪ���кϲ�|ROWΪ���кϲ�)
		@param a		�������1
		@param b		�������2
		@param merge	�ϲ���ʽ
		*/
		Mat(const Mat &a, const Mat &b, RANK merge);
		/**
		��������
		*/
		template<typename _Tp> Mat(const _Tp *m, int32 w, int32 h, int32 c = 1);
		/**
		���캯��
		���m
		@param m ����
		*/
		Mat(const MatCommaInitializer_<mat_t> &m, bool copyData = true);
		/**
		����1*w*1������,Ԫ��Ϊdata
		@param w		����
		@param data		����
		*/
		Mat(int32 w, const void *data, bool copyData = false);
		/**
		����h*w*1�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param data		����Ԫ��
		*/
		Mat(int32 w, int32 h, const void *data, bool copyData = false);
		/**
		����h*w*c�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		@param data		����Ԫ��
		*/
		Mat(int32 w, int32 h, int32 c, const void *data, bool copyData = false);
		/**
		����size�ľ���,Ԫ��Ϊdata
		@param size		����ߴ�
		@param data		����Ԫ��
		*/
		Mat(Size size, const void *data, bool copyData = false);
		/**
		����size*c�ľ���,Ԫ��Ϊdata
		@param size		����ߴ�
		@param data		����Ԫ��
		*/
		Mat(Size size, int32 c, const void *data, bool copyData = false);
		/**
		����size�ľ���,Ԫ��Ϊdata
		@param size		����ߴ�
		@param data		����Ԫ��
		*/
		Mat(Size3 size, const void *data, bool copyData = false);
		/**
		����size�ľ���,Ԫ��Ϊdata
		@param size		����ߴ�
		@param data		����Ԫ��
		@param refcount	���������
		*/
		Mat(Size3 size, MatStep stride, const void *data, const int *refcount);
		/**
		����size�ľ���,Ԫ��Ϊdata
		@param size		����ߴ�
		@param data		����Ԫ��
		@param refcount	���������
		*/
		Mat(int32 w, int32 h, int32 c, MatStep stride, const void *data, const int *refcount);
		/**
		����h*w*c�ľ���,Ԫ��Ϊdata
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		@param stride		����
		@param data		����Ԫ��
		*/
		Mat(int32 w, int32 h, int32 c, MatStep stride, const mat_t *data);
		/**
		����size�ľ���
		Mat m(Size(10), 1,2,3,4,5,6,7,8,9,10);
		@param size		�����С
		@param v		��һ������(ȷ������)
		@param ...		��������
		*/
		template <typename ... _Tp>
		Mat(Size3 size, const _Tp & ... args);
		template<typename _Tp> Mat(std::initializer_list<_Tp> _Ilist);
		template<typename _Tp> Mat(std::initializer_list<std::initializer_list<_Tp>> _Ilist);
		template<typename _Tp> Mat(std::initializer_list<std::initializer_list<std::initializer_list<_Tp>>> _Ilist);
		/**
		����ת����
		@param vec		����
		@param dirc		������
		*/
		template<class _Tp> Mat(const std::vector<_Tp> &vec);
		template<class _Tp> Mat(const std::vector<std::vector<_Tp>> &vec);
		template<class _Tp, int32 N> Mat(const Vec<_Tp, N> &vec);
		template<class _Tp> Mat(const Point2_<_Tp> &p);
		template<class _Tp> Mat(const Point3_<_Tp> &p);
		//Mat(const _InputArray &input);
		~Mat();
		void addref();
		/**
		����w������
		@param w		����
		*/
		void create(int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		����w*h�ľ���
		@param w		����
		@param h		����
		*/
		void create(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		����w*h*c������
		@param w		����
		@param h		����
		@param c		ͨ����
		*/
		void create(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		����size�ľ���
		@param size		�����С
		*/
		void create(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		����size�ľ���
		@param size		�����С
		*/
		void create(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		����size������
		@param size		�����С
		*/
		void create(Size3 size, ColorGamut cg = ColorGamutCustom);
		void createColorGamut(ColorGamut colorGamut, Size sz, int32 flags);
		/**
		@brief ����ɫ��
		*/
		ColorGamut colorGamut()const;
		/**
		@brief �����ڴ����
		*/
		const int32* memCount()const;
		mat_t* data();
		const mat_t* data()const;
		mat_t* begin();
		const mat_t* begin()const;
		mat_t* end();
		const mat_t* end()const;
		iterator begin(RANK r);
		const_iterator begin(RANK r)const;
		iterator end(RANK r);
		const_iterator end(RANK r)const;
		Point toPoint()const;
		Point2m toPoint2D()const;
		Point3m toPoint3D()const;
		MatPtr<mat_t> ptr()const;
		mat_t& first()const;
		mat_t& last()const;
		Rect rect()const;
		void set(const Mat &m);
		/**
		@brief �ڴ泤��
		*/
		uint32 memsize()const;
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
		int32 element()const;
		int32 total_()const;
		MatStep step()const;
		Point center()const;
		/**
		@brief ����ά��
		*/
		int32 dims()const;
		/**
		@brief ��������
		*/
		int32 depth()const;
		/**
		@brief ��������
		*/
		int32 type()const;
		/**
		@brief ��������
		*/
		int32 rows()const;
		/**
		@brief ��������
		*/
		int32 cols()const;
		/**
		@brief ����ͨ����
		*/
		int32 channels()const;
		int32 s()const;
		int32 o()const;
		int32 h()const;
		int32 w()const;
		int32 c()const;
		/**
		@brief ��������
		*/
		int32 rank()const;
		/**
		@brief ���ؾ����СSize(h_,w_)
		*/
		Size size()const;
		int32 rankSize(RANK r)const;
		int32 toidx(int32 i)const;
		int32 toidx(int32 i, int32 j)const;
		int32 toidx(int32 i, int32 j, int32 k)const;
		void setStep(MatStep st);
		bool checkIndex(Rect rect)const;
		bool checkIndex(int32 x0, int32 x1, int32 y0, int32 y1)const;
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
		@brief ���ؾ����Ƿ�Ϊ��
		*/
		bool isDeepCopy()const;
		/**
		@brief ���ؾ����Ƿ�Ϊ����
		*/
		bool isSquare()const;
		/**
		�������ݵ�mat
		@param mat		����
		*/
		void copyTo(Mat& mat)const;
		void setTo(Mat& mat)const;
		void setTo(Mat&& mat)const;
		void convertTo(Mat &m, int32 type)const;
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
		mat_t& at(int32 w);
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		*/
		mat_t& at(int32 h, int32 w);
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		@param c		ͨ������
		*/
		mat_t& at(int32 h, int32 w, int32 c);
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		mat_t& at(Point p);
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		mat_t& at(Point3i p);
		/**
		@brief ���������ؾ���Ԫ��
		@param w		����
		*/
		const mat_t & at(int32 w)const;
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		*/
		const mat_t & at(int32 h, int32 w)const;
		/**
		@brief ���������ؾ���Ԫ��
		@param w		������
		@param h		������
		@param c		ͨ������
		*/
		const mat_t & at(int32 h, int32 w, int32 c)const;
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		const mat_t & at(Point p)const;
		/**
		@brief ���ض�Ӧ����Ԫ��
		@param p ����
		*/
		const mat_t & at(Point3i p)const;
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
		int32 find(mat_t value)const;
		/**
		@brief ���ؾ���Ԫ�����ֵ������
		*/
		int32 maxAt()const;
		/**
		@brief ���ؾ���Ԫ����Сֵ������
		*/
		int32 minAt()const;
		bool isZero(int32 i)const;
		bool isZero(int32 i, Val_ offset)const;
		bool isOne(int32 i)const;
		bool compare(int32 i1, int32 i2, int32 op)const;
		bool compare(int32 i, Val_ v, int32 op)const;
		bool compare(int32 i, int32 j, Val_ v, int32 op);
		bool compare(int32 i, int32 j, int32 k, Val_ v, int32 op);
		void fill(mat_t v);
		/**
		@brief ���ؾ�������ֵ
		@param is_abs	�Ƿ����ֵ
		*/
		mat_t max(bool is_abs = false)const;
		/**
		@brief ���ؾ������Сֵ
		@param is_abs	�Ƿ����ֵ
		*/
		mat_t min(bool is_abs = false)const;
		/**
		@brief ���ؾ���Ԫ�����ֵ
		*/
		mat_t& findmax();
		/**
		@brief ���ؾ���Ԫ����Сֵ
		*/
		mat_t& findmin();
		/**
		@brief �������������򿽱�Ԫ�ص�src������
		@param src			����������
		@param Row_Start	��ȡ�г�ʼ����ֵ
		@param Row_End		��ȡ�н�������ֵ
		@param Col_Start	��ȡ�г�ʼ����ֵ
		@param Col_End		��ȡ�н�������ֵ
		*/
		void copy(Mat &src, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End)const;
		/**
		@brief �����󿽱���src
		@param src ����������
		*/
		void swap(Mat &src);
		/**
		@brief mChannel ��src���ǵ���c_ͨ��
		@param src		����
		@param c_	ͨ����
		*/
		void mChannel(const Mat &src, int32 c);
		/**
		@brief mChannel ��src���ǵ���c_ͨ��
		@param src		����
		@param c_	ͨ����
		*/
		void mChannel(const Mat &src, int32 w, int32 h);
		/**
		@brief ���þ���ά��
		������ı���󳤶�
		@param size		�����С
		*/
		Mat reshape(Size3 size);
		/**
		@brief ���þ���ά��
		������ı���󳤶�
		@param h_		��������
		@param w_		��������
		@param c_	����ͨ��
		*/
		Mat reshape(int32 w, int32 h = 1, int32 c = 1);
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
		void setvalue(const Mat &src);
		/**
		����ɫ��
		@param cg	ָ��ɫ��
		*/
		void setColorGamut(ColorGamut cg);
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
		@brief ����ȡ��
		*/
		void setOpp();
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
		Mat row(int32 h);
		/**
		@brief ����h�о���
		@param ����
		*/
		const Mat row(int32 h)const;
		/**
		@brief ����w�о���
		@param ����
		*/
		Mat col(int32 w);
		/**
		@brief ����w�о���
		@param ����
		*/
		const Mat col(int32 w)const;
		/**
		@brief ����cͨ������
		@param ͨ������
		*/
		Mat channel(int32 c);
		/**
		@brief ����cͨ������
		@param ͨ������
		*/
		const Mat channel(int32 c)const;

		Mat addvalue(Dire dire, Val_ v) const;
		/**
		@brief �ھ�������߻����ұ����һ��1
		@param dire		ѡ����ӷ�ʽ
		*/
		Mat addones(Dire dire = RIGHT)const;
		/**
		@brief �ھ�������߻����ұ����һ��0
		@param dire		ѡ����ӷ�ʽ
		*/
		Mat addzeros(Dire dire = RIGHT)const;
		/**
		@brief ����start��end����
		@param start	��ʼ����
		@param end		��������
		*/
		Mat range(int32 start, int32 end)const;
		/**
		@brief ����roi��Χ�ľ���
		@param roi	��ʼ����
		*/
		Mat range(Rect roi)const;
		/**
		@brief ����roi��Χ�ľ���
		@param roi	��ʼ����
		*/
		Mat range(int32 h_start, int32 h_end, int32 w_start, int32 w_end)const;
		/**
		@brief ����h_start��h_end�о���
		@param h_start	�п�ʼ����
		@param h_end	�н�������
		*/
		Mat rowRange(int32 h_start, int32 h_end)const;
		/**
		@brief ����w_start��w_end�о���
		@param w_start	�п�ʼ����
		@param w_end	�н�������
		*/
		Mat colRange(int32 w_start, int32 w_end)const;
		/**
		@brief ����c_start��c_endͨ������
		@param c_start	ͨ����ʼ����
		@param c_end	ͨ����������
		*/
		Mat channelRange(int32 c_start, int32 c_end)const;
		/**
		@brief �����������
		*/
		Mat clone()const;
		/**
		@brief ����ȡ������
		*/
		Mat opp()const;
		/**
		@brief ���ؾ���ֵ����
		*/
		Mat abs()const;
		/**
		@brief ���ذ�num���ݾ���
		@param num ����
		*/
		Mat mPow(int32 num)const;
		/**
		@brief ���ذ�num���ݾ���
		@param num ����
		*/
		Mat pow(Val_ num)const;
		/**
		@brief ���ذ�Ԫ��ȡָ������
		*/
		Mat exp()const;
		/**
		@brief ���ذ�Ԫ��ȡ��������
		*/
		Mat log()const;
		/**
		@brief ���ذ�Ԫ��ȡ��������
		*/
		Mat sqrt()const;
		/**
		@brief ���ذ������
		*/
		Mat adj()const;
		/**
		@brief ����ת�þ���
		*/
		Mat t()const;
		/**
		@brief ���������
		*/
		Mat inv()const;
		/**
		@brief ���������
		*/
		Mat diag(int32 k = 0)const;
		/**
		@brief �����������
		�������������
		*/
		Mat reverse()const;
		/**
		@brief ������������(δ���)
		*/
		Mat EigenvectorsMax(mat_t offset = 1e-8)const;
		/**
		@brief sigmoid����
		*/
		Mat sigmoid()const;
		/**
		@brief tanh����
		*/
		Mat tanh()const;
		/**
		@brief relu����
		*/
		Mat relu()const;
		/**
		@brief elu����
		*/
		Mat elu()const;
		/**
		@brief selu����
		*/
		Mat selu()const;
		/**
		@brief leaky_relu����
		*/
		Mat leaky_relu()const;
		/**
		@brief softmax����
		*/
		Mat softmax()const;

		Mat sin()const;
		Mat cos()const;
		Mat tan()const;
		Mat asin()const;
		Mat acos()const;
		Mat atan()const;
		/**
		@brief ���ؾ���ļ�
		*/
		mat_t tr()const;
		/**
		@brief ��������ʽ
		*/
		mat_t det();
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
		mat_t cof(int32 x, int32 y);
		mat_t EigenvalueMax(mat_t offset = 1e-8)const;
		/**
		@brief ���������ȡ�ľ���Ԫ��
		*/
		mat_t RandSample();
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
		@brief ���ط���
		*/
		mat_t D()const;
		Mat add(const Mat &m)const;
		Mat reduce(const Mat &m)const;
		Mat div(const Mat &m)const;
		Mat safediv(const Mat &m, Val def = 0)const;
		Mat mul(const _InputArray &m)const;

		Mat add(mat_t v)const;
		Mat reduce(mat_t v, Dire dire)const;
		Mat div(mat_t v, Dire dire)const;
		Mat mul(mat_t v)const;

		const Array toArray(int type = MAT_T)const;

		/**
		@brief ���������=
		���
		*/
		Mat & operator = (const Mat &temp);
		/**
		@brief ���������=
		���
		*/
		Mat & operator = (const Array &temp);
		/**
		@brief ���������=
		����
		*/
		template<class _Tp> Mat & operator=(const MatCommaInitializer_<_Tp>& temp);
		/**
		@brief ���������==
		�жϾ����Ƿ����
		*/
		bool operator == (const Mat &a)const;
		/**
		@brief ���������!=
		�жϾ����Ƿ����
		*/
		bool operator != (const Mat &a)const;

		mat_t& operator () (int32 i);
		mat_t& operator () (int32 i0, int32 i1);
		mat_t& operator () (int32 i0, int32 i1, int32 i2);
		mat_t& operator () (Point p);
		mat_t& operator () (Point3i p);
		const mat_t& operator () (int32 i)const;
		const mat_t& operator () (int32 i0, int32 i1)const;
		const mat_t& operator () (int32 i0, int32 i1, int32 i2)const;
		const mat_t& operator () (Point p)const;
		const mat_t& operator () (Point3i p)const;
		Mat operator () (Rect roi)const;
		Mat operator () (Range row, Range col)const;
		/**
		@brief ���ؾ����Ӧ�������л���
		@param index	����
		@param rc		������ʽ
		*/
		Mat cut(int32 index, RANK rc)const;
		/**
		@brief ���ؾ����Ӧ�������л���
		@param index	����
		@param rc		������ʽ
		*/
		Mat cut(int32 v1, int32 v2, RANK rc)const;
		mat_t& operator [] (int32 idx) const;

		bool operator > (const Mat &b) const;
		bool operator < (const Mat &b) const;

		//operator Val_ *();
		//operator const Val_ *() const;

		//operator mat_t *();
		//operator const mat_t *() const;

		/**
		@brief ����Ԫ��Ϊ0��w����
		@param w		��������
		*/
		static Mat zeros(int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��h*w����
		@param w		��������
		@param h		��������
		*/
		static Mat zeros(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��h*w*c����
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static Mat zeros(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��size����
		@param size �����С
		*/
		static Mat zeros(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��size����
		@param size �����С
		*/
		static Mat zeros(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ0��size����
		@param size �����С
		*/
		static Mat zeros(Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��w����
		@param v		���Ԫ��
		@param w		��������
		*/
		static Mat value(Val v, int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��h*w����
		@param v		���Ԫ��
		@param w		��������
		@param h		��������
		*/
		static Mat value(Val v, int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��h_*w_*c_����
		@param v		���Ԫ��
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static Mat value(Val v, int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊv��h_*w_*c_����
		@param v		���Ԫ��
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static Mat value(Val v, Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��w����
		@param w		��������
		*/
		static Mat ones(int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��h*w����
		@param w	��������
		@param h	��������
		*/
		static Mat ones(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��h*w*c����
		@param w		��������
		@param h		��������
		@param c		����ͨ����
		*/
		static Mat ones(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��size����
		@param size �����С
		*/
		static Mat ones(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��size����
		@param size �����С
		*/
		static Mat ones(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief ����Ԫ��Ϊ1��size����
		@param size �����С
		*/
		static Mat ones(Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief �������ɵ�n*n*1��λ����
		@param n �����С
		*/
		static Mat eye(int32 n);
		/**
		���ô�ӡ���
		@param w ���
		*/
		static void setPrintW(lzh::uint32 w);
		/**
		���ô�ӡ��Ч����
		@param n λ��
		*/
		static void setPrintSignificantDigits(lzh::uint32 n);
		/**
		���ô�ӡ����
		@param t ��ӡ����
		*/
		static void setPrintType(lzh::PrintType t);
		/**
		������С�ֱ���
		@param v ��Сֵ
		*/
		static void setPrintMinimum(bool flag, lzh::mat_t v);
	protected:
		static lzh::uint32 print_width;
		static lzh::uint32 print_precision;
		static lzh::PrintType print_type;
		static lzh::mat_t minimum;
		static bool setZero;

		void init();
#ifdef LZH_DEBUG
		void checkindex(int32 index);
		void checkindex(int32 index_x, int32 index_y);
#endif // LZH_DEBUG
		void setsize(int32 w, int32 h = 1, int32 c = 1);

		int32 h_;
		int32 w_;
		int32 c_;
		int32 dim;
		MatStep stride;
		mat_t* m_;
		ColorGamut _type;
		bool square;
	private:
		int32 *refcount = 0;
	};

	typedef const Mat& IMat;
	typedef Mat OMat;
	typedef Mat& IOMat;
	typedef Mat& LMat;
	typedef Mat&& RMat;
	//typedef Vec3m Scalar;
	typedef mat_t(*NF)(mat_t);
	typedef mat_t(*Fun)(IMat x);
	typedef OMat(*F)(IMat a, IMat x);
	typedef OMat(*DF)(IMat a, IMat x, IMat dy);

	class LZHAPI MatIter :
		public std::iterator<std::random_access_iterator_tag, Mat>
	{
	public:
		MatIter(const Mat *p = nullptr, RANK dire = COL, int32 idx = 0);
		const MatIter operator = (const MatIter& iter);
		int32 operator - (const MatIter& iter)const;
		int32 operator + (const MatIter& iter)const;
		bool operator != (const MatIter &iter)const;
		bool operator == (const MatIter &iter)const;
		bool operator > (const MatIter &iter);
		bool operator < (const MatIter &iter);
		const MatIter operator ++ ();
		const MatIter operator ++ (int32);
		const MatIter operator + (int32 count);
		/*friend const MatIter operator + (const MatIter & iter, const int32 count)
		{
			MatIter Iter(iter);
			while (count--) {
				Iter->_ptr = Iter->_ptr->next;
			}
			return Iter;
		}*/
		const MatIter operator += (int32 count);
		const MatIter operator -- ();
		const MatIter operator -- (int32);
		const MatIter operator - (int32 count);
		/*friend const MatIter operator - (const MatIter & iter, const int32 count)
		{
			MatIter Iter(iter);
			while (count--) {
				Iter->_ptr = Iter->_ptr->pre;
			}
			return Iter;
		}*/
		const MatIter operator -= (int32 count);
		Mat& operator * ()const;
		Mat* operator ()()const;
		void update();
	private:
		Mat m;
		Mat* p;
		Mat* _ptr;
		RANK flag;
		int32 idx;
	};

	typedef std::vector<lzh::Mat>									vec_mat;
	typedef std::vector<std::vector<lzh::Mat>>						vecs_mat;
}
#include "mat_inl.hpp"
#endif //  __MAT_H__
