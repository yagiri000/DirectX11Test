//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "MyVertexTypes.h"
#include "ParticleSystem.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;




class Game
{
public:
    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();
    void Clear();
    void Present();
    void CreateDevice();
    void CreateResources();
    void OnDeviceLost();

	std::unique_ptr<ParticleSystem> m_particleSystem;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
};