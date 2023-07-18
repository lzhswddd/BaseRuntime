#define LZH_EXPORTS
#include "costfunction.h"
#include "nn.h"
using namespace lzh;

nn::costfunction::costfunction(string name)
	: Layer(name), f(nullptr), df(nullptr)
{
	isinit = false;
	isforword = false;
	isback = false;
	isupdate = false;
	isparam = false;
	type = LOSS;
}
void nn::costfunction::setfunction(LF loss_f)
{
	SetFunc(loss_f, &this->f, &this->df);
	if (loss_f == lzh::CrossEntropy || loss_f == lzh::SoftmaxCrossEntropy)
		ignore_active = true;
}
void nn::costfunction::setfunction(ReduceType loss_f)
{
	switch (loss_f)
	{
	case nn::NORM_L2:
		SetFunc(lzh::L2, &this->f, &this->df);
		break;
	case nn::QUADRATIC:
		SetFunc(lzh::Quadratic, &this->f, &this->df);
		break;
	case nn::CROSSENTROPY:
		SetFunc(lzh::CrossEntropy, &this->f, &this->df);
		ignore_active = true;
		break;
	case nn::SOFTMAXCROSSENTROPY:
		SetFunc(lzh::SoftmaxCrossEntropy, &this->f, &this->df);
		ignore_active = true;
		break;
	default:
		break;
	}
}
nn::costfunction::~costfunction()
{
}
void nn::costfunction::forword(const Mat & in, Mat &out) const
{
	out = in;
}
void nn::costfunction::forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> & )
{
	out = in;
}
void nn::costfunction::back(const vector<Mat> & in, vector<Mat> & out, vector<Mat> *, int *) const
{
	out = in;
}
void nn::costfunction::forword(const Mat & label, const Mat & output, Mat & dst) const
{
	dst = f(label, output);
}
mat_t nn::costfunction::forword(TrainData::iterator label, const vector<vector<Mat>>* output, size_t idx) const
{
	mat_t error = _T(0); Mat t;
	for (size_t i = 0; i < label->size(); ++i) {
		t = f(label->at(i)->label[idx], output->at(idx)[i]);
		error += t.norm();
	}
	error /= (mat_t)label->size();
	return error;
}
void nn::costfunction::back(const Mat & label, const Mat & output, Mat & dst) const
{
	dst = weight * df(label, output);
}
void nn::costfunction::back(TrainData::iterator label, vector<vector<Mat>>* output, size_t idx) const
{
	Mat t;
	//Mat out = zeros(output->at(idx)[0].size3());
	for (size_t i = 0; i < label->size(); ++i) {
		output->at(idx)[i] = weight * df(label->at(i)->label[idx], output->at(idx)[i]);
	}
	//out /= (mat_t)label->size();
	//for (size_t i = 0; i < output->at(idx).size(); ++i) {
	//	output->at(idx)[i] = out;
	//}
}
Size3 nn::costfunction::initialize(Size3 param_size)
{
	return param_size;
}
void nn::costfunction::save(json * , FILE * ) const
{
}
void nn::costfunction::load(json & , FILE * )
{
}
void nn::costfunction::show(std::ostream &out) const
{
	out << name << "{" << std::endl;
	out << "Layer-> Loss" << std::endl;
	out << "loss-> " << Func2String(f) << std::endl;
	out << "ignore_active-> " << (ignore_active ? "true" : "false") << std::endl;
	out << "}";
}
