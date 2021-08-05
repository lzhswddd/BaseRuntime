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
		CONV2D			�����
		POOL			�ػ���
		FULL_CONNECTION ȫ���Ӳ�
		ACTIVATION		�����
		RESHAPE			����ά�Ȳ�
		DROPOUT			���ʹ�ܲ�
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
		None				���ṩ�Ż�����
		GradientDescent		�ṩ�ݶ��½���
		Momentum			�ṩ�����ݶ��½���
		NesterovMomentum	�ṩԤ�⶯���ݶ��½���
		Adagrad				�ṩ����Ӧѧϰ���ݶ��½���
		RMSProp				�ṩ��������Ӧѧϰ���ݶ��½���
		Adam				�ṩ����Ӧѧϰ�ʶ����ݶ��½���
		NesterovAdam		�ṩ����Ӧѧϰ��Ԥ�⶯���ݶ��½���
		*/
		enum OptimizerMethod
		{
			None = 0,		//!< ���ṩ�Ż�����
			GradientDescent,//!< �ṩ�ݶ��½���
			Momentum,		//!< �ṩ�����ݶ��½���
			NesterovMomentum,//!< �ṩԤ�⶯���ݶ��½���
			Adagrad,		//!< �ṩ����Ӧѧϰ���ݶ��½���
			RMSProp,		//!< �ṩ��������Ӧѧϰ���ݶ��½���
			Adam,			//!< �ṩ����Ӧѧϰ�ʶ����ݶ��½���
			NesterovAdam	//!< �ṩ����Ӧѧϰ��Ԥ�⶯���ݶ��½���
		};
		/**
		@brief Activation �������
		��Ա
		Act activation_f �����
		Act activation_df ���������
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
		@brief ConvInfo ���������
		��Ա
		Size strides ��������
		Point anchor ���ض�Ӧ���������
		anchorĬ��ΪPoint(-1,-1), ���ض�Ӧ���������
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
