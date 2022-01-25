#pragma once

#ifndef __TOOLS_H__
#define __TOOLS_H__
#include "geometry.h"
#include "image_process.h"
#include "operator.h"

#define MEASURE_TIME(function) do{\
		lzh::Timer _timer_;\
		_timer_.Start();\
		function;\
		print(0x0002, "running function %s -> cast time is %0.4f sec\n", STR2(function), _timer_.End() / 1000000.0);\
		}while(0)

namespace lzh
{
	class LZHAPI TimeData
	{
	public:
		TimeData(){
			memset(this, 0, sizeof(TimeData));
		}
		TimeData(uint64 t) : t(t) {
			us = uint32((t * 1000) % 1000);
			ms = uint32((t) % 1000);
			s = uint32((t / 1000) % 60);
			m = uint32((t / 1000) / 60 % 60);
			h = uint32((t / 1000) / 3600 % 60);
			day = uint32((t / 1000) / (3600 * 24) % 31);
			month = 0;
			year = 0;
		}
		uint32 year;
		uint32 month;
		uint32 day;
		uint32 h;
		uint32 m;
		uint32 s;
		uint32 ms;
		uint32 us;
		uint64 t;

		std::string toString()const;
		std::string toTime()const;
	};
	extern LZHAPI TimeData NowTime();
	class LZHAPI Timer
	{
	public:
		Timer();
		~Timer();
		void Start();
		uint64 End();
		TimeData EndTime();
	protected:
		void* start;
		void* end;
		void* fc;
	};
	using StringList = typename std::vector<std::string>;
	/**
	命令行输出
	*/
	template<typename _Tp> TEMPLATE_API void output(const _Tp &t);
	template<typename _Tp> TEMPLATE_API std::vector<_Tp> range(_Tp top, Val gap = 1);
	template<typename _Tp> TEMPLATE_API std::vector<_Tp> range(_Tp low, _Tp top, Val gap = 1);
	template<typename _Tp> TEMPLATE_API Rect_<_Tp> toRect(const Vec<_Tp, 4> &vec);
	template<typename _Tp> TEMPLATE_API Rect_<_Tp> toRect(const Range_<_Tp> &r, const Range_<_Tp> &c);
	template<typename _Tp, int N> TEMPLATE_API Vec<_Tp, N> toVec(const Mat &m);
	template<typename _Tp, int N> TEMPLATE_API Vec<_Tp, N> toVec(const Array &m);
	template<typename _Tp> TEMPLATE_API std::vector<_Tp> str2Data(const std::vector<std::string> &str);
	/**
	@brief 返回将向量转换成1*point.size()*1的矩阵
	@param point 向量
	*/
	template<typename _Tp> TEMPLATE_API const Mat Vec2Mat(const std::vector<_Tp> &point);
	/**
	@brief 返回将向量转换成point.size()*point[0].size()*1的矩阵
	@param points 向量
	*/
	template<typename _Tp> TEMPLATE_API const Mat Vec2Mat(const std::vector<std::vector<_Tp>> &points);
	template<typename _Tp> TEMPLATE_API std::vector<Point2_<_Tp>> MatToPoint2(IMat m);
	template<typename _Tp> TEMPLATE_API std::vector<Point3_<_Tp>> MatToPoint3(IMat m);
	template<typename _Tp> TEMPLATE_API const Mat Point2ToMat(const std::vector<Point2_<_Tp>> & ps);
	template<typename _Tp> TEMPLATE_API const Mat Point3ToMat(const std::vector<Point3_<_Tp>> & ps);
	/**
	@brief 返回将矩阵转换成一维向量
	@param src 矩阵
	*/
	template<typename _Tp> TEMPLATE_API std::vector<_Tp> Mat2Vec(IMat src);
	/**
	@brief 返回将矩阵转换成向量
	@param src 矩阵
	*/
	template<typename _Tp> TEMPLATE_API std::vector<std::vector<_Tp>> Mat2Vecs(IMat src);

	template<typename _Tp> TEMPLATE_API typename std::vector<_Tp>::iterator find(std::vector<_Tp> & vec, const _Tp & key);
	template<typename _Tp> TEMPLATE_API typename std::vector<_Tp>::const_iterator find(const std::vector<_Tp> & vec, const _Tp & key);

	/**
	暂停程序
	*/
	extern LZHAPI void pause();
	extern LZHAPI int32 console(const std::string &str);
	extern LZHAPI mat_t saveDiv(mat_t div, mat_t v, mat_t def = (std::numeric_limits<mat_t>::max)());
	extern LZHAPI void binaryPrint(int32 value, int32 space = 0);
	extern LZHAPI void Wait(uint32 ms);
	extern LZHAPI void Frequency();
	extern LZHAPI void StartCounter();
	extern LZHAPI mat_t EndCounter();

	extern LZHAPI void Command(const char* cmd);
	extern LZHAPI char* WideCharToANSI(const wchar_t* szWidechar);
	extern LZHAPI wchar_t* ANSIToWideChar(const char* szWidechar);
	extern LZHAPI std::string WStr2Str(const std::wstring& str);
	extern LZHAPI std::wstring Str2WStr(const std::string& str);
	extern LZHAPI std::vector<std::string> WStr2Str(const std::vector<std::wstring>& str);
	extern LZHAPI std::vector<std::wstring> Str2WStr(const std::vector<std::string>& str);
	extern LZHAPI std::string UTF8ToString(const std::string & str);
	extern LZHAPI std::string StringToUTF8(const std::string & str);
	extern LZHAPI Vector toVector(const Mat &mat);
	extern LZHAPI std::string removeAt(const std::string &str, char ch);
	extern LZHAPI std::string _STR(const char* str);
	extern LZHAPI std::vector<std::string> strSplit(const std::string &str, int8 ch);
	extern LZHAPI std::vector<std::string> getFiles(std::string path);
	extern LZHAPI void getFiles(std::string path, std::vector<std::string>& files);
	extern LZHAPI bool isNumber(std::string str);
	extern LZHAPI lzh::Array FromBitmap(int height, int width, int stride, void* data, int type, bool isCopy);

	extern LZHAPI void saveString(FILE*file, const std::string & str);
	extern LZHAPI void readString(FILE*file, std::string & str);

	extern LZHAPI const Array read(std::string filename);
	extern LZHAPI void save(std::string filename, IArray mat);
	extern LZHAPI void save(std::string filename, IMat mat);

	extern LZHAPI const Array readcsv(std::string filename);
	extern LZHAPI void savecsv(std::string filename, IArray mat);
	extern LZHAPI const Array readbin(std::string filename);
	extern LZHAPI void savebin(std::string filename, IArray mat);
	extern LZHAPI const Array readtxt(std::string filename);
	extern LZHAPI void savetxt(std::string filename, IArray mat);

	extern LZHAPI void read(std::string filename, vec_array &vec);
	extern LZHAPI void save(std::string filename, const vec_array &mat);

	extern LZHAPI void readcsv(std::string filename, vec_array &vec);
	extern LZHAPI void savecsv(std::string filename, const vec_array &mat);
	extern LZHAPI void readbin(std::string filename, vec_array &vec);
	extern LZHAPI void savebin(std::string filename, const vec_array &mat);
	extern LZHAPI void readtxt(std::string filename, vec_array &vec);
	extern LZHAPI void savetxt(std::string filename, const vec_array &mat);
	extern LZHAPI vec_mat tranVec(const vec_array &vec);
	extern LZHAPI vec_array tranVec(const vec_mat &vec);
	extern LZHAPI void check(int32 w, int32 h = 1, int32 depth = 1);

	extern LZHAPI std::string getModuleFileName();
	extern LZHAPI FILE* createFile(std::string filename, const char * mode);
	extern LZHAPI int32 releaseFile(FILE* file);
	extern LZHAPI std::string getFileName(std::string filename);
	extern LZHAPI std::string getFolder(std::string filename);
	extern LZHAPI std::string getLastFolder(std::string filename);
	extern LZHAPI std::string getFileType(std::string filename);
	extern LZHAPI std::string ReplaceFileSuffix(std::string filename, std::string suffix);
	extern LZHAPI bool isImageFile(std::string filename);
	extern LZHAPI bool isFolder(std::string path);
	extern LZHAPI bool isFile(std::string path);
	extern LZHAPI bool isExists(std::string path);
	extern LZHAPI StringList getImageFiles(std::string folder_path);
	extern LZHAPI StringList getImageFiles(const StringList & files);
	extern LZHAPI std::string show_path();
	extern LZHAPI size_t getFileSize(std::string file_path);
	extern LZHAPI size_t getFileSize(std::ifstream & file);
	extern LZHAPI bool removeFile(std::string path);
	extern LZHAPI bool createDirectory(std::string path);
	extern LZHAPI std::string appendPath(const std::initializer_list<std::string> & path);

	extern LZHAPI std::string Enum2String(PrintType type);
	extern LZHAPI std::string Enum2String(BorderType type);
	extern LZHAPI std::string Enum2String(MatErrorInfo type);
	extern LZHAPI std::string Enum2String(EQUATION type);
	extern LZHAPI std::string Enum2String(ORDER type);
	extern LZHAPI std::string Enum2String(RANK type);
	extern LZHAPI std::string Enum2String(Dire type);
	extern LZHAPI std::string Enum2String(InterpolationMothed type);
	extern LZHAPI std::string Enum2String(RotateAngle type);
	extern LZHAPI std::string Enum2String(BSpline::BSplineType type);
	extern LZHAPI std::string Enum2String(ColorGamut type);
	extern LZHAPI ColorGamut String2ColorGamut(const char * str);
	extern LZHAPI BorderType String2BorderType(const char * str);
	extern LZHAPI int32 String2Type(const char * str);
	extern LZHAPI std::string Enum2String(Color::ColorTab type);
	extern LZHAPI std::string Type2String(int32 type);
}
#include "tools_inl.hpp"
#endif //!__TOOLS_H__

