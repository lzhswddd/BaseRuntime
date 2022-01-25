#ifndef JSON_SUPPORT
#define JSON_SUPPORT

#include "json.h"
#include "../core/base.h"

#define CHECK_JSON(json)
#define CHECK_JSON_TYPE CHECK_JSON(json)
#define JSON_TYPE "Json::Type"
#define REGISTER_JSON_TYPE  json[JSON_TYPE] = typeid(value_type).name()


#define DESERIALIZE       static value_type deserialize(const lzh::Json& json)
#define SERIALIZE         static lzh::Json serialize(const value_type& v)
#define GEN_VALUE         value_type v;CHECK_JSON_TYPE
#define GEN_JSON          lzh::Json json
#define RET_VALUE         return v
#define RET_JSON          REGISTER_JSON_TYPE;return json
#define SET_JSON_PTR(str)   do{json[#str] = ToJson(v->str);}while(0)
#define GET_JSON_PTR(str)   do{if(json.contains(#str))ToValue(json[#str], v->str);}while(0)
#define GET_JSON_VAL(str)   do{if(json.contains(#str))ToValue(json[#str], v.str);}while(0)
#define SET_JSON_VAL(str)   do{json[#str] = ToJson(v.str);}while(0)

namespace lzh {
    template<typename _Tp> bool TryGetValue(const lzh::Json& json, std::string key, _Tp& value) {
        lzh::Json::const_iterator iter = json.find(key);
        if (iter != json.end()) {
            value = iter.value();
            return true;
        }
        else {
            return false;
        }
    }
    template<typename _Tp> struct JsonInterFace
    {
        using value_type = _Tp;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) { return json; }
        static lzh::Json serialize(const value_type& value) { return value; }

    };
    template<typename _Tp> lzh::Json ToJson(const _Tp& val) {
        return JsonInterFace<_Tp>::serialize(val);
    }
    template<typename _Tp> void ToValue(const lzh::Json& json, _Tp& val) {
        LZH_ACCESS(if (JsonInterFace<_Tp>::Custom()), THROW_INFO(lzh::ERR_INFO_SUPPORT));
        val = JsonInterFace<_Tp>::deserialize(json);
    }

    template<typename _Tp> _Tp ReadJson(std::string path) {
        std::string filePath = path;
        LZH_ACCESS(if (lzh::isExists(filePath)), THROW_INFO(lzh::ERR_INFO_FILE));
        LZH_ACCESS(if (JsonInterFace<_Tp>::Custom()), THROW_INFO(lzh::ERR_INFO_SUPPORT));
        lzh::Json json;
        std::ifstream in(filePath);
        if (in.is_open()) {
            in >> json;
            in.close();
        }
        return JsonInterFace<_Tp>::deserialize(json);
    }

    template<typename _Tp> void WriteJson(std::string path, const _Tp& value, int indent = 4) {
        std::string filePath = path;
        LZH_ACCESS(if (JsonInterFace<_Tp>::Custom()), THROW_INFO(lzh::ERR_INFO_SUPPORT));
        lzh::Json json = JsonInterFace<_Tp>::serialize(value);
        std::ofstream out(filePath);
        if (out.is_open()) {
            if (indent > 0)out << json.dump(indent);
            else out << json;
            out.close();
        }
    }

    template<typename _Tp> struct lzh::JsonInterFace<Point2_<_Tp>>
    {
        using value_type = Point2_<_Tp>;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(x);
            GET_JSON_VAL(y);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(x);
            SET_JSON_VAL(y);
            RET_JSON;
        }
    };
    template<typename _Tp> struct lzh::JsonInterFace<Point3_<_Tp>>
    {
        using value_type = Point3_<_Tp>;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(x);
            GET_JSON_VAL(y);
            GET_JSON_VAL(z);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(x);
            SET_JSON_VAL(y);
            SET_JSON_VAL(z);
            RET_JSON;
        }
    };
    template<> struct lzh::JsonInterFace<Val>
    {
        using value_type = Val;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(v);
            GET_JSON_VAL(type);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(v);
            SET_JSON_VAL(type);
            RET_JSON;
        }
    };
    template<typename _Tp> struct lzh::JsonInterFace<Range_<_Tp>>
    {
        using value_type = Range_<_Tp>;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(v1);
            GET_JSON_VAL(v2);
            GET_JSON_VAL(gap_);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(v1);
            SET_JSON_VAL(v2);
            SET_JSON_VAL(gap_);
            RET_JSON;
        }
    };
    template<typename _Tp> struct lzh::JsonInterFace<Size_<_Tp>>
    {
        using value_type = Size_<_Tp>;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(w);
            GET_JSON_VAL(h);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(w);
            SET_JSON_VAL(h);
            RET_JSON;
        }
    };
    template<typename _Tp> struct lzh::JsonInterFace<Size3_<_Tp>>
    {
        using value_type = Size3_<_Tp>;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(w);
            GET_JSON_VAL(h);
            GET_JSON_VAL(c);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(w);
            SET_JSON_VAL(h);
            SET_JSON_VAL(c);
            RET_JSON;
        }
    };
    template<typename _Tp> struct lzh::JsonInterFace<Rect_<_Tp>>
    {
        using value_type = Rect_<_Tp>;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(x);
            GET_JSON_VAL(y);
            GET_JSON_VAL(width);
            GET_JSON_VAL(height);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(x);
            SET_JSON_VAL(y);
            SET_JSON_VAL(width);
            SET_JSON_VAL(height);
            RET_JSON;
        }
    };
    template<> struct lzh::JsonInterFace<MatStep>
    {
        using value_type = MatStep;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(stride[0]);
            GET_JSON_VAL(stride[1]);
            GET_JSON_VAL(stride[2]);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(stride[0]);
            SET_JSON_VAL(stride[1]);
            SET_JSON_VAL(stride[2]);
            RET_JSON;
        }
    };
    template<> struct lzh::JsonInterFace<Complex>
    {
        using value_type = Complex;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(r);
            GET_JSON_VAL(i);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(r);
            SET_JSON_VAL(i);
            RET_JSON;
        }
    };
    template<class _Tp, int32 N> struct lzh::JsonInterFace<Vec<_Tp, N>>
    {
        using value_type = Vec<_Tp, N>;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            GEN_VALUE;
            GET_JSON_VAL(N);
            for (int32 i : range(N))
                ToValue(json["data"][i], v[i]);
            RET_VALUE;
        }
        static lzh::Json serialize(const value_type& v) {
            GEN_JSON;
            SET_JSON_VAL(N);
            lzh::Json data;
            for (int32 i : range(N))
                data.push_back(ToJson(v[i]));
            json["data"] = data;
            RET_JSON;
        }
    };
    template<> struct JsonInterFace<lzh::Array>
    {
        using value_type = lzh::Array;
        static bool Custom() { return true; }
        static value_type deserialize(const lzh::Json& json) {
            CHECK_JSON(json);
            int rows = json["info"]["rows"];
            int cols = json["info"]["cols"];
            int channels = json["info"]["channels"];
            int type = json["info"]["type"];
            int format = json["info"]["format"];
            if (rows == 0 || cols == 0 || channels == 0)return value_type();
            value_type value(cols, rows, channels, type, (lzh::ColorGamut)format);
            const lzh::Json& data = json["data"];
            for (int i = 0; i < rows; i++) {
                const lzh::Json& arr = data[i];
                for (int j = 0; j < cols; j++) {
                    for (int k = 0; k < channels; k++) {
                        const lzh::Json& val = arr[j * channels + k];
                        if (val.is_number_integer())
                            value.set(i, j, k, (int)val);
                        else if (val.is_number_float())
                            value.set(i, j, k, (float)val);
                    }
                }
            }
            return value;
        }
        static lzh::Json serialize(const value_type& value) {
            lzh::Json json;
            json["info"]["rows"] = value.rows;
            json["info"]["cols"] = value.cols;
            json["info"]["channels"] = value.channels();
            json["info"]["type"] = value.type();
            json["info"]["format"] = (int)value.colorGamut();
            lzh::Json data;
            bool isFloat = lzh::isFloat(value.type());
            for (int row = 0; row < value.rows; row++) {
                lzh::Json arr;
                for (int col = 0; col < value.cols; col++) {
                    for (int channel = 0; channel < value.channels(); channel++) {
                        if (isFloat) {
                            arr.push_back(value.get<float>(row, col, channel));
                        }
                        else {
                            arr.push_back(value.get<int>(row, col, channel));
                        }
                    }
                }
                data.push_back(arr);
            }
            json["data"] = data;
            REGISTER_JSON_TYPE;
            return json;
        }
    };
}

#endif // !JSON_SUPPORT