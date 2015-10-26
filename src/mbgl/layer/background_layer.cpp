#include <mbgl/layer/background_layer.hpp>
#include <mbgl/style/property_parsing.hpp>
#include <mbgl/renderer/bucket.hpp>

namespace mbgl {

void BackgroundPaintProperties::parse(const JSVal& layer) {
    eachPaint(layer, [&] (const JSVal& value, ClassID classID) {
        opacity.parse("background-opacity", value, classID);
        color.parse("background-color", value, classID);
        pattern.parse("background-pattern", value, classID);
    });
}

void BackgroundPaintProperties::cascade(const StyleCascadeParameters& parameters) {
    opacity.cascade(parameters);
    color.cascade(parameters);
    pattern.cascade(parameters);
}

RenderPass BackgroundPaintProperties::recalculate(const StyleCalculationParameters& parameters) {
//    removeExpiredTransitions(parameters.now);

    opacity.calculate(parameters);
    color.calculate(parameters);
    pattern.calculate(parameters);

    return opacity > 0 ? RenderPass::Translucent : RenderPass::None;
}

std::unique_ptr<StyleLayer> BackgroundLayer::clone() const {
    std::unique_ptr<BackgroundLayer> result = std::make_unique<BackgroundLayer>();
    result->copy(*this);
    result->paint = paint;
    return std::move(result);
}

void BackgroundLayer::parsePaints(const JSVal& layer) {
    paint.parse(layer);
}

void BackgroundLayer::cascade(const StyleCascadeParameters& parameters) {
    paint.cascade(parameters);
}

void BackgroundLayer::recalculate(const StyleCalculationParameters& parameters) {
    passes = paint.recalculate(parameters);
}

bool BackgroundLayer::hasTransitions() const {
    return paint.hasTransitions();
}

std::unique_ptr<Bucket> BackgroundLayer::createBucket(StyleBucketParameters&) const {
    return nullptr;
}

}
