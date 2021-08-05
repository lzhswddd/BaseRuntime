#define LZH_EXPORTS
#include "convolution.h"
#include "nn.h"
using namespace lzh;

nn::convolution::convolution(string name)
	: parametrics(name)
{
	type = CONV2D;
}
nn::convolution::~convolution()
{
}
Size3 nn::convolution::initialize(Size3 input_size)
{
	param = Xavier(kern_size, kern_size, input_size.c*channel, input_size.c, channel);
	if (regularization)
		updateregular();
	bias = Mat::zeros(1, 1, channel);
	if (!is_copy_border)
		return CalSize(input_size, Size3(kern_size, kern_size, channel*input_size.c), anchor, strides);
	else
		input_size.c = channel;
	return input_size;
}
void nn::convolution::updateregular()
{
	regular = param;
}
void nn::convolution::update(const vector<Mat> &d, int *idx)
{
	param += d[(*idx)++];
	bias += d[(*idx)++];
	if (regularization)
		updateregular();
}
void nn::convolution::forword(const Mat & in, Mat & out) const
{
	out = conv2d(in, param) + bias;
	if (isact)
		out = active.f(out);
}
void nn::convolution::forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> & variable)
{
	this->variable[0] = variable;
	for (size_t idx = 0; idx < in.size(); ++idx)
		out[idx] = conv2d(in[idx], param) + bias;
	variable = out;
	this->variable[1] = variable;
	if (isact)
		for (Mat &m : out)
			m = active.f(m);
}
void nn::convolution::back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const
{
	Mat t;
	for (size_t idx = 0; idx < in.size(); ++idx) {
		if (!last && isact) {
			Mult(in[idx], active.df(variable[1][idx]), out[idx]);
		}
		else
			out = in;
		Sum(out[idx], t, CHANNEL);
		(*dlayer)[dlayer->size() - 1 - *number] += t;
		(*dlayer)[dlayer->size() - 2 - *number] += iconv2d(variable[0][idx], out[idx], param.size3(), false);
		if (!start) {
			Mat r;
			rotate(param, ROTATE_180_ANGLE, r);
			out[idx] = iconv2d(out[idx], r, Size3(), true);
		}
	}
	(*dlayer)[dlayer->size() - 1 - *number] /= (mat_t)in.size();
	(*dlayer)[dlayer->size() - 2 - *number] /= (mat_t)in.size();
	if (regularization)
		(*dlayer)[dlayer->size() - 2 - *number] += lambda * regular;
	*number += 2;
}
void nn::convolution::save(json * jarray, FILE * file) const
{
	json info;
	info["type"] = Layer::Type2String(type);
	info["name"] = name;
	info["layer"] = layer_index;
	info["isact"] = isact;
	if (isact)
		info["activate"] = Func2String(active.f);
	info["channel"] = channel;
	info["kernSize"] = kern_size;
	info["strides"]["height"] = strides.h;
	info["strides"]["width"] = strides.w;
	info["anchor"]["x"] = anchor.x;
	info["anchor"]["y"] = anchor.y;
	info["is_copy_border"] = is_copy_border;
	jarray->push_back(info);
	save_param(file);
}
void nn::convolution::load(json & info, FILE * file)
{
	type = Layer::String2Type(info["type"]);
	layer_index = info["layer"];
	anchor.x = info["anchor"]["x"];
	anchor.y = info["anchor"]["y"];
	channel = info["channel"];
	kern_size = info["kernSize"];
	is_copy_border = info["is_copy_border"];
	strides.h = info["strides"]["height"];
	strides.w = info["strides"]["width"];
	isact = info["isact"];
	if (isact)
		SetFunc(info["activate"], &active.f, &active.df);
	load_param(file);
}
void nn::convolution::save_param(FILE * file) const
{
	nn::save_mat(file, param);
	nn::save_mat(file, bias);
}
void nn::convolution::load_param(FILE * file)
{
	nn::load_mat(file, param);
	nn::load_mat(file, bias);
}
void nn::convolution::show(std::ostream &out) const
{
	out << name << "{" << std::endl;
	out << "Layer-> Conv2D" << std::endl;
	out << "kern size-> " << param.size3() << std::endl;
	out << "bias size-> " << bias.size3() << std::endl;
	out << "strides-> " << strides << std::endl;
	out << "anchor-> " << anchor << std::endl;
	out << "is_copy_border-> " << (is_copy_border ? "true" : "false") << std::endl;
	if (isact)
		out << "activation-> " << Func2String(active.f) << std::endl;
	out << "}";
}
mat_t nn::convolution::norm(int num) const
{
	return param.sum(num, true);
}
void nn::convolution::append_size(vector<Size3>* size)
{
	size->push_back(param.size3());
	size->push_back(bias.size3());
}
const Mat nn::convolution::iconv2d(const Mat & input, const Mat & kern, Size3 E_kern_size, bool copyborder) const
{
	Mat output;
	Size3 area;
	int left, right, top, bottom, anchor_x, anchor_y;
	if (anchor == Point(-1, -1)) {
		anchor_y = kern.rows() / 2;
		anchor_x = kern.cols() / 2;
	}
	if (E_kern_size.h == 0 || E_kern_size.w == 0 || E_kern_size.c == 0) {

		area.c = kern.channels() / input.channels();
		if (is_copy_border) {
			area.h = input.rows() * strides.h;
			area.w = input.cols() * strides.w;
		}
		else {
			top = anchor_y;
			bottom = kern.rows() - anchor_y - 1;
			left = anchor_x;
			right = kern.cols() - anchor_x - 1;
			area.h = (input.rows() + top + bottom) * strides.h;
			area.w = (input.cols() + left + right) * strides.w;
		}
		Mat in;
		if (!is_copy_border)
			copyMakeBorder(input, in, top, bottom, left, right);
		else
			in = input;
		Conv2D(in, output, kern, MAT_T, anchor, strides, copyborder);
		//for (int i = 0; i < area.c; i++) {
		//	Mat sum = output.Channel(i);
		//	for (int j = 0; j < input.channels(); j++) {
		//		if (!is_copy_border)
		//			sum += Filter2D(in[j], kern[i*input.channels() + j], anchor, strides, is_copy_border);
		//		else
		//			sum += Filter2D(in[j], kern[i*input.channels() + j], anchor, strides, is_copy_border);
		//	}
		//}
	}
	else {
		area.c = E_kern_size.c;
		output = Mat::zeros(E_kern_size.h, E_kern_size.w, area.c);
		Mat in;
		if (is_copy_border)
			copyMakeBorder(input, in, E_kern_size.h / 2, E_kern_size.h / 2, E_kern_size.w / 2, E_kern_size.w / 2);
		else
			in = input;
		CalSize(in.size3(), kern.size3(), anchor, strides, left, right, top, bottom);
		//for (int i = 0; i < kern.channels(); i++) {
		//	for (int j = 0; j < input.channels(); j++) {
		//		if (is_copy_border)
		//			Filter2D(in[j], kern[i], anchor, strides, is_copy_border).copyTo(output.Channel(i*input.channels() + j));
		//		else
		//			Filter2D(in[j], kern[i], anchor, strides, is_copy_border).copyTo(output.Channel(i*input.channels() + j));
		//	}
		//}
		int kern_row = kern.rows();
		int kern_col = kern.cols();

		for (int kern_c = 0; kern_c < kern.channels(); kern_c++) {
			for (int in_c = 0; in_c < input.channels(); in_c++) {
				int c = kern_c * input.channels() + in_c;
				for (int row = top, x = 0; row < in.rows() - bottom; row += strides.h, x++)
					for (int col = left, y = 0; col < in.cols() - right; col += strides.w, y++) {
						int px = col - anchor_x;
						int py = row - anchor_y;
						mat_t value = 0;
						for (int i = 0; i < kern_row; ++i) {
							for (int j = 0; j < kern_col; ++j) {
								value += in(py + i, px + j, in_c)*kern(i, j, kern_c);
							}
						}
						output(x, y, c) = value;
					}
			}
		}
	}
	return output;
}
const Mat nn::convolution::conv2d(const Mat & input, const Mat & kern)const
{
	Mat output;
	Conv2D(input, output, kern, MAT_T, anchor, strides, is_copy_border);
	return output;
}