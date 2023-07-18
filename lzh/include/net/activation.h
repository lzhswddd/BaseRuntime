#pragma once
#include "layer.h"

namespace lzh {
	namespace nn
	{
		class LZHAPI activation :
			public Layer
		{
		public:
			activation(string name = "");
			~activation();

			void forword(const Mat &in, Mat &out)const;
			void forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> &variable);
			void back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const;
			Size3 initialize(Size3 param_size);
			void save(json * jarray, FILE* file)const;
			void load(json & info, FILE* file);
			void show(std::ostream &out)const;

			STLApi<vector<Mat>> variable;
			ActivationInfo active;
		};

		class LZHAPI sigmoid :
			public Activate
		{
		public:
			sigmoid(string name = "");
			sigmoid(bool freeze, string name);
			sigmoid(bool freeze, Mat alpha);
			~sigmoid();
			void f(mat_t *p, const mat_t *mat, const mat_t *ai)const;
			void df(mat_t *p, const mat_t *mat, const mat_t *ai)const;
		};

		class LZHAPI tanh :
			public Activate
		{
		public:
			tanh(string name = "");
			tanh(bool freeze, string name);
			tanh(bool freeze, Mat alpha);
			~tanh();

			void f(mat_t *p, const mat_t *mat, const mat_t *ai)const;
			void df(mat_t *p, const mat_t *mat, const mat_t *ai)const;
		};
	}
}

