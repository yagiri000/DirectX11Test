#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "pch.h"

using namespace DirectX;

class Resource
{
public:
	Resource();
	Resource& operator=(const Resource& a) = delete;
	Resource(const Resource& a) = delete;
	static Resource& Get();

	// Šeí‰Šú‰»‚ğs‚¤
	static void Initialize(ID3D11Device* device);

	static void Load(ID3D11Device * device);

	static void Draw(ID3D11DeviceContext * deviceContext, const std::wstring & key, FXMMATRIX world);


	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unordered_map<std::wstring, std::unique_ptr<DirectX::Model>> m_models;
};