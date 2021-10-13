/*!
* @brief	フォント。
*/
#include "stdafx.h"
#include "FontEngine.h"
#include "font.h"

namespace nsMyGame {

	namespace nsFont {

		void CFont::Begin(CRenderContext& rc)
		{
			auto& fontEngine = g_graphicsEngine->GetFontEngine();
			fontEngine.BeginDraw(rc);
		}

		void CFont::End(CRenderContext& rc)
		{
			auto& fontEngine = g_graphicsEngine->GetFontEngine();
			fontEngine.EndDraw(rc);
		}
		void CFont::Draw(
			wchar_t const* text,
			const CVector2& position,
			const CVector4& color,
			float rotation,
			float scale,
			CVector2 pivot)
		{
			if (text == nullptr) {
				return;
			}
			pivot.y = 1.0f - pivot.y;
			DirectX::XMFLOAT2 tkFloat2Zero(0, 0);
			//座標系をスプライトと合わせる。
			CVector2 pos = position;
			float frameBufferHalfWidth = g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
			float frameBufferHalfHeight = g_graphicsEngine->GetFrameBufferHeight() * 0.5f;
			pos.x += frameBufferHalfWidth;
			pos.y = -pos.y + frameBufferHalfHeight;

			auto& fontEngine = g_graphicsEngine->GetFontEngine();

			if (m_isDrawShadow) {
				//影を書く。
				CVector2 offsetTbl[] = {
					{ m_shadowOffset , 0.0f},
					{ -m_shadowOffset , 0.0f },
					{ 0.0f , m_shadowOffset },
					{ 0.0f , -m_shadowOffset },

					{ m_shadowOffset ,  m_shadowOffset },
					{ m_shadowOffset ,  -m_shadowOffset },
					{ -m_shadowOffset , m_shadowOffset },
					{ -m_shadowOffset , -m_shadowOffset },
				};
				for (auto offset : offsetTbl) {

					CVector2 sPos = pos;
					sPos.x += offset.x;
					sPos.y += offset.y;
					fontEngine.Draw(
						text,
						sPos,
						m_shadowColor,
						rotation,
						scale,
						pivot

					);
				}

			}
			fontEngine.Draw(
				text,
				pos,
				color,
				rotation,
				scale,
				pivot
			);
		}
	}
}
