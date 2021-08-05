#define LZH_EXPORTS
#include "optimalize.h"
#include "../lzh.h"
using namespace lzh;
using namespace nn;
/*
============================    优化器基类    =========================
*/
Optimizer::Optimizer()
	:step(1e-2f), obj(nullptr), jacobi(nullptr), method(None) {}
Optimizer::Optimizer(mat_t step) : step(step), obj(nullptr), jacobi(nullptr), method(None) {}
Optimizer::~Optimizer() {}
void Optimizer::Register(Jacobi jacobi, const void* obj)
{
	this->jacobi = jacobi; this->obj = (void*)obj;
}
bool Optimizer::Enable(const Mat & x, const vector<Mat> & y) const
{
	return !(x.empty() || y.empty() || jacobi == nullptr);
}
void Optimizer::RegisterMethod(OptimizerMethod Net)
{
	this->method = Net;
}
OptimizerMethod Optimizer::OpMethod() const
{
	return method;
}
/*
========================  注册模型函数(无优化器) ========================
*/
Method::Method() :Optimizer() {}
Method::Method(mat_t step) : Optimizer(step) {}
void Method::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error) {}
Optimizer* Method::minimize()const
{
	Optimizer* train = new Method(*this);
	train->RegisterMethod(None);
	return train;
}
void Method::copyTo(Optimizer * op) const
{
	*((Method*)op) = *this;
}
Mat Method::Params() const
{
	return Mat();
}
/*
=======================    GradientDescent优化器    =======================
*/
GradientDescentOptimizer::GradientDescentOptimizer(mat_t step) : Optimizer(step) {}
void GradientDescentOptimizer::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error)
{
	/**
	a = a - step * df(a, x)
	*/
	if (!x || !jacobi)return;
	jacobi(obj, x, dlayer, error);
	for (size_t layer_num = 0; layer_num < dlayer.size(); ++layer_num) {
		dlayer[layer_num] = -step * dlayer[layer_num];
	}
}
Optimizer* GradientDescentOptimizer::minimize()const {
	Optimizer* train = new GradientDescentOptimizer(*this);
	train->RegisterMethod(GradientDescent);
	return train;
}
Mat GradientDescentOptimizer::data(vector<string> &value_name)const
{
	Mat mat(1, 1);
	mat(0) = step;
	value_name.resize(1);
	value_name[0] = "step";
	return mat;
}
void GradientDescentOptimizer::copyTo(Optimizer * op) const
{
	*((GradientDescentOptimizer*)op) = *this;
}
Mat GradientDescentOptimizer::Params() const
{
	return Mat();
}
/*
============================    Momentum优化器    =========================
*/
MomentumOptimizer::MomentumOptimizer(mat_t step) : Optimizer(step) {}
void MomentumOptimizer::init(vector<Size3>& size)
{
	if (size.empty())return;
	ma.resize(size.size());
	for (size_t layer_num = 0; layer_num < size.size(); ++layer_num)
		ma[layer_num] = Mat::zeros(size[layer_num]);
}
void nn::MomentumOptimizer::save(string file) const
{
	FILE *out = fopen(file.c_str(), "wb");
	if (out) {
		SaveParam(out, ma.d_ptr());
		fclose(out);
	}
}
void nn::MomentumOptimizer::load(string file)
{
	FILE *in = fopen(file.c_str(), "rb");
	if (in) {
		LoadParam(in, ma.d_ptr());
		fclose(in);
	}
}
void MomentumOptimizer::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error)
{
	/**
	ma = momentum*ma + step * df(a, x)
	a = a - ma
	*/
	if (!x || !jacobi)return;
	jacobi(obj, x, dlayer, error);
	for (size_t layer_num = 0; layer_num < dlayer.size(); ++layer_num) {
		ma[layer_num] = momentum * ma[layer_num] + step * dlayer[layer_num];
		dlayer[layer_num] = -ma[layer_num];
	}
}
Optimizer* MomentumOptimizer::minimize(mat_t momentum)const {
	MomentumOptimizer* train = new MomentumOptimizer(*this);
	train->momentum = momentum;
	train->RegisterMethod(Momentum);
	return (Optimizer*)train;
}
Mat MomentumOptimizer::data(vector<string> &value_name)const
{
	Mat mat(2, 1);
	mat(0) = step;
	mat(1) = momentum;
	vector<string>(2).swap(value_name);
	value_name[0] = "step";
	value_name[1] = "momentum";
	return mat;
}
void MomentumOptimizer::copyTo(Optimizer * op) const
{
	*((MomentumOptimizer*)op) = *this;
}
Mat MomentumOptimizer::Params() const
{
	return (Mat_(1, 1) << momentum);
}
/*
=======================    NesterovMomentum优化器    ======================
*/
NesterovMomentumOptimizer::NesterovMomentumOptimizer(mat_t step) : Optimizer(step) {}
void NesterovMomentumOptimizer::init(vector<Size3>& size)
{
	if (size.empty())return;
	ma.resize(size.size());
	for (size_t layer_num = 0; layer_num < size.size(); ++layer_num)
		ma[layer_num] = Mat::zeros(size[layer_num]);
}
void nn::NesterovMomentumOptimizer::save(string file) const
{
	FILE *out = fopen(file.c_str(), "wb");
	if (out) {
		SaveParam(out, ma.d_ptr());
		fclose(out);
	}
}
void nn::NesterovMomentumOptimizer::load(string file)
{
	FILE *in = fopen(file.c_str(), "rb");
	if (in) {
		LoadParam(in, ma.d_ptr());
		fclose(in);
	}
}
void NesterovMomentumOptimizer::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error)
{
	/**
	ma = momentum*ma + step * df(a - momentum*ma, x)
	a = a - ma
	*/
	if (!x || !jacobi)return;
	for (size_t layer_num = 0; layer_num < ma.size(); ++layer_num) {
		dlayer[layer_num] = -momentum * ma[layer_num];
	}
	jacobi(obj, x, dlayer, error);
	for (size_t layer_num = 0; layer_num < dlayer.size(); ++layer_num) {
		ma[layer_num] = momentum * ma[layer_num] + step * dlayer[layer_num];
		dlayer[layer_num] = -ma[layer_num];
	}
}
Optimizer* NesterovMomentumOptimizer::minimize(mat_t momentum)const {
	NesterovMomentumOptimizer* train = new NesterovMomentumOptimizer(*this);
	train->momentum = momentum;
	train->RegisterMethod(NesterovMomentum);
	return (Optimizer*)train;
}
Mat NesterovMomentumOptimizer::data(vector<string> &value_name)const
{
	Mat mat(2, 1);
	mat(0) = step;
	mat(1) = momentum;
	vector<string>(2).swap(value_name);
	value_name[0] = "step";
	value_name[1] = "momentum";
	return mat;
}
void NesterovMomentumOptimizer::copyTo(Optimizer * op) const
{
	*((NesterovMomentumOptimizer*)op) = *this;
}
Mat NesterovMomentumOptimizer::Params() const
{
	return (Mat_(1, 1) << momentum);
}
/*
=============================    Adagrad优化器    ============================
*/
AdagradOptimizer::AdagradOptimizer(mat_t step) : Optimizer(step) {}
void AdagradOptimizer::init(vector<Size3>& size)
{
	if (size.empty())return;
	alpha.resize(size.size());
	for (size_t layer_num = 0; layer_num < size.size(); ++layer_num)
		alpha[layer_num] = Mat::zeros(size[layer_num]);
}
void nn::AdagradOptimizer::save(string file) const
{
	FILE *out = fopen(file.c_str(), "wb");
	if (out) {
		SaveParam(out, alpha.d_ptr());
		fclose(out);
	}
}
void nn::AdagradOptimizer::load(string file)
{
	FILE *in = fopen(file.c_str(), "rb");
	if (in) {
		LoadParam(in, alpha.d_ptr());
		fclose(in);
	}
}
void AdagradOptimizer::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error)
{
	/**
	alpha = alpha + df(a, x)^2
	a = a - step/sqrt(alpha + epsilon)*df(a, x)
	*/
	if (!x || !jacobi)return;
	jacobi(obj, x, dlayer, error);
	for (size_t layer_num = 0; layer_num < dlayer.size(); ++layer_num) {
		alpha[layer_num] = alpha[layer_num] + dlayer[layer_num].pow(2);
		dlayer[layer_num] = -(step / (alpha[layer_num] + epsilon).sqrt()).mul(dlayer[layer_num]);
	}
}
Optimizer * AdagradOptimizer::minimize(mat_t epsilon) const
{
	AdagradOptimizer* train = new AdagradOptimizer(*this);
	train->epsilon = epsilon;
	train->RegisterMethod(Adagrad);
	return (Optimizer *)train;
}
Mat AdagradOptimizer::data(vector<string> &value_name)const
{
	Mat mat(2, 1);
	mat(0) = step;
	mat(1) = epsilon;
	vector<string>(2).swap(value_name);
	value_name[0] = "step";
	value_name[1] = "epsilon";
	return mat;
}
void AdagradOptimizer::copyTo(Optimizer * op) const
{
	*((AdagradOptimizer*)op) = *this;
}
Mat nn::AdagradOptimizer::Params() const
{
	return (Mat_(1, 1) << epsilon);
}
/*
=============================    RMSProp优化器    ============================
*/
RMSPropOptimizer::RMSPropOptimizer(mat_t step) : Optimizer(step) {}
void RMSPropOptimizer::init(vector<Size3>& size)
{
	if (size.empty())return;
	alpha.resize(size.size());
	for (size_t layer_num = 0; layer_num < size.size(); ++layer_num)
		alpha[layer_num] = Mat::zeros(size[layer_num]);
}
void nn::RMSPropOptimizer::save(string file) const
{
	FILE *out = fopen(file.c_str(), "wb");
	if (out) {
		SaveParam(out, alpha.d_ptr());
		fclose(out);
	}
}
void nn::RMSPropOptimizer::load(string file)
{
	FILE *in = fopen(file.c_str(), "rb");
	if (in) {
		LoadParam(in, alpha.d_ptr());
		fclose(in);
	}
}
void RMSPropOptimizer::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error)
{
	/**
	alpha = beta*alpha + (1 - beta)*df(a, x)^2
	a = a - step/sqrt(alpha + epsilon)*df(a, x)
	*/
	if (!x || !jacobi)return;
	jacobi(obj, x, dlayer, error);
	for (size_t layer_num = 0; layer_num < dlayer.size(); ++layer_num) {
		alpha[layer_num] = decay * alpha[layer_num] + (1 - decay) * dlayer[layer_num].pow(2);
		dlayer[layer_num] = -(step / (alpha[layer_num] + epsilon).sqrt()).mul(dlayer[layer_num]);
	}
}

Optimizer * RMSPropOptimizer::minimize(mat_t decay, mat_t epsilon) const
{
	RMSPropOptimizer* train = new RMSPropOptimizer(*this);
	train->decay = decay;
	train->epsilon = epsilon;
	train->RegisterMethod(RMSProp);
	return (Optimizer*)train;
}
Mat RMSPropOptimizer::data(vector<string> &value_name)const
{
	Mat mat(3, 1);
	mat(0) = step;
	mat(1) = decay;
	mat(2) = epsilon;
	vector<string>(3).swap(value_name);
	value_name[0] = "step";
	value_name[1] = "decay";
	value_name[2] = "epsilon";
	return mat;
}
void RMSPropOptimizer::copyTo(Optimizer * op) const
{
	*((RMSPropOptimizer*)op) = *this;
}
Mat RMSPropOptimizer::Params() const
{
	return (Mat_(2, 1) << decay, epsilon);
}
/*
=============================    Adam优化器    ============================
*/
AdamOptimizer::AdamOptimizer(Val step)
	: Optimizer(step), ma(), alpha() {}
void AdamOptimizer::init(vector<Size3>& size)
{
	if (size.empty())return;
	ma.resize(size.size());
	alpha.resize(size.size());
	for (size_t layer_num = 0; layer_num < size.size(); ++layer_num) {
		ma[layer_num] = Mat::zeros(size[layer_num]);
		alpha[layer_num] = Mat::zeros(size[layer_num]);
	}
}
void nn::AdamOptimizer::save(string file) const
{
	FILE *out = fopen(file.c_str(), "wb");
	if (out) {
		SaveParam(out, ma.d_ptr());
		SaveParam(out, alpha.d_ptr());
		fclose(out);
	}
}
void nn::AdamOptimizer::load(string file)
{
	FILE *in = fopen(file.c_str(), "rb");
	if (in) {
		LoadParam(in, ma.d_ptr());
		LoadParam(in, alpha.d_ptr());
		fclose(in);
	}
}
void AdamOptimizer::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error)
{
	/**
	ma = beta1*ma + (1 - beta1)*df(a, x)
	alpha = beta2*alpha + (1 - beta2)*df(a, x)^2
	a = a - step/sqrt(alpha + epsilon)*ma
	*/
	if (!x || !jacobi)return;
	jacobi(obj, x, dlayer, error);
	for (size_t layer_num = 0; layer_num < dlayer.size(); ++layer_num) {
		Mat d = dlayer[layer_num];
		ma[layer_num] = beta1 * ma[layer_num] + (1 - beta1)*dlayer[layer_num];
		alpha[layer_num] = beta2 * alpha[layer_num] + (1 - beta2)*dlayer[layer_num].pow(2);
		/*ma[layer_num] /= (1 - beta1);
		alpha[layer_num] /= (1 - beta2);*/
		dlayer[layer_num] = -(step / (alpha[layer_num] + epsilon).sqrt()).mul(ma[layer_num]);
	}
}
Optimizer * AdamOptimizer::minimize(mat_t beta1, mat_t beta2, mat_t epsilon) const
{
	AdamOptimizer* train = new AdamOptimizer(*this);
	train->beta1 = beta1;
	train->beta2 = beta2;
	train->epsilon = epsilon;
	train->RegisterMethod(Adam);
	return (Optimizer*)train;
}
Mat AdamOptimizer::data(vector<string> &value_name)const
{
	Mat mat(4, 1);
	mat(0) = step;
	mat(1) = beta1;
	mat(2) = beta2;
	mat(3) = epsilon;
	value_name.resize(4);
	value_name[0] = "step";
	value_name[1] = "beta1";
	value_name[2] = "beta2";
	value_name[3] = "epsilon";
	return mat;
}
void AdamOptimizer::copyTo(Optimizer * op) const
{
	*((AdamOptimizer*)op) = *this;
}
Mat nn::AdamOptimizer::Params() const
{
	return (Mat_(3, 1) << beta1, beta2, epsilon);
}
/*
=============================    NesterovAdam优化器    ============================
*/
NesterovAdamOptimizer::NesterovAdamOptimizer(mat_t step)
	: Optimizer(step), ma(), alpha() {}
void NesterovAdamOptimizer::init(vector<Size3>& size)
{
	if (size.empty())return;
	ma.resize(size.size());
	alpha.resize(size.size());
	for (size_t layer_num = 0; layer_num < size.size(); ++layer_num) {
		ma[layer_num] = Mat::zeros(size[layer_num]);
		alpha[layer_num] = Mat::zeros(size[layer_num]);
	}
}
void nn::NesterovAdamOptimizer::save(string file) const
{
	FILE *out = fopen(file.c_str(), "wb");
	if (out) {
		SaveParam(out, ma.d_ptr());
		SaveParam(out, alpha.d_ptr());
		fclose(out);
	}
}
void nn::NesterovAdamOptimizer::load(string file)
{
	FILE *in = fopen(file.c_str(), "rb");
	if (in) {
		LoadParam(in, ma.d_ptr());
		LoadParam(in, alpha.d_ptr());
		fclose(in);
	}
}
void NesterovAdamOptimizer::Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error)
{
	/**
	ma = beta1*ma + (1 - beta1)*df(a - step/sqrt(alpha + epsilon)*ma, x)
	alpha = beta2*alpha + (1 - beta2)*df(a - step/sqrt(alpha + epsilon)*ma, x)^2
	a = a - step/sqrt(alpha + epsilon)*ma
	*/
	if (!x || !jacobi)return;
	for (size_t layer_num = 0; layer_num < ma.size(); ++layer_num) {
		dlayer[layer_num] = -(step / (alpha[layer_num] + epsilon).sqrt()).mul(ma[layer_num]);
	}
	jacobi(obj, x, dlayer, error);
	for (size_t layer_num = 0; layer_num < dlayer.size(); ++layer_num) {
		Mat d = dlayer[layer_num];
		ma[layer_num] = beta1 * ma[layer_num] + (1 - beta1)*dlayer[layer_num];
		alpha[layer_num] = beta2 * alpha[layer_num] + (1 - beta2)*dlayer[layer_num].pow(2);
		dlayer[layer_num] = -(step / (alpha[layer_num] + epsilon).sqrt()).mul(ma[layer_num]);
	}
}
Optimizer * NesterovAdamOptimizer::minimize(mat_t beta1, mat_t beta2, mat_t epsilon) const
{
	NesterovAdamOptimizer* train = new NesterovAdamOptimizer(*this);
	train->beta1 = beta1;
	train->beta2 = beta2;
	train->epsilon = epsilon;
	train->RegisterMethod(NesterovAdam);
	return (Optimizer*)train;
}

Mat NesterovAdamOptimizer::data(vector<string> &value_name)const
{
	Mat mat(4, 1);
	mat(0) = step;
	mat(1) = beta1;
	mat(2) = beta2;
	mat(3) = epsilon;
	vector<string>(4).swap(value_name);
	value_name[0] = "step";
	value_name[1] = "beta1";
	value_name[2] = "beta2";
	value_name[3] = "epsilon";
	return mat;
}

void NesterovAdamOptimizer::copyTo(Optimizer * op) const
{
	*((NesterovAdamOptimizer*)op) = *this;
}

Mat NesterovAdamOptimizer::Params() const
{
	return (Mat_(3, 1) << beta1, beta2, epsilon);
}


string nn::Optimizer::Method2String(OptimizerMethod Net)
{
	switch (Net)
	{
	case None:return "None";
	case GradientDescent:return "GradientDescent";
	case Momentum:return "Momentum";
	case NesterovMomentum:return "NesterovMomentum";
	case Adagrad:return "Adagrad";
	case RMSProp:return "RMSProp";
	case Adam:return "Adam";
	case NesterovAdam:return "NesterovAdam";
	default:
		return "";
	}
}

OptimizerMethod nn::Optimizer::String2Method(string name)
{
	if (name == "None")return None;
	if (name == "GradientDescent")return GradientDescent;
	if (name == "Momentum")return Momentum;
	if (name == "NesterovMomentum")return NesterovMomentum;
	if (name == "Adagrad")return Adagrad;
	if (name == "RMSProp")return RMSProp;
	if (name == "Adam")return Adam;
	if (name == "NesterovAdam")return NesterovAdam;
	return None;
}

Optimizer * Optimizer::CreateOptimizer(Optimizer * op)
{
	return CreateOptimizer(op->OpMethod(), op->Step(), op->Params());
}
Optimizer * Optimizer::CreateOptimizer(OptimizerInfo op)
{
	switch (op.type)
	{
	case None:
		return Method().minimize();
	case GradientDescent:
		return GradientDescentOptimizer(op.step).minimize();
	case Momentum:
		return MomentumOptimizer(op.step).minimize(op.momentum);
	case NesterovMomentum:
		return NesterovMomentumOptimizer(op.step).minimize(op.momentum);
	case Adagrad:
		return AdagradOptimizer(op.step).minimize(op.epsilon);
	case RMSProp:
		return RMSPropOptimizer(op.step).minimize(op.decay, op.epsilon);
	case Adam:
		return AdamOptimizer(op.step).minimize(op.beta1, op.beta2, op.epsilon);
	case NesterovAdam:
		return NesterovAdamOptimizer(op.step).minimize(op.beta1, op.beta2, op.epsilon);
	default:
		return nullptr;
	}
}
Optimizer * Optimizer::CreateOptimizer(OptimizerMethod opm, mat_t step, const Mat & value)
{
	switch (opm)
	{
	case None:
		return Method().minimize();
	case GradientDescent:
		return GradientDescentOptimizer(step).minimize();
	case Momentum:
		return MomentumOptimizer(step).minimize(value(0));
	case NesterovMomentum:
		return NesterovMomentumOptimizer(step).minimize(value(0));
	case Adagrad:
		return AdagradOptimizer(step).minimize(value(2));
	case RMSProp:
		return RMSPropOptimizer(step).minimize(value(0), value(2));
	case Adam:
		return AdamOptimizer(step).minimize(value(0), value(1), value(2));
	case NesterovAdam:
		return NesterovAdamOptimizer(step).minimize(value(0), value(1), value(2));
	default:
		return nullptr;
	}
}

void Optimizer::SaveParam(FILE * out, const vector<Mat>* vec)
{
	size_t len = vec->size();
	fwrite(&len, sizeof(size_t), 1, out);
	for (const Mat &mat : *vec) {
		int param[3] = { mat.rows(), mat.cols(), mat.channels() };
		fwrite(param, sizeof(int) * 3, 1, out);
		fwrite(mat.data(), mat.memsize(), 1, out);
	}
}

void Optimizer::LoadParam(FILE * in, vector<Mat>* vec)
{
	size_t len;
	fread(&len, sizeof(size_t), 1, in);
	vec->resize(len);
	for (Mat &mat : *vec) {
		int param[3] = { 0 };
		fread(param, sizeof(int) * 3, 1, in);
		mat.create(param[0], param[1], param[2]);
		fread(mat.data(), mat.memsize(), 1, in);
	}
}
