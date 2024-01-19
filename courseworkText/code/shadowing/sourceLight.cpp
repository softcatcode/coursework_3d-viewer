Color sourceLight(
    LightSource const& source,
    vec3 const& point,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
) {
    float k = 1.f, passedDist = 0.f;
    vec3 srcVec = source.location - point;
    vec3 srcDir = normalize(srcVec);
    float srcDist = length(srcVec);
    BeamSegment beam = { Ray(point + 0.01f * srcDir, srcDir), source.color, 0 };
    bool nextCollisionExists = true;
    while (nextCollisionExists) {
        unique_ptr< pair<Collision, unsigned> > searchResPtr = findNextCollision(beam, objects, spheres);
        if (searchResPtr) {
            float collisionDist = searchResPtr->first.dist;
            passedDist += collisionDist;
            if (passedDist < srcDist) {
                beam.ray.setOrigin(beam.ray.calcPosition(collisionDist + 0.01f));
                k *= searchResPtr->first.transmission();
            } else
                nextCollisionExists = false;
        }
        else
            nextCollisionExists = false;
    }
    return k * source.color;
}