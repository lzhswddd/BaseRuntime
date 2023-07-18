/*base_64.hÎÄ¼ş*/
#ifndef BASE_64_H
#define BASE_64_H
/**
 * Base64 ±àÂë/½âÂë
 */
#include "core.h"
#include <string>
namespace lzh {
    class LZHAPI Base64 {
    public:
        static std::string Encode(const char* str, int bytes);
        static std::string Decode(const char* str, int bytes);
    private:
        static const char _base64_table[66];
        static const char base64_pad = '=';
    };
}
#endif
