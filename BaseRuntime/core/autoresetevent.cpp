#define LZH_EXPORTS
#include "autoresetevent.h"

#include <windows.h>

void lzh::AutoResetEventHandle::create(bool bManualReset)
{
    ptr = CreateEvent(NULL, bManualReset, false, NULL);
}

void lzh::AutoResetEventHandle::release()
{
    if (ptr)
    {
        CloseHandle(ptr);
        ptr = NULL;
    }
}

lzh::AutoResetEvent::AutoResetEvent(bool bManualReset)
{
    m_waitEvent = AllocArgv<AutoResetEventHandle>(bManualReset);
}

lzh::AutoResetEvent::~AutoResetEvent()
{

}

bool lzh::AutoResetEvent::WaitOne(uint32 dwWaitTimeMillSeconds)
{
    return WaitForSingleObject(*m_waitEvent, dwWaitTimeMillSeconds) == WAIT_OBJECT_0;
}

bool lzh::AutoResetEvent::Set()
{
    return SetEvent(*m_waitEvent);
}

bool lzh::AutoResetEvent::Reset()
{
    return ResetEvent(*m_waitEvent);
}

