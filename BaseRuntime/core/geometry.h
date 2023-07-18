#pragma once
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "array.h"

namespace lzh
{
	class LZHAPI Line :public Vec2m
	{
	public:
		explicit Line();
		Line(IMat a);
		Line(const Vec2m& v);
		Line(Val_ a, Val_ b);
		Line(std::initializer_list<mat_t> list);
		Line(Point2m p0, Point2m p1);
		Val_ a()const;
		Val_ b()const;
		Vec2m operator ()(Val_ x)const;
		Mat operator ()(Mat X)const;
		static Vec2m fit(IMat x, IMat y, Mat* r = nullptr);
	};
	class LZHAPI Ellipse : public Vec4m
	{
	public:
		explicit Ellipse();
		Ellipse(IMat a);
		Ellipse(const Vec4m & v);
		Ellipse(Val_ x0, Val_ y0, Val_ a, Val_ b);
		Ellipse(std::initializer_list<mat_t> list);
		template<typename _Tp> Ellipse(Point2_<_Tp> center, Val_ a, Val_ b) : Ellipse(center.x, center.y, a, b) {}
		Val_ S()const;
		Val_ C()const;
		Val_ a()const;
		Val_ b()const;
		Val_ x(Val_ y)const;
		Val_ y(Val_ x)const;
		Point2m center()const;
		Vec2m operator ()(Val_ theta)const;
		Mat operator ()(Mat theta)const;
		virtual bool contains(Point2m p)const;
		static Vec4m fit(IMat x, IMat y, Mat* r = nullptr);
	};
	class LZHAPI Circle : public Ellipse
	{
	public:
		explicit Circle() : Ellipse() {}
		Circle(IMat a) : Ellipse(a) {}
		Circle(const Vec3m & v) : Ellipse(Vec4m(v[0], v[1], v[2], v[2])) {}
		Circle(Val_ x0, Val_ y0, Val_ radius = 1) : Ellipse(x0, y0, radius, radius) {}
		Circle(std::initializer_list<mat_t> list) : Ellipse(list) {}
		template<typename _Tp> Circle(Point2_<_Tp> center, Val_ radius = 1) : Ellipse(center, radius, radius) {}
		static Vec3m fit(IMat x, IMat y, Mat* r = nullptr);
		bool contains(Point2m p)const;
		Val_ radius()const;
	};
	class LZHAPI Ellipsoid : public Vec<mat_t, 6>
	{
	public:
		explicit Ellipsoid();
		Ellipsoid(IMat a);
		Ellipsoid(const Vec<mat_t, 6> & v);
		Ellipsoid(Val_ x0, Val_ y0, Val_ z0, Val_ a, Val_ b, Val_ c);
		Ellipsoid(std::initializer_list<mat_t> list);
		template<typename _Tp> Ellipsoid(Point3_<_Tp> center, Val_ a, Val_ b, Val_ c) : Vec<mat_t, 6>(center.x, center.y, center.z, a, b, c) {}
		Val_ V()const;
		Val_ S()const;
		Vec3m center()const;
		Vec3m operator ()(Val_ theta, Val_ fai)const;
		Mat operator ()(Mat theta_fai)const;
		//static Vec<mat_t, 6> fit(IMat x, IMat y, Mat* r = nullptr);
	};
	class LZHAPI Sphere : public Ellipsoid
	{
	public:
		explicit Sphere() : Ellipsoid() {}
		Sphere(IMat a) : Ellipsoid(a) {}
		Sphere(const Vec<mat_t, 4> & v) : Ellipsoid(Vec<mat_t, 6>(v[0], v[1], v[2], v[3], v[3], v[3])) {}
		Sphere(Val_ x0, Val_ y0, Val_ z0, Val_ radius = 1) : Ellipsoid(x0, y0, z0, radius, radius, radius) {}
		Sphere(std::initializer_list<mat_t> list) :Ellipsoid(list) {}
		template<typename _Tp> Sphere(Point3_<_Tp> center, Val_ radius = 1) : Vec3m(center, radius, radius, radius) {}
		//static Vec<mat_t, 4> fit(IMat x, IMat y, Mat* r = nullptr);
	};
	class LZHAPI Polynomial
	{
	public:
		explicit Polynomial();
		Polynomial(IMat a);
		Polynomial(int power);
		Polynomial(std::initializer_list<mat_t> list);
		template <typename ... _Tp> Polynomial(int power, const _Tp & ... args) : param(power, args) {}
		Val_ operator ()(Val_ x)const;
		OMat operator ()(IMat x)const;
		Mat& get();
		operator Mat& () { return param; }
		operator const Mat& ()const { return param; }

		static Polynomial fit(IMat x, IMat y, int32 power, Mat* r = nullptr);
	protected:
		Mat param;
	};

	/**
	B-����������
	*/
	class LZHAPI BSpline
	{
	public:
		/**
		UNIFORM			����B��������
		QUASI_UNIFORM	׼����B��������
		*/
		enum BSplineType
		{
			UNIFORM = 0,
			QUASI_UNIFORM
		};

		explicit BSpline();
		BSpline(BSplineType type, int32 k = 1, IMat p = Mat());
		/**
		@brief ����B����������t�ĵ�
		t�Ķ�������[0,1]
		@param t	���������Ա���
		*/
		void setCtrlPoint(IMat p);
		/**
		ע��B�������߽ڵ�����
		@param node	�ڵ�����
		ȱʡʱ�����������Զ�����
		*/
		void NodeVector(IMat node = Mat());
		/**
		@brief ����B�������߿��Ƶ�
		*/
		Mat CtrlPoint()const;
		/**
		@brief ����B�������߽ڵ�����
		*/
		Mat Node()const;
		/**
		@brief ����B�������ߵ㼯
		@param T	���������Ա���
		TΪ��������[0,1]�ĵ�������
		*/
		Mat BPiont(IMat T)const;
		/**
		@brief ����B�������ߵ㼯
		@param number	[0,1]�ȼ��number�ȷ�
		*/
		Mat BPoint(int32 number)const;
		/**
		@brief ����B����������t�ĵ�
		t�Ķ�������[0,1]
		@param t	���������Ա���
		*/
		Mat operator ()(Val t)const;
		/**
		@brief ����B��������
		�㼯���B��������
		@param P	�㼯
		@param n	���Ƶ�����
		@param k	k��B��������
		*/
		static BSpline fit(IMat P, int32 n, int32 k);
	protected:
		mat_t BF(int32 i, int32 k, Val t)const;
		Mat BaseFunction(Val t)const;
	private:
		int32 k;
		Mat P;
		Mat nodevector;
		BSplineType type;
	};
	class LZHAPI ChessBoard
	{
	public:
		explicit ChessBoard();
		ChessBoard(const Array& chessboard, mat_t squaresize, Size concersize);
		ChessBoard(const Array& chessboard, mat_t squaresize, Size concersize, Size2f realsize);

		void setSquareSize(mat_t size);
		void setConcerSize(Size size);
		void setRealSize(Size2f size);
		void setImage(const Array& chessboard);

		mat_t getSquareSize()const;
		Size getConcerSize()const;
		Size2f getRealSize()const;
		Array getImage()const;

		static ChessBoard generateChessboard(mat_t square_size, Size2f real_size, bool lockConcer);
		static const mat_t scale;
	protected:
		mat_t squareSize;
		Size concerSize;
		Size2f realSize;
		Array chessBoard;
	};
}

#endif //!__GEOMETRY_H__
