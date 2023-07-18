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
		生成w_的向量
		@param w		向量长度
		*/
		Array(int32 w, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成w_*h_的矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		*/
		Array(int32 w, int32 h, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成h_*w_*depth的矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		Array(int32 w, int32 h, int32 c, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成size*1的矩阵
		@param size	矩阵尺寸
		*/
		Array(Size size, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成size*c的矩阵
		@param size	矩阵尺寸
		*/
		Array(Size size, int32 c, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成size的矩阵
		@param size	矩阵尺寸
		*/
		Array(Size3 size, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成指定色域, 大小为size的矩阵
		@param colorGamut	指定色域
		*/
		Array(ColorGamut colorGamut, Size size, int32 flags);
		/**
		拷贝函数
		*/
		explicit Array(const Vector &src, bool copyData = false);
		/**
		拷贝函数
		*/
		Array(const Vector &src, Size3 sz, bool copyData = false);
		/**
		深拷贝函数
		*/
		Array(const void * p, Size3 sz, int32 flags);
		/**
		拷贝函数
		@param src		拷贝对象
		*/
		Array(const Array &src);
		/**
		引用对象
		@param inputarray	接口输入
		*/
		Array(const _InputArray & inputarray);
		/**
		将矩阵a和b合并(COL为按列合并|ROW为按行合并)
		@param a		输入矩阵1
		@param b		输入矩阵2
		@param merge	合并方式
		*/
		Array(const lzh::Array &a, const lzh::Array &b, RANK merge);
		/**
		构造函数
		深拷贝m
		@param m 矩阵
		*/
		template<typename _Tp> Array(const MatCommaInitializer_<_Tp> &m);
		/**
		生成1*w*1的向量,元素为data
		@param w		列数
		@param data		矩阵
		*/
		Array(int32 w, const void *data, int32 flags, bool copyData = false);
		/**
		生成h*w*1的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param data		矩阵元素
		*/
		Array(int32 w, int32 h, const void *data, int32 flags, bool copyData = false);
		/**
		生成h*w*c的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		@param data		矩阵元素
		*/
		Array(int32 w, int32 h, int32 c, const void *data, int32 flags, bool copyData = false);
		/**
		生成size的矩阵,元素为data
		@param size		尺寸
		@param data		矩阵元素
		@param refcount	计数器
		*/
		Array(Size3 size, MatStep step, const void *data, const int *refcount, int32 flags);
		/**
		生成h*w*c的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		@param data		矩阵元素
		@param refcount	计数器
		*/
		Array(int32 w, int32 h, int32 c, MatStep step, const void *data, const int *refcount, int32 flags);
		/**
		生成size的向量,元素为data
		@param size		尺寸
		@param data		矩阵
		*/
		Array(Size size, const void *data, int32 flags, bool copyData = false);
		/**
		生成size的向量,元素为data
		@param size		尺寸
		@param data		矩阵
		*/
		Array(Size size, int32 c, const void *data, int32 flags, bool copyData = false);
		/**
		生成size的向量,元素为data
		@param size		尺寸
		@param data		矩阵
		*/
		Array(Size3 size, const void *data, int32 flags, bool copyData = false);
		/**
		生成h*w*c的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		@param step		步长
		@param data		矩阵元素
		*/
		Array(int32 w, int32 h, int32 c, MatStep step, const void *data, int32 flags);
		/**
		生成h*w*c的矩阵,元素为data
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		@param step		步长
		@param data		矩阵元素
		*/
		Array(int32 w, int32 h, int32 step, const uint8 *data, ColorGamut colorgaumt);
		/**
		生成size的矩阵
		Mat m(Size(10), 1,2,3,4,5,6,7,8,9,10);
		@param size		矩阵大小
		@param v		第一个参数(确定类型)
		@param ...		不定参数
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
		生成w的向量
		@param w		列数
		*/
		void create(int32 w, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成w*h的矩阵
		@param w		列数
		@param h		行数
		*/
		void create(int32 w, int32 h, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成w*h*c的张量
		@param w		列数
		@param h		行数
		@param c		通道数
		*/
		void create(int32 w, int32 h, int32 c, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成size的矩阵
		@param size		矩阵大小
		*/
		void create(Size size, int32 flags, ColorGamut cg = ColorGamutCustom);
		/**
		生成size的张量
		@param size		矩阵大小
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
		@brief 内存长度
		*/
		uint32 memsize()const;
		/**
		@brief 内存深度
		*/
		uint32 memdepth()const;
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
		/**
		@brief 返回矩阵偏移
		*/
		int32 total_()const;
		/**
		@brief 返回维度
		*/
		int32 dims()const;
		/**
		@brief 返回步长
		*/
		MatStep stride()const;
		/**
		@brief 矩形
		*/
		Rect rect()const;
		/**
		@brief 中心
		*/
		Point center()const;
		/**
		@brief 返回类型
		*/
		int32 depth()const;
		/**
		@brief 返回类型
		*/
		int32 type()const;
		/**
		@brief 返回通道数
		*/
		int32 channels()const;
		/**
		@brief 返回行秩
		*/
		int32 rank()const;
		/**
		@brief 返回色域
		*/
		ColorGamut colorGamut()const;
		/**
		@brief 返回色域
		*/
		ColorGamut format()const;
		/**
		@brief 返回矩阵大小Size(w_,h_)
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
		@brief 返回矩阵大小(h_*w_*c_)
		*/
		int32 elementlen()const;
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
		@brief 返回矩阵是否为矩阵
		*/
		bool isSquare()const;
		/**
		数据深拷贝到mat
		@param mat		输入
		*/
		void copyTo(Array& mat)const;
		/**
		数据覆盖到mat
		@param mat		输入
		*/
		void setTo(Array mat)const;
		/**
		数据转换
		@param m		输出
		@param type		数据类型
		*/
		void convertTo(Array &m, int32 type)const;
		/**
		数据转换
		@brief 返回type数据类型矩阵
		@param type		数据类型
		*/
		const Array convert(int32 type)const;
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
		template<typename _Tp> _Tp& at(int32 w);
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		*/
		template<typename _Tp> _Tp& at(int32 h, int32 w);
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		@param c		通道索引
		*/
		template<typename _Tp> _Tp& at(int32 h, int32 w, int32 c);
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		template<typename _Tp> _Tp& at(Point p);
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		template<typename _Tp> _Tp& at(Point3i p);
		/**
		@brief 按索引返回矩阵元素
		@param w		索引
		*/
		template<typename _Tp> const _Tp& at(int32 w)const;
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		*/
		template<typename _Tp> const _Tp& at(int32 h, int32 w)const;
		/**
		@brief 按索引返回矩阵元素
		@param w		列索引
		@param h		行索引
		@param c		通道索引
		*/
		template<typename _Tp> const _Tp& at(int32 h, int32 w, int32 c)const;
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		template<typename _Tp> const _Tp& at(Point p)const;
		/**
		@brief 返回对应索引元素
		@param p 索引
		*/
		template<typename _Tp> const _Tp& at(Point3i p)const;
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
		template<typename _Tp>
		int32 find(_Tp value)const;
		/**
		@brief 返回矩阵元素最大值的索引
		*/
		int32 maxAt(mat_t *v = nullptr)const;
		/**
		@brief 返回矩阵元素最小值的索引
		*/
		int32 minAt(mat_t *v = nullptr)const;
		/**
		@brief 返回矩阵是否包含value
		@param value	元素
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
		@brief 返回矩阵的最大值
		@param is_abs	是否绝对值
		*/
		mat_t max(Array mask = Array())const;
		/**
		@brief 返回矩阵的最大值
		@param is_abs	是否绝对值
		*/
		mat_t max(bool is_abs, Array mask = Array())const;
		/**
		@brief 返回矩阵的最大值
		@param is_abs	是否绝对值
		*/
		mat_t min(Array mask = Array())const;
		/**
		@brief 返回矩阵的最小值
		@param is_abs	是否绝对值
		*/
		mat_t min(bool is_abs, Array mask = Array())const;
		/**
		@brief 返回矩阵元素最大值
		*/
		template<typename _Tp> _Tp& findmax();
		/**
		@brief 返回矩阵元素最小值
		*/
		template<typename _Tp> _Tp& findmin();
		/**
		@brief 将矩阵按索引区域拷贝元素到src矩阵中
		@param src			被拷贝矩阵
		@param Row_Start	截取行初始索引值
		@param Row_End		截取行结束索引值
		@param Col_Start	截取列初始索引值
		@param Col_End		截取列结束索引值
		*/
		void copy(Array &src, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End)const;
		void copy(Array &src, Rect iou)const;
		/**
		@brief 将矩阵拷贝到src
		@param src 被拷贝矩阵
		*/
		void swap(Array &src);
		/**
		@brief mChannel 将src覆盖到第c_通道
		@param src		矩阵
		@param c_	通道数
		*/
		void mChannel(const lzh::Array &src, int32 c);
		/**
		@brief mChannel 将src覆盖到第c_通道
		@param src		矩阵
		@param c_	通道数
		*/
		void mChannel(const lzh::Array &src, int32 w, int32 h);
		/**
		@brief 设置矩阵维度
		不允许改变矩阵长度
		@param size		矩阵大小
		*/
		Array reshape(Size3 size);
		/**
		@brief 设置矩阵维度
		不允许改变矩阵长度
		@param h_		矩阵行数
		@param w_		矩阵列数
		@param c_	矩阵通道
		*/
		Array reshape(int32 w, int32 h, int32 c);
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
		void setvalue(const lzh::Array &src);
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
		/**
		设置色域
		@param cg	指定色域
		*/
		void setColorGamut(ColorGamut cg);


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
		Array row(int32 h);
		/**
		@brief 返回h行矩阵
		@param 索引
		*/
		const Array row(int32 h)const;
		/**
		@brief 返回w列矩阵
		@param 索引
		*/
		Array col(int32 w);
		/**
		@brief 返回w列矩阵
		@param 索引
		*/
		const Array col(int32 w)const;
		/**
		@brief 返回c通道矩阵
		@param 通道索引
		*/
		Array channel(int32 c);
		/**
		@brief 返回c通道矩阵
		@param 通道索引
		*/
		const Array channel(int32 c)const;
		template<typename _Tp> const Array addvalue(Dire dire, _Tp v) const;
		/**
		@brief 在矩阵最左边或最右边添加一列1
		@param dire		选择添加方式
		*/
		const Array addones(Dire dire = RIGHT)const;
		/**
		@brief 在矩阵最左边或最右边添加一列0
		@param dire		选择添加方式
		*/
		const Array addzeros(Dire dire = RIGHT)const;
		/**
		@brief 返回start到end矩阵
		@param start	开始索引
		@param end		结束索引
		*/
		const Array range(int32 start, int32 end)const;
		/**
		@brief 返回roi范围的矩阵
		@param roi	开始索引
		*/
		const Array range(Rect roi)const;
		/**
		@brief 返回roi范围的矩阵
		@param roi	开始索引
		*/
		const Array range(int32 h_start, int32 h_end, int32 w_start, int32 w_end)const;
		/**
		@brief 返回h_start到h_end行矩阵
		@param h_start	行开始索引
		@param h_end	行结束索引
		*/
		const Array rowRange(int32 h_start, int32 h_end)const;
		/**
		@brief 返回w_start到w_end列矩阵
		@param w_start	列开始索引
		@param w_end	列结束索引
		*/
		const Array colRange(int32 w_start, int32 w_end)const;
		/**
		@brief 返回c_start到c_end通道矩阵
		@param c_start	通道开始索引
		@param c_end	通道结束索引
		*/
		const Array channelRange(int32 c_start, int32 c_end)const;
		/**
		@brief 返回深拷贝矩阵
		*/
		const Array clone()const;
		/**
		@brief 返回取反矩阵
		*/
		const Array opp()const;
		/**
		@brief 返回绝对值矩阵
		*/
		const Array abs()const;
		/**
		@brief 返回按num次幂矩阵
		@param num 次幂
		*/
		const Array mPow(int32 num)const;
		/**
		@brief 返回按num次幂矩阵
		@param num 次幂
		*/
		const Array pow(mat_t num)const;
		/**
		@brief 返回按元素取指数矩阵
		*/
		const Array exp()const;
		/**
		@brief 返回按元素取对数矩阵
		*/
		const Array log()const;
		/**
		@brief 返回按元素取开方矩阵
		*/
		const Array sqrt()const;
		/**
		@brief 返回伴随矩阵
		*/
		const Array adj()const;
		/**
		@brief 返回转置矩阵
		*/
		const Array t()const;
		/**
		@brief 返回逆矩阵
		*/
		const Array inv()const;
		/**
		@brief 返回逆矩阵
		*/
		const Array diag(int32 k = 0)const;
		/**
		@brief 返回逆序矩阵
		矩阵必须是向量
		*/
		const Array reverse()const;
		/**
		@brief 返回特征向量(未完成)
		*/
		const Array EigenvectorsMax(mat_t offset = 1e-8)const;
		/**
		@brief sigmoid函数
		*/
		const Array sigmoid()const;
		/**
		@brief tanh函数
		*/
		const Array tanh()const;
		/**
		@brief relu函数
		*/
		const Array relu()const;
		/**
		@brief elu函数
		*/
		const Array elu()const;
		/**
		@brief selu函数
		*/
		const Array selu()const;
		/**
		@brief leaky_relu函数
		*/
		const Array leaky_relu()const;
		/**
		@brief softmax函数
		*/
		const Array softmax()const;
		/**
		@brief 返回行列式
		*/
		mat_t Det();
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
		mat_t Cof(int32 x, int32 y);
		mat_t EigenvalueMax(mat_t offset = 1e-8)const;
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
		@brief 返回标准差
		*/
		mat_t D()const;
		/**
		@brief 重载运算符=
		拷贝
		*/
		Array & operator = (const Array &temp);
		/**
		@brief 重载运算符=
		拷贝
		*/
		//Array & operator = (const Mat &temp);
		/**
		@brief 重载运算符=
		拷贝
		*/
		template<class _Tp> Array & operator=(const MatCommaInitializer_<_Tp>& temp);
		/**
		@brief 重载运算符==
		判断矩阵是否相等
		*/
		bool operator == (const lzh::Array &a)const;
		/**
		@brief 重载运算符!=
		判断矩阵是否不相等
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
		@brief 返回矩阵对应索引的列或行
		@param index	索引
		@param rc		索引方式
		*/
		Array operator () (int32 index, RANK rc)const;
		/**
		@brief 返回矩阵对应索引的列或行
		@param index	索引
		@param rc		索引方式
		*/
		Array operator () (int32 v1, int32 v2, RANK rc)const;
		const Array operator [] (int32 idx) const;

		bool operator > (const Array &b) const;
		bool operator < (const Array &b) const;

		template<typename _Tp> operator _Tp *();
		template<typename _Tp> operator const _Tp *() const;

		/**
		@brief 返回元素为0的w向量
		@param w		向量长度
		*/
		static const Array zeros(int32 w, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的h*w矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		*/
		static const Array zeros(int32 w, int32 h, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的h*w*c矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static const Array zeros(int32 w, int32 h, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的size矩阵
		@param size 矩阵大小
		*/
		static const Array zeros(Size size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的size矩阵
		@param size 矩阵大小
		*/
		static const Array zeros(Size size, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为0的size矩阵
		@param size 矩阵大小
		*/
		static const Array zeros(Size3 size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的w向量
		@param v		填充元素
		@param w		向量长度
		*/
		static const Array value(Val v, int32 w, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的h*w矩阵
		@param v		填充元素
		@param w		矩阵列数
		@param h		矩阵行数
		*/
		static const Array value(Val v, int32 w, int32 h, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的h_*w_*c_矩阵
		@param v		填充元素
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static const Array value(Val v, int32 w, int32 h, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为v的h_*w_*c_矩阵
		@param v		填充元素
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static const Array value(Val v, Size3 size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的w向量
		@param w		向量长度
		*/
		static const Array ones(int32 w, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的h*w矩阵
		@param w	矩阵列数
		@param h	矩阵行数
		*/
		static const Array ones(int32 w, int32 h, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的h*w*c矩阵
		@param w		矩阵列数
		@param h		矩阵行数
		@param c		矩阵通道数
		*/
		static const Array ones(int32 w, int32 h, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的size矩阵
		@param size 矩阵大小
		*/
		static const Array ones(Size size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的size矩阵
		@param size 矩阵大小
		*/
		static const Array ones(Size size, int32 c, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回元素为1的size矩阵
		@param size 矩阵大小
		*/
		static const Array ones(Size3 size, int32 flag, ColorGamut cg = ColorGamutCustom);
		/**
		@brief 返回生成的n*n*1单位矩阵
		@param n 矩阵大小
		*/
		static const Array eye(int32 n, int32 flag);


		/**
		设置打印宽度
		@param w 宽度
		*/
		static void setPrintW(uint32 w);
		/**
		设置打印有效数字
		@param n 位数
		*/
		static void setPrintSignificantDigits(uint32 n);
		/**
		设置打印方法
		@param t 打印方法
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
	@brief Array_ 工具类
	继承Mat类，用于实现
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
		@brief 生成w的向量
		@param w		长度
		*/
		Array_(int32 w) : Array(w, DataType<_Tp>::type) {}
		/**
		@brief 生成h*w的矩阵
		@param w		列数
		@param h		行数
		*/
		Array_(int32 w, int32 h) : Array(w, h, DataType<_Tp>::type) {}
		/**
		@brief 生成h*w*c的张量
		@param w		列数
		@param h		行数
		@param depth	通道数
		*/
		Array_(int32 w, int32 h, int32 c) : Array(w, h, c, DataType<_Tp>::type) {}
		/**
		@brief 生成size的矩阵
		@param size		尺寸
		*/
		Array_(Size size) : Array(size, DataType<_Tp>::type) {}
		/**
		@brief 生成size的矩阵
		@param size		尺寸
		*/
		Array_(Size size, int32 c) : Array(size, c, DataType<_Tp>::type) {}
		/**
		@brief 生成size的矩阵
		@param size		尺寸
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
	@brief MatCommaInitializer_ 工具类
	作为迭代器，用于实现
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