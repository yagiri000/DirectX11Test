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
		WCHAR szDirectoryName[MAX_PATH];
		GetCurrentDirectory(sizeof(szDirectoryName) / sizeof(szDirectoryName[0]), szDirectoryName);
		SetCurrentDirectory(L"../Debug/");
		ins.m_models.emplace(filename, Model::CreateFromCMO(device, filename.c_str(), *(ins.m_fxFactory)));
		SetCurrentDirectory(szDirectoryName);

	};
#else
	auto load = [&](const std::wstring& filename) {
		std::wstring fn = L"Data/Models/" + filename;
		ins.m_models.emplace(filename, Model::CreateFromCMO(device, fn.c_str(), *(ins.m_fxFactory)));
	};
#endif


	load(L"FAIRY_texed.cmo");
	load(L"Floor100.cmo");

}

void Resource::Draw(ID3D11DeviceContext * deviceContext, const std::wstring & key, FXMMATRIX world)
{
	Resource& ins = Get();
	// •`‰æ‡‚É‚æ‚è”¼“§–¾•`‰æ‚ª‚¨‚©‚µ‚­‚È‚é‚Ì‚ÅA— –Ê‚ð•`‰æ‚µ‚½ŒãƒIƒ‚ƒe–Ê‚ð•`‰æ‚·‚é
	ins.m_models[key]->Draw(deviceContext, *ins.m_states, world, Camera::View(), Camera::Projection(), false, [&]() {
		deviceContext->OMSetBlendState(ins.m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		deviceContext->RSSetState(ins.m_states->CullNone());
	}); 
	
}




