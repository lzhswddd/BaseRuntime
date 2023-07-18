#pragma once
#include "BaseCamera.h"

#ifdef LOCAL

namespace lzh {
	class LZHAPI LocalCamera : public Camera
	{
	public:
		LocalCamera();
		~LocalCamera();

		enum Param
		{
			Path = 0,
			TriggerMode = 1,
			ReceiveAsynchronous = 2,
			Mode = 3
		};

		bool Open();
		bool Close();
		void OneShot();
		void ContinueShot();
		void Stop();

		int Width()const;
		int Height()const;

		bool IsOpen()const;
		bool IsGrab()const;

		lzh::Image Read();
		void operator >> (lzh::Image& image);

		bool TrySetParameter(const keyType & key, const lzh::Element & elem);
		void SetParameterAfter(const keyType & key, const lzh::Element & elem) override;

		static const lzh::STLApi<lzh::StringList> params;
	protected:
		void Initialization() override;

		bool currentOnEnd();

		void OpenDir();
		void OpenFile();

		void Clear();
		void ClearPath();
	private:
		lzh::Pointer<lzh::ElemList::const_iterator> currentHandle;
		bool asynchronous = false;
		lzh::int32 GrabNum = 0;
		lzh::MultThread<lzh::int32> process;

		lzh::Size imageSize;
	};
	extern LZHAPI std::string GetLocalEnumStr(LocalCamera::Param type);
}
#endif //LOCAL