#define LZH_EXPORTS
#include "dllapi.h"
#include <Windows.h>

#ifdef UNICODE
#ifdef _WIN64
typedef LPCWSTR LPCSTR__;
#else
typedef LPCSTR LPCSTR__;
#endif
#else
typedef LPCSTR  LPCSTR__;
#endif // !UNICODE

using namespace std;
using namespace lzh;

FunctionApi::FunctionApi()
{
}

FunctionApi::FunctionApi(const void *fun, const std::string & port)
{
	func.d_ptr()->operator[](port) = fun;
}

FunctionApi::FunctionApi(const std::vector<const void *> & fun, const std::vector<std::string> & port)
{
	if (fun.empty() || port.empty())return;
	if (fun.size() != port.size())return;
	for (size_t i = 0; i < fun.size(); i++) {
		set(port[i], fun[i]);
    }
}

FunctionApi::~FunctionApi()
{
    func.clear();
}

const void * FunctionApi::f(keyType key) const
{
	if (key.empty())return nullptr;
	std::map<keyType, const void *>::const_iterator iter = func.d_ptr()->find(key);
	return (iter == func.d_ptr()->end()) ? nullptr : iter->second;
}

void FunctionApi::set(keyType key, const void * fun)
{
	if (key.empty() || fun == nullptr)return;
	func.d_ptr()->operator[](key) = fun;
}

void lzh::FunctionApi::erase(keyType key)
{
	if (key.empty())return;
	func.d_ptr()->erase(key);
}

void FunctionApi::clear()
{
	func.d_ptr()->clear();
}
bool FunctionApi::enable() const
{
	return !func.d_ptr()->empty();
}

bool FunctionApi::base() const
{
	return true;
}

void lzh::FunctionApi::append(keyType port)
{
}

DynamicLibrary::DynamicLibrary(std::string dll_name)
	: hDll(NULL), FunctionApi(), name(dll_name)
{
	if (name.empty())return;
	hDll = LoadLibrary(LPCSTR__(name.data()));
}

DynamicLibrary::DynamicLibrary(std::string dll_name, const std::string & dll_port)
	: hDll(NULL), FunctionApi(), name(dll_name) 
{
	if (name.empty() || dll_port.empty())return;
	HMODULE HDll = LoadLibrary(LPCSTR__(name.data()));
	if (!(HDll))return;
	hDll = HDll;
	set(dll_port, GetProcAddress((HMODULE)hDll, dll_port.data()));
}

DynamicLibrary::DynamicLibrary(std::string dll_name, const std::vector<std::string> & dll_port)
	: hDll(NULL), FunctionApi(), name(dll_name)
{
	if (name.empty())return;
	HMODULE HDll = LoadLibrary(LPCSTR__(name.data()));
	if (!(HDll))return;
	hDll = HDll;
	for (const std::string& port : dll_port) {
		set(port, GetProcAddress((HMODULE)hDll, port.data()));
	}
}


DynamicLibrary::~DynamicLibrary()
{
	clear();
}

void DynamicLibrary::clear()
{
	if (!empty())FreeLibrary((HMODULE)hDll);
	hDll = NULL;
	FunctionApi::clear();
}

bool lzh::DynamicLibrary::empty() const noexcept
{
	return hDll == NULL;
}

void DynamicLibrary::setDllName(std::string dll_name) {
	if (dll_name.empty())return;
	name = dll_name;
	if (!empty())FreeLibrary((HMODULE)hDll);
	hDll = NULL;
	*(HMODULE*)hDll = LoadLibrary((LPCSTR__)name.data());
}

std::string DynamicLibrary::getDllName() const {
	return name;
}

bool lzh::DynamicLibrary::tryPause(keyType port) const
{
	if (empty())return false;
	return GetProcAddress((HMODULE)hDll, port.data()) != nullptr;
}

void DynamicLibrary::append(keyType port)
{
	if (empty() || port.empty())return;
	set(port, GetProcAddress((HMODULE)hDll, port.data()));
}
