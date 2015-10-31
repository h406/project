//==============================================================================
//
// ps_bace3d[ps_bace3d.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************


#define SAMPLES 10
#define invSamples  (1.0 / SAMPLES);

sampler rayMap;
sampler normalMap;
sampler depthMap;

// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR {
  float totStrength = 1.58;
  float strength = 0.05;
  float offset = 18.0;
  float falloff = 0.002;
  float rad = 0.003;

  float4 currentPixelSample = tex2D(normalMap,uv);
  float currentPixelDepth = tex2D(depthMap,uv).r;
  float3 norm = currentPixelSample.xyz * 2.0f - 1.0f;

  float bl = 0.0;
  float radD = rad / currentPixelDepth;

  float3 ray,occNorm;
  float2 se;
  float occluderDepth,depthDifference,normDiff;

  for(int i = 0; i < SAMPLES; ++i) {
    float3 fres = tex2D(rayMap,float2(float(i) / 16.f,0)).xyz*2.f - 1.f;
    ray = radD * fres;
    se = uv + sign(dot(ray,norm)) * ray.xy * float2(1.0f,-1.0f);
    float4 occluderFragment = tex2D(normalMap,se.xy);
    occNorm = occluderFragment.xyz * 2.0f - 1.0f;
    depthDifference = currentPixelDepth - tex2D(depthMap,se.xy).r;
    normDiff = (1.0 - dot(normalize(occNorm),normalize(norm)));
    bl += step(falloff,depthDifference) * normDiff * (1.0 - smoothstep(falloff,strength,depthDifference));
  }
  //float ao = ;

  float4 color =  totStrength * bl * invSamples;
  //color.a = 1;
  return color;
}