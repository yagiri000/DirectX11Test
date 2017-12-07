#include "pch.h"
#include "Resource.h"
#include "Camera.h"

using namespace DirectX;

Resource::Resource()
{
}

Resource & Resource::Get()
{
	static Resource r;
	return r;
}

void Resource::Initialize(ID3D11Device* device)
{
	Resource& ins = Get();
	ins.m_states = std::make_unique<CommonStates>(device);
	ins.m_fxFactory = std::make_unique<EffectFactory>(device);
}

void Resource::Load(ID3D11Device* device)
{
	Resource& ins = Get();

#ifdef _DEBUG
	auto load = [&](const std::wstring& filename) {
		std::wstring fn = L"../Debug/" + filename;
		ins.m_models.emplace(filename, Model::CreateFromCMO(device, fn.c_str(), *(ins.m_fxFactory)));
	};
#else
	auto load = [&](const std::wstring& filename) {
		std::wstring fn = L"Data/Models/" + filename;
		ins.m_models.emplace(filename, Model::CreateFromCMO(device, fn.c_str(), *(ins.m_fxFactory)));
	};
#endif

	load(L"Crystal.cmo");
	load(L"AlphaBox.cmo");
	load(L"panda.cmo");
}

void Resource::Draw(ID3D11DeviceContext * deviceContext, const std::wstring & key, FXMMATRIX world)
{
	Resource& ins = Get();
	// 描画順により半透明描画がおかしくなるので、裏面を描画した後オモテ面を描画する
	ins.m_models[key]->Draw(deviceContext, *ins.m_states, world, Camera::View(), Camera::Projection(), false, [&]() {
		deviceContext->OMSetBlendState(ins.m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		deviceContext->RSSetState(ins.m_states->CullClockwise());
	}); 
	
	ins.m_models[key]->Draw(deviceContext, *ins.m_states, world, Camera::View(), Camera::Projection(), false, [&]() {
		deviceContext->OMSetBlendState(ins.m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		deviceContext->RSSetState(ins.m_states->CullCounterClockwise());
	});
}




