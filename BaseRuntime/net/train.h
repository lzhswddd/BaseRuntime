#ifndef __TRAIN_H__
#define __TRAIN_H__

#include "net_core.h"
#include "optimalize.h"

namespace lzh {
	namespace nn {

		/**
		TrainOption 配置参数类
		@param TrainData *trainData				训练集
		@param uint32 epoch_number				训练次数
		@param int show_epoch					输出轮数(取余)
		@param bool op_init = true				是否初始化优化器
		@param bool everytime_show = false		是否每批次都输出
		@param LOSS_INFO loss_info = nullptr	回调损失信息
		@param bool save = false				是否保存数据
		@param int save_epoch					保存数据轮数(取余)
		@param string savemodel					保存模型路径
		@param string savemethod				保存优化器路径
		*/
		class LZHAPI TrainOption
		{
		public:
			typedef void(*LOSS_INFO)(void*, int, vector<mat_t>*, vector<mat_t>*);
			TrainData *trainData;
			uint32 epoch_number;
			int show_epoch;
			bool op_init = true;
			bool everytime_show = false;
			STLApi<vector<bool(*)(const Mat &, const Mat &)>> evaluate = vector<bool(*)(const Mat &, const Mat &)>();
			LOSS_INFO loss_info = nullptr;
			bool logloss = false;
			STLApi<string> logpath;
			bool save = false;
			int save_epoch;
			STLApi<string> savemodel;
			STLApi<string> savemethod;
		};
		class LZHAPI Train
		{
		public:
			Train(Net *net = nullptr);
			~Train();
			void Running(bool run);
			size_t outputSize()const;
			void RegisterNet(Net *net);
			void RegisterOptimizer(Optimizer *optimizer);
			/**
			@brief FutureJacobi 预测雅可比矩阵
			模型参数先进行迭代后计算梯度矩阵
			@param input 输入
			@param output 输出
			@param d_layer 输出梯度矩阵
			*/
			void FutureJacobi(TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> &error);
			/**
			@brief FutureJacobi 雅可比矩阵
			计算梯度矩阵
			@param input 输入
			@param output 输出
			@param d_layer 输出梯度矩阵
			*/
			void Jacobi(TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> &error);
			/**
			@brief TrainModel 多批次训练模型
			@param input 输入
			@param label 标签
			@param error 损失函数输出
			*/
			void Fit(TrainData::iterator data, vector<mat_t> *error = nullptr);

			void Fit(TrainOption *option);
			void Fit(OptimizerInfo op_info, TrainOption *option);
			void Fit(Optimizer *op, TrainOption *option);
			void Fit(Net *net, Optimizer *op, TrainOption *option);
			void Fit(Net *net, OptimizerInfo op_info, TrainOption *option);
			void initialize();

			mat_t lambda = 1.0f;
			bool regularization = false;
			void *pre = nullptr;
			volatile bool running = true;
		protected:
			/**
			@brief BackPropagation 反向传播
			@param input 输入
			@param output 输出
			@param d_layer 输出梯度值
			*/
			void BackPropagation(TrainData::iterator data, vector<Mat> & d_layer, vector<mat_t> & error);
			void JacobiMat(TrainData::iterator label, vector<Mat> &d_layer, vector<vector<Mat>> &output);
			static void JacobiCallBack(Object obj, TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> &error);
		private:
			Net *net;
			Optimizer* op;
			STLApi<vector<Mat>> dlayer;
			STLApi<vector<Size3>> size;
			STLApi<vector<NetNode<Layer*>*>> loss;
		};
	}
}

#endif // __TRAIN_H__