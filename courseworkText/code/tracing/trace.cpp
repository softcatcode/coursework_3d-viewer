Color getColor(Beam const& beam)
{
    Color color(0.f, 0.f, 0.f);
    Color white(255.f, 255.f, 255.f);
    unsigned count = 0U;
    for (auto const& seg: beam) {
        if (seg.collisionCount > 0) {
            color += min(seg.color, white);
            ++count;
        }
    }
    Color result;
    if (count > 0)
        result = color / count;
    else
        result = backgroundColor;
    return result;
}

Color trace(RayTracer& tracer, unsigned i, unsigned j)
{
    BrightnessCalcArgs&& args = getBrightnessCalcArgsStruct(tracer);
    return traceBeam(tracer.data[i][j], tracer.objects, tracer.spheres, args);
}

Surface trace(RayTracer& tracer, unsigned step)
{
    unsigned width = (tracer.imgWidth + step - 1) / step;
    unsigned height = (tracer.imgHeight + step - 1) / step;
    Surface result((int32_t)width, (int32_t)height, false);
    auto iter = result.getIter();
    unsigned i = 0, j = 0;
    while (iter.line()) {
        while (iter.pixel()) {
            Color color = trace(tracer, i, j);
            //cout << i << ' ' << j << ' ' << color << endl;
            iter.r() = (unsigned char) color.r;
            iter.g() = (unsigned char) color.g;
            iter.b() = (unsigned char) color.b;
            j += step;
        }
        i += step;
        j = 0;
    }
    result = enlarge(result, step);
    log("trace");
    return result;
}