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

cbuffer SceneConstantBuffer : register(b0)
{
   float4x4 wvp;
};

cbuffer CameraBuffer : register(b1)
{
   float4x4 view;
   float4x4 projection;
   float4 position;
   float4 look;
   float4 up;
};

//Texture2D diffuseMap : register(t0);
//Texture2D normalMap : register(t1);

//SamplerState sampleClamp : register(s0);
//SamplerState sampleWrap : register(s1);

struct PSInput
{
   float4 position : SV_POSITION;
   float4 color : COLOR;
};

PSInput VSMain(
   float3 position : POSITION, 
   float3 normal : NORMAL,
   float4 color : COLOR,
   float2 uv : TEXCOORD)
{
   PSInput result;
   float4 pos = float4(position.xyz, 1.0f);
   result.position = mul(pos, wvp);
   result.color = color;

   return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
   return input.color;
}
