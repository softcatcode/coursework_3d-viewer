struct ObjectProperties
{
    Color color;
    float
        transmission, reflection, diffuseRefl,
        optDensity,
        shineRatio = defaultShineRatio;
};

struct Stage
{
    vector<Object> objects;
    vector<LightSource> sources;
    Observer camera;
};

struct Collision
{
    vec3 n;
    ObjectProperties objProp;
    float x, y;
    union {
        float dist, z;
    };
    Color color = backgroundColor;
    float delta = 0.f;
    
    vec3 point() const { return vec3(x, y, dist); }
    float optDensity() const { return objProp.optDensity; }
    float reflection() const { return objProp.reflection; }
    float transmission() const { return objProp.transmission; }
    float diffuse() const { return objProp.diffuseRefl; }
    float shineRatio() const { return objProp.shineRatio; }
};

struct Transformer
{
    mat3 matrix;
    vec3 bias;
};

struct Line2d
{
    float k, b;
    bool vertical; // If the line is vertical then k and b are meaningless.
    float location; // x coordinate of a vertical line
};

struct PlaneEquation
{
    float a, b, c, d;
};

struct Triangle
{
    unsigned a, b, c;
    vec3 n;
    PlaneEquation plane;
    
    float depthOf(float x, float y) const;
    bool perpendicularToCamera() const;
    void setPlane(vec4 const& ratios);
};

using ReflectiveShadowMapElem = vector<Collision>;

struct DrawAlgoData
{
    Stage stage;
    RayTracer tracer;
    unsigned imgWidth, imgHeight, step;
    Surface image;
    bool updateReq = true;
    BrightnessCalcMethod shadowingMethod = BrightnessCalcMethod::fong;
    
    bool needsUpdate() const { return updateReq; }
    void resumeUpdates() { updateReq = true; }
    void stopUpdate() { updateReq = false; }
};

using ReflectiveShadowMap = vector< vector<ReflectiveShadowMapElem> >;

struct Object
{
    vector<Triangle> triangles;
    vector<vec3> points;
    ObjectProperties properties;
    
    mat3 trianglePoints(size_t i) const;
    vec3 triangleNorm(size_t i) const { return triangles[i].n; };
    Color getColor() const { return properties.color; }
};

struct Observer
{
    vec3 location, x, y, z;
    
    vec3 getLocation() const { return location; }
};

struct LightSource
{
    vec3 location, x, y, z;
    Color color;
};

struct RayTracer
{
    // common fields
    vector< vector<Beam> > data;
    vector<Object> objects;
    vector<Sphere> spheres;
    unsigned imgWidth, imgHeight;
    BrightnessCalcMethod method;
    // for rsm brightness calculation method
    vector<Transformer> transformers;
    vector<ReflectiveShadowMap> sourceMaps;
    // for tracing brightness calculation method
    vector<LightSource> sources;
    vec3 cameraLocation;
};
