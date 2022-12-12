//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#define NUM_LIGHTS 1
#define SHADOW_DEPTH_BIAS 0.00005f

struct LightState
{
   float3 position;
   float3 direction;
   float4 color;
   float4 falloff;
   float4x4 view;
   float4x4 projection;
};

cbuffer SceneConstantBuffer : register(b0)
{
   float4x4 model;
   float4x4 view;
   float4x4 projection;
   float4 ambientColor;
   bool sampleShadowMap;
   LightState lights[NUM_LIGHTS];
   float4 viewport;
   float4 clipPlane;
};

// Diffuse lighting calculation, with angle and distance falloff.
export float4 CalcLightingColor(
   float3 vLightPos, 
   float3 vLightDir, 
   float4 vLightColor, 
   float4 vFalloffs, 
   float3 vPosWorld, 
   float3 vPerPixelNormal)
{
   float3 vLightToPixelUnNormalized = vPosWorld - vLightPos;

    // Dist falloff = 0 at vFalloffs.x, 1 at vFalloffs.x - vFalloffs.y
   float fDist = length(vLightToPixelUnNormalized);

   float fDistFalloff = saturate((vFalloffs.x - fDist) / vFalloffs.y);

    // Normalize from here on.
   float3 vLightToPixelNormalized = vLightToPixelUnNormalized / fDist;

    // Angle falloff = 0 at vFalloffs.z, 1 at vFalloffs.z - vFalloffs.w
   float fCosAngle = dot(vLightToPixelNormalized, vLightDir / length(vLightDir));
   float fAngleFalloff = saturate((fCosAngle - vFalloffs.z) / vFalloffs.w);

    // Diffuse contribution.
   float fNDotL = saturate(-dot(vLightToPixelNormalized, vPerPixelNormal));

    // Ignore angle falloff for a point light.
   fAngleFalloff = 1.0f;

   return vLightColor * fNDotL * fDistFalloff * fAngleFalloff;
}
