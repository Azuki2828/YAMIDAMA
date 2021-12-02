#pragma once

namespace nsMyGame {

	//スプライト出現クラス
	//アイテム取得時、ドアのロック判定時など、一定時間で消えるスプライトを扱うクラス
	class CAppearSprite : public CIGameObject
	{
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool Start()override final;

		/**
		 * @brief 更新関数。
		*/
		void Update()override final;
	public:

		/**
		 * @brief テキストを設定する関数。
		 * @param text テキスト
		*/
		void SetText(const wchar_t* text) {

			m_text = text;
		}

		/**
		 * @brief テキストの座標を設定する関数。
		 * @param textPos 座標
		*/
		void SetTextPosition(const CVector2& textPos) {

			m_textPosition = textPos;
		}
	private:
		/**
		 * @brief スプライトとテキストを表示する関数。
		*/
		void AppearSpriteAndText();

		/**
		 * @brief スプライトとテキストを非表示にする関数。
		*/
		void DisappearSpriteAndText();

		/**
		 * @brief 確認ボタンが入力された？
		 * @return 入力されたかどうかのフラグ
		*/
		const bool IsPressButton()const {

			return m_isPressButton;
		}
	private:
		const wchar_t* m_text = nullptr;			//テキスト
		CVector2 m_textPosition = CVector2::Zero;	//テキストの座標
		bool m_isPressButton = false;				//確認ボタンが入力された？
		float m_textSpriteTranslucent = 0.0f;		//スプライトの透明度
		CSpriteRender* m_textSprite = nullptr;		//スプライト
		nsFont::CFontRender* m_font = nullptr;		//テキストの設定
	};
}

