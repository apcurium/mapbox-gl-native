#ifndef MBGL_LAYOUT_PROPERTY
#define MBGL_LAYOUT_PROPERTY

#include <mbgl/style/property_parsing.hpp>
#include <mbgl/style/function_properties.hpp>

#include <rapidjson/document.h>

namespace mbgl {

using JSVal = rapidjson::Value;

template <typename T>
class LayoutProperty {
public:
    LayoutProperty(T v) : value(v) {}

    void parse(const char * name, const JSVal& layout) {
        if (layout.HasMember(name)) {
            parsedValue = detail::parseProperty<Function<T>>(name, layout[name]);
        }
    }

    T calculate(float z) const {
        return parsedValue
            ? mapbox::util::apply_visitor(PropertyEvaluator<T>(z), *parsedValue)
            : value;
    }

    operator T() const { return value; }

    mapbox::util::optional<Function<T>> parsedValue;
    T value;
};

}

#endif
