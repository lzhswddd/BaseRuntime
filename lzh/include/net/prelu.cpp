#define LZH_EXPORTS
#include "prelu.h"
#include "nn.h"
using namespace lzh;

nn::PReLU::PReLU(bool freeze, string name)
	: Activate(freeze, name) {}

nn::PReLU::PReLU(bool freeze, Mat alpha)
	: Activate(freeze, alpha) {}

nn::PReLU::~PReLU()
{
}
Size3 lzh::nn::PReLU::initialize(Size3 param_size)
{
	if (param_size.c == 1)
		a = Mat::zeros(1, param_size.h, 1) + 0.25f;
	else
		a = Mat::zeros(1, 1, param_size.c) + 0.25f;
	if (!freeze && regularization)
		updateregular();
	return param_size;
}
//Size3 nn::PReLU::initialize(Size3 param_size)
//{
//	if (param_size.c == 1)
//		a = Mat::zeros(1, param_size.h, 1) + 0.25f;
//	else
//		a = Mat::zeros(1, 1, param_size.c) + 0.25f;
//	if (regularization)
//		updateregular();
//	return param_size;
//}
//void nn::PReLU::updateregular()
//{
//	regular = a;
//}
//void nn::PReLU::update(const vector<Mat> &d, int *idx)
//{
//	a += d[*idx];
//	if (regularization)
//		updateregular();
//	*idx += 1;
//}
//void nn::PReLU::forword(const Mat & in, Mat & out) const
//{
//	out = prelu(in);
//}
//void nn::PReLU::forword_train(const vector<Mat> &in, vector<Mat> & out, vector<Mat> & variable)
//{
//	this->variable = variable;
//	for (size_t idx = 0; idx < in.size(); ++idx)
//		out[idx] = prelu(in[idx]);
//}
//void nn::PReLU::back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number) const
//{
//	for (size_t idx = 0; idx < in.size(); ++idx) {
//		if (a.dims() == 3) {
//			Mat t;
//			Sum(in[idx], t, CHANNEL);
//			(*dlayer)[dlayer->size() - 1 - *number] += t;
//		}
//		else
//			(*dlayer)[dlayer->size() - 1 - *number] += in[idx];
//		Mult(in[idx], d_prelu(variable[idx]), out[idx]);
//	}
//	(*dlayer)[dlayer->size() - 1 - *number] /= (mat_t)in.size();
//	if (regularization)
//		(*dlayer)[dlayer->size() - 1 - *number] += lambda * regular;
//	*number += 1;
//}
//void nn::PReLU::append_size(vector<Size3>* size)
//{
//	size->push_back(a.size3());
//}
//void nn::PReLU::save(json * jarray, FILE * file) const
//{
//	json info;
//	info["type"] = Layer::Type2String(type);
//	info["name"] = name;
//	info["layer"] = layer_index;
//	jarray->push_back(info);
//	save_param(file);
//}
//void nn::PReLU::load(json & info, FILE * file)
//{
//	type = Layer::String2Type(info["type"]);
//	layer_index = info["layer"];
//	load_param(file);
//}
//void nn::PReLU::save_param(FILE * file) const
//{
//	save_mat(file, a);
//}
//void nn::PReLU::load_param(FILE * file)
//{
//	load_mat(file, a);
//}
//void nn::PReLU::show(std::ostream & out) const
//{
//	out << name << "{" << std::endl;
//	out << "Layer-> PReLU" << std::endl;
//	out << "ai size-> " << a.size3() << std::endl;
//	out << "}";
//}
//mat_t nn::PReLU::norm(int num) const
//{
//	return a.norm(num);
//}
//const Mat nn::PReLU::prelu(const Mat & x) const
//{
//	Mat y(x.size3());
//	const mat_t *ai = a.data();
//	if (a.dims() == 3) {
//		int c = y.channels();
//		mat_t *total_y = y.data();
//		const mat_t *total_x = x.data();
//		for (int j = 0; j < c; ++j) {
//			mat_t *p = total_y + j;
//			const mat_t *mat = total_x + j;
//			for (int i = 0; i < y.rows()*y.cols(); ++i) {
//				prelu(p, mat, ai);
//				p += c;
//				mat += c;
//			}
//			ai++;
//		}
//	}
//	else {
//		mat_t *p = y.data();
//		const mat_t *mat = x.data();
//		for (int i = 0; i < y.rows(); ++i) {
//			for (int j = 0; j < y.cols(); ++j) {
//				prelu(p, mat, ai);
//				p++;
//				mat++;
//			}
//			ai++;
//		}
//	}
//	return y;
//}
//const Mat nn::PReLU::d_prelu(const Mat & x) const
//{
//	Mat y(x.size3());
//	const mat_t *ai = a.data();
//	if (a.dims() == 3) {
//		int c = y.channels();
//		mat_t *total_y = y.data();
//		const mat_t *total_x = x.data();
//		for (int j = 0; j < c; ++j) {
//			mat_t *p = total_y + j;
//			const mat_t *mat = total_x + j;
//			for (int i = 0; i < y.rows()*y.cols(); ++i) {
//				d_prelu(p, mat, ai);
//				p += c;
//				mat += c;
//			}
//			ai++;
//		}
//	}
//	else {
//		mat_t *p = y.data();
//		const mat_t *mat = x.data();
//		for (int i = 0; i < y.rows(); ++i) {
//			for (int j = 0; j < y.cols(); ++j) {
//				d_prelu(p, mat, ai);
//				p++;
//				mat++;
//			}
//			ai++;
//		}
//	}
//	return y;
//}
void nn::PReLU::f(mat_t *p, const mat_t *mat, const mat_t *ai)const
{
	if (*mat < 0)
		*p = *mat*(*ai);
	else if (*mat > 0)
		*p = *mat;
	else
		*p = 0;
}
void nn::PReLU::df(mat_t *p, const mat_t *mat, const mat_t *ai)const
{
	if (*mat < 0)
		*p = *ai;
	else if (*mat > 0)
		*p = 1;
	else
		*p = 0;
}