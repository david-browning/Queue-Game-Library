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

//cbuffer SceneConstantBuffer : register(b0)
//{
//   float4x4 model;
//   float4x4 view;
//   float4x4 projection;
//};

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
   float4 position : POSITION, 
   float4 color : COLOR)
{
   PSInput result;

   //result.position = mul(position, model);
   result.position = position;
   result.color = color;

   return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
   return input.color;
}
