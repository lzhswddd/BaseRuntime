#pragma once
#include "activation.h"

namespace lzh {
	namespace nn
	{
		class LZHAPI softmax :
			public Activate
		{
		public:
			softmax(string name = "");
			softmax(bool freeze, string name);
			softmax(bool freeze, Mat alpha);
			~softmax();
			Size3 initialize(Size3 param_size)override;
			void back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const override;
			Mat f(const Mat & x)const override;
			Mat df(const Mat & x)const override;
			virtual void f(mat_t *p, const mat_t *mat, const mat_t *ai)const {}
			virtual void df(mat_t *p, const mat_t *mat, const mat_t *ai)const {}
		};
	}
}
