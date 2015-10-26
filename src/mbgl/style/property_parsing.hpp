#ifndef MBGL_PROPERTY_PARSING
#define MBGL_PROPERTY_PARSING

#include <mbgl/style/types.hpp>

#include <mapbox/optional.hpp>
#include <rapidjson/document.h>

#include <functional>

namespace mbgl {

using JSVal = rapidjson::Value;

template <typename T>
using optional = mapbox::util::optional<T>;

template <typename T>
optional<T> parseProperty(const char* name, const JSVal&);

template <typename T>
optional<T> parseProperty(const char* name, const JSVal&, const JSVal& transition);

void eachPaint(const JSVal& layer, std::function<void (const JSVal&, ClassID)> function);

}

#endif
