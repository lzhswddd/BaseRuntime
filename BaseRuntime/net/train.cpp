#define LZH_EXPORTS
#include "train.h"
#include "nn.h"
#include <fstream>
using namespace lzh;


nn::Train::Train(Net * nn)
	: net(nn), op(0)
{
}
nn::Train::~Train()
{
	if (op != nullptr) {
		delete op;
		op = nullptr;
	}
}
void nn::Train::Running(bool run)
{
	running = run;
}
size_t nn::Train::outputSize() const
{
	return loss.size();
}
void nn::Train::RegisterNet(Net * nn)
{
	this->net = nn;
}
void nn::Train::RegisterOptimizer(Optimizer * optimizer)
{
	op = optimizer;
}
void nn::Train::Fit(TrainData::iterator data, vector<mat_t> *error)
{
	if (op == nullptr)return;
	if (data->empty())return;
	if (!net->isEnable()) {
		fprintf(stderr, "error: 网络未初始化!\n");
		throw std::exception("error: 网络未初始化!");
	}
	for (size_t layer_num = 0; layer_num < size.size(); ++layer_num) {
		dlayer[layer_num] = Mat::zeros(size[layer_num]);
	}
	vector<mat_t> err(loss.size());
	for (mat_t &v : err)
		v = 0;
	BackPropagation(data, dlayer, err);
	int idx = 0;
	Net::update_layer(net->NetTree(), dlayer, idx);
	if (error != nullptr)
		err.swap(*error);
	//}
}
void nn::Train::Fit(TrainOption *option)
{
	Timer t;
	Timer t1;
	vector<mat_t> error(loss.size());
	for (mat_t &v : error)
		v = 0;
	t.Start();
	std::ofstream logfile;
	if (option->logloss)logfile.open(option->logpath);
	for (uint32 epoch = 0; epoch < option->epoch_number; ++epoch) {
		t1.Start();
		option->trainData->reset();
		for (int &v : option->trainData->range) {
			if (!running)return;
			TrainData::iterator netData = option->trainData->batches();
			vector<mat_t> err;
			Fit(netData, &err);
			for (size_t i = 0; i < err.size(); ++i)
				error[i] += err[i];
			if (option->everytime_show) {
				if (option->loss_info)
					option->loss_info(pre, 1, &err, nullptr);
				else {
					printf("batch number %d, error ", v + 1);
					for (mat_t &e : err)
						printf("%0.6lf ", e);
					printf("\n");
				}
			}
		}
		if (option->save && epoch % option->save_epoch == option->save_epoch - 1) {
			net->save_param(option->savemodel);
			op->save(option->savemethod);
		}
		if (!option->everytime_show && epoch % option->show_epoch == option->show_epoch - 1) {
			for (mat_t &e : error) {
				e /= (mat_t)option->trainData->len();
			}
			vec_m vaild_err;
			vec_m vaild_acc(loss.size()*option->evaluate.size());
			if(!vaild_acc.empty())for (size_t i = 0; i < loss.size(); ++i) vaild_acc[i] = 0;
			if (!option->evaluate.empty()) {
				Mat vailderr = Mat::zeros((int)loss.size());
				for (const NetData& data : *option->trainData->vaild()) {
					vec_mat output = net->forward(data.input);
					for (size_t i = 0; i < loss.size(); ++i) {
						Mat vailderr_;
						((const costfunction*)(loss[i]->data))->forword(data.label[i], output[i], vailderr_);
						for (int32 idx : range((int32)option->evaluate.size())) {
							if (option->evaluate[idx](data.label[i], output[i])) {
								vaild_acc[i * (int32)option->evaluate.size() + idx] += 1;
							}
						}
						vailderr((int)i) += vailderr_((int)i);
					}
				}
				vailderr /= (mat_t)option->trainData->vaild()->size();
				for (size_t i = 0; i < vaild_acc.size(); ++i) vaild_acc[i] = vaild_acc[i] / option->trainData->vaild()->size() * 100;
				vaild_err = Mat2Vec<mat_t>(vailderr);
			}
			if (option->loss_info)
				option->loss_info(pre, epoch + 1, &error, &vaild_err);
			else {
				printf("epoch %d/%d, learing rate %e, error", epoch + 1, option->epoch_number, op->Step());
				for (mat_t &e : error) {
					printf(" %0.4e", e);
				}
				if (!vaild_err.empty()) {
					printf(" vaild error");
					for (mat_t &e : vaild_err) {
						printf(" %0.4e", e);
					}
					printf(" vaild acc");
					for (mat_t &e : vaild_acc) {
						printf(" %0.2f%%", e);
					}
				}
				TimeData time = t.EndTime();
				TimeData _time(t1.End() * option->epoch_number);
				printf("  cost time %u:%u:%us estimated time %u:%u:%us\n", 
					time.h, time.m, time.s, _time.h, _time.m, _time.s);
			}
			if (option->logloss && logfile) {
				logfile << "epoch," << epoch + 1 << "," << op->Step() << ",";
				logfile << "train_error,";
				for (mat_t &e : error) {
					if(e != *(error.end()-1))
						logfile << e << ",";
					else
						logfile << e;
				}
				if (!vaild_err.empty()) {
					logfile << ",vaild_error,";
					for (mat_t &e : vaild_err) {
						if (e != *(vaild_err.end() - 1))
							logfile << e << ",";
						else
							logfile << e;
					}
					logfile << ",vaild_acc,";
					for (mat_t &e : vaild_acc) {
						if (e != *(vaild_acc.end() - 1))
							logfile << e << ",";
						else
							logfile << e;
					}
				}
				logfile << std::endl;
			}
		}
		for (mat_t &e : error)
			e = 0;
	}
	if (option->logloss)logfile.close();
	if (option->save) {
		net->save_param(option->savemodel);
		op->save(option->savemethod);
	}
}
void nn::Train::Fit(OptimizerInfo op_info, TrainOption *option)
{
	if (!net) {
		fprintf(stderr, "获取模型失败!\n");
		return;
	}
	OptimizerP optimizer = Optimizer::CreateOptimizer(op_info);
	if (optimizer) {
		if (op != nullptr) {
			delete op;
			op = nullptr;
		}
		op = optimizer;
		initialize();
		op->init(size);
		Fit(option);
		delete op;
		op = nullptr;
	}
	else {
		fprintf(stderr, "获取优化方法失败!\n");
	}
}
void nn::Train::Fit(Optimizer *optimizer, TrainOption *option)
{
	if (!net) {
		fprintf(stderr, "获取模型失败!\n");
		return;
	}
	if (optimizer) {
		if (op != nullptr && op != optimizer) {
			delete op;
			op = nullptr;
		}
		op = optimizer;
		initialize();
		if (option->op_init)
			op->init(size);
		Fit(option);
	}
	else {
		fprintf(stderr, "获取优化方法失败!\n");
	}
}
void nn::Train::Fit(Net * nn, Optimizer *op, TrainOption *option)
{
	net = nn;
	Fit(op, option);
}
void nn::Train::Fit(Net * nn, OptimizerInfo op_info, TrainOption *option)
{
	net = nn;
	option->op_init = true;
	Fit(op_info, option);
}
void nn::Train::BackPropagation(TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> & error)
{
	if (op != 0)
		op->Run(d_layer, data, error);
}
void nn::Train::FutureJacobi(TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t>& error)
{
	int idx = 0;
	Net::update_layer(*net, d_layer, idx);
	vector<Mat> variable(data->size());
	vector<vector<Mat>> transmit(1, vector<Mat>(data->size()));
	for (int idx = 0; idx < (int)data->size(); ++idx) {
		variable[idx] = data->at(idx)->input;
		transmit[0][idx] = data->at(idx)->input;
	}
	Net::forward_train(*net, variable, transmit, 0);
	for (Mat &m : d_layer)
		m.setOpp();
	idx = 0;
	Net::update_layer(*net, d_layer, idx);
	if (loss.empty())
	{
		fprintf(stderr, "没有注册损失函数!");
		throw std::exception("没有注册损失函数!");
	}
	for (size_t i = 0; i < loss.size(); ++i) {
		error[i] = ((const costfunction*)loss[i]->data)->forword(data, &transmit, i);
	}
	JacobiMat(data, d_layer, transmit);
}
void nn::Train::Jacobi(TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> & error)
{
	vector<Mat> variable(data->size());
	vector<vector<Mat>> transmit(1, vector<Mat>(data->size()));
	for (int idx = 0; idx < (int)data->size(); ++idx) {
		variable[idx] = data->at(idx)->input;
		transmit[0][idx] = data->at(idx)->input;
	}
	Net::forward_train(*net, variable, transmit, 0);
	if (loss.empty())
	{
		fprintf(stderr, "没有注册损失函数!");
		throw std::exception("没有注册损失函数!");
	}
	for (size_t i = 0; i < loss.size(); ++i) {
		error[i] = ((const costfunction*)loss[i]->data)->forword(data, &transmit, i);
	}
	JacobiMat(data, d_layer, transmit);
}
void nn::Train::JacobiMat(TrainData::iterator label, vector<Mat> &dlayer, vector<vector<Mat>> &output)
{
	int idx = 0;
	int number = 0;
	for (const NetNode<Layer*> *lossNode : loss) {
		((const costfunction*)lossNode->data)->back(label, &output, idx);
		idx += 1;
	}
	Net::back_train(loss[0]->parent, dlayer, output, number, 0);
}
void nn::Train::JacobiCallBack(Object obj, TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t>& error)
{
	Train* train = obj.empty() ? nullptr : obj.ptr<Train>();
	if (train) train->Jacobi(data, d_layer, error);
}
void nn::Train::initialize()
{
	if (net) {
		loss.clear();
		size.size();
		if (regularization) {
			Net::regularization(*net, lambda);
		}
		Net::initialize_loss(*net, loss.d_ptr());
		Net::initialize_mat(*net, size.d_ptr());
		dlayer.resize(size.size());
		reverse(loss.begin(), loss.end());
		op->Register(JacobiCallBack, this);
	}
}
