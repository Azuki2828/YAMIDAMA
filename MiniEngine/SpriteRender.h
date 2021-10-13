#pragma once

namespace nsMyGame {

	class CSpriteRender : public CIGameObject
	{
	private:
		bool Start()override final;
		void Update()override final;

	public:
		void Init(const char* filepath, float width, float height, AlphaBlendMode mode = AlphaBlendMode::AlphaBlendMode_Trans);
		//void Render(RenderContext& rc) override;
		void Render2D(CRenderContext& rc)override;
		void SetPosition(const CVector3& pos) { m_pos = pos; }
		void SetRotation(const CQuaternion& rot) { m_rot = rot; }
		void SetScale(const CVector3& sca) { m_sca = sca; }
		void SetBackSpriteFlg(bool backSpriteFlg = true) {

			m_backSpriteFlag = backSpriteFlg;
		}
		CVector3 GetScale() { return m_sca; }
		void SetMulColor(const CVector4& mulCol) {
			m_mulColor = mulCol;
		}
	private:
		bool m_backSpriteFlag = false;
		CSprite m_sprite;
		SpriteInitData m_initData;
		CVector3 m_pos = CVector3::Zero;
		CQuaternion m_rot = CQuaternion::Identity;
		CVector3 m_sca = CVector3::One;
		CVector4 m_mulColor = CVector4::White;	//乗算カラー。
	};
}

