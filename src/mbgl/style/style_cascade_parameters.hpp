#ifndef STYLE_CASCADE_PARAMETERS
#define STYLE_CASCADE_PARAMETERS

#include <mbgl/util/chrono.hpp>

#include <vector>

namespace mbgl {

class PropertyTransition;

class StyleCascadeParameters {
public:
    StyleCascadeParameters(const std::vector<std::string>& classes_,
                           const TimePoint& now_,
                           const PropertyTransition& defaultTransition_)
        : classes(classes_),
          now(now_),
          defaultTransition(defaultTransition_) {}

    const std::vector<std::string>& classes;
    const TimePoint& now;
    const PropertyTransition& defaultTransition;
};

}

#endif
