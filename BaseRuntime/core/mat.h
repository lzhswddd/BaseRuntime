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
		生成w_*h_的矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		*/
		Mat(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		生成h_*w_*depth的矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		Mat(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		生成size*1的矩阵
		@param size_	矩阵尺寸
		*/
		Mat(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		生成size*1的矩阵
		@param size_	矩阵尺寸
		*/
		Mat(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		生成size的矩阵
		@param size_	矩阵尺寸
		*/
		Mat(Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		生成指定色域, 大小为size的矩阵
		@param colorGamut	指定色域
		*/
		Mat(ColorGamut colorGamut, Size sz);
		/**
		拷贝函数
		*/
		Mat(const MatPtr<mat_t> &src, Size3 sz, bool copyData = false);
		/**
		拷贝函数
		@param src		拷贝对象
		*/
		Mat(const Mat &src);
		/**
		Array转换Mat
		@param src		Array
		*/
		Mat(const Array &src);
		/**
		将矩阵a和b合并(COL为按列合并|ROW为按行合并)
		@param a		输入矩阵1
		@param b		输入矩阵2
		@param merge	合并方式
		*/
		Mat(const Mat &a, const Mat &b, RANK merge);
		/**
		拷贝函数
		*/
		template<typename _Tp> Mat(const _Tp *m, int32 w, int32 h, int32 c = 1);
		/**
		构造函数
		深拷贝m
		@param m 矩阵
		*/
		Mat(const MatCommaInitializer_<mat_t> &m, bool copyData = true);
		/**
		生成1*w*1的向量,元素为data
		@param w		列数
		@param data		矩阵
		*/
		Mat(int32 w, const void *data, bool copyData = false);
		/**
		生成h*w*1的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param data		矩阵元素
		*/
		Mat(int32 w, int32 h, const void *data, bool copyData = false);
		/**
		生成h*w*c的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		@param data		矩阵元素
		*/
		Mat(int32 w, int32 h, int32 c, const void *data, bool copyData = false);
		/**
		生成size的矩阵,元素为data
		@param size		矩阵尺寸
		@param data		矩阵元素
		*/
		Mat(Size size, const void *data, bool copyData = false);
		/**
		生成size*c的矩阵,元素为data
		@param size		矩阵尺寸
		@param data		矩阵元素
		*/
		Mat(Size size, int32 c, const void *data, bool copyData = false);
		/**
		生成size的矩阵,元素为data
		@param size		矩阵尺寸
		@param data		矩阵元素
		*/
		Mat(Size3 size, const void *data, bool copyData = false);
		/**
		生成size的矩阵,元素为data
		@param size		矩阵尺寸
		@param data		矩阵元素
		@param refcount	矩阵计数器
		*/
		Mat(Size3 size, MatStep stride, const void *data, const int *refcount);
		/**
		生成size的矩阵,元素为data
		@param size		矩阵尺寸
		@param data		矩阵元素
		@param refcount	矩阵计数器
		*/
		Mat(int32 w, int32 h, int32 c, MatStep stride, const void *data, const int *refcount);
		/**
		生成h*w*c的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		@param stride		步长
		@param data		矩阵元素
		*/
		Mat(int32 w, int32 h, int32 c, MatStep stride, const mat_t *data);
		/**
		生成size的矩阵
		Mat m(Size(10), 1,2,3,4,5,6,7,8,9,10);
		@param size		矩阵大小
		@param v		第一个参数(确定类型)
		@param ...		不定参数
		*/
		template <typename ... _Tp>
		Mat(Size3 size, const _Tp & ... args);
		template<typename _Tp> Mat(std::initializer_list<_Tp> _Ilist);
		template<typename _Tp> Mat(std::initializer_list<std::initializer_list<_Tp>> _Ilist);
		template<typename _Tp> Mat(std::initializer_list<std::initializer_list<std::initializer_list<_Tp>>> _Ilist);
		/**
		向量转矩阵
		@param vec		向量
		@param dirc		矩阵方向
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
		生成w的向量
		@param w		列数
		*/
		void create(int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		生成w*h的矩阵
		@param w		列数
		@param h		行数
		*/
		void create(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		生成w*h*c的张量
		@param w		列数
		@param h		行数
		@param c		通道数
		*/
		void create(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		生成size的矩阵
		@param size		矩阵大小
		*/
		void create(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		生成size的矩阵
		@param size		矩阵大小
		*/
		void create(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		生成size的张量
		@param size		矩阵大小
		*/
		void create(Size3 size, ColorGamut cg = ColorGamutCustom);
		void createColorGamut(ColorGamut colorGamut, Size sz, int32 flags);
		/**
		@brief 返回色域
		*/
		ColorGamut colorGamut()const;
		/**
		@brief 返回内存计数
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
		@brief 内存长度
		*/
		uint32 memsize()const;
		/**
		@brief 检查维度
		*/
		void DimCheck()const;
		/**
		@brief 返回矩阵尺寸(h_,w_,c_)
		*/
		Size3 size3()const;
		/**
		@brief 返回矩阵偏移
		*/
		int32 total()const;
		int32 element()const;
		int32 total_()const;
		MatStep step()const;
		Point center()const;
		/**
		@brief 返回维度
		*/
		int32 dims()const;
		/**
		@brief 返回类型
		*/
		int32 depth()const;
		/**
		@brief 返回类型
		*/
		int32 type()const;
		/**
		@brief 返回行数
		*/
		int32 rows()const;
		/**
		@brief 返回列数
		*/
		int32 cols()const;
		/**
		@brief 返回通道数
		*/
		int32 channels()const;
		int32 s()const;
		int32 o()const;
		int32 h()const;
		int32 w()const;
		int32 c()const;
		/**
		@brief 返回行秩
		*/
		int32 rank()const;
		/**
		@brief 返回矩阵大小Size(h_,w_)
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
		保存矩阵
		@param file		保存文件名
		@param binary	选择文本还是二进制
		binary = false	选择文本
		binary = true	选择二进制
		*/
		void save(std::string file, bool binary = true)const;
		/**
		二进制保存矩阵
		@param file		保存文件指针
		*/
		void save(FILE *file)const;
		/**
		读取矩阵
		@param file		读取文件名
		只支持二进制读取
		*/
		void load(std::string file);
		/**
		读取矩阵
		@param file		读取文件指针
		只支持二进制读取
		*/
		void load(FILE *file);
		/**
		@brief 返回矩阵大小(h_*w_*c_)
		*/
		int32 len()const;
		/**
		@brief 返回矩阵状态
		0为矩阵
		-1为空矩阵
		-2为非方阵
		*/
		int32 enable()const;
		/**
		@brief 返回矩阵是否为空
		*/
		bool empty()const;
		/**
		@brief 返回矩阵是否为空
		*/
		bool isDeepCopy()const;
		/**
		@brief 返回矩阵是否为矩阵
		*/
		bool isSquare()const;
		/**
		拷贝数据到mat
		@param mat		输入
		*/
		void copyTo(Mat& mat)const;
		void setTo(Mat& mat)const;
		void setTo(Mat&& mat)const;
		void convertTo(Mat &m, int32 type)const;
		/**
		在矩阵最左边或最右边添加一列1
		@param dire		选择添加方式
		*/
		void setAddOnes(Dire dire = RIGHT);
		/**
		在矩阵最左边或最右边添加一列0
		@param dire		选择添加方式
		*/
		void setAddZeros(Dire dire = RIGHT);
		/**
		释放内存
		*/
		void release();
		/**
		@brief 按索引返回矩阵元素
		@param w		索引
		*/
		mat_t& at(int32 w);
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		*/
		mat_t& at(int32 h, int32 w);
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		@param c		通道索引
		*/
		mat_t& at(int32 h, int32 w, int32 c);
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		mat_t& at(Point p);
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		mat_t& at(Point3i p);
		/**
		@brief 按索引返回矩阵元素
		@param w		索引
		*/
		const mat_t & at(int32 w)const;
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		*/
		const mat_t & at(int32 h, int32 w)const;
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		@param c		通道索引
		*/
		const mat_t & at(int32 h, int32 w, int32 c)const;
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		const mat_t & at(Point p)const;
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		const mat_t & at(Point3i p)const;
		/**
		@brief 将索引转换为对应矩阵列索引
		@param index	索引
		*/
		int32 toX(int32 index)const;
		/**
		@brief 将索引转换为对应矩阵行索引
		@param index	索引
		*/
		int32 toY(int32 index)const;
		/**
		@brief 将索引转换为对应矩阵通道索引
		@param index	索引
		*/
		int32 toZ(int32 index)const;

		/**
		@brief 返回矩阵与value相等的第一个元素索引
		@param value	元素
		*/
		int32 find(mat_t value)const;
		/**
		@brief 返回矩阵元素最大值的索引
		*/
		int32 maxAt()const;
		/**
		@brief 返回矩阵元素最小值的索引
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
		@brief 返回矩阵的最大值
		@param is_abs	是否绝对值
		*/
		mat_t max(bool is_abs = false)const;
		/**
		@brief 返回矩阵的最小值
		@param is_abs	是否绝对值
		*/
		mat_t min(bool is_abs = false)const;
		/**
		@brief 返回矩阵元素最大值
		*/
		mat_t& findmax();
		/**
		@brief 返回矩阵元素最小值
		*/
		mat_t& findmin();
		/**
		@brief 将矩阵按索引区域拷贝元素到src矩阵中
		@param src			被拷贝矩阵
		@param Row_Start	截取行初始索引值
		@param Row_End		截取行结束索引值
		@param Col_Start	截取列初始索引值
		@param Col_End		截取列结束索引值
		*/
		void copy(Mat &src, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End)const;
		/**
		@brief 将矩阵拷贝到src
		@param src 被拷贝矩阵
		*/
		void swap(Mat &src);
		/**
		@brief mChannel 将src覆盖到第c_通道
		@param src		矩阵
		@param c_	通道数
		*/
		void mChannel(const Mat &src, int32 c);
		/**
		@brief mChannel 将src覆盖到第c_通道
		@param src		矩阵
		@param c_	通道数
		*/
		void mChannel(const Mat &src, int32 w, int32 h);
		/**
		@brief 设置矩阵维度
		不允许改变矩阵长度
		@param size		矩阵大小
		*/
		Mat reshape(Size3 size);
		/**
		@brief 设置矩阵维度
		不允许改变矩阵长度
		@param h_		矩阵行数
		@param w_		矩阵列数
		@param c_	矩阵通道
		*/
		Mat reshape(int32 w, int32 h = 1, int32 c = 1);
		/**
		@brief 设置矩阵大小
		如果矩阵原大小不等于h_*w_*1则元素全部重置为0
		@param h_	矩阵行数
		@param w_	矩阵列数
		*/
		bool setSize(int32 w, int32 h, int32 c);
		/**
		@brief 拷贝矩阵src
		@param src	拷贝矩阵
		*/
		void setvalue(const Mat &src);
		/**
		设置色域
		@param cg	指定色域
		*/
		void setColorGamut(ColorGamut cg);
		/**
		@brief 设置逆矩阵
		*/
		void setInv();
		/**
		@brief 设置矩阵的num次幂
		@param num	次幂
		*/
		void setPow(mat_t num);
		/**
		@brief 设置取反
		*/
		void setOpp();
		/**
		@brief 设置单位矩阵
		*/
		void setIden();
		/**
		@brief 设置伴随矩阵
		*/
		void setAdj();
		/**
		@brief 设置转置矩阵
		*/
		void setTran();

		std::string infomation()const;
		/**
		@brief 命令行输出矩阵
		*/
		void showInfomation()const;
		/**
		@brief 命令行输出矩阵
		*/
		void show(bool showInfo = false)const;
		/**
		@brief 输出矩阵
		*/
		std::ostream & show(std::ostream & out, bool showInfo)const;

		/**
		@brief 返回h行矩阵
		@param 索引
		*/
		Mat row(int32 h);
		/**
		@brief 返回h行矩阵
		@param 索引
		*/
		const Mat row(int32 h)const;
		/**
		@brief 返回w列矩阵
		@param 索引
		*/
		Mat col(int32 w);
		/**
		@brief 返回w列矩阵
		@param 索引
		*/
		const Mat col(int32 w)const;
		/**
		@brief 返回c通道矩阵
		@param 通道索引
		*/
		Mat channel(int32 c);
		/**
		@brief 返回c通道矩阵
		@param 通道索引
		*/
		const Mat channel(int32 c)const;

		Mat addvalue(Dire dire, Val_ v) const;
		/**
		@brief 在矩阵最左边或最右边添加一列1
		@param dire		选择添加方式
		*/
		Mat addones(Dire dire = RIGHT)const;
		/**
		@brief 在矩阵最左边或最右边添加一列0
		@param dire		选择添加方式
		*/
		Mat addzeros(Dire dire = RIGHT)const;
		/**
		@brief 返回start到end矩阵
		@param start	开始索引
		@param end		结束索引
		*/
		Mat range(int32 start, int32 end)const;
		/**
		@brief 返回roi范围的矩阵
		@param roi	开始索引
		*/
		Mat range(Rect roi)const;
		/**
		@brief 返回roi范围的矩阵
		@param roi	开始索引
		*/
		Mat range(int32 h_start, int32 h_end, int32 w_start, int32 w_end)const;
		/**
		@brief 返回h_start到h_end行矩阵
		@param h_start	行开始索引
		@param h_end	行结束索引
		*/
		Mat rowRange(int32 h_start, int32 h_end)const;
		/**
		@brief 返回w_start到w_end列矩阵
		@param w_start	列开始索引
		@param w_end	列结束索引
		*/
		Mat colRange(int32 w_start, int32 w_end)const;
		/**
		@brief 返回c_start到c_end通道矩阵
		@param c_start	通道开始索引
		@param c_end	通道结束索引
		*/
		Mat channelRange(int32 c_start, int32 c_end)const;
		/**
		@brief 返回深拷贝矩阵
		*/
		Mat clone()const;
		/**
		@brief 返回取反矩阵
		*/
		Mat opp()const;
		/**
		@brief 返回绝对值矩阵
		*/
		Mat abs()const;
		/**
		@brief 返回按num次幂矩阵
		@param num 次幂
		*/
		Mat mPow(int32 num)const;
		/**
		@brief 返回按num次幂矩阵
		@param num 次幂
		*/
		Mat pow(Val_ num)const;
		/**
		@brief 返回按元素取指数矩阵
		*/
		Mat exp()const;
		/**
		@brief 返回按元素取对数矩阵
		*/
		Mat log()const;
		/**
		@brief 返回按元素取开方矩阵
		*/
		Mat sqrt()const;
		/**
		@brief 返回伴随矩阵
		*/
		Mat adj()const;
		/**
		@brief 返回转置矩阵
		*/
		Mat t()const;
		/**
		@brief 返回逆矩阵
		*/
		Mat inv()const;
		/**
		@brief 返回逆矩阵
		*/
		Mat diag(int32 k = 0)const;
		/**
		@brief 返回逆序矩阵
		矩阵必须是向量
		*/
		Mat reverse()const;
		/**
		@brief 返回特征向量(未完成)
		*/
		Mat EigenvectorsMax(mat_t offset = 1e-8)const;
		/**
		@brief sigmoid函数
		*/
		Mat sigmoid()const;
		/**
		@brief tanh函数
		*/
		Mat tanh()const;
		/**
		@brief relu函数
		*/
		Mat relu()const;
		/**
		@brief elu函数
		*/
		Mat elu()const;
		/**
		@brief selu函数
		*/
		Mat selu()const;
		/**
		@brief leaky_relu函数
		*/
		Mat leaky_relu()const;
		/**
		@brief softmax函数
		*/
		Mat softmax()const;

		Mat sin()const;
		Mat cos()const;
		Mat tan()const;
		Mat asin()const;
		Mat acos()const;
		Mat atan()const;
		/**
		@brief 返回矩阵的迹
		*/
		mat_t tr()const;
		/**
		@brief 返回行列式
		*/
		mat_t det();
		/**
		@brief 返回num范数
		@param num 几范数
		*/
		mat_t norm(int32 num = 1)const;
		/**
		@brief 返回对应索引的余子式
		@param x 列索引
		@param y 行索引
		*/
		mat_t cof(int32 x, int32 y);
		mat_t EigenvalueMax(mat_t offset = 1e-8)const;
		/**
		@brief 返回随机抽取的矩阵元素
		*/
		mat_t RandSample();
		/**
		@brief 返回矩阵元素和
		@param num	设置次幂
		@param _abs 是否取绝对值
		*/
		mat_t sum(int32 num = 1, bool _abs = false)const;
		/**
		@brief 返回平均值
		*/
		mat_t mean()const;
		/**
		@brief 返回标准差
		*/
		mat_t std()const;
		/**
		@brief 返回方差
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
		@brief 重载运算符=
		深拷贝
		*/
		Mat & operator = (const Mat &temp);
		/**
		@brief 重载运算符=
		深拷贝
		*/
		Mat & operator = (const Array &temp);
		/**
		@brief 重载运算符=
		拷贝
		*/
		template<class _Tp> Mat & operator=(const MatCommaInitializer_<_Tp>& temp);
		/**
		@brief 重载运算符==
		判断矩阵是否相等
		*/
		bool operator == (const Mat &a)const;
		/**
		@brief 重载运算符!=
		判断矩阵是否不相等
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
		@brief 返回矩阵对应索引的列或行
		@param index	索引
		@param rc		索引方式
		*/
		Mat cut(int32 index, RANK rc)const;
		/**
		@brief 返回矩阵对应索引的列或行
		@param index	索引
		@param rc		索引方式
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
		@brief 返回元素为0的w向量
		@param w		向量长度
		*/
		static Mat zeros(int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的h*w矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		*/
		static Mat zeros(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的h*w*c矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static Mat zeros(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的size矩阵
		@param size 矩阵大小
		*/
		static Mat zeros(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的size矩阵
		@param size 矩阵大小
		*/
		static Mat zeros(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的size矩阵
		@param size 矩阵大小
		*/
		static Mat zeros(Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的w向量
		@param v		填充元素
		@param w		向量长度
		*/
		static Mat value(Val v, int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的h*w矩阵
		@param v		填充元素
		@param w		矩阵列数
		@param h		矩阵行数
		*/
		static Mat value(Val v, int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的h_*w_*c_矩阵
		@param v		填充元素
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static Mat value(Val v, int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的h_*w_*c_矩阵
		@param v		填充元素
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static Mat value(Val v, Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的w向量
		@param w		向量长度
		*/
		static Mat ones(int32 w, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的h*w矩阵
		@param w	矩阵列数
		@param h	矩阵行数
		*/
		static Mat ones(int32 w, int32 h, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的h*w*c矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static Mat ones(int32 w, int32 h, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的size矩阵
		@param size 矩阵大小
		*/
		static Mat ones(Size size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的size矩阵
		@param size 矩阵大小
		*/
		static Mat ones(Size size, int32 c, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的size矩阵
		@param size 矩阵大小
		*/
		static Mat ones(Size3 size, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回生成的n*n*1单位矩阵
		@param n 矩阵大小
		*/
		static Mat eye(int32 n);
		/**
		设置打印宽度
		@param w 宽度
		*/
		static void setPrintW(lzh::uint32 w);
		/**
		设置打印有效数字
		@param n 位数
		*/
		static void setPrintSignificantDigits(lzh::uint32 n);
		/**
		设置打印方法
		@param t 打印方法
		*/
		static void setPrintType(lzh::PrintType t);
		/**
		设置最小分辨率
		@param v 最小值
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
