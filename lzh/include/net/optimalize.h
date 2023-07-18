#ifndef __OPTIMALIZE_H__
#define __OPTIMALIZE_H__

#include "net_core.h"
#include "traindata.h"

namespace lzh {
	namespace nn
	{
		/**
		@brief Optimizer优化器基类
		注册模型的损失函数loss
		提供优化方法
		*/
		class LZHAPI Optimizer
		{
		public:
			typedef void(*Jacobi)(Object, TrainData::iterator, vector<Mat>&, vector<mat_t>&);
			explicit Optimizer();
			Optimizer(mat_t step);
			~Optimizer();
			/**
			@brief 初始化优化器参数
			@param size 优化矩阵尺寸
			*/
			virtual void init(vector<Size3>& size) = 0;
			virtual void save(string file)const = 0;
			virtual void load(string file) = 0;
			/**
			@brief 运行优化器迭代1次
			@param x 网络输入
			@param y 网络输出
			@param da 变化量
			@param size
			@param size
			*/
			virtual void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error) = 0;
			void Register(Jacobi jacobi, const void* obj = nullptr);
			bool Enable(const Mat &x, const vector<Mat> &y)const;
			void RegisterMethod(OptimizerMethod method);
			OptimizerMethod OpMethod()const;
			virtual void copyTo(Optimizer* op)const = 0;
			virtual Mat Params()const = 0;
			mat_t& Step() { return step; }

			static string Method2String(OptimizerMethod method);
			static OptimizerMethod String2Method(string name);
			static Optimizer* CreateOptimizer(Optimizer* op);
			static Optimizer* CreateOptimizer(OptimizerInfo optimizer_info);
			/**
			@brief CreateOptimizer 创建优化器
			返回优化器
			@param opm 优化器类型
			@param step 学习率
			@param loss_f 损失函数
			@param value 超参
			*/
			static Optimizer* CreateOptimizer(OptimizerMethod opm, mat_t step = 1e-2f, const Mat &value = Mat(Size(3), 0.9f, 0.999e-3f, 1e-7f));
			static void SaveParam(FILE * file, const vector<Mat> *vec);
			static void LoadParam(FILE * file, vector<Mat> *vec);
		protected:
			//网络实体
			void* obj;
			Jacobi jacobi;
			//学习率
			mat_t step;
			//继承基类的类型
			OptimizerMethod method;
		};
		/**
		@brief Method网络的函数配置
		注册模型的损失函数loss
		不提供优化方法
		*/
		class LZHAPI Method :public Optimizer
		{
		public:
			explicit Method();
			Method(mat_t step);
            void init(vector<Size3>& ) {}
            void save(string )const {}
            void load(string ) {}
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			Optimizer* minimize()const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		};
		/**
		@brief GradientDescentOptimizer网络的函数配置
		注册模型的损失函数loss
		提供优化方法GradientDescent
		a = a - step * df(a, x)
		*/
		class LZHAPI GradientDescentOptimizer :public Optimizer
		{
		public:
			/**
			@brief 优化器构造函数
			@param step 学习率
			*/
			explicit GradientDescentOptimizer(mat_t step = 1e-2);
            void init(vector<Size3>& ) {}
            void save(string )const {}
            void load(string ) {}
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief 配置模型函数，返回注册的优化器Optimizer
			*/
			Optimizer* minimize()const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		};
		/**
		@brief MomentumOptimizer网络的函数配置
		注册模型的损失函数loss
		提供优化方法MomentumGradientDescent
		ma = momentum*ma + step * df(a, x)
		a = a - ma
		*/
		class LZHAPI MomentumOptimizer :public Optimizer
		{
		public:
			/**
			@brief 优化器构造函数
			@param step 学习率
			@param gama 动量系数
			*/
			explicit MomentumOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief 配置模型函数，返回注册的优化器Optimizer
			@param loss_ 损失函数
			@param output_ 输出函数
			@param activation_ 激活函数
			*/
			Optimizer* minimize(mat_t momentum = 0.9f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			mat_t momentum;
		};
		/**
		@brief NesterovMomentumOptimizer网络的函数配置
		注册模型的损失函数loss
		提供优化方法NesterovMomentumGradientDescent
		ma = momentum*ma + step * df(a - momentum*ma, x)
		a = a - ma
		*/
		class LZHAPI NesterovMomentumOptimizer :public Optimizer
		{
		public:
			/**
			@brief 优化器构造函数
			@param step 学习率
			@param gama 动量系数
			*/
			explicit NesterovMomentumOptimizer(mat_t step = 1e-2);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief 配置模型函数，返回注册的优化器Optimizer
			@param loss_ 损失函数
			@param output_ 输出函数
			@param activation_ 激活函数
			*/
			Optimizer* minimize(mat_t momentum = 0.9f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			mat_t momentum;
		};
		/**
		@brief AdagradOptimizer网络的函数配置
		注册模型的损失函数loss
		提供优化方法AdagradGradientDescent
		alpha = alpha + df(a, x)^2
		a = a - step/sqrt(alpha + epsilon)*df(a, x)
		*/
		class LZHAPI AdagradOptimizer :public Optimizer
		{
		public:
			/**
			@brief 优化器构造函数
			@param step 学习率
			@param epsilon 偏置(避免分母=0)
			*/
			explicit AdagradOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief 配置模型函数，返回注册的优化器Optimizer
			@param loss_ 损失函数
			@param output_ 输出函数
			@param activation_ 激活函数
			*/
			Optimizer* minimize(mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> alpha;
			mat_t epsilon;
		};
		/**
		@brief RMSPropOptimizer网络的函数配置
		注册模型的损失函数loss
		提供优化方法RMSPropGradientDescent
		alpha = beta*alpha + (1 - beta)*df(a, x)^2
		a = a - step/sqrt(alpha + epsilon)*df(a, x)
		*/
		class LZHAPI RMSPropOptimizer :public Optimizer
		{
		public:
			/**
			@brief 优化器构造函数
			@param step 学习率
			@param decay 自适应参数
			@param epsilon 偏置(避免分母=0)
			*/
			explicit RMSPropOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief 配置模型函数，返回注册的优化器Optimizer
			@param loss_ 损失函数
			@param output_ 输出函数
			@param activation_ 激活函数
			*/
			Optimizer* minimize(mat_t decay = 0.9f, mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> alpha;
			mat_t decay;
			mat_t epsilon;
		};
		/**
		@brief AdamOptimizer网络的函数配置
		注册模型的损失函数loss
		提供优化方法AdamGradientDescent
		ma = beta1*ma + (1 - beta1)*df(a, x)
		alpha = beta2*alpha + (1 - beta2)*df(a, x)^2
		a = a - step/sqrt(alpha + epsilon)*ma
		*/
		class LZHAPI AdamOptimizer :public Optimizer
		{
		public:
			/**
			@brief 优化器构造函数
			@param step 学习率
			@param beta1 动量系数
			@param beta2 自适应系数
			@param epsilon 偏置(避免分母=0)
			*/
			explicit AdamOptimizer(Val step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief 配置模型函数，返回注册的优化器Optimizer
			@param loss_ 损失函数
			@param output_ 输出函数
			@param activation_ 激活函数
			*/
			Optimizer* minimize(mat_t beta1 = 0.9f, mat_t beta2 = 0.999f, mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			STLApi<vector<Mat>> alpha;
			mat_t epsilon;
			mat_t beta1;
			mat_t beta2;
		};
		/**
		@brief NesterovAdamOptimizer网络的函数配置
		注册模型的损失函数loss
		提供优化方法NesterovAdamGradientDescent
		ma = beta1*ma + (1 - beta1)*df(a - step/sqrt(alpha + epsilon)*ma, x)
		alpha = beta2*alpha + (1 - beta2)*df(a - step/sqrt(alpha + epsilon)*ma, x)^2
		a = a - step/sqrt(alpha + epsilon)*ma
		*/
		class LZHAPI NesterovAdamOptimizer :public Optimizer
		{
		public:
			/**
			@brief 优化器构造函数
			@param step 学习率
			@param beta1 动量系数
			@param beta2 自适应系数
			@param epsilon 偏置(避免分母=0)
			*/
			explicit NesterovAdamOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief 配置模型函数，返回注册的优化器Optimizer
			@param loss_ 损失函数
			@param output_ 输出函数
			@param activation_ 激活函数
			*/
			Optimizer* minimize(mat_t beta1 = 0.9f, mat_t beta2 = 0.999f, mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			STLApi<vector<Mat>> alpha;
			mat_t epsilon;
			mat_t beta1;
			mat_t beta2;
		};
		typedef Optimizer* OptimizerP;

	}
}

#endif // !__NET_CORE_H__
