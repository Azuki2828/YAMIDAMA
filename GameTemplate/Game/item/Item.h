#pragma once
namespace nsMyGame {

	namespace nsItem {

		//アイテムクラス
		class CItem : public CIGameObject
		{
		private:
			//アイテムリスト
			enum ItemList {

				enItem_Key,		//鍵
			};
		private:
			/**
			 * @brief Update()関数の前に一度だけ呼ばれる関数。
			 * @return StartSub()関数が成功した？
			*/
			bool Start()override final;

			/**
			 * @brief 更新関数。
			*/
			void Update()override final;

			/**
			 * @brief エフェクトを更新する関数。
			*/
			void EffectUpdate();

			/**
			 * @brief プレイヤーとの距離が規定以内か調べる関数。
			 * @param pos 
			*/
			const float CheckLengthToPlayer()const;
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
		};
	}
}

