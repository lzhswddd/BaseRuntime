#pragma once
#ifndef __BASECAMERA_H__
#define __BASECAMERA_H__

#include "../core/base.h"
#include <string>
#include <map>

#include "CameraSupport.h"

namespace lzh {
	class LZHAPI FunctionHandler
	{
	public:
		lzh::EventHandler<lzh::Image> Grab;
		lzh::EventHandler<void> GrabStart;
		lzh::EventHandler<void> GrabEnd;
		lzh::EventHandler<std::string> Error;
	};
	enum class CameraIO
	{
		Input = 0,
		Output = 1
	};
	enum class TriggerMode
	{
		InValid = -1,
		AcquisitionActive,  //!< Selects the acquisition active trigger for configuration - Applies to: CameraLink and GigE
		AcquisitionEnd,  //!< Selects the acquisition end trigger for configuration - Applies to: CameraLink and GigE
		AcquisitionStart,  //!< The Acquisition Start trigger can be configured - Applies to: CamEmu, CameraLink and GigE
		ExposureActive,  //!< The Exposure Active trigger can be configured - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB and boost CoaXPress
		ExposureEnd,  //!< The Exposure End trigger can be configured - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB and boost CoaXPress
		ExposureStart,  //!< The Exposure Start trigger can be configured - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB and boost CoaXPress
		FrameActive,  //!< The Frame Active trigger can be configured - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB and boost CoaXPress
		FrameBurstActive,  //!< The Frame Burst Active trigger can be configured - Applies to: ace 2 GigE, ace 2 USB and boost CoaXPress
		FrameBurstEnd,  //!< The Frame Burst End trigger can be configured - Applies to: ace 2 GigE, ace 2 USB and boost CoaXPress
		FrameBurstStart,  //!< The Frame Burst Start trigger can be configured - Applies to: ace 2 GigE, ace 2 USB, ace USB and boost CoaXPress
		FrameEnd,  //!< The Frame End trigger can be configured - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB and boost CoaXPress
		FrameStart,  //!< The Frame Start trigger can be configured - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB, boost CoaXPress, dart BCON, dart USB and pulse USB
		LineStart  //!< The Line Start trigger can be configured - Applies to: CameraLink and GigE
	};
	enum class ActivationMode
	{
		InValid = -1,
		AnyEdge,  //!< The selected trigger is activated by the falling or rising edge of the source signal - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB and boost CoaXPress
		FallingEdge,  //!< The selected trigger is activated by the falling edge of the source signal - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB, boost CoaXPress, dart BCON and dart USB
		LevelHigh,  //!< The selected trigger is activated when the source signal is high - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB and boost CoaXPress
		LevelLow,  //!< The selected trigger is activated when the source signal is low - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB and boost CoaXPress
		RisingEdge  //!< The selected trigger is activated by the rising edge of the source signal - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB, boost CoaXPress, dart BCON, dart USB and pulse USB
	};
	enum class BalanceRatioMode
	{
		InValid = -1,
		BalanceWhiteAuto_Continuous,  //!< White balance is adjusted continuously while images are being acquired - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB, boost CoaXPress, dart BCON, dart USB and pulse USB
		BalanceWhiteAuto_Off,  //!< The Balance White Auto auto function is disabled - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB, boost CoaXPress, dart BCON, dart USB and pulse USB
		BalanceWhiteAuto_Once  //!< White balance is adjusted automatically to reach the specified target value - Applies to: CameraLink, GigE, ace 2 GigE, ace 2 USB, ace USB, boost CoaXPress, dart BCON, dart USB and pulse USB
	};
	class LZHAPI Camera
	{
	public:
		using keyType = typename std::string;
		using iterator = typename std::map<keyType, lzh::Element>::iterator;
		using const_iterator = typename std::map<keyType, lzh::Element>::const_iterator;
		Camera(CameraIO io = CameraIO::Input);
		virtual ~Camera();

		virtual bool Open() = 0;
		virtual bool Close() = 0;
		virtual void OneShot() = 0;
		virtual void ContinueShot() = 0;
		virtual void Stop() = 0;

		virtual bool IsOpen()const = 0;
		virtual bool IsGrab()const = 0;

		virtual int Width()const = 0;
		virtual int Height()const = 0;

		virtual bool SetExposure(double value);
		virtual double GetExposure();

		virtual bool SetGain(double value);
		virtual double GetGain();

		virtual bool SetROI(lzh::Rect rect);
		virtual int GetROI();

		virtual bool SetReverseX(bool state);
		virtual bool GetReverseX();
		virtual bool SetReverseY(bool state);
		virtual bool GetReverseY();

		virtual bool SetBalanceRatio(BalanceRatioMode mode);
		virtual BalanceRatioMode GetBalanceRatio();

		virtual bool SetTriggerMode(TriggerMode mode);
		virtual TriggerMode GetTriggerMode();
		virtual bool SetTriggerActivation(ActivationMode mode);
		virtual ActivationMode GetTriggerActivation();

		virtual bool TrySetParameter(const keyType& key, const lzh::Element& elem) = 0;
		virtual void SetParameter(const keyType& key, const lzh::Element& elem);
		virtual void SetParameterBefore(const keyType& key, const lzh::Element& elem);
		virtual void SetParameterAfter(const keyType& key, const lzh::Element& elem);
		virtual bool TryGetParameter(const keyType& key, lzh::Element& elem)const;
		virtual const lzh::Element& GetParameter(const keyType& key)const;
		virtual lzh::Image Read();
		virtual void Write(const lzh::Image& image);

		bool isInput()const noexcept;
		bool isOutput()const noexcept;

		FunctionHandler funcHandler;
	protected:
		virtual void Initialization();
		bool TrySetParameter(const keyType& key, const lzh::Element& elem, const lzh::StringList& params, int* index = nullptr);
		void SetParameterUnCheck(const keyType& key, const lzh::Element& elem);

		bool is_open = false;
		CameraIO io = CameraIO::Input;
		lzh::Element& at(const keyType& key);
		lzh::Element& operator [](const keyType& key);
		lzh::Pointer<std::map<keyType, lzh::Element>> parameters;

	private:
		Camera(const Camera&) {}
		Camera& operator = (const Camera&) { return *this; }
	};
}

#endif // !__BASECAMERA_H__