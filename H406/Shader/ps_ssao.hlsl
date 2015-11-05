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

//*
float4 main(float2 uv : TEXCOORD0) : COLOR{

  float Occlusion = 0.07f;

  float2 rand2[16] =
  {
    float2(0.53812504,0.18565957),
    float2(0.13790712,0.24864247),
    float2(0.33715037,0.56794053),
    float2(-0.6999805,-0.04511441),
    float2(0.06896307,-0.15983082),
    float2(0.056099437,0.006954967),
    float2(-0.014653638,0.14027752),
    float2(0.010019933,-0.1924225),
    float2(-0.35775623,-0.5301969),
    float2(-0.3169221,0.106360726),
    float2(0.010350345,-0.58698344),
    float2(-0.08972908,-0.49408212),
    float2(0.7119986,-0.0154690035),
    float2(-0.053382345,0.059675813),
    float2(0.035267662,-0.063188605),
    float2(-0.47761092,0.2847911)
  };

  float depth = tex2D(norDepthMap,uv).a;

  float color = 1.0f;

  for(int i = 0; i < 16; i++) {
    float2 d = rand2[i] * 0.015f;

      float depth1 = tex2D(norDepthMap,uv + d).a;
    float depth2 = tex2D(norDepthMap,uv - d).a;

    if(depth >(depth1 + depth2) * 0.5f + 0.005f
      && depth < (depth1 + depth2) * 0.5f + 0.02f) {
      color -= Occlusion;
    }
  }

  float4 outDiffuse = color;
    outDiffuse.a = 1.0f;

  return outDiffuse;
}

/*/
#define SAMPLES 10

float4 main(float2 uv : TEXCOORD0) : COLOR{

  float density = 7.0;
  float strength = 0.005;
  float falloff = 0.000001;
  float rad = 0.004;
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
//*/