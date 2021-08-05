#ifndef __NETAPI_H__
#define __NETAPI_H__

#include "net_core.h"

namespace lzh {
	namespace nn
	{
		//通过函数名注册函数
		extern LZHAPI void SetFunc(string func_name, LF *f, LF *df);
		//通过函数名注册函数
		extern LZHAPI void SetFunc(string func_name, Act *f, Act *df);
		//通过函数指针注册函数
		extern LZHAPI void SetFunc(LF func, LF *f, LF *df);
		//通过函数指针注册函数
		extern LZHAPI void SetFunc(Act func, Act *f, Act *df);
		//函数转函数名
		extern LZHAPI string Func2String(Act f);
		//函数转函数名
		extern LZHAPI string Func2String(LF f);

		extern LZHAPI void nms(std::vector<Bbox> &boundingBox_, const mat_t overlap_threshold, string modelname = "Union");
		extern LZHAPI void generateBbox(const Mat &score, const Mat &location, std::vector<Bbox> &boundingBox_, mat_t scale, mat_t threshold);
		extern LZHAPI void refine(vector<Bbox> &vecBbox, const int &height, const int &width, bool square);
		extern LZHAPI void normalization(const Mat& src, Mat&dst);
		extern LZHAPI void save_mat(FILE * file, const Mat & m);
		extern LZHAPI void load_mat(FILE * file, Mat & m);
	}
}

#endif // !__NET_CORE_H__
