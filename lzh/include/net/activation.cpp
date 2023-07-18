#define LZH_EXPORTS
#include "activation.h"
#include "nn.h"
using namespace lzh;

nn::activation::activation(string name)
	: Layer(name)
{
	isinit = false;
	isforword = true;
	isback = true;
	isupdate = false;
	isparam = false;
	type = ACTIVATION;
}
nn::activation::~activation()
{
}
void nn::activation::forword(const Mat & in, Mat &out) const
{
	out = active.f(in);
}
void nn::activation::forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> & variable)
{
	this->variable = variable;
	for (size_t idx = 0; idx < in.size(); ++idx)
		out[idx] = active.f(in[idx]);
}
void nn::activation::back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *, int *)const
{
	for (size_t idx = 0; idx < in.size(); ++idx) {
		out[idx] = in[idx].mul(active.df(variable[idx]));
	}
}
Size3 nn::activation::initialize(Size3 param_size)
{
	return param_size;
}
void nn::activation::save(json * jarray, FILE * ) const
{
	json info;
	info["type"] = Layer::Type2String(type);
	info["name"] = name;
	info["layer"] = layer_index;
	info["activate"] = Func2String(active.f);
	jarray->push_back(info);
}
void nn::activation::load(json & info, FILE * )
{
	type = Layer::String2Type(info["type"]);
	layer_index = info["layer"];
	SetFunc(info["activate"], &active.f, &active.df);
}
void nn::activation::show(std::ostream &out) const
{
	out << name << "{" << std::endl;
	out << "Layer-> Activation" << std::endl;
	out << "activation-> " << Func2String(active.f) << std::endl;
	out << "}";
}

nn::sigmoid::sigmoid(string name)
	: Activate(true, name)
{
}

nn::sigmoid::sigmoid(bool freeze, string name)
	: Activate(freeze, name) 
{
}

nn::sigmoid::sigmoid(bool freeze, Mat alpha)
	: Activate(freeze, alpha) 
{
}

nn::sigmoid::~sigmoid(){}

void nn::sigmoid::f(mat_t * p, const mat_t * mat, const mat_t * ai) const
{
	*p = 1 / (1 + exp(-*ai**mat));
}

void nn::sigmoid::df(mat_t * p, const mat_t * mat, const mat_t * ai) const
{
	mat_t v;
	f(&v, mat, ai);
	*p = v * (1 - v);
}

nn::tanh::tanh(string name)
	: Activate(true, name)
{
}

nn::tanh::tanh(bool freeze, string name)
	: Activate(freeze, name)
{
}

nn::tanh::tanh(bool freeze, Mat alpha)
	: Activate(freeze, alpha)
{
}

nn::tanh::~tanh()
{
}

void nn::tanh::f(mat_t * p, const mat_t * mat, const mat_t * ai)const
{
	*p = (exp(*ai**mat) - exp(-*ai**mat)) / (exp(*ai**mat) + exp(-*ai**mat));
}

void nn::tanh::df(mat_t * p, const mat_t * mat, const mat_t * ai)const
{
	mat_t v;
	f(&v, mat, ai);
	*p = 1 - v * v;
}
