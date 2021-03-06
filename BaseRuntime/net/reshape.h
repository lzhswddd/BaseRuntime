#pragma once
#include "layer.h"

namespace lzh {
	namespace nn
	{
		class LZHAPI reshape :
			public Layer
		{
		public:
			reshape(string name = "");
			~reshape();

			void forword(const Mat &in, Mat &out)const;
			void forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> &variable);
			void back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const;
			Size3 initialize(Size3 param_size);
			void save(json * jarray, FILE* file)const;
			void load(json & info, FILE* file);
			void show(std::ostream &out)const;

			Size3 size;
			Size3 front_size;
		};
	}
}
