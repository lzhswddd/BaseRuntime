#define LZH_EXPORTS
#include "DllAdmin.h"
#include "../json/json.h"
#include <fstream>

using namespace std;
using namespace lzh;

#define APPEND_PTR(dir, ptr, function_name, dll, port) if((dir).find(function_name) == (dir).end())(dir)[function_name] = std::map<std::string, DllAdmin::Elem>();(dir)[function_name][dll]=ptr;
#define APPEND_FUNC(fun, dll) append(fun, dll, fun)

DllAdmin::DllAdmin() : function(), islock(false)
{
	FunctionApi *api = new FunctionApi();
	function.d_ptr()->operator[](defaultName) = api;
}

DllAdmin::~DllAdmin()
{
	clear();
}

bool lzh::DllAdmin::checkImport(const std::string & dll, const std::string & port) const
{
	DynamicLibrary api(dll);
	if (api.empty())return false;
	if (!port.empty()) {
		return api.tryPause(port);
	}
	return true;
}

bool DllAdmin::tryImport(const std::string & dll, const std::string & port) {
	Container *funMap = function.d_ptr();
	Container::iterator iter = funMap->find(dll);
	if (iter == funMap->end()) {
		DynamicLibrary* api = new DynamicLibrary(dll);
		if (api->empty()) { delete api; return false; }
		if (!api->tryPause(port)) api->append(port);
		else return false;
		funMap->operator[](dll) = api;
	}
	else {
		if (!iter->second->base()) {
			if (!static_cast<DynamicLibrary*>(iter->second)->tryPause(port))
				iter->second->append(port);
			else return false;
		}
		else return false;
	}
	return true;
}
const FunctionApi* DllAdmin::findDll(const std::string & dll)const {
	Container::const_iterator iter = function.d_ptr()->find(dll);
	return (iter == function.d_ptr()->end()) ? nullptr : iter->second;
}
std::intptr_t DllAdmin::findPort(const std::string & port)const {
	for (Container::const_iterator iter = function.d_ptr()->begin(); iter != function.d_ptr()->end(); iter++) {
		const void * item = iter->second->f(port);
		if (item != nullptr)return std::intptr_t(item);
	}
	return 0;
}
std::intptr_t DllAdmin::find(const std::string & dll, const std::string & port)const {
	Container::const_iterator iter = function.d_ptr()->find(dll);
	if (iter == function.d_ptr()->end())return 0;
	const void * item = iter->second->f(dll);
	return std::intptr_t(item);
}

bool DllAdmin::appendfun(std::string port, const void * fun)
{
	if (port.empty())return false;
	function.d_ptr()->at(defaultName)->set(port, fun);
	return true;
}

bool DllAdmin::append(std::string dll, std::string port)
{
	if (dll.empty() || port.empty())return false;
	Container *funMap = function.d_ptr();
	Container::iterator iter = funMap->find(dll);
	if (iter == funMap->end()) { 
		DynamicLibrary* api = new DynamicLibrary(dll);
		if (api->empty())throw std::exception("Î´ÄÜÕÒµ½¶¯Ì¬¿â");
		api->append(port);
		funMap->operator[](dll) = api;
	}
	else {
		iter->second->append(port);
	}
	return true;
}

void lzh::DllAdmin::erase(std::string dll, std::string port)
{
	if(islock)throw std::exception("Ëø¶¨×´Ì¬ÏÂ, ½ûÖ¹ÒÆ³ýÄÚÈÝ!");
	Container::iterator iter = function.d_ptr()->find(dll);
	if (iter != function.d_ptr()->end()) {
		if (port.empty()) {
			delete iter->second;
			function.d_ptr()->erase(iter);
		}
		else
			iter->second->erase(port);
	}
}

DllAdmin::Elem DllAdmin::fun(const char* dll, const char* name)
{
	Container::iterator iter = function.d_ptr()->find(dll);
	if (!islock) {
		if (iter == function.d_ptr()->end()) {
			DynamicLibrary* api = new DynamicLibrary(dll);
			if (api->empty())throw std::exception("Î´ÄÜÕÒµ½¶¯Ì¬¿â");
			api->append(name);
			function.d_ptr()->operator[](dll) = api;
			return api;
		}
		else {
			iter->second->append(name);
			return iter->second;
		}
	}
	else {
		if (iter == function.d_ptr()->end())return nullptr;
		else return iter->second;
	}
}

void lzh::DllAdmin::lock()
{
	islock = true;
}

void lzh::DllAdmin::unlock()
{
	islock = false;
}

void DllAdmin::clear()
{
	for (pair<const string, Elem>& iter : (*function)) {
		FREE_PTR(iter.second);
	}
	(*function).clear();
}
