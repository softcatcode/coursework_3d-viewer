pair<BeamSegment, BeamSegment> split(BeamSegment const& beamSeg, Collision const& collision, Object const& obj)
{
    vec3 point = collision.point();
    vec3 reflDir = reflectionDir(beamSeg.ray.getDirection(), collision.n);
    Ray transRay = transmittedRay(beamSeg.ray, obj, point, collision.n, collision.optDensity());
    
    // calculating beam intensity
    Color light;
    if (beamSeg.collisionCount > 0)
        light = min(collision.color, beamSeg.color);
    else
        light = collision.color;
    
    Color transLight = light * collision.transmission();
    Color reflLight = min(light - transLight, obj.getColor());
    
    BeamSegment reflected = {
        Ray(point + 0.01f * reflDir, reflDir),
        reflLight,
        beamSeg.collisionCount + 1
    };
    BeamSegment transmitted = {
        transRay,
        transLight,
        beamSeg.collisionCount + 1
    };
    return { reflected, transmitted };
}