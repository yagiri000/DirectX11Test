#pragma once

#include <SpriteFont.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <string>
#include <memory>
#include <vector>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Font
{
public:
	struct FontDrawInfo
	{
		FontDrawInfo(const std::wstring& text, const Vector2& pos);
		std::wstring text;
		Vector2 pos;
	};

	Font();
	Font& operator=(const Font& a) = delete;
	Font(const Font& a) = delete;

	// ������
	static void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wchar_t const* fileName);

	// �`��L���[�ɒǉ�
	static void DrawQueue(const std::wstring& text, const Vector2& pos);

	// �o�b�`�������s��
	static void Batch();

	// �f�o�C�X���X�g�΍�
	static void OnDeviceLost();


private:
	static Font& Get();
	std::unique_ptr<DirectX::SpriteFont> m_font;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<std::vector<FontDrawInfo>> m_collection;
};

