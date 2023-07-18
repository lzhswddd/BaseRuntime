#define LZH_EXPORTS
#include "runnable.h"

class DefaultRunnable : public lzh::Runnable
{
	std::function<void()> functionToRun;
public:
	explicit DefaultRunnable(std::function<void()> functionToRun) 
		: lzh::Runnable(), functionToRun(functionToRun){}
	~DefaultRunnable() { functionToRun = nullptr; }
	void run() {
		if (functionToRun)
			functionToRun();
	}
};

lzh::Runnable::~Runnable() {}

lzh::Runnable* lzh::Runnable::create(std::function<void()> functionToRun)
{
	return new DefaultRunnable(functionToRun);
}
