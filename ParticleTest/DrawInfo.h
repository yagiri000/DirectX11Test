#pragma once
#include <SimpleMath.h>
#include <d3d11_1.h>
using namespace DirectX::SimpleMath;

struct DrawInfo
{
public:
	DrawInfo();
	~DrawInfo();

	Matrix world;
	Matrix view;
	Matrix proj;
	UINT vertexCount;
};

