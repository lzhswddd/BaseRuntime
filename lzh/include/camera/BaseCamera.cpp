#define LZH_EXPORTS
#include "BaseCamera.h"

namespace lzh {
	Camera::Camera(CameraIO io)
		:io(io)
	{
		Initialization();
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialization()
	{
	}

	bool Camera::TrySetParameter(const keyType & key, const lzh::Element & elem, const lzh::StringList & params, int* index)
	{
		lzh::StringList::const_iterator iter = std::find(params.begin(), params.end(), key);
		bool accept = iter != params.end();
		if (accept) { SetParameter(key, elem); if (index != nullptr)*index = (int)(iter - params.begin()); }
		else { if (index != nullptr)*index = -1; }
		return accept;
	}

	void Camera::SetParameterUnCheck(const keyType & key, const lzh::Element & elem)
	{
		(*parameters)[key] = elem;
	}

	void Camera::SetParameter(const keyType & key, const lzh::Element & elem)
	{
		SetParameterBefore(key, elem);
		(*parameters)[key] = elem;
		SetParameterAfter(key, elem);
	}

    void Camera::SetParameterBefore(const keyType & , const lzh::Element & )
	{
	}

    void Camera::SetParameterAfter(const keyType & , const lzh::Element & )
	{
	}

	bool Camera::TryGetParameter(const keyType & key, lzh::Element & elem)const
	{
		const_iterator iter = parameters->find(key);
		if (iter == parameters->end()) {
			elem = lzh::noElem();
			return false;
		}
		else {
			elem = iter->second;
			return true;
		}
	}

	const lzh::Element & Camera::GetParameter(const keyType & key)const
	{
		const_iterator iter = parameters->find(key);
		if (iter == parameters->end())throw std::exception("参数库类未能找到此变量");
		return iter->second;
	}

	lzh::Image Camera::Read()
	{
		return lzh::Image();
	}

    void Camera::Write(const lzh::Image& )
	{
	}

	bool Camera::isInput() const noexcept
	{
		return io == CameraIO::Input;
	}

	bool Camera::isOutput() const noexcept
	{
		return io == CameraIO::Output;
	}

	lzh::Element & Camera::at(const keyType & key)
	{
		return (*parameters)[key];
	}

	lzh::Element & Camera::operator[](const keyType & key)
	{
		return (*parameters)[key];
	}

    bool Camera::SetExposure(double )
	{
		return false;
	}
	double Camera::GetExposure()
	{
		return 0.0;
	}
    bool Camera::SetGain(double )
	{
		return false;
	}
	double Camera::GetGain()
	{
		return 0.0;
	}
    bool Camera::SetROI(lzh::Rect )
	{
		return false;
	}
	int Camera::GetROI()
	{
		return 0;
	}
    bool Camera::SetReverseX(bool )
	{
		return false;
	}
	bool Camera::GetReverseX()
	{
		return false;
	}
    bool Camera::SetReverseY(bool )
	{
		return false;
	}
	bool Camera::GetReverseY()
	{
		return false;
	}
    bool Camera::SetBalanceRatio(BalanceRatioMode )
	{
		return false;
	}
	BalanceRatioMode Camera::GetBalanceRatio()
	{
		return BalanceRatioMode();
	}
    bool Camera::SetTriggerMode(TriggerMode )
	{
		return false;
	}
	TriggerMode Camera::GetTriggerMode()
	{
		return TriggerMode();
	}
    bool Camera::SetTriggerActivation(ActivationMode )
	{
		return false;
	}
	ActivationMode Camera::GetTriggerActivation()
	{
		return ActivationMode();
	}
}
