__kernel void Brightness(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar r = input[offset + 0];
    uchar g = input[offset + 1];
    uchar b = input[offset + 2];

   
    b = (uchar)(b + x)
    g = (uchar)(g + x);
    r = (uchar)(r + x);
     

    output[offset + 0] = b;
    output[offset + 1] = g;
    output[offset + 2] = r;
}