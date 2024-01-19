Color traceBrightness(
    Collision const& collision,
    vector<LightSource> const& sources,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
) {
    vec3 point = collision.point();
    Color result(0.f, 0.f, 0.f);
    float difRatio = collision.diffuse();
    for (auto const& src: sources)
        result += sourceLight(src, point, objects, spheres) * difRatio;
    return result;
}