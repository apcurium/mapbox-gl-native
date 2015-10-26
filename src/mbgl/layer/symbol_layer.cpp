#include <mbgl/layer/symbol_layer.hpp>
#include <mbgl/renderer/symbol_bucket.hpp>
#include <mbgl/map/tile_id.hpp>
#include <mbgl/style/property_evaluator.hpp>
#include <mbgl/style/property_parsing.hpp>
#include <mbgl/style/style_bucket_parameters.hpp>

namespace mbgl {

void SymbolLayoutProperties::parse(const JSVal& value) {
    placement.parse("symbol-placement", value);
    spacing.parse("symbol-spacing", value);
    avoidEdges.parse("symbol-avoid-edges", value);

    icon.allowOverlap.parse("icon-allow-overlap", value);
    icon.ignorePlacement.parse("icon-ignore-placement", value);
    icon.optional.parse("icon-optional", value);
    icon.rotationAlignment.parse("icon-rotation-alignment", value);
    icon.size.parse("icon-size", value);
    icon.image.parse("icon-image", value);
    icon.rotate.parse("icon-rotate", value);
    icon.padding.parse("icon-padding", value);
    icon.keepUpright.parse("icon-keep-upright", value);
    icon.offset.parse("icon-offset", value);

    text.rotationAlignment.parse("text-rotation-alignment", value);
    text.field.parse("text-field", value);
    text.font.parse("text-font", value);
    text.size.parse("text-size", value);
    text.maxWidth.parse("text-max-width", value);
    text.lineHeight.parse("text-line-height", value);
    text.letterSpacing.parse("text-letter-spacing", value);
    text.justify.parse("text-justify", value);
    text.anchor.parse("text-anchor", value);
    text.maxAngle.parse("text-max-angle", value);
    text.rotate.parse("text-rotate", value);
    text.padding.parse("text-padding", value);
    text.keepUpright.parse("text-keep-upright", value);
    text.transform.parse("text-transform", value);
    text.offset.parse("text-offset", value);
    text.allowOverlap.parse("text-allow-overlap", value);
    text.ignorePlacement.parse("text-ignore-placement", value);
    text.optional.parse("text-optional", value);
}

void SymbolLayoutProperties::calculate(SymbolLayoutProperties& out, float z) const {
    out.placement.value = placement.calculate(z);
    if (out.placement.value == PlacementType::Line) {
        out.icon.rotationAlignment = RotationAlignmentType::Map;
        out.text.rotationAlignment = RotationAlignmentType::Map;
    };
    out.spacing = spacing.calculate(z);
    out.avoidEdges = avoidEdges.calculate(z);

    out.icon.allowOverlap = icon.allowOverlap.calculate(z);
    out.icon.ignorePlacement = icon.ignorePlacement.calculate(z);
    out.icon.optional = icon.optional.calculate(z);
    out.icon.rotationAlignment = icon.rotationAlignment.calculate(z);
    out.icon.image = icon.image.calculate(z);
    out.icon.padding = icon.padding.calculate(z);
    out.icon.rotate = icon.rotate.calculate(z);
    out.icon.keepUpright = icon.keepUpright.calculate(z);
    out.icon.offset = icon.offset.calculate(z);

    out.text.rotationAlignment = text.rotationAlignment.calculate(z);
    out.text.field = text.field.calculate(z);
    out.text.font = text.font.calculate(z);
    out.text.maxWidth = text.maxWidth.calculate(z);
    out.text.lineHeight = text.lineHeight.calculate(z);
    out.text.letterSpacing = text.letterSpacing.calculate(z);
    out.text.maxAngle = text.maxAngle.calculate(z);
    out.text.rotate = text.rotate.calculate(z);
    out.text.padding = text.padding.calculate(z);
    out.text.ignorePlacement = text.ignorePlacement.calculate(z);
    out.text.optional = text.optional.calculate(z);
    out.text.justify = text.justify.calculate(z);
    out.text.anchor = text.anchor.calculate(z);
    out.text.keepUpright = text.keepUpright.calculate(z);
    out.text.transform = text.transform.calculate(z);
    out.text.offset = text.offset.calculate(z);
    out.text.allowOverlap = text.allowOverlap.calculate(z);

    out.icon.size = out.icon.size.calculate(z + 1);
    out.text.size = out.text.size.calculate(z + 1);
    out.iconMaxSize = out.icon.size.calculate(18);
    out.textMaxSize = out.text.size.calculate(18);
}

void SymbolPaintProperties::parse(const JSVal& layer) {
    eachPaint(layer, [&] (const JSVal& value, ClassID classID) {
        icon.opacity.parse("icon-opacity", value, classID);
        icon.color.parse("icon-color", value, classID);
        icon.haloColor.parse("icon-halo-color", value, classID);
        icon.haloWidth.parse("icon-halo-width", value, classID);
        icon.haloBlur.parse("icon-halo-blur", value, classID);
        icon.translate.parse("icon-translate", value, classID);
        icon.translateAnchor.parse("icon-translate-anchor", value, classID);

        text.opacity.parse("text-opacity", value, classID);
        text.color.parse("text-color", value, classID);
        text.haloColor.parse("text-halo-color", value, classID);
        text.haloWidth.parse("text-halo-width", value, classID);
        text.haloBlur.parse("text-halo-blur", value, classID);
        text.translate.parse("text-translate", value, classID);
        text.translateAnchor.parse("text-translate-anchor", value, classID);
    });
}

void SymbolPaintProperties::cascade(const StyleCascadeParameters& parameters) {
    icon.opacity.cascade(parameters);
    icon.color.cascade(parameters);
    icon.haloColor.cascade(parameters);
    icon.haloWidth.cascade(parameters);
    icon.haloBlur.cascade(parameters);
    icon.translate.cascade(parameters);
    icon.translateAnchor.cascade(parameters);
    text.opacity.cascade(parameters);
    text.color.cascade(parameters);
    text.haloColor.cascade(parameters);
    text.haloWidth.cascade(parameters);
    text.haloBlur.cascade(parameters);
    text.translate.cascade(parameters);
    text.translateAnchor.cascade(parameters);
}

RenderPass SymbolPaintProperties::recalculate(const StyleCalculationParameters& parameters) {
//    paints.removeExpiredTransitions(parameters.now);

    icon.opacity.calculate(parameters);
    icon.color.calculate(parameters);
    icon.haloColor.calculate(parameters);
    icon.haloWidth.calculate(parameters);
    icon.haloBlur.calculate(parameters);
    icon.translate.calculate(parameters);
    icon.translateAnchor.calculate(parameters);
    text.opacity.calculate(parameters);
    text.color.calculate(parameters);
    text.haloColor.calculate(parameters);
    text.haloWidth.calculate(parameters);
    text.haloBlur.calculate(parameters);
    text.translate.calculate(parameters);
    text.translateAnchor.calculate(parameters);

    return (icon.isVisible() || text.isVisible()) ? RenderPass::Translucent : RenderPass::None;
}

std::unique_ptr<StyleLayer> SymbolLayer::clone() const {
    std::unique_ptr<SymbolLayer> result = std::make_unique<SymbolLayer>();
    result->copy(*this);
    result->layout = layout;
    result->paint = paint;
    return std::move(result);
}

void SymbolLayer::parseLayout(const JSVal& value) {
    layout.parse(value);
}

void SymbolLayer::parsePaints(const JSVal& layer) {
    paint.parse(layer);
}

void SymbolLayer::cascade(const StyleCascadeParameters& parameters) {
    paint.cascade(parameters);
}

bool SymbolLayer::hasTransitions() const {
    return paint.hasTransitions();
}

void SymbolLayer::recalculate(const StyleCalculationParameters& parameters) {
    passes = paint.recalculate(parameters);

    // text-size and icon-size are layout properties but they also need to be evaluated as paint properties:
    paint.icon.size = layout.icon.size.calculate(parameters.z);
    paint.text.size = layout.text.size.calculate(parameters.z);
}

std::unique_ptr<Bucket> SymbolLayer::createBucket(StyleBucketParameters& parameters) const {
    auto bucket = std::make_unique<SymbolBucket>(parameters.tileID.overscaling,
                                                 parameters.tileID.z);

    layout.calculate(bucket->layout, parameters.tileID.z);

    bucket->parseFeatures(parameters.layer, filter);

    if (bucket->needsDependencies(parameters.glyphStore, parameters.sprite)) {
        parameters.partialParse = true;
    }

    // We do not add features if the parser is in a "partial" state because
    // the layer ordering needs to be respected when calculating text
    // collisions. Although, at this point, we requested all the resources
    // needed by this tile.
    if (!parameters.partialParse) {
        bucket->addFeatures(parameters.tileUID,
                            parameters.spriteAtlas,
                            parameters.glyphAtlas,
                            parameters.glyphStore,
                            parameters.collisionTile);
    }

    return std::move(bucket);
}

}
