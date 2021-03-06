#pragma once
#include "constPlayer.h"
#include "PlayerAction.h"
#include "PlayerAnimation.h"
#include "PlayerSelect.h"

namespace nsMyGame {

	namespace nsPlayer {

		//エイリアス宣言
		class CPlayerAnimation;

		//プレイヤークラス
		class CPlayer : public CIGameObject
		{
			//ステータス
			struct SStatus {
				int hp = 0;
				int attack = 0;
			};

			/**
			 * @brief ライトカメラを更新する関数。
			*/
			void LightCameraUpdate();

			/**
			 * @brief 最初に一回だけ呼ばれる関数。
			 * @return 成功した？
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
			 * @brief ステータスを初期化する関数。
			*/
			void InitStatus() {

				m_status.hp = c_playerMaxHP;
				m_status.attack = 10;
			}

			/**
			 * @brief プレイヤーの前方向を更新する関数。
			*/
			void UpdateForward();

			/**
			 * @brief 通常の更新処理。
			*/
			void CommonStateProcess();

			/**
			 * @brief ローリング中の更新処理。
			*/
			void IsRollingStateProcess();

			/**
			 * @brief 死んでいる状態の更新処理。
			*/
			void IsDeathStateProcess();

			/**
			 * @brief 被弾中の更新処理。
			*/
			void IsDamagedStateProcess();

			/**
			 * @brief ガード中の更新処理。
			*/
			void IsGuardStateProcess();

			/**
			 * @brief 攻撃中の更新処理。
			*/
			void IsAttackStateProcess();
		public:

			/**
			 * @brief HPを取得する関数。
			 * @return HP
			*/
			const int GetHP()const {

				return m_status.hp;
			}

			/**
			 * @brief ダメージ判定をする関数。
			*/
			void JudgeDamage(const CVector3& effectPos);

			/**
			 * @brief 死んでいるかどうか判定する関数。
			 * @return HPが0以下？
			*/
			const bool IsDeath()const {

				return m_status.hp <= 0;
			}

			/**
			 * @brief 座標を取得する関数。
			 * @return 座標
			*/
			const CVector3& GetPosition()const {

				return m_position;
			}

			/**
			 * @brief 前方向を取得する関数。
			 * @return 前方向
			*/
			const CVector3& GetForward()const {

				return m_forward;
			}

			/**
			 * @brief モデルレンダーのインスタンスを取得する関数。
			 * @return モデルレンダーのインスタンス
			*/
			CModelRender* GetModelRender() {

				return m_modelRender;
			}

			/**
			 * @brief キャラクターコントローラーを取得する関数。
			 * @return キャラクターコントローラー
			*/
			CharacterController& GetCharacterController() {

				return m_playerAction.GetCharacterController();
			}

			/**
			 * @brief ダメージ状態にする関数。
			*/
			void ReceiveDamage(const CVector3& effectPos) {

				//被弾。
				m_status.hp -= 15;

				//死んだなら
				if (m_status.hp <= 0) {

					//HPを0に設定。
					m_status.hp = 0;

					//死亡時のボイスを再生。
					CSoundManager::GetInstance()->Play(enSE_DeathVoice);
				}

				//ダメージ状態に。
				m_playerAction.ReceiveDamage();
				m_playerState = enState_Damage;

				//血しぶきエフェクトの座標を初期化。
				CVector3 effectPosition = m_position;
				effectPosition.y += 100.0f;

				//血しぶきエフェクトを再生。
				Effect* bloodEffect = NewGO<Effect>(enPriority_Zeroth);
				bloodEffect->Init(c_bloodEffectFilePath);
				bloodEffect->SetPosition(effectPosition);
				bloodEffect->SetScale(c_bloodEffectSize);
				bloodEffect->Play();
			}

			/**
			 * @brief 攻撃を弾かれたときに呼ばれる関数。
			*/
			void AttackBreak() {

				m_playerState = enState_AttackBreak;
				m_playerAction.AttackBreak();
			}

			/**
			 * @brief ガード中？
			 * @return ガードしているかどうかのフラグ
			*/
			bool IsGuard() {

				return m_playerAction.IsGuard();
			}

			/**
			 * @brief 鍵を所持している？
			 * @return 鍵の所持数が０より大きいかどうかの判定
			*/
			const bool HasKey()const {

				return m_hasKeyNum > 0;
			}

			/**
			 * @brief 鍵を取得する関数。
			*/
			void GetKey() {

				m_hasKeyNum++;
			}

			/**
			 * @brief 鍵を消費する関数。
			*/
			void ConsumeKey() {

				m_hasKeyNum--;
			}

			/**
			 * @brief 何かを選んでいるかどうかのフラグを設定する関数。
			 * @param selectFlag 何かを選んでいるかどうかのフラグ
			*/
			void SetSelectFlag(const bool selectFlag) {

				m_isSelect = selectFlag;
			}

			/**
			 * @brief 何かを選んでいる状態？
			 * @return 何かを選んでいるかどうかのフラグ
			*/
			const bool IsSelect()const {

				return m_isSelect;
			}

			/**
			 * @brief オブジェクトを選択状態にする関数。
			 * @param selectObj オブジェクト
			*/
			void SetSelectObject(CIGameObject* selectObj) {

				m_playerSelect.SetSelectObject(selectObj, m_position);
			}

			/**
			 * @brief アニメーションイベントを設定する関数。
			 * @param animation アニメーション
			 * @param modelRender モデルレンダー
			*/
			void AddAnimationEvent(Animation& animation, CModelRender& modelRender) {

				//アニメーションイベント用の関数を設定する。
				modelRender.AddAnimationEvent(animation, [&](const wchar_t* clipName, const wchar_t* eventName) {

					m_playerAction.OnAnimationEvent(clipName, eventName);
				});
			}

		private:
			bool m_isSelect = false;							//何かを選んでいる状態？（近くの何かに反応している？）
			int m_hasKeyNum = 0;								//鍵の所持数
			SStatus m_status;									//ステータス
			CModelRender* m_modelRender = nullptr;				//モデル
			CPlayerAction m_playerAction;						//プレイヤーアクションクラス
			CPlayerAnimation m_playerAnimation;					//プレイヤーアニメーションクラス
			CPlayerSelect m_playerSelect;						//プレイヤーの選択状態クラス
			CVector3 m_position = CVector3::Zero;				//座標
			CQuaternion m_rotation = CQuaternion::Identity;		//回転
			CVector3 m_forward = CVector3::Zero;				//前方向
			EnPlayerState m_playerState;						//プレイヤーのステート
		};
	}
}

