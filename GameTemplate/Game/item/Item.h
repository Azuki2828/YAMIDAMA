#pragma once
namespace nsMyGame {

	//エイリアス宣言
	class CAppearSprite;

	namespace nsItem {

		//アイテムクラス
		class CItem : public CIGameObject
		{
		private:
			//アイテムリスト
			enum EnItemList {

				enItem_Key,		//鍵
			};
		private:
			/**
			 * @brief Update()関数の前に一度だけ呼ばれる関数。
			 * @return StartSub()関数が成功した？
			*/
			bool Start()override final;

			/**
			 * @brief 削除関数。
			*/
			void OnDestroy()override final;

			/**
			 * @brief 更新関数。
			*/
			void Update()override final;

			/**
			 * @brief エフェクトの初期化関数。
			*/
			void InitEffect();

			/**
			 * @brief メッセージウィンドウ初期化関数。
			*/
			void InitSpriteAndText();

			/**
			 * @brief エフェクトを更新する関数。
			*/
			void EffectUpdate();

			/**
			 * @brief プレイヤーとの距離が規定以内か調べる関数。
			 * @param pos 
			*/
			const float CheckLengthToPlayer()const;

			/**
			 * @brief このアイテムが選択されている？
			 * @return アイテムが選択されているかどうかのフラグ
			*/
			const bool IsSelected()const {

				return m_isSelected;
			}

			/**
			 * @brief スプライトとテキストを表示する関数。
			*/
			void AppearSpriteAndText();

			/**
			 * @brief スプライトとテキストを非表示にする関数。
			*/
			void DisappearSpriteAndText();
		public:

			/**
			 * @brief このアイテムが選択状態にある？
			 * @return 選択状態かどうかのフラグ。
			*/
			bool IsSelected() {

				return m_isSelected;
			}

			/**
			 * @brief 座標を設定する関数。
			 * @param pos 座標
			*/
			void SetPosition(const CVector3& pos) {

				m_position = pos;
			}

			/**
			 * @brief 座標を取得する関数。
			 * @return 座標
			*/
			const CVector3& GetPosition()const {

				return m_position;
			}
		private:
			bool m_isSelected = false;				//選択状態にある？
			float m_itemEffectReplayTime = 0.0f;	//アイテムエフェクトを再生成するための時間
			CVector3 m_position = CVector3::Zero;	//座標
			Effect* m_itemEffect = nullptr;			//アイテムの印
			EnItemList m_item = enItem_Key;

			CSpriteRender* m_textSprite = nullptr;				//テキストのスプライト
			CAppearSprite* m_getSprite = nullptr;				//アイテム取得時のスプライト
			float m_textSpriteTranslucent = 0.0f;				//スプライトの透明度
			nsFont::CFontRender* m_text = nullptr;				//テキスト
		};
	}
}

