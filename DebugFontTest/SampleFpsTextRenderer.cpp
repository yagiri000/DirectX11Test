#include "pch.h"
#include "SampleFpsTextRenderer.h"

#include "DirectXHelper.h"
#include <utility>

using namespace DX11UniversalTest;
using namespace Microsoft::WRL;

// テキスト レンダリングで使用する D2D リソースを初期化します。
SampleFpsTextRenderer::SampleFpsTextRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) : 
	m_text(L""),
	m_deviceResources(deviceResources)
{
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	//デバイスに依存するリソースを作成します。
	ComPtr<IDWriteTextFormat> textFormat;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			32.0f,
			L"en-US",
			&textFormat
			)
		);

	DX::ThrowIfFailed(
		textFormat.As(&m_textFormat)
		);

	DX::ThrowIfFailed(
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
		);

	DX::ThrowIfFailed(
		m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);

	CreateDeviceDependentResources();
}

// 表示するテキストを更新します。
void SampleFpsTextRenderer::Update(DX::StepTimer const& timer)
{
	// 表示するテキストを更新します。
	UINT fps = timer.GetFramesPerSecond();

	m_text = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";

	ComPtr<IDWriteTextLayout> textLayout;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextLayout(
			m_text.c_str(),
			(UINT) m_text.length(),
			m_textFormat.Get(),
			240.0f, // 入力テキストの最大幅。
			50.0f, // 入力テキストの最大高さ。
			&textLayout
			)
		);

	DX::ThrowIfFailed(
		textLayout.As(&m_textLayout)
		);

	DX::ThrowIfFailed(
		m_textLayout->GetMetrics(&m_textMetrics)
		);
}

// フレームを画面に描画します。
void SampleFpsTextRenderer::Render()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	// TODO : 修正
	// 右下隅に配置
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
		800 - m_textMetrics.layoutWidth,
		600 - m_textMetrics.height
		);

	context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());

	DX::ThrowIfFailed(
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING)
		);

	context->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		m_textLayout.Get(),
		m_whiteBrush.Get()
		);

	//D2DERR_RECREATE_TARGET をここで無視します。このエラーは、デバイスが失われたことを示します。
	// これは、Present に対する次回の呼び出し中に処理されます。
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void SampleFpsTextRenderer::CreateDeviceDependentResources()
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), m_whiteBrush.GetAddressOf())
		);
}
void SampleFpsTextRenderer::ReleaseDeviceDependentResources()
{
	m_whiteBrush.Reset();
}