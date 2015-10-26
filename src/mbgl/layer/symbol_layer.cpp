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

std::unique_ptr<StyleLayer> SymbolLayer::clone() const {
    std::unique_ptr<SymbolLayer> result = std::make_unique<SymbolLayer>();
    result->copy(*this);
    result->layout = layout;
    result->paints.paints = paints.paints;
    return std::move(result);
}

void SymbolLayer::parseLayout(const JSVal& value) {
    layout.parse(value);
}

void SymbolLayer::parsePaints(const JSVal& layer) {
    paints.parseEach(layer, [&] (ClassProperties& paint, const JSVal& value) {
        parseProperty<Function<float>>("icon-opacity", PropertyKey::IconOpacity, paint, value);
        parseProperty<PropertyTransition>("icon-opacity-transition", PropertyKey::IconOpacity, paint, value);
        parseProperty<Function<Color>>("icon-color", PropertyKey::IconColor, paint, value);
        parseProperty<PropertyTransition>("icon-color-transition", PropertyKey::IconColor, paint, value);
        parseProperty<Function<Color>>("icon-halo-color", PropertyKey::IconHaloColor, paint, value);
        parseProperty<PropertyTransition>("icon-halo-color-transition", PropertyKey::IconHaloColor, paint, value);
        parseProperty<Function<float>>("icon-halo-width", PropertyKey::IconHaloWidth, paint, value);
        parseProperty<PropertyTransition>("icon-halo-width-transition", PropertyKey::IconHaloWidth, paint, value);
        parseProperty<Function<float>>("icon-halo-blur", PropertyKey::IconHaloBlur, paint, value);
        parseProperty<PropertyTransition>("icon-halo-blur-transition", PropertyKey::IconHaloBlur, paint, value);
        parseProperty<Function<std::array<float, 2>>>("icon-translate", PropertyKey::IconTranslate, paint, value);
        parseProperty<PropertyTransition>("icon-translate-transition", PropertyKey::IconTranslate, paint, value);
        parseProperty<Function<TranslateAnchorType>>("icon-translate-anchor", PropertyKey::IconTranslateAnchor, paint, value);

        parseProperty<Function<float>>("text-opacity", PropertyKey::TextOpacity, paint, value);
        parseProperty<PropertyTransition>("text-opacity-transition", PropertyKey::TextOpacity, paint, value);
        parseProperty<Function<Color>>("text-color", PropertyKey::TextColor, paint, value);
        parseProperty<PropertyTransition>("text-color-transition", PropertyKey::TextColor, paint, value);
        parseProperty<Function<Color>>("text-halo-color", PropertyKey::TextHaloColor, paint, value);
        parseProperty<PropertyTransition>("text-halo-color-transition", PropertyKey::TextHaloColor, paint, value);
        parseProperty<Function<float>>("text-halo-width", PropertyKey::TextHaloWidth, paint, value);
        parseProperty<PropertyTransition>("text-halo-width-transition", PropertyKey::TextHaloWidth, paint, value);
        parseProperty<Function<float>>("text-halo-blur", PropertyKey::TextHaloBlur, paint, value);
        parseProperty<PropertyTransition>("text-halo-blur-transition", PropertyKey::TextHaloBlur, paint, value);
        parseProperty<Function<std::array<float, 2>>>("text-translate", PropertyKey::TextTranslate, paint, value);
        parseProperty<PropertyTransition>("text-translate-transition", PropertyKey::TextTranslate, paint, value);
        parseProperty<Function<TranslateAnchorType>>("text-translate-anchor", PropertyKey::TextTranslateAnchor, paint, value);
    });
}

void SymbolLayer::cascade(const StyleCascadeParameters& parameters) {
    paints.cascade(parameters);
}

bool SymbolLayer::hasTransitions() const {
    return paints.hasTransitions();
}

void SymbolLayer::recalculate(const StyleCalculationParameters& parameters) {
    paints.removeExpiredTransitions(parameters.now);

    paints.calculateTransitioned(PropertyKey::IconOpacity, properties.icon.opacity, parameters);
    paints.calculateTransitioned(PropertyKey::IconColor, properties.icon.color, parameters);
    paints.calculateTransitioned(PropertyKey::IconHaloColor, properties.icon.halo_color, parameters);
    paints.calculateTransitioned(PropertyKey::IconHaloWidth, properties.icon.halo_width, parameters);
    paints.calculateTransitioned(PropertyKey::IconHaloBlur, properties.icon.halo_blur, parameters);
    paints.calculateTransitioned(PropertyKey::IconTranslate, properties.icon.translate, parameters);
    paints.calculate(PropertyKey::IconTranslateAnchor, properties.icon.translate_anchor, parameters);

    paints.calculateTransitioned(PropertyKey::TextOpacity, properties.text.opacity, parameters);
    paints.calculateTransitioned(PropertyKey::TextColor, properties.text.color, parameters);
    paints.calculateTransitioned(PropertyKey::TextHaloColor, properties.text.halo_color, parameters);
    paints.calculateTransitioned(PropertyKey::TextHaloWidth, properties.text.halo_width, parameters);
    paints.calculateTransitioned(PropertyKey::TextHaloBlur, properties.text.halo_blur, parameters);
    paints.calculateTransitioned(PropertyKey::TextTranslate, properties.text.translate, parameters);
    paints.calculate(PropertyKey::TextTranslateAnchor, properties.text.translate_anchor, parameters);

    // text-size and icon-size are layout properties but they also need to be evaluated as paint properties:
    properties.icon.size = layout.icon.size.calculate(parameters.z);
    properties.text.size = layout.text.size.calculate(parameters.z);

    passes = properties.isVisible() ? RenderPass::Translucent : RenderPass::None;
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
