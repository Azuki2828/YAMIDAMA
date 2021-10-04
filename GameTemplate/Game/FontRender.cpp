#include "stdafx.h"
#include "FontRender.h"

void FontRender::Init(
	const wchar_t* text,
	const Vector2& position,
	const Vector4& color,
	const float rotation,
	const float scale,
	const Vector2& pivot
) {
	//各々のパラメーターを設定。
	swprintf_s(m_fontValue.text, text);
	m_fontValue.position = position;
	m_fontValue.color = color;
	m_fontValue.rotation = rotation;
	m_fontValue.scale = scale;
	m_fontValue.pivot = pivot;
}

void FontRender::Render(RenderContext& rc) {


	//描画モードがフォント用の時だけ描画する。
	if (rc.GetRenderMode() == RenderContext::EnRender_Mode::enRenderMode_Font) {
		m_font.Begin(rc);

		//フォントを描画。
		m_font.Draw(
			m_fontValue.text,
			m_fontValue.position,
			m_fontValue.color,
			m_fontValue.rotation,
			m_fontValue.scale,
			m_fontValue.pivot
		);

		m_font.End(rc);
	}
}