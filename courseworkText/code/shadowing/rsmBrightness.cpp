Color reflectionBrightness(vec3 point, vec3 n, ReflectiveShadowMapElem const& rsmElem)
{
    Color result(0.f, 0.f, 0.f);
    for (auto const& collision: rsmElem) {
        vec3 w = collision.point() - point;
        Color brVal = collision.color * collision.diffuse();
        brVal *= max(0.f, dot(collision.n, w));
        brVal *= max(0.f, dot(collision.n, -w));
        float sqrLen = w.x * w.x + w.y * w.y + w.z * w.z;
        brVal /= sqrLen;
        result += brVal;
    }
    return result;
}

Color directBrightness(vec3 point, ReflectiveShadowMap const& shadowMap, Transformer const& transformer)
{
    transformPoint(point, transformer);
    auto mapSize = sizeOf(shadowMap);
    Color result = Color(0.f, 0.f, 0.f);
    if (
        0 <= point.y && point.y < mapSize.y &&
        0 <= point.x && point.x <= mapSize.x
    ) {
        uint16_t i = uint16_t(point.y), j = uint16_t(point.x);
        auto const& rsmElem = shadowMap[i][j];
        size_t collisionCount = rsmElem.size(), k = 0;
        while (k < collisionCount && abs(point.z - rsmElem[k].dist) >= rsmElem[k].delta)
            ++k;
        if (k < collisionCount)
            result = rsmElem[k].color;
    }
    return result;
}

Color rsmBrightness(
    Collision const& collision,
    vector<ReflectiveShadowMap> const& shadowMaps,
    vector<Transformer> const& transformers
) {
    Color result(0.f, 0.f, 0.f);
    size_t n = shadowMaps.size();
    vec3 point = collision.point();
    float diffuseRatio = collision.diffuse();
    for (size_t i = 0; i < n; ++i) {
        result += directBrightness(point, shadowMaps[i], transformers[i]) * diffuseRatio;
        auto mapSize = sizeOf(shadowMaps[i]);
        for (unsigned j = 0; j < SAMPLE_SIZE; ++j) {
            ivec2 position = getRandomElemPosition(mapSize);
            auto rsmElem = shadowMaps[i][position.y][position.x];
            result += reflectionBrightness(point, collision.n, rsmElem);
        }
    }
    return result;
}