#ifndef JSON_SUPPORT
#define JSON_SUPPORT

#include <fstream>
#include "json.h"
#include "../core/base.h"

#define CHECK_JSON(json)do{ \
                            std::string jsonType;\
                            GET_JSON_ITEM(jsonType);\
                            if(!jsonType.empty()){\
                                if(jsonType != typeid(value_type).name())\
                                    return;\
                            }\
                        }while(0)
#define CHECK_JSON_TYPE CHECK_JSON(json)
#define JSON_TYPE "jsonType"
#define REGISTER_JSON_TYPE  json[JSON_TYPE] = typeid(value_type).name()


#define DESERIALIZE       static value_type deserialize(const lzh::Json& json)
#define SERIALIZE         static lzh::void serialize(const value_type& v, Json&json)
#define GEN_VALUE         CHECK_JSON_TYPE
#define GEN_JSON
#define RET_VALUE
#define RET_JSON          REGISTER_JSON_TYPE;
#define SET_JSON_PTR(str)   do{json[#str] = ToJson(v->str);}while(0)
#define GET_JSON_PTR(str)   do{if(json.contains(#str))ToValue(json[#str], v->str);}while(0)
#define GET_JSON_VAL(str)   do{if(json.contains(#str))ToValue(json[#str], v.str);}while(0)
#define SET_JSON_VAL(str)   do{json[#str] = ToJson(v.str);}while(0)
#define SET_JSON_ITEM(val)   do{json[#val] = ToJson(val);}while(0)
#define GET_JSON_ITEM(val)   do{if(json.contains(#val))ToValue(json[#val], val);}while(0)

namespace lzh {
    template<typename _Tp> bool TryGetValue(const Json& json, std::string key, _Tp& value) {
        Json::const_iterator iter = json.find(key);
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
        static void deserialize(const Json& json, value_type& val) { val = json; }
        static void serialize(const value_type& value, Json& json) { json = value; }
    };
    template<typename _Tp> Json ToJson(const _Tp& val) {
        Json json;
        JsonInterFace<_Tp>::serialize(val, json);
        return json;
    }
    template<typename _Tp> _Tp ToValue(const Json& json) {
        LZH_ACCESS(if (JsonInterFace<_Tp>::Custom()), THROW_INFO(ERR_INFO_SUPPORT));
        _Tp val;
        JsonInterFace<_Tp>::deserialize(json, val);
        return val;
    }
    template<typename _Tp> void ToValue(const Json& json, _Tp& val) {
        LZH_ACCESS(if (JsonInterFace<_Tp>::Custom()), THROW_INFO(ERR_INFO_SUPPORT));
        JsonInterFace<_Tp>::deserialize(json, val);
    }

    template<typename _Tp> bool ReadJson(std::string path, _Tp& value) {
        std::string filePath = path;
        bool state = true;
        if (!isExists(filePath))state = false;
        if (!JsonInterFace<_Tp>::Custom())state = false;
        if (!state)return false;
        Json json;
        std::ifstream in(filePath);
        if (in.is_open()) {
            in >> json;
            in.close();
        }
        else return false;
        JsonInterFace<_Tp>::deserialize(json, value);
        return true;
    }
    template<typename _Tp> _Tp ReadJson(std::string path) {
        std::string filePath = path;
        LZH_ACCESS(if (isExists(filePath)), THROW_INFO(ERR_INFO_FILE));
        LZH_ACCESS(if (JsonInterFace<_Tp>::Custom()), THROW_INFO(ERR_INFO_SUPPORT));
        Json json;
        std::ifstream in(filePath);
        if (in.is_open()) {
            in >> json;
            in.close();
        }
        _Tp value;
        JsonInterFace<_Tp>::deserialize(json, value);
        return value;
    }
    template<typename _Tp> void WriteJson(std::string path, const _Tp& value, int indent = 4) {
        std::string filePath = path;
        LZH_ACCESS(if (JsonInterFace<_Tp>::Custom()), THROW_INFO(ERR_INFO_SUPPORT));
        Json json ;
        JsonInterFace<_Tp>::serialize(value, json);
        std::ofstream out(filePath);
        if (out.is_open()) {
            if (indent > 0)out << json.dump(indent);
            else out << json;
            out.close();
        }
    }
    template<> struct JsonInterFace<std::string>
    {
        using value_type = std::string;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type& val) { std::string str = json; val = str; }
        static void serialize(const value_type& value, Json& json) { json = value; }
    };
    template<typename _Tp> struct JsonInterFace<Point2_<_Tp>>
    {
        using value_type = Point2_<_Tp>;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(x);
            GET_JSON_VAL(y);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(x);
            SET_JSON_VAL(y);
            RET_JSON;
        }
    };
    template<typename _Tp> struct JsonInterFace<Point3_<_Tp>>
    {
        using value_type = Point3_<_Tp>;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(x);
            GET_JSON_VAL(y);
            GET_JSON_VAL(z);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(x);
            SET_JSON_VAL(y);
            SET_JSON_VAL(z);
            RET_JSON;
        }
    };
    template<> struct JsonInterFace<Val>
    {
        using value_type = Val;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type &v) {
            GEN_VALUE;
            GET_JSON_VAL(v);
            GET_JSON_VAL(type);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(v);
            SET_JSON_VAL(type);
            RET_JSON;
        }
    };
    template<typename _Tp> struct JsonInterFace<Range_<_Tp>>
    {
        using value_type = Range_<_Tp>;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(v1);
            GET_JSON_VAL(v2);
            GET_JSON_VAL(gap_);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(v1);
            SET_JSON_VAL(v2);
            SET_JSON_VAL(gap_);
            RET_JSON;
        }
    };
    template<typename _Tp> struct JsonInterFace<Size_<_Tp>>
    {
        using value_type = Size_<_Tp>;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(w);
            GET_JSON_VAL(h);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(w);
            SET_JSON_VAL(h);
            RET_JSON;
        }
    };
    template<typename _Tp> struct JsonInterFace<Size3_<_Tp>>
    {
        using value_type = Size3_<_Tp>;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(w);
            GET_JSON_VAL(h);
            GET_JSON_VAL(c);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(w);
            SET_JSON_VAL(h);
            SET_JSON_VAL(c);
            RET_JSON;
        }
    };
    template<typename _Tp> struct JsonInterFace<Rect_<_Tp>>
    {
        using value_type = Rect_<_Tp>;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(x);
            GET_JSON_VAL(y);
            GET_JSON_VAL(width);
            GET_JSON_VAL(height);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(x);
            SET_JSON_VAL(y);
            SET_JSON_VAL(width);
            SET_JSON_VAL(height);
            RET_JSON;
        }
    };
    template<> struct JsonInterFace<MatStep>
    {
        using value_type = MatStep;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(stride[0]);
            GET_JSON_VAL(stride[1]);
            GET_JSON_VAL(stride[2]);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(stride[0]);
            SET_JSON_VAL(stride[1]);
            SET_JSON_VAL(stride[2]);
            RET_JSON;
        }
    };
    template<> struct JsonInterFace<Complex>
    {
        using value_type = Complex;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            GET_JSON_VAL(r);
            GET_JSON_VAL(i);
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            SET_JSON_VAL(r);
            SET_JSON_VAL(i);
            RET_JSON;
        }
    };
    template<class _Tp, int32 N> struct JsonInterFace<Vec<_Tp, N>>
    {
        using value_type = Vec<_Tp, N>;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            Json data;
            if (TryGetValue(json, "data", data)) {
                if (data.is_array() && data.size() == N)
                    for (int32 i : range(N))
                        ToValue(data[i], v[i]);
            }
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            Json data;
            for (int32 i : range(N))
                data.push_back(ToJson(v[i]));
            json["data"] = data;
            RET_JSON;
        }
    };
    template<> struct JsonInterFace<Array>
    {
        using value_type = Array;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            CHECK_JSON(json);
            int rows = json["info"]["rows"];
            int cols = json["info"]["cols"];
            int channels = json["info"]["channels"];
            int type = json["info"]["type"];
            int format = json["info"]["format"];
            if (rows == 0 || cols == 0 || channels == 0)return;
            v = value_type(cols, rows, channels, type, (ColorGamut)format);
            const Json& data = json["data"];
            for (int i = 0; i < rows; i++) {
                const Json& arr = data[i];
                for (int j = 0; j < cols; j++) {
                    for (int k = 0; k < channels; k++) {
                        const Json& val = arr[j * channels + k];
                        if (val.is_number_integer())
                            v.set(i, j, k, (int)val);
                        else if (val.is_number_float())
                            v.set(i, j, k, (float)val);
                    }
                }
            }
        }
        static Json serialize(const value_type& value) {
            Json json;
            json["info"]["rows"] = value.rows;
            json["info"]["cols"] = value.cols;
            json["info"]["channels"] = value.channels();
            json["info"]["type"] = value.type();
            json["info"]["format"] = (int)value.colorGamut();
            Json data;
            bool isfloat = isFloat(value.type());
            for (int row = 0; row < value.rows; row++) {
                Json arr;
                for (int col = 0; col < value.cols; col++) {
                    for (int channel = 0; channel < value.channels(); channel++) {
                        if (isfloat) {
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
    template<> struct JsonInterFace<Mat>
    {
        using value_type = Mat;
        static bool Custom() { return true; }
        static void deserialize(const Json& json, value_type&v) {
            GEN_VALUE;
            Size3 size;
            GET_JSON_ITEM(size);
            v.create(size);
            Json data;
            if (TryGetValue(json, "data", data)) {
                for (int h : range(size.h)) {
                    std::string str = data[h];
                    auto arr = str2Data<mat_t>(strSplit(str, ','));
                    for (int w : range(size.w)) {
                        for (int c : range(size.c)) {
                            v(h, w, c) = arr[w * size.c + c];
                        }
                    }
                }
            }
            RET_VALUE;
        }
        static void serialize(const value_type& v, Json&json) {
            GEN_JSON;
            auto size = v.size3();
            SET_JSON_ITEM(size);
            Json data;
            for (int h : range(size.h)) {
                std::string str;
                for (int w : range(size.w)) {
                    for (int c : range(size.c)) {
                        str += __STR(v(h, w, c)) + ",";
                    }
                }
                data.push_back(str);
            }
            SET_JSON_ITEM(data);
            RET_JSON;
        }
    };
    
}

#endif // !JSON_SUPPORT
