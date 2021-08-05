#define LZH_EXPORTS
#include "batchnormalization.h"
#include "nn.h"
using namespace lzh;

nn::batchnormalization::batchnormalization(string name)
	: parametrics(name)
{
	type = BATCHNORMALIZATION;
}
nn::batchnormalization::~batchnormalization()
{
}
Size3 nn::batchnormalization::initialize(Size3 param_size)
{
	Size3 size(1, 1, 1);
	if (param_size.c == 1) {
		size.h = param_size.h; isVec = true;
	}
	else {
		size.c = param_size.c; isVec = false;
	}
	gamma = Mat::ones(size);
	beta = Mat::zeros(size);
	moving_mean = Mat::zeros(size);
	moving_var = Mat::zeros(size);
	return param_size;
}
void nn::batchnormalization::updateregular()
{
	regular = gamma;
}
void nn::batchnormalization::update(const vector<Mat>& d, int * idx)
{
	gamma += d[(*idx)++];
	beta += d[(*idx)++];
	if (regularization)
		updateregular();
}
void nn::batchnormalization::forword(const Mat & in, Mat & out) const
{
	out = (in - moving_mean) / (moving_var + epsilon).sqrt();
}
void nn::batchnormalization::forword_train(const vector<Mat>& in, vector<Mat>& out, vector<Mat>& variable)
{
	this->variable[0] = in;
	mat_t size = (mat_t)in[0].size3().area();
	mean = Mat::zeros(gamma.size3());
	var = Mat::zeros(gamma.size3());
	vector<Mat> temp(in.size());
	vector<Mat>::iterator iter = temp.begin();
	for (const Mat &m : in) {
		if (isVec)
			*iter = m;
		else
			Sum(m, *iter, CHANNEL);
		mean += *iter;
		iter += 1;
	}
	mean /= (mat_t)in.size()*size;
	for (const Mat &m : temp)
		var += (m - mean).pow(2);
	var /= (mat_t)in.size()*size;
	moving_mean = momentum * moving_mean + (1.0f - momentum) * mean;
	moving_var = momentum * moving_var + (1.0f - momentum) * var;
	for (size_t idx = 0; idx < in.size(); ++idx) {
		out[idx] = (in[idx] - mean) / (var + epsilon).sqrt();
	}
	this->variable[1] = out;
	for (size_t idx = 0; idx < in.size(); ++idx) {
		Mult(out[idx], gamma, out[idx]);
		out[idx] += beta;
		//out[idx].save("test.txt", false);
	}
	variable = out;
}
void nn::batchnormalization::back(const vector<Mat>& in, vector<Mat>& out, vector<Mat>* dlayer, int * number) const
{
	mat_t N = (mat_t)in.size();
	mat_t size = (mat_t)in[0].size3().area();
	vector<Mat> d(in.size());
	vector<Mat> dx(in.size());
	Mat dv = Mat::zeros(in[0].size3());
	Mat dm = Mat::zeros(in[0].size3());
	Mat err = var + epsilon;
	Mat t, t1;
	for (size_t idx = 0; idx < in.size(); ++idx) {
		if (isVec) {
			(*dlayer)[dlayer->size() - 1 - *number] += in[idx];
			Mult(in[idx], variable[1][idx], t);
			(*dlayer)[dlayer->size() - 2 - *number] += t;
		}
		else {
			Sum(in[idx], t, CHANNEL);
			(*dlayer)[dlayer->size() - 1 - *number] += t;
			Mult(in[idx], variable[1][idx], t);
			Sum(t, t, CHANNEL);
			(*dlayer)[dlayer->size() - 2 - *number] += t;
		}
		Mult(in[idx], gamma, dx[idx]);
		d[idx] = variable[0][idx] - mean;
		Mult(dx[idx], d[idx], t);
		Mult(t, -0.5f / err.pow(3).sqrt(), t);
		dv += t;
		Mult(dx[idx], -1.0f / err.sqrt(), t);
		dm += t;
	}
	dv /= N;
	dm /= N;
	if (isVec) {
		(*dlayer)[dlayer->size() - 1 - *number] /= N;
		(*dlayer)[dlayer->size() - 2 - *number] /= N;
	}
	else {
		(*dlayer)[dlayer->size() - 1 - *number] /= N * size;
		(*dlayer)[dlayer->size() - 2 - *number] /= N * size;
	}
	for (size_t idx = 0; idx < in.size(); ++idx) {
		Mult(dx[idx], 1.0f / err.sqrt(), t);
		Mult(dv, (2.0f*(d[idx])) / N, t1);
		out[idx] = t + t1 + dm * 1.0f / N;
		//out[idx].save("test.txt", false);
	}
	*number += 2;
}
void nn::batchnormalization::append_size(vector<Size3>* size)
{
	size->push_back(gamma.size3());
	size->push_back(beta.size3());
}
void nn::batchnormalization::save(json * jarray, FILE * file) const
{
	json info;
	info["type"] = Layer::Type2String(type);
	info["name"] = name;
	info["layer"] = layer_index;
	jarray->push_back(info);
	save_param(file);
}
void nn::batchnormalization::load(json & info, FILE * file)
{
	layer_index = info["layer"];
	load_param(file);
}
void nn::batchnormalization::save_param(FILE * file) const
{
	nn::save_mat(file, moving_mean);
	nn::save_mat(file, moving_var);
}
void nn::batchnormalization::load_param(FILE * file)
{
	nn::load_mat(file, moving_mean);
	nn::load_mat(file, moving_var);
}
void nn::batchnormalization::show(std::ostream & out) const
{
	out << name << "{" << std::endl;
	out << "Layer-> batchnormalization" << std::endl;
	out << "size-> " << gamma.size3() << std::endl;
	out << "}";
}
mat_t nn::batchnormalization::norm(int num) const
{
	return gamma.sum(num, true) + beta.sum(num, true);
}
