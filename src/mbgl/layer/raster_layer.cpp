#include <mbgl/layer/raster_layer.hpp>
#include <mbgl/style/property_parsing.hpp>
#include <mbgl/renderer/bucket.hpp>

namespace mbgl {

void RasterPaintProperties::parse(const JSVal& layer) {
    eachPaint(layer, [&] (const JSVal& value, ClassID classID) {
        opacity.parse("raster-opacity", value, classID);
        hueRotate.parse("raster-hue-rotate", value, classID);
        brightnessMin.parse("raster-brightness-min", value, classID);
        brightnessMax.parse("raster-brightness-max", value, classID);
        saturation.parse("raster-saturation", value, classID);
        contrast.parse("raster-contrast", value, classID);
        fadeDuration.parse("raster-fade-duration", value, classID);
    });
}

void RasterPaintProperties::cascade(const StyleCascadeParameters& parameters) {
    opacity.cascade(parameters);
    hueRotate.cascade(parameters);
    brightnessMin.cascade(parameters);
    brightnessMax.cascade(parameters);
    saturation.cascade(parameters);
    contrast.cascade(parameters);
    fadeDuration.cascade(parameters);
}

RenderPass RasterPaintProperties::recalculate(const StyleCalculationParameters& parameters) {
//    paints.removeExpiredTransitions(parameters.now);

    opacity.calculate(parameters);
    hueRotate.calculate(parameters);
    brightnessMin.calculate(parameters);
    brightnessMax.calculate(parameters);
    saturation.calculate(parameters);
    contrast.calculate(parameters);
    fadeDuration.calculate(parameters);

    return opacity > 0 ? RenderPass::Translucent : RenderPass::None;
}

std::unique_ptr<StyleLayer> RasterLayer::clone() const {
    std::unique_ptr<RasterLayer> result = std::make_unique<RasterLayer>();
    result->copy(*this);
    result->paint = paint;
    return std::move(result);
}

void RasterLayer::parsePaints(const JSVal& layer) {
    paint.parse(layer);
}

void RasterLayer::cascade(const StyleCascadeParameters& parameters) {
    paint.cascade(parameters);
}

bool RasterLayer::hasTransitions() const {
    return paint.hasTransitions();
}

void RasterLayer::recalculate(const StyleCalculationParameters& parameters) {
    passes = paint.recalculate(parameters);
}

std::unique_ptr<Bucket> RasterLayer::createBucket(StyleBucketParameters&) const {
    return nullptr;
}

}
