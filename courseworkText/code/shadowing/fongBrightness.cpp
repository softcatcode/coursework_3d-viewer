Color fongBrightness(
    Collision const& collision,
    vector<LightSource> const& sources,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    vec3 const& cameraLocation,
    vec3 const& rayDir
) {
    vec3 n = collision.n;
    vec3 point = collision.point();
    vec3 camDir = normalize(cameraLocation - point);
    float d = collision.diffuse();
    float r = collision.reflection();
    Color result = backgroundLight;
    for (auto const& src: sources) {
        vec3 srcDir = normalize(src.location - point);
        Color light = sourceLight(src, point, objects, spheres);
        result += d * max(dot(srcDir, n), 0.f) * light;
        float tmp = 2 * dot(n, srcDir) * dot(n, camDir) - dot(srcDir, camDir);
        tmp = max(0.0f, tmp);
        result += r * pow(tmp, collision.reflection()) * light;
    }
    return result;
}