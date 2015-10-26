#include <mbgl/layer/fill_layer.hpp>
#include <mbgl/style/property_parsing.hpp>
#include <mbgl/style/style_bucket_parameters.hpp>
#include <mbgl/renderer/fill_bucket.hpp>

namespace mbgl {

void FillPaintProperties::parse(const JSVal& layer) {
    eachPaint(layer, [&] (const JSVal& value, ClassID classID) {
        antialias.parse("fill-antialias", value, classID);
        opacity.parse("fill-opacity", value, classID);
        color.parse("fill-color", value, classID);
        outlineColor.parse("fill-outline-color", value, classID);
        translate.parse("fill-translate", value, classID);
        translateAnchor.parse("fill-translate-anchor", value, classID);
        pattern.parse("fill-pattern", value, classID);
    });
}

void FillPaintProperties::cascade(const StyleCascadeParameters& parameters) {
    antialias.cascade(parameters);
    opacity.cascade(parameters);
    color.cascade(parameters);
    outlineColor.cascade(parameters);
    translate.cascade(parameters);
    translateAnchor.cascade(parameters);
    pattern.cascade(parameters);
}

RenderPass FillPaintProperties::recalculate(const StyleCalculationParameters& parameters) {
//    paints.removeExpiredTransitions(parameters.now);

    antialias.calculate(parameters);
    opacity.calculate(parameters);
    color.calculate(parameters);
    outlineColor.calculate(parameters);
    translate.calculate(parameters);
    translateAnchor.calculate(parameters);
    pattern.calculate(parameters);

    RenderPass passes = RenderPass::None;

    if (antialias) {
        passes |= RenderPass::Translucent;
    }

    if (!pattern.from.empty() || (color.value[3] * opacity) < 1.0f) {
        passes |= RenderPass::Translucent;
    } else {
        passes |= RenderPass::Opaque;
    }

    return passes;
}

std::unique_ptr<StyleLayer> FillLayer::clone() const {
    std::unique_ptr<FillLayer> result = std::make_unique<FillLayer>();
    result->copy(*this);
    result->paint = paint;
    return std::move(result);
}

void FillLayer::parsePaints(const JSVal& layer) {
    paint.parse(layer);
}

void FillLayer::cascade(const StyleCascadeParameters& parameters) {
    paint.cascade(parameters);
}

bool FillLayer::hasTransitions() const {
    return paint.hasTransitions();
}

void FillLayer::recalculate(const StyleCalculationParameters& parameters) {
    passes = paint.recalculate(parameters);
}

std::unique_ptr<Bucket> FillLayer::createBucket(StyleBucketParameters& parameters) const {
    auto bucket = std::make_unique<FillBucket>();

    parameters.eachFilteredFeature(filter, [&] (const auto& feature) {
        bucket->addGeometry(feature.getGeometries());
    });

    return std::move(bucket);
}

}
