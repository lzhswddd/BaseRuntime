#pragma once

#include "core/type.h"
#include "core/core.h"

#include "core/pointer.h"
#include "core/point.h"
#include "core/vector.h"
#include "core/color.h"
#include "core/mat.h"
#include "core/array.h"
#include "core/interface.h"
#include "core/element.h"
#include "core/elemmatrix.h"
#include "core/dictionary.h"
#include "core/convert.h"

#include "core/image.h"
#include "core/operator.h"

#include "core/algorithm.h"
#include "core/image_process.h"
#include "core/geometry.h"

#include "core/tools.h"
#include "core/image.h"
#include "core/data_stream.h"

#include "core/dllapi.h"
#include "core/dlladmin.h"

#include "core/csv.h"
#include "core/mult_thread.h"
#include "core/action.h"
#include "core/signal.h"
#include "core/autoresetevent.h"
#include "core/delegate.h"

#define CHECK_PTR_TO_OBJ(objtype, objlist, ptr, res) if ((ptr) == nullptr)return res; objtype& objlist = *(objtype*)(ptr)

#define CATCH catch (...) {return False;}return True

#define USE_JSON
#define USE_NN

#ifdef USE_JSON
#include "json/json.h"
#endif
#ifdef USE_NN
#include "net/nn.h"
#endif