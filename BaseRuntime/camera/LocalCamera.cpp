#define LZH_EXPORTS
#include "LocalCamera.h"

#ifdef LOCAL

namespace lzh {
	const STLApi<StringList> LocalCamera::params = { "Path", "TriggerMode", "ReceiveAsynchronous", "Mode" };
	LocalCamera::LocalCamera()
		: Camera(CameraIO::Input), process(1)
	{
		SetParameter(params[Mode], "Once");
		process.Process += [=](int32 idx) {
			if (idx < GrabNum || GrabNum < 0) {
				funcHandler.Grab(Read());
				if (GetParameter(params[Mode]).toString() == "Once") {
					if (idx == GrabNum - 1) {
						process.Stop();
					}
				}
			}
			else {
				process.Stop();
			}
		};
	}
	LocalCamera::~LocalCamera()
	{
		Close();
	}
	bool LocalCamera::Open()
	{
		if (GetParameter(params[Path]).empty())return false;
		const char* str = GetParameter(params[TriggerMode]);
		if (strcmp(str, "FileMode") == 0) OpenFile();
		else if (strcmp(str, "FolderMode") == 0) OpenDir();
		else throw std::exception("不支持的模式");
		is_open = !GetParameter(params[Path]).empty();
		asynchronous = GetParameter(params[ReceiveAsynchronous]);
		GrabNum = GetParameter(params[Path]).len();
		return is_open;
	}
	bool LocalCamera::Close()
	{
		if (asynchronous) {
			Stop();
			process.Join();
		}
		Clear(); is_open = false; return true;
	}
	void LocalCamera::OneShot()
	{
		static int count = 0;
		if (IsOpen()) {
			if (!IsGrab()) {
				process.Append(count);
				if (!process.IsRunning()) {
					process.WaitExit();
					process.Start();
				}
			}
			else {
				process.Append(count);
			}
			count = (count + 1) % GrabNum;
		}
	}
	void LocalCamera::ContinueShot()
	{
		if (IsOpen() && !IsGrab()) {
			auto arr = range(0, GrabNum);
			for (auto i : arr)process.Append(i);
			process.Start();
		}
	}
	void LocalCamera::Stop()
	{
		if (IsGrab())process.Terminate();
	}
	int LocalCamera::Width()const
	{
		return imageSize.w;
	}
	int LocalCamera::Height()const
	{
		return imageSize.h;
	}
	Image LocalCamera::Read()
	{
		if (IsOpen()) {
			if (*currentHandle == GetParameter(params[Path]).end().as<ElemList::iterator>())
				*currentHandle = GetParameter(params[Path]).begin().as<ElemList::iterator>();
			Image img = imread((*currentHandle)->toString());
			imageSize = img.size();
			++(*currentHandle);
			return img;
		}
		else return Image();
	}
	void LocalCamera::operator>>(Image& image)
	{
		image = Read();
	}
	bool LocalCamera::TrySetParameter(const keyType& key, const Element& elem)
	{
		return Camera::TrySetParameter(key, elem, params);
	}
	void LocalCamera::SetParameterAfter(const keyType& key, const Element& elem)
	{

	}
	bool LocalCamera::IsOpen() const
	{
		return is_open;
	}
	bool LocalCamera::IsGrab() const
	{
		if (!is_open)return false;
		if (asynchronous)return process.IsRunning();
		if (GetParameter(params[Mode]).toString() == "Once")return *currentHandle != GetParameter(params[Path]).end().as<ElemList::iterator>();
		else return true;
	}
	void LocalCamera::Initialization()
	{
		(*parameters)[params[Path]] = ElemList();
		(*parameters)[params[TriggerMode]] = "FileMode";
	}
	bool LocalCamera::currentOnEnd()
	{
		return *currentHandle == GetParameter(params[Path]).end().as<ElemList::iterator>();
	}
	void LocalCamera::OpenDir()
	{
		StringList list;
		for (const std::string& path : GetParameter(params[Path])) {
			StringList files = getImageFiles(path);
			if (files.empty())continue;
			list.insert(list.end(), files.begin(), files.end());
		}
		if (!list.empty()) {
			SetParameterUnCheck(params[Path], list);
			currentHandle = GetParameter(params[Path]).begin().as<ElemList::iterator>();
		}
	}
	void LocalCamera::OpenFile()
	{
		StringList list;
		for (const std::string& file : GetParameter(params[Path])) {
			if (isImageFile(file))
				list.push_back(file);
		}
		SetParameterUnCheck(params[Path], list);
		currentHandle = GetParameter(params[Path]).begin().as<ElemList::iterator>();
	}
	void LocalCamera::Clear()
	{
		at(params[Path]) = noElem();
	}
	void LocalCamera::ClearPath()
	{
		at(params[Path]) = noElem();

	}

	std::string GetLocalEnumStr(LocalCamera::Param type)
	{
		return LocalCamera::params[type];
	}
}

#endif //LOCAL