#ifndef __JSON_H__
#define __JSON_H__
#include "fifo_map.hpp"
#include "json_inl.hpp"

namespace lzh {
	template<class K, class V, class dummy_compare, class A>
	using json_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
	using Json = TEMPLATE_API nlohmann::basic_json<json_workaround_fifo_map>;
}
#endif // !__JSON_H__
