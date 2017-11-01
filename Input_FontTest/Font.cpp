#include "pch.h"
#include "Font.h"


Font::FontDrawInfo::FontDrawInfo(const std::wstring& text, const Vector2& pos) :
	text(text),
	pos(pos)
{
}

Font::Font()
{
}

void Font::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wchar_t const* fileName)
{
	Font& font = Get();
	font.m_collection = std::make_unique<std::vector<FontDrawInfo>>();
	font.m_font = std::make_unique<SpriteFont>(device, fileName);
	font.m_spriteBatch = std::make_unique<SpriteBatch>(deviceContext);
}

// キューに描画命令を入れる
void Font::DrawQueue(const std::wstring & text, const Vector2 & pos)
{
	Font& font = Get();
	font.m_collection->emplace_back(FontDrawInfo(text, pos));
}

// 全ての文字列を描画し、キューを空にする
void Font::Batch()
{
	Font& font = Get();
	font.m_spriteBatch->Begin();

	for (auto&& i : *(font.m_collection)) {
		const wchar_t* output = i.text.c_str();

		font.m_font->DrawString(font.m_spriteBatch.get(), output,
			i.pos, Colors::White, 0.f, Vector2::Zero);
	}

	font.m_spriteBatch->End();

	font.m_collection->clear();
}

void Font::OnDeviceLost()
{
	Font& font = Get();
	font.m_font.reset();
	font.m_spriteBatch.reset();
}

Font& Font::Get()
{
	static Font ins;
	return ins;
}