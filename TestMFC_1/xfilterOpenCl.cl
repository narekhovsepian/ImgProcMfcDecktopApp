__kernel void Grayscale(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    //printf("%d\n",grid);
    uchar r = input[offset + 0];
    uchar g = input[offset + 1];
    uchar b = input[offset + 2];

    // fast integer grayscale conversion
    uchar gray = (uchar)((77 * r + 150 * g + 29 * b) >> 8);

    output[offset + 0] = gray;
    output[offset + 1] = gray;
    output[offset + 2] = gray;
}


__kernel void BlackAndWhite(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar r = input[offset + 0];
    uchar g = input[offset + 1];
    uchar b = input[offset + 2];

    // fast integer grayscale conversion
    uchar gray = (uchar)((77 * r + 150 * g + 29 * b) >> 8);

    if(gray < x){
        gray = 0;
    }
    else {
        gray = 0xff;
    }

    output[offset + 0] = gray;
    output[offset + 1] = gray;
    output[offset + 2] = gray;
}

__kernel void OnlyRed(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar r = input[offset + 2];

    output[offset + 0] = 0;
    output[offset + 1] = 0;
    output[offset + 2] = r;
}

__kernel void OnlyGreen(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar g = input[offset + 1];

    output[offset + 0] = 0;
    output[offset + 1] = g;
    output[offset + 2] = 0;
}

__kernel void OnlyBlue(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar b = input[offset + 0];

    output[offset + 0] = b;
    output[offset + 1] = 0;
    output[offset + 2] = 0;
}

__kernel void GraycaleNegative(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar r = input[offset + 0];
    uchar g = input[offset + 1];
    uchar b = input[offset + 2];

    // fast integer grayscale conversion
    uchar gray = (uchar)((77 * r + 150 * g + 29 * b) >> 8);
    
    uchar diff = 255 - gray;
    
    output[offset + 0] = diff;
    output[offset + 1] = diff;
    output[offset + 2] = diff;
}

__kernel void Negative(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar b = input[offset + 0];
    uchar g = input[offset + 1];
    uchar r = input[offset + 2];

  
    output[offset + 0] = 255 - b;
    output[offset + 1] = 255 - g;
    output[offset + 2] = 255 - r;
}

__kernel void BlackAndWhiteNegative(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar r = input[offset + 0];
    uchar g = input[offset + 1];
    uchar b = input[offset + 2];

    // fast integer grayscale conversion
    uchar gray = (uchar)((77 * r + 150 * g + 29 * b) >> 8);

    if(gray > x){
        gray = 0;
    }
    else {
        gray = 0xff;
    }

    output[offset + 0] = gray;
    output[offset + 1] = gray;
    output[offset + 2] = gray;
}


__kernel void Contrast(
    __global const uchar* input,
    __global uchar* output,
    const float x)
{
    int gid = get_global_id(0);
    int offset = gid * 3;
    
    uchar r = input[offset + 0];
    uchar g = input[offset + 1];
    uchar b = input[offset + 2];

    b = (uchar)(((x * b -128) + 128));
    g = (uchar)(((x * g -128) + 128));
    r = (uchar)(((x * r -128) + 128));
     

    output[offset + 0] = b;
    output[offset + 1] = g;
    output[offset + 2] = r;
}






