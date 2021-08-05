#define LZH_EXPORTS
#include "layer.h"
#include "nn.h"
using namespace lzh;
using namespace lzh::nn;

string nn::Layer::Type2String(LayerType type)
{
	string type_name = "";
	switch (type)
	{
	case nn::NONE:
		type_name = "NONE";
		break;
	case nn::CONV2D:
		type_name = "CONV2D";
		break;
	case nn::POOL:
		type_name = "POOL";
		break;
	case nn::BATCHNORMALIZATION:
		type_name = "BATCH_NORMALIZATION";
		break;
	case nn::FULLCONNECTION:
		type_name = "FULL_CONNECTION";
		break;
	case nn::ACTIVATION:
		type_name = "ACTIVATION";
		break;
	case nn::RESHAPE:
		type_name = "RESHAPE";
		break;
	case nn::DROPOUT:
		type_name = "DROPOUT";
		break;
	case nn::PRELU:
		type_name = "PRELU";
		break;
	case nn::LOSS:
		type_name = "LOSS";
		break;
	default:
		break;
	}
	return type_name;
}

LayerType nn::Layer::String2Type(string str)
{
	if (str == "NONE")return NONE;
	else if (str == "CONV2D")return CONV2D;
	else if (str == "POOL")return POOL;
	else if (str == "FULL_CONNECTION")return FULLCONNECTION; 
	else if (str == "BATCH_NORMALIZATION")return BATCHNORMALIZATION;
	else if (str == "ACTIVATION")return ACTIVATION;
	else if (str == "RESHAPE")return RESHAPE;
	else if (str == "DROPOUT")return DROPOUT;
	else if (str == "PRELU")return PRELU;
	else if (str == "LOSS")return LOSS;
	else return NONE;
}

std::ostream & nn::operator<<(std::ostream & out, const Layer * layer)
{
	layer->show(out);
	return out;
}


Layer * nn::CreateLayer(json & info, FILE* file)
{
	Layer *layer = nullptr;
	switch (Layer::String2Type(info["type"]))
	{
	case nn::NONE:
		layer = (Layer*)new Node(info["name"]);
		break;
	case nn::CONV2D:
		layer = (Layer*)new convolution(info["name"]);
		break;
	case nn::POOL:
		layer = (Layer*)new pool(info["name"]);
		break;
	case nn::FULLCONNECTION:
		layer = (Layer*)new fullconnection(info["name"]);
		break;
	case nn::BATCHNORMALIZATION:
		layer = (Layer*)new batchnormalization(info["name"]);
		break;
	case nn::ACTIVATION:
		layer = (Layer*)new activation(info["name"]);
		break;
	case nn::RESHAPE:
		layer = (Layer*)new reshape(info["name"]);
		break;
	case nn::PRELU:
		layer = (Layer*)new PReLU(info["name"]);
	case nn::SIGMOID:
		layer = (Layer*)new sigmoid(info["name"]);
	case nn::TANH:
		layer = (Layer*)new tanh(info["name"]);
		break;
	default:
		break;
	}
	layer->load(info, file);
	return layer;
}
Layer * nn::PRelu(string name)
{
	PReLU *info = new PReLU(true, name);
	return (Layer*)info;
}
Layer * nn::Sigmoid(bool freeze, string name)
{
	sigmoid *info = new sigmoid(freeze, name);
	return (Layer*)info;
}
Layer * nn::Tanh(bool freeze, string name)
{
	tanh *info = new tanh(freeze, name);
	return (Layer*)info;
}
Layer * lzh::nn::Softmax(bool freeze, string name)
{
	softmax *info = new softmax(freeze, name);
	return (Layer*)info;
}
Layer * nn::BatchNorm(string name)
{
	batchnormalization *info = new batchnormalization(name);
	return (Layer*)info;
}
Layer* nn::Loss(LF loss_f, mat_t weight, string name)
{
	costfunction *info = new costfunction(name);
	info->setfunction(loss_f);
	info->weight = weight;
	return (Layer*)info;
}
Layer* nn::Loss(ReduceType loss_f, mat_t weight, string name)
{
	costfunction *info = new costfunction(name);
	info->setfunction(loss_f);
	info->weight = weight;
	return (Layer*)info;
}
Layer* nn::Loss(LF f, LF df, bool ignore_activate, mat_t weight, string name)
{
	costfunction *info = new costfunction(name);
	info->f = f;
	info->df = df;
	info->ignore_active = ignore_activate;
	info->weight = weight;
	info->weight = weight;
	return (Layer*)info;
}
Layer* nn::Dropout(mat_t dropout_, string name)
{
	dropout *info = new dropout(name);
	info->dropout_probability = dropout_;
	return (Layer*)info;
}
Layer* nn::Reshape(Size3 size, string name)
{
	reshape *info = new reshape(name);
	info->size = size;
	return (Layer*)info;
}
Layer* nn::Activation(Act acti, string name)
{
	activation *info = new activation(name);
	info->active = ActivationInfo(acti);
	return (Layer*)info;
}
Layer* nn::Activation(ActivationType acti, string name)
{
	activation *info = new activation(name);
	info->active = ActivationInfo(acti);
	return (Layer*)info;
}
Layer* nn::Activation(Act f, Act df, string name)
{
	activation *info = new activation(name);
	info->active.f = f;
	info->active.df = df;
	return (Layer*)info;
}
Layer* nn::MaxPool(Size poolsize, int strides, string name)
{
	pool *info = new pool(name);
	info->pool_type = pool::max_pool;
	info->ksize = poolsize;
	info->strides = strides;
	return (Layer*)info;
}
Layer* nn::AveragePool(Size poolsize, int strides, string name)
{
	pool *info = new pool(name);
	info->pool_type = pool::average_pool;
	info->ksize = poolsize;
	info->strides = strides;
	return (Layer*)info;
}
Layer* nn::Dense(int layer_size, Act act_f, string name)
{
	return FullConnect(layer_size, act_f, name);
}
Layer* nn::FullConnect(int layer_size, Act act_f, string name)
{
	fullconnection *info = new fullconnection(name);
	info->size = layer_size;
	info->active = ActivationInfo(act_f);
	info->isact = (act_f != nullptr);
	return (Layer*)info;
}
Layer* nn::Conv2D(ConvInfo cinfo, Act act_f, string name)
{
	convolution *info = new convolution(name);
	info->channel = cinfo.channel;
	info->kern_size = cinfo.kern_size;
	info->strides = cinfo.strides;
	info->anchor = cinfo.anchor;
	info->is_copy_border = cinfo.is_copy_border;
	info->active = ActivationInfo(act_f);
	info->isact = (act_f != nullptr);
	return (Layer*)info;
}
Layer* nn::Conv2D(int channel, int kern_size, bool is_copy_border, Act act_f, string name, Size strides, Point anchor)
{
	convolution *info = new convolution(name);
	info->channel = channel;
	info->kern_size = kern_size;
	info->is_copy_border = is_copy_border;
	info->strides = strides;
	info->anchor = anchor;
	info->active = ActivationInfo(act_f);
	info->isact = (act_f != nullptr);
	return (Layer*)info;
}
nn::ActivationInfo::ActivationInfo(ActivationType acti)
{
	switch (acti)
	{
	case nn::ACTI_SIGMOID:
		SetFunc(lzh::sigmoid, &f, &df);
		break;
	case nn::ACTI_TANH:
		SetFunc(lzh::tanh, &f, &df);
		break;
	case nn::ACTI_RELU:
		SetFunc(lzh::relu, &f, &df);
		break;
	case nn::ACTI_ELU:
		SetFunc(lzh::elu, &f, &df);
		break;
	case nn::ACTI_LReLU:
		SetFunc(lzh::leaky_relu, &f, &df);
		break;
	case nn::ACTI_SELU:
		SetFunc(lzh::selu, &f, &df);
		break;
	case nn::ACTI_SOFTMAX:
		SetFunc(lzh::softmax, &f, &df);
		break;
	default:
		break;
	}
}
nn::ActivationInfo::ActivationInfo(Act acti)
{
	SetFunc(acti, &f, &df);
}

void lzh::nn::parametrics::freeze_enable(bool freeze)
{
	this->freeze = freeze;
}

bool lzh::nn::parametrics::isfreeze() const
{
	return freeze;
}

nn::Activate::Activate(bool freeze, string name)
	: parametrics(freeze, name)
{
	type = ACTIVATION;
}

nn::Activate::Activate(bool freeze, Mat alpha)
	: parametrics(freeze), a(alpha)
{
}

nn::Activate::~Activate()
{
}

void nn::Activate::updateregular()
{
	regular = a;
}

void nn::Activate::update(const vector<Mat>& d, int * idx)
{
	if (freeze) a += d[*idx];
	if (!freeze && regularization)
		updateregular();
	*idx += 1;
}

void nn::Activate::forword(const Mat & in, Mat & out) const
{
	out = f(in);
}

void nn::Activate::forword_train(const vector<Mat>& in, vector<Mat>& out, vector<Mat>& variable)
{
	this->variable = variable;
	for (size_t idx = 0; idx < in.size(); ++idx)
		out[idx] = f(in[idx]);
}

void nn::Activate::back(const vector<Mat>& in, vector<Mat>& out, vector<Mat>* dlayer, int * number) const
{
	for (size_t idx = 0; idx < in.size(); ++idx) {
		if (!freeze) {
			if (a.dims() == 3) {
				Mat t;
				Sum(in[idx], t, CHANNEL);
				(*dlayer)[dlayer->size() - 1 - *number] += t;
			}
			else
				(*dlayer)[dlayer->size() - 1 - *number] += in[idx];
		}
		Mult(in[idx], df(variable[idx]), out[idx]);
	}
	if (!freeze)(*dlayer)[dlayer->size() - 1 - *number] /= (mat_t)in.size();
	if (!freeze && regularization)
		(*dlayer)[dlayer->size() - 1 - *number] += lambda * regular;
	*number += 1;
}

Size3 nn::Activate::initialize(Size3 param_size)
{
	if (param_size.c == 1)
		a = Mat::ones(1, param_size.h, 1);
	else
		a = Mat::ones(1, 1, param_size.c);
	if (!freeze && regularization)
		updateregular();
	return param_size;
}

void nn::Activate::append_size(vector<Size3>* size)
{
	size->push_back(a.size3());
}

void nn::Activate::save(json * jarray, FILE * file) const
{
	json info;
	info["type"] = Layer::Type2String(type);
	info["name"] = name;
	info["layer"] = layer_index;
	jarray->push_back(info);
	save_param(file);
}

void nn::Activate::load(json & info, FILE * file)
{
	type = Layer::String2Type(info["type"]);
	layer_index = info["layer"];
	load_param(file);
}

void nn::Activate::save_param(FILE * file) const
{
	save_mat(file, a);
}

void nn::Activate::load_param(FILE * file)
{
	load_mat(file, a);
}

void nn::Activate::show(std::ostream & out) const
{
	out << name << "{" << std::endl;
	out << "Layer-> Activate" << std::endl;
	out << "ai size-> " << a.size3() << std::endl;
	out << "}";
}

mat_t nn::Activate::norm(int num) const
{
	return a.norm(num);
}

Mat nn::Activate::f(const Mat & x) const
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
			for (int i = 0; i < y.rows()*y.cols(); ++i) {
				f(p, mat, ai);
				p += c;
				mat += c;
			}
			ai++;
		}
	}
	else {
		mat_t *p = y.data();
		const mat_t *mat = x.data();
		for (int i = 0; i < y.rows(); ++i) {
			for (int j = 0; j < y.cols(); ++j) {
				f(p, mat, ai);
				p++;
				mat++;
			}
			ai++;
		}
	}
	return y;
}

Mat nn::Activate::df(const Mat & x) const
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
			for (int i = 0; i < y.rows()*y.cols(); ++i) {
				df(p, mat, ai);
				p += c;
				mat += c;
			}
			ai++;
		}
	}
	else {
		mat_t *p = y.data();
		const mat_t *mat = x.data();
		for (int i = 0; i < y.rows(); ++i) {
			for (int j = 0; j < y.cols(); ++j) {
				df(p, mat, ai);
				p++;
				mat++;
			}
			ai++;
		}
	}
	return y;
}
