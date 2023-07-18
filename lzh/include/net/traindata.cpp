#define LZH_EXPORTS
#include "traindata.h"
#include "../lzh.h"
#include <fstream>
#include <random>

using namespace lzh;
nn::TrainData::TrainData()
{
}
nn::TrainData::TrainData(string rootpath, string imglist,
	int batch_size, const vector<Mat>(*label_process)(const Mat&))
	: batch_size(batch_size), rootpath(rootpath)
{
	load_train_data(rootpath, imglist, batch_size, label_process);
}
nn::TrainData::TrainData(const vec_mat & data, const vec_mat & label, int batch_size, const vector<Mat>(*)(const Mat&))
	: batch_size(batch_size)
{
	if (data.size() == label.size()) {
		clear();
		batch_number = (int)data.size() / batch_size;
		range = lzh::range(batch_number);
		traindata.resize(data.size());
		batchdata.resize(batch_number);
		for (int32 i : lzh::range(_I(data.size()))) {
			traindata[i] = { data[i],  vec_mat(1, label[i]) };
		}
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(traindata.begin(), traindata.end(), g);
		index = 0;
		for (vector<NetData*> &vec : batchdata)
		{
			vec.resize(batch_size);
			for (NetData* &mat : vec)
			{
				mat = &traindata[index++];
			}
		}
	}
}
nn::TrainData::TrainData(const vec_mat & data, const vecs_mat & label, int batch_size, const vector<Mat>(*)(const Mat &))
	: batch_size(batch_size)
{
	if (data.size() == label.size()) {
		clear();
		batch_number = (int)data.size() / batch_size;
		range = lzh::range(batch_number);
		traindata.resize(data.size());
		batchdata.resize(batch_number);
		for (int32 i : lzh::range(_I(data.size()))) {
			traindata[i] = { data[i], label[i] };
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(traindata.begin(), traindata.end(), g);
		index = 0;
		for (vector<NetData*> &vec : batchdata)
		{
			vec.resize(batch_size);
			for (NetData* &mat : vec)
			{
				mat = &traindata[index++];
			}
		}
	}
}
nn::TrainData::TrainData(
	const vector<NetData> &data,
	const vector<NetData> &vaild,
	const vector<NetData> &test, int batch_size)
	: batch_size(batch_size), vailddata(vaild), testdata(test)
{
	clear();
	batch_number = (int)data.size() / batch_size;
	range = lzh::range(batch_number);
	traindata.resize(data.size());
	batchdata.resize(batch_number);
	for (int32 i : lzh::range(_I(data.size()))) {
		traindata[i] = data[i];
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(traindata.begin(), traindata.end(), g);
	index = 0;
	for (vector<NetData*> &vec : batchdata)
	{
		vec.resize(batch_size);
		for (NetData* &mat : vec)
		{
			mat = &traindata[index++];
		}
	}
}
void nn::TrainData::load_train_data(string rootpath, string imglist,
	int batch_size, const vector<Mat>(*label_process)(const Mat&))
{
	clear();
	this->rootpath = rootpath;
	this->batch_size = batch_size;
	get_train_data(imglist, label_process);
	batch_number = (int)imgpath.size() / batch_size;
	data.resize(batch_size);
	traindata.resize(batch_number * batch_size);
	range = lzh::range(batch_size);
}
nn::TrainData::~TrainData()
{
}
void nn::TrainData::Running(bool run)
{
	running = run;
}
void nn::TrainData::reset()
{
    if (batchdata.size() == range.size()) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(traindata.begin(), traindata.end(), g);
		index = 0;
		for (vector<NetData*> &vec : batchdata)
		{
			for (NetData* &mat : vec)
			{
				mat = &traindata[index++];
			}
		}
	}
	index = 0;
}
void nn::TrainData::clear()
{
	vector<vector<NetData*>>().swap(batchdata);
	vector<NetData>().swap(traindata);
	vector<int>().swap(range);
}
bool nn::TrainData::isEnable() const
{
	return enable;
}
int nn::TrainData::batchSize() const
{
	return batch_size;
}
bool nn::TrainData::all_load() const
{
	return batchdata.size() == range.size();
}
int nn::TrainData::len() const
{
	return (int)range.size();
}
nn::TrainData::iterator lzh::nn::TrainData::batches()
{
	vector<NetData*>* batch;
    if (batchdata.size() == (size_t)batch_number) {
		batch = &batchdata[index];
	}
	else {
		next();
		batch = &batchdata[index];
        if (batchdata.size() == (size_t)batch_number) {
			vector<vector<Mat>>().swap(label);
			vector<Mat>().swap(data);
			vector<string>().swap(imgpath);
		}
	}
	index = (index + 1) % batch_number;
	return batch;
}
const vector<nn::NetData>* lzh::nn::TrainData::vaild() const
{
	return vailddata.d_ptr();
}
const vector<nn::NetData>* lzh::nn::TrainData::test() const
{
	return testdata.d_ptr();
}
nn::TrainData::DataBox lzh::nn::TrainData::all_batches()
{
	return (DataBox)batchdata.d_ptr();
}
nn::TrainData::ConstDataBox nn::TrainData::all_batches() const
{
	return (ConstDataBox)batchdata.d_ptr();
}
void nn::TrainData::load_all_data(bool is_show, void(*print)(void*, const char *))
{
	index = 0;
    if (traindata.size() == (size_t)(batch_number * batch_size)) {
		for (int &v : range) {
			if (!running) {
				return;
			}
			next();
			index += 1;
			if (is_show) {
				string info = "load batch " + std::to_string(v + 1) + ", all batches " + std::to_string(batch_number);
				if (print)
					print(pre, info.c_str());
				else
					printf("%s\n", info.c_str());
			}
		}
	}
	vector<vector<Mat>>().swap(label);
	vector<Mat>().swap(data);
	vector<string>().swap(imgpath);
	index = 0;
}
void nn::TrainData::register_process(void(*mat)(const Mat&, Mat&))
{
	process_mat = mat;
}
void nn::TrainData::next()
{
	vector<NetData*> netData(batch_size);
	for (int i = 0; i < batch_size; ++i) {
		if (!running) {
			return;
		}
		Mat img = imread(imgpath[index*batch_size + i]);
		if (process_mat != nullptr)process_mat(img, img);
		traindata[index*batch_size + i].input = img;
		traindata[index*batch_size + i].label = label[index*batch_size + i];
		netData[i] = &traindata[index*batch_size + i];
	}
	batchdata.push_back(netData);
}
void nn::TrainData::get_train_data(string imglist, const vector<Mat>(*label_process)(const Mat&))
{
	vector<string>().swap(imgpath);
	std::ifstream in(imglist);
	vector<string> file;
	if (in.is_open()) {
		string str;
		while (std::getline(in, str)) {
			if (!running) {
				in.close(); return;
			}
			file.push_back(str);
		}
        in.close();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(file.begin(), file.end(), g);
		for (string &s : file) {
			if (!running) {
				return;
			}
			vector<string> v = strSplit(s, ' ');
			imgpath.push_back(rootpath.obj() + "\\" + v[0]);
			v.erase(v.begin());
			vec_m vec = str2Data<mat_t>(v);
			Mat label_(1, (int)vec.size(), 1, &vec[0], true);
			if (label_process != nullptr)
				label.push_back(label_process(label_));
			else
				label.push_back(labelProcess(label_));
		}
		enable = true;
	}
}
const vector<Mat> nn::TrainData::labelProcess(const Mat &l)
{
	return vector<Mat>(1, l);
}
