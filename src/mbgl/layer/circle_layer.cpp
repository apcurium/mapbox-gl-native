#include <mbgl/layer/circle_layer.hpp>
#include <mbgl/style/property_parsing.hpp>
#include <mbgl/style/style_bucket_parameters.hpp>
#include <mbgl/renderer/circle_bucket.hpp>

namespace mbgl {

void CirclePaintProperties::parse(const JSVal& layer) {
    eachPaint(layer, [&] (const JSVal& value, ClassID classID) {
        radius.parse("circle-radius", value, classID);
        color.parse("circle-color", value, classID);
        opacity.parse("circle-opacity", value, classID);
        translate.parse("circle-translate", value, classID);
        translateAnchor.parse("circle-translate-anchor", value, classID);
        blur.parse("circle-blur", value, classID);
    });
}

void CirclePaintProperties::cascade(const StyleCascadeParameters& parameters) {
    radius.cascade(parameters);
    color.cascade(parameters);
    opacity.cascade(parameters);
    translate.cascade(parameters);
    translateAnchor.cascade(parameters);
    blur.cascade(parameters);
}

RenderPass CirclePaintProperties::recalculate(const StyleCalculationParameters& parameters) {
//    paints.removeExpiredTransitions(parameters.now);

    radius.calculate(parameters);
    color.calculate(parameters);
    opacity.calculate(parameters);
    translate.calculate(parameters);
    translateAnchor.calculate(parameters);
    blur.calculate(parameters);

    return (radius > 0 && color.value[3] > 0 && opacity > 0) ? RenderPass::Translucent : RenderPass::None;
}

std::unique_ptr<StyleLayer> CircleLayer::clone() const {
    std::unique_ptr<CircleLayer> result = std::make_unique<CircleLayer>();
    result->copy(*this);
    result->paint = paint;
    return std::move(result);
}

void CircleLayer::parsePaints(const JSVal& layer) {
    paint.parse(layer);
}

void CircleLayer::cascade(const StyleCascadeParameters& parameters) {
    paint.cascade(parameters);
}

bool CircleLayer::hasTransitions() const {
    return paint.hasTransitions();
}

void CircleLayer::recalculate(const StyleCalculationParameters& parameters) {
    passes = paint.recalculate(parameters);
}

std::unique_ptr<Bucket> CircleLayer::createBucket(StyleBucketParameters& parameters) const {
    auto bucket = std::make_unique<CircleBucket>();

    parameters.eachFilteredFeature(filter, [&] (const auto& feature) {
        bucket->addGeometry(feature.getGeometries());
    });

    return std::move(bucket);
}

}
