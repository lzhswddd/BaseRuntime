#pragma once

#include "core/base.h"

#define CHECK_PTR_TO_OBJ(objtype, objlist, ptr, res) if ((ptr) == nullptr)return res; objtype& objlist = *(objtype*)(ptr)

#define CATCH catch (...) {return False;}return True

#define USE_JSON
#define USE_NN

#ifdef USE_JSON
#include "json/json_support.h"
#endif
#ifdef USE_NN
#include "net/nn.h"
#endif