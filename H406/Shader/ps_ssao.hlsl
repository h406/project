//==============================================================================
//
// ps_bace3d[ps_bace3d.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************

sampler norDepthMap : register(s0);
sampler RandomMap : register(s1);



#define SAMPLES 10

float4 main(float2 uv : TEXCOORD0) : COLOR{

  //float2 centerOrigin = uv * 2.0 - 1.0;
  //float cornerShade = 1.0 - pow(length(centerOrigin * 0.75),10.0);
  //// sinを使ってノイズを表現
  //float3 noiseLine = (3.0 + sin(uv.y * 200.0 )) / 4.0;
  //return float4(noiseLine * 0.5f, 1);



  float density = 5.0;
  float strength = 0.05;
  float falloff = 0.0001;
  float rad = 0.005;
  float blur = 1.0;
  float dotRange = 0.5;
  float optimize = 1.0;


  float fmRange = 0.8f;
  float rnd_offset = 18.0;
  float invSamples = 1.0 / SAMPLES;
  int   blur_limit = 4;

  float pix_offsets[16] = {
    -1.0f,1.0f,
    -2.0f,2.0f,
    -3.0f,3.0f,
    -4.0f,4.0f,
    -5.0f,5.0f,
    -6.0f,6.0f,
    -7.0f,7.0f,
    -8.0f,8.0f
  };

  float3 samples[10] = {
    float3(-0.010735935,0.01647018,0.0062425877),
    float3(-0.06533369,0.3647007,-0.13746321),
    float3(-0.6539235,-0.016726388,-0.53000957),
    float3(0.40958285,0.0052428036,-0.5591124),
    float3(-0.1465366,0.09899267,0.15571679),
    float3(-0.44122112,-0.5458797,0.04912532),
    float3(0.03755566,-0.10961345,-0.33040273),
    float3(0.019100213,0.29652783,0.066237666),
    float3(0.8765323,0.011236004,0.28265962),
    float3(0.29264435,-0.40794238,0.15964167)
  };

  float3 fres = normalize((tex2D(RandomMap,uv * rnd_offset).xyz*2.0) - 1.0);
  float3 currentPixelSample = tex2D(norDepthMap,uv).xyz;
  float currentPixelDepth = tex2D(norDepthMap,uv).a;
  float3 norm = normalize(currentPixelSample.xyz * 2.0f - 1.0f);

  float bl = 0.0;
  float radD = rad / currentPixelDepth;

  float3 ray,occNorm;
  float2 se;
  float occluderDepth,depthDifference,normDiff;

  float finalColor = 0.0f;

  for(int i = 0; i < SAMPLES; i++) {
    ray = radD * reflect(samples[i],fres);
    se = uv + sign(dot(ray,norm)) * ray.xy * float2(1.0f,-1.0f);

    float4 occluderFragment = tex2D(norDepthMap,se);

    occNorm = normalize(occluderFragment.xyz * 2.0f - 1.0f);

    depthDifference = currentPixelDepth - tex2D(norDepthMap,se).a;
    normDiff = 1.0 - (occNorm.x*norm.x + occNorm.y*norm.y + occNorm.z*norm.z);

    bl += step(falloff,depthDifference) *normDiff * (1.0 - smoothstep(falloff,strength,depthDifference));

  }

  float ao = 1.0 - density * bl * invSamples;

  return  float4(ao,ao,ao,1);
}