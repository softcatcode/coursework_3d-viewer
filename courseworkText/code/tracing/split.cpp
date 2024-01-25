Ray transmittedRay(
    Ray const& ray,
    vec3 const& point,
    vec3 const& n,
    float optDensity
) {
    vec3 dir = afterTransitionDir(ray.getDirection(), n, spaceOptDensity, optDensity);
    Ray result(point + 0.01f * dir, dir);
    return result;
}

Ray reflectedRay(Ray const& ray, vec3 const& point, vec3 const& n)
{
    vec3 dir = reflectionDir(ray.getDirection(), n);
    Ray result(point + 0.01f * dir, dir);
    return result;
}

pair<BeamSegment, BeamSegment> split(BeamSegment const& beamSeg, Collision const& collision, Object const& obj)
{
    Ray const& ray = beamSeg.ray;
    vec3 const& n = collision.n;
    vec3 const& point = collision.point();
    float k = collision.optDensity();
    float t = collision.transmission(), r = collision.reflection();
    
    Ray reflRay = reflectedRay(ray, point, n);
    Ray transRay = transmittedRay(ray, point, n, k);
    Color transLight = collision.color * t;
    Color reflLight = min(collision.color - transLight, obj.getColor());
    
    BeamSegment reflected = {
        reflRay,
        beamSeg.color + reflLight * beamSeg.nextReflectionWeight,
        beamSeg.collisionCount + 1,
        .nextReflectionWeight = beamSeg.nextReflectionWeight * r
    };
    BeamSegment transmitted = {
        transRay,
        transLight,
        beamSeg.collisionCount + 1,
        .nextReflectionWeight = t
    };
    return { reflected, transmitted };
}