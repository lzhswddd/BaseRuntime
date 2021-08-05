#define LZH_EXPORTS
#include "softmax.h"
#include "nn.h"
using namespace lzh;


nn::softmax::softmax(string name)
	: Activate(true, name)
{
}

nn::softmax::softmax(bool freeze, string name)
	: Activate(freeze, name)
{
}

nn::softmax::softmax(bool freeze, Mat alpha)
	: Activate(freeze, alpha)
{
}

nn::softmax::~softmax()
{
}

Size3 nn::softmax::initialize(Size3 param_size)
{
	a = Mat::ones(param_size);
	if (!freeze && regularization)
		updateregular();
	return param_size;
}

Mat nn::softmax::f(const Mat & x) const
{
	Mat y(x.size3());
	const mat_t *ai = a.data();
	if (a.dims() == 3) {
		int c = y.channels();
		mat_t *total_y = y.data();
		const mat_t *total_x = x.data();
		for (int j = 0; j < c; ++j) {
			mat_t *p = total_y + j;
			const mat_t *mat = total_x + j;
			mat_t sum = 0;
			mat_t max = x.channel(j).max();
			for (int i = 0; i < y.rows() * y.cols(); ++i) {
				*p = exp(*ai*(*mat - max));
				sum += *p;
				p += c;
				mat += c;
				ai += c;
			}
			y.channel(j) /= sum;
		}
	}
	else {
		mat_t *p = y.data();
		const mat_t *mat = x.data();
		for (int i = 0; i < y.rows(); ++i) {
			mat_t sum = 0;
			mat_t max = x.row(i).max();
			for (int j = 0; j < y.cols(); ++j) {
				*p = exp(*ai*(*mat - max));
				sum += *p;
				p++;
				mat++;
				ai++;
			}
			y.row(i) /= sum;
		}
	}
	return y;
}

Mat nn::softmax::df(const Mat & x) const
{
	return x.mul(1 - x);
}

void nn::softmax::back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number) const
{
	for (size_t idx = 0; idx < in.size(); ++idx) {
		if (!freeze)(*dlayer)[dlayer->size() - 1 - *number] += in[idx];
		Mult(in[idx], df(variable[idx]), out[idx]);
	}
	if (!freeze)(*dlayer)[dlayer->size() - 1 - *number] /= (mat_t)in.size();
	if (!freeze &&regularization)
		(*dlayer)[dlayer->size() - 1 - *number] += lambda * regular;
	*number += 1;
}
