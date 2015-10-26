#include <mbgl/layer/line_layer.hpp>
#include <mbgl/style/property_parsing.hpp>
#include <mbgl/style/style_bucket_parameters.hpp>
#include <mbgl/renderer/line_bucket.hpp>
#include <mbgl/map/tile_id.hpp>

namespace mbgl {

void LineLayoutProperties::parse(const JSVal& value) {
    cap.parse("line-cap", value);
    join.parse("line-join", value);
    miterLimit.parse("line-miter-limit", value);
    roundLimit.parse("line-round-limit", value);
}

void LineLayoutProperties::calculate(LineLayoutProperties& out, float z) const {
    out.cap.value = cap.calculate(z);
    out.join.value = join.calculate(z);
    out.miterLimit.value = miterLimit.calculate(z);
    out.roundLimit.value = roundLimit.calculate(z);
}

void LinePaintProperties::parse(const JSVal& layer) {
    eachPaint(layer, [&] (const JSVal& value, ClassID classID) {
        opacity.parse("line-opacity", value, classID);
        color.parse("line-color", value, classID);
        translate.parse("line-translate", value, classID);
        translateAnchor.parse("line-translate-anchor", value, classID);
        width.parse("line-width", value, classID);
        gapWidth.parse("line-gap-width", value, classID);
        blur.parse("line-blur", value, classID);
        dasharray.parse("line-dasharray", value, classID);
        pattern.parse("line-pattern", value, classID);
    });
}

void LinePaintProperties::cascade(const StyleCascadeParameters& parameters) {
    opacity.cascade(parameters);
    color.cascade(parameters);
    translate.cascade(parameters);
    translateAnchor.cascade(parameters);
    width.cascade(parameters);
    gapWidth.cascade(parameters);
    blur.cascade(parameters);
    dasharray.cascade(parameters);
    pattern.cascade(parameters);
}

RenderPass LinePaintProperties::recalculate(const StyleCalculationParameters& parameters) {
//    paints.removeExpiredTransitions(parameters.now);

    // for scaling dasharrays
    StyleCalculationParameters dashArrayParams = parameters;
    dashArrayParams.z = std::floor(dashArrayParams.z);
    width.calculate(dashArrayParams);
    dashLineWidth = width;

    opacity.calculate(parameters);
    color.calculate(parameters);
    translate.calculate(parameters);
    translateAnchor.calculate(parameters);
    width.calculate(parameters);
    gapWidth.calculate(parameters);
    blur.calculate(parameters);
    dasharray.calculate(parameters);
    pattern.calculate(parameters);

    return (opacity > 0 && color.value[3] > 0 && width > 0) ? RenderPass::Translucent : RenderPass::None;
}

std::unique_ptr<StyleLayer> LineLayer::clone() const {
    std::unique_ptr<LineLayer> result = std::make_unique<LineLayer>();
    result->copy(*this);
    result->layout = layout;
    result->paint = paint;
    return std::move(result);
}

void LineLayer::parseLayout(const JSVal& value) {
    layout.parse(value);
}

void LineLayer::parsePaints(const JSVal& layer) {
    paint.parse(layer);
}

void LineLayer::cascade(const StyleCascadeParameters& parameters) {
    paint.cascade(parameters);
}

bool LineLayer::hasTransitions() const {
    return paint.hasTransitions();
}

void LineLayer::recalculate(const StyleCalculationParameters& parameters) {
    passes = paint.recalculate(parameters);
}

std::unique_ptr<Bucket> LineLayer::createBucket(StyleBucketParameters& parameters) const {
    auto bucket = std::make_unique<LineBucket>();

    layout.calculate(bucket->layout, parameters.tileID.z);

    parameters.eachFilteredFeature(filter, [&] (const auto& feature) {
        bucket->addGeometry(feature.getGeometries());
    });

    return std::move(bucket);
}

}
