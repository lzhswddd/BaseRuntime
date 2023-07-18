#pragma once
#ifndef __NET_CORE_H__
#define __NET_CORE_H__

#include "../core/algorithm.h"

using std::vector;
using std::string;

namespace lzh {
	namespace nn
	{
		enum ActivationType {
			ACTI_SIGMOID,
			ACTI_TANH,
			ACTI_RELU,
			ACTI_ELU,
			ACTI_LReLU,
			ACTI_SELU,
			ACTI_SOFTMAX
		};
		enum ReduceType {
			NORM_L2,
			QUADRATIC,
			CROSSENTROPY,
			SOFTMAXCROSSENTROPY,
		};
		/**
		CONV2D			卷积层
		POOL			池化层
		FULL_CONNECTION 全连接层
		ACTIVATION		激活层
		RESHAPE			重置维度层
		DROPOUT			随机使能层
		*/
		enum LayerType {
			NONE = 0,
			POOL,
			CONV2D,
			FULLCONNECTION,
			BATCHNORMALIZATION,
			ACTIVATION,
			RESHAPE,
			DROPOUT,
			PRELU,
			SIGMOID,
			TANH,
			LOSS
		};
		/**
		None				不提供优化功能
		GradientDescent		提供梯度下降法
		Momentum			提供动量梯度下降法
		NesterovMomentum	提供预测动量梯度下降法
		Adagrad				提供自适应学习率梯度下降法
		RMSProp				提供改良自适应学习率梯度下降法
		Adam				提供自适应学习率动量梯度下降法
		NesterovAdam		提供自适应学习率预测动量梯度下降法
		*/
		enum OptimizerMethod
		{
			None = 0,		//!< 不提供优化功能
			GradientDescent,//!< 提供梯度下降法
			Momentum,		//!< 提供动量梯度下降法
			NesterovMomentum,//!< 提供预测动量梯度下降法
			Adagrad,		//!< 提供自适应学习率梯度下降法
			RMSProp,		//!< 提供改良自适应学习率梯度下降法
			Adam,			//!< 提供自适应学习率动量梯度下降法
			NesterovAdam	//!< 提供自适应学习率预测动量梯度下降法
		};
		/**
		@brief Activation 激活函数类
		成员
		Act activation_f 激活函数
		Act activation_df 激活函数导数
		*/
		class LZHAPI ActivationInfo
		{
		public:
			ActivationInfo() : f(nullptr), df(nullptr) {}
			ActivationInfo(ActivationType acti);
			ActivationInfo(Act acti);
			Act f;
			Act df;
		};
		/**
		@brief ConvInfo 卷积参数类
		成员
		Size strides 滑动步长
		Point anchor 像素对应卷积核坐标
		anchor默认为Point(-1,-1), 像素对应卷积核中心
		*/
		class LZHAPI ConvInfo
		{
		public:
			explicit ConvInfo() {}
			bool isact = false;
			int channel;
			int kern_size;
			Size strides;
			Point anchor;
			bool is_copy_border;
		};
		class LZHAPI NetData
		{
		public:
			NetData() : input(), label() {}
			NetData(const Mat &input, const Mat & label) 
				: input(input), label(vector<Mat>(1, label)) {}
			NetData(const Mat &input, const vector<Mat> & label)
				: input(input), label(label) {}
			Mat input;
			STLApi<vector<Mat>> label;
		};

		class LZHAPI OptimizerInfo
		{
		public:
			explicit OptimizerInfo() :type(OptimizerMethod::None) {}
			OptimizerInfo(OptimizerMethod type, Val step = 1e-3)
				: type(type), step(step)
			{
				momentum = 0.9f;
				decay = 0.9f;
				epsilon = 1e-7f;
				beta1 = 0.9f;
				beta2 = 0.999f;
			}
			OptimizerMethod type;
			mat_t step;
			mat_t momentum;
			mat_t decay;
			mat_t epsilon;
			mat_t beta1;
			mat_t beta2;
		};

		class LZHAPI Bbox {
		public:
			mat_t score;
			int x1;
			int y1;
			int x2;
			int y2;
			mat_t area;
			mat_t ppoint[10];
			mat_t regreCoord[4];
		};
	}
}

#endif // !__NET_CORE_H__
