#pragma once
#include "activation.h"

namespace lzh {
	namespace nn
	{
		class LZHAPI PReLU :
			public Activate
		{
		public:
			PReLU(bool freeze = true, string name = "");
			PReLU(bool freeze, Mat alpha);
			~PReLU();

			Size3 initialize(Size3 param_size) override;
			//void updateregular();
			//void update(const vector<Mat> &d, int *idx);
			//void forword(const Mat &in, Mat &out)const;
			//void forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> &variable);
			//void back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const;
			//Size3 initialize(Size3 param_size);
			//void append_size(vector<Size3> *size);
			//void save(json * jarray, FILE* file)const;
			//void load(json & info, FILE* file);
			//void save_param(FILE* file)const;
			//void load_param(FILE* file);
			//void show(std::ostream &out)const;
			//mat_t norm(int num)const;

			//const Mat prelu(const Mat & x)const;
			//const Mat d_prelu(const Mat & x)const;
			void f(mat_t *p, const mat_t *mat, const mat_t *ai)const;
			void df(mat_t *p, const mat_t *mat, const mat_t *ai)const;
		};
	}
}
