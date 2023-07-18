#pragma once
#ifndef __AUTORESETEVENT_H__
#define __AUTORESETEVENT_H__

#include "core.h"
#include "pointer.h"
#include "threadlock.h"

namespace lzh
{
	class LZHAPI AutoResetEventHandle 
		: public Handle 
	{
	public:
		void create(bool bManualReset = false);
		void release();
	};
	template<> struct TEMPLATE_API PointerInterFace<AutoResetEventHandle>
	{
        using value_type = AutoResetEventHandle;
        static bool Custom() { return true; }
		static AutoResetEventHandle* Create(bool bManualReset) { AutoResetEventHandle* handle = new AutoResetEventHandle(); handle->create(bManualReset); return handle; }
		static void Release(AutoResetEventHandle* ptr) { ptr->release(); }
	};
	class LZHAPI AutoResetEvent
	{
	public:
		AutoResetEvent(bool bManualReset = false);
		~AutoResetEvent();
        bool WaitOne(uint32 dwWaitTimeMillSeconds = -1);
        bool Wait(std::mutex * mutex, uint32 dwWaitTimeMillSeconds = -1);
		bool Set();
        bool Reset();

    private:
		Pointer<AutoResetEventHandle> m_waitEvent;
	};
}
#endif //__AUTORESETEVENT_H__
