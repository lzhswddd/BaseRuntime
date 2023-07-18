#define LZH_EXPORTS
#include "pool.h"
#include "nn.h"
using namespace lzh;

nn::pool::pool(string name)
	: Layer(name)
{
	isinit = true;
	isforword = true;
	isback = true;
	isupdate = false;
	isparam = false;
	type = POOL;
}
nn::pool::~pool()
{
}
void nn::pool::forword(const Mat & in, Mat &out) const
{
	if (pool_type == max_pool)
	{
		out = MaxPool(in);
	}
	else if (pool_type == average_pool) {
		out = AveragePool(in);
	}
}
void nn::pool::forword_train(const vector<Mat> &in, vector<Mat> & out, vector<Mat> & variable)
{
	insize.resize(in.size());
	for (size_t idx = 0; idx < in.size(); ++idx) {
		insize[idx] = in[idx].size3();
	}
	if (pool_type == max_pool)
	{
		if (markpoint.empty()) {
			markpoint.resize(in.size());
			for (size_t idx = 0; idx < in.size(); ++idx) {
				markpoint[idx].create(2, insize[idx].h*insize[idx].w, insize[idx].c);
				out[idx] = MaxPool(in[idx], idx);
			}
		}
		else {
			for (size_t idx = 0; idx < in.size(); ++idx)
				out[idx] = MaxPool(in[idx], idx);
		}
	}
	else if (pool_type == average_pool) {
		for (size_t idx = 0; idx < in.size(); ++idx) {
			out[idx] = AveragePool(in[idx]);
		}
	}
	variable = out;
}
void nn::pool::back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *, int *)const
{
	for (size_t idx = 0; idx < in.size(); ++idx)
		out[idx] = upsample(in[idx], idx);
}
Size3 nn::pool::initialize(Size3 input_size)
{
	return CalSize(input_size, Size(ksize.h, ksize.w), Point(0, 0), Size(strides, strides));;
}
void nn::pool::save(json * jarray, FILE * ) const
{
	json info;
	info["type"] = Layer::Type2String(type);
	info["name"] = name;
	info["layer"] = layer_index;
	info["pool_type"] = pool::Pooltype2String(pool_type);
	info["strides"] = strides;
	info["size"]["height"] = ksize.h;
	info["size"]["width"] = ksize.w;
	jarray->push_back(info);
}
void nn::pool::load(json & info, FILE * )
{
	pool_type = pool::String2Pooltype(info["pool_type"]);
	strides = info["strides"];
	ksize.h = info["size"]["height"];
	ksize.w = info["size"]["width"];
	type = Layer::String2Type(info["type"]);
	layer_index = info["layer"];
}
void nn::pool::show(std::ostream & out) const
{
	out << name << "{" << std::endl;
	if (pool_type == max_pool)
		out << "Layer-> MaxPool" << std::endl;
	else if (pool_type == average_pool)
		out << "Layer-> AveragePool" << std::endl;
	out << "size-> " << ksize << std::endl;
	out << "strides-> " << strides << std::endl;
	out << "}";
}
const Mat nn::pool::upsample(const Mat & input, size_t idx)const
{
	if (pool_type == average_pool)
		return iAveragePool(input, idx);
	else if (pool_type == max_pool)
		return iMaxPool(input, idx);
	else
		return input;
}
const Mat nn::pool::MaxPool(const Mat & input)const
{
	Mat dst(CalSize(input.size3(), Size(ksize.h, ksize.w), Point(0, 0), Size(strides, strides)));
	for (int z = 0; z < input.channels(); z++)
		for (int row = 0, x = 0; row < input.rows() - ksize.h; row += strides, x++)
			for (int col = 0, y = 0; col < input.cols() - ksize.w; col += strides, y++) {
				mat_t value = input(row, col, z);
				for (int i = 0; i < ksize.h; ++i) {
					for (int j = 0; j < ksize.w; ++j) {
						mat_t v = input(row + i, col + j, z);
						if (value < v) {
							value = v;
						}
					}
				}
				dst(x, y, z) = value;
			}
	return dst;
}
const Mat nn::pool::MaxPool(const Mat & input, size_t idx)
{
	Mat dst(CalSize(input.size3(), Size(ksize.h, ksize.w), Point(0, 0), Size(strides, strides)));
	Point p;
	for (int z = 0; z < input.channels(); z++)
		for (int row = 0, x = 0; row <= input.rows() - ksize.h; row += strides, x++)
			for (int col = 0, y = 0; col <= input.cols() - ksize.w; col += strides, y++) {
				mat_t value = input(row, col, z);
				p.x = row;
				p.y = col;
				for (int i = 0; i < ksize.h; ++i) {
					for (int j = 0; j < ksize.w; ++j) {
						mat_t v = input(row + i, col + j, z);
						if (value < v) {
							value = v;
							p.x = row + i;
							p.y = col + j;
						}
					}
				}
				markpoint[idx](x*dst.cols() + y, 0, z) = (mat_t)p.x;
				markpoint[idx](x*dst.cols() + y, 1, z) = (mat_t)p.y;
				dst(x, y, z) = value;
			}
	return dst;
}
const Mat nn::pool::iMaxPool(const Mat & input, size_t idx)const
{
	Mat dst = Mat::zeros(insize[idx]);
	for (int z = 0; z < input.channels(); z++) {
		int row = 0;
		for (int i = 0; i < input.rows(); i++)
			for (int j = 0; j < input.cols(); j++) {
				dst((int)markpoint[idx](row, 0, z), (int)markpoint[idx](row, 1, z), z) = input(i, j, z);
				row += 1;
			}
	}
	return dst;
}
const Mat nn::pool::AveragePool(const Mat & input)const
{
	Mat dst(CalSize(input.size3(), Size(ksize.h, ksize.w), Point(0, 0), Size(strides, strides)));
	for (int z = 0; z < input.channels(); z++)
		for (int row = 0, x = 0; row <= input.rows() - ksize.h; row += strides, x++)
			for (int col = 0, y = 0; col <= input.cols() - ksize.w; col += strides, y++) {
				mat_t value = 0;
				for (int i = 0; i < ksize.h; ++i) {
					for (int j = 0; j < ksize.w; ++j) {
						value += input(row, col, z);
					}
				}
				dst(x, y, z) = value / mat_t(ksize.h*ksize.w);
			}
	return dst;
}
const Mat nn::pool::iAveragePool(const Mat & input, size_t idx)const
{
	Mat dst = Mat::zeros(insize[idx]);
	for (int z = 0; z < input.channels(); z++)
		for (int row = 0; row < input.rows(); row++)
			for (int col = 0; col < input.rows(); col++) {
				for (int i = row * ksize.h; i < (row + 1)*ksize.h; ++i) {
					for (int j = col * ksize.w; j < (col + 1)*ksize.w; ++j) {
						dst(i, j, z) = input(row, col, z);
					}
				}
			}
	return dst;
}
nn::pool::pooltype nn::pool::String2Pooltype(string str)
{
	if (str == "max_pool")
		return max_pool;
	else if (str == "average_pool")
		return average_pool;
	else if (str == "gobal_max_pool")
		return gobal_max_pool;
	else if (str == "gobal_average_pool")
		return gobal_average_pool;
	else
		return max_pool;
}
string nn::pool::Pooltype2String(pooltype type)
{
	if (type == max_pool)
		return "max_pool";
	else if (type == average_pool)
		return "average_pool";
	else if (type == gobal_max_pool)
		return "gobal_max_pool";
	else if (type == gobal_average_pool)
		return "gobal_average_pool";
	else
		return "";
}
