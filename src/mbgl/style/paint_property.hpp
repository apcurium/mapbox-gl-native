#ifndef MBGL_PAINT_PROPERTY
#define MBGL_PAINT_PROPERTY

#include <mbgl/style/types.hpp>
#include <mbgl/style/property_parsing.hpp>
#include <mbgl/style/property_evaluator.hpp>
#include <mbgl/style/function_properties.hpp>
#include <mbgl/style/property_transition.hpp>
#include <mbgl/style/style_cascade_parameters.hpp>
#include <mbgl/style/style_calculation_parameters.hpp>

#include <rapidjson/document.h>

#include <unordered_map>

namespace mbgl {

using JSVal = rapidjson::Value;

template <typename T>
class PaintProperty {
public:
    PaintProperty(T fallbackValue) {
        values[ClassID::Fallback] = fallbackValue;
    }

    void parse(const char * name, const JSVal& paint, ClassID classID) {
        if (paint.HasMember(name)) {
            auto v = parseProperty<Function<T>>(name, paint[name]);
            if (v) {
                values[classID] = *v;
            }
        }

        std::string transitionName = { name };
        transitionName += "-transition";

        if (paint.HasMember(transitionName.c_str())) {
            auto v = parseProperty<PropertyTransition>(name, paint[transitionName.c_str()]);
            if (v) {
                transitions[classID] = *v;
            }
        }
    }

    void cascade(const StyleCascadeParameters& parameters) {
//        Duration delay = transition.delay ? *transition.delay : *parameters.defaultTransition.delay;
//        Duration duration = transition.duration ? *transition.duration : *parameters.defaultTransition.duration;
//        const TimePoint begin = parameters.now + delay;
//        const TimePoint end = begin + duration;
//        const PropertyValue &value = propertyPair.second;
//        appliedProperties.add(classID, begin, end, value);

        for (auto classID : parameters.classes) {
            if (values.find(classID) != values.end()) {
                cascadedValue = values[classID];
            }

            if (transitions.find(classID) != transitions.end()) {
                cascadedTransition = transitions[classID];
            }
        }
    }

    void calculate(const StyleCalculationParameters& parameters);
//        const PropertyEvaluator<T> evaluator(parameters);
//        for (auto& property : applied.propertyValues) {
//            if (parameters.now >= property.end) {
//                // We overwrite the current property with the new value.
//                target = mapbox::util::apply_visitor(evaluator, property.value);
//            } else if (parameters.now >= property.begin) {
//                // We overwrite the current property partially with the new value.
//                float progress = std::chrono::duration<float>(parameters.now - property.begin) / (property.end - property.begin);
//                target = util::interpolate(target, mapbox::util::apply_visitor(evaluator, property.value), progress);
//                hasPendingTransitions = true;
//            } else {
//                // Do not apply this property because its transition hasn't begun yet.
//            }
//        }
//    }

    operator T() const { return value; }

    std::unordered_map<ClassID, Function<T>> values;
    std::unordered_map<ClassID, PropertyTransition> transitions;

    Function<T> cascadedValue;
    PropertyTransition cascadedTransition;

    T value;
    PropertyTransition transition;
};

template <typename T>
class PiecewiseConstantPaintProperty {
public:
    PiecewiseConstantPaintProperty(T v);

    void parse(const char * name, const JSVal& value, ClassID classID);
    void cascade(const StyleCascadeParameters&);
    void calculate(const StyleCalculationParameters&);

//    template <typename T>
//    void calculate(PropertyKey key, T& target, const StyleCalculationParameters& parameters) {
//        auto it = appliedStyle.find(key);
//        if (it != appliedStyle.end()) {
//            AppliedClassPropertyValues &applied = it->second;
//            // Iterate through all properties that we need to apply in order.
//            const PropertyEvaluator<T> evaluator(parameters);
//            for (auto& property : applied.propertyValues) {
//                if (parameters.now >= property.begin) {
//                    // We overwrite the current property with the new value.
//                    target = mapbox::util::apply_visitor(evaluator, property.value);
//                } else {
//                    // Do not apply this property because its transition hasn't begun yet.
//                }
//            }
//        }
//    }

    T from;
    float fromScale;
    T to;
    float toScale;
    float t;
};

}

#endif
