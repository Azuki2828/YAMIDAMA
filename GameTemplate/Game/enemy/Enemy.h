#pragma once
#include "EnemyAction.h"
#include "EnemyAnimation.h"
#include "../player/Player.h"
#include "../../../ExEngine/physics/CharacterController.h"

namespace nsMyGame {

	namespace nsPlayer {

		class CPlayer;
	}

	namespace nsEnemy {

		//エネミークラス
		class CEnemy : public CIGameObject
		{
		protected:

			//ステータス
			struct SStatus {
				int hp = 0;			//体力
				int attack = 0;		//攻撃力
			};

			/**
			 * @brief モジュールをインポートする関数。
			 * @param moduleName モジュール名(Pythonファイル名)
			*/
			void ImportModule(const char* moduleName) {
				// モジュールをインポート
				try {
					m_enemyPyModule = pybind11::module::import(moduleName);
				}
				catch (pybind11::error_already_set& e) {
					MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
				}
			}

			/**
			 * @brief 前方向を更新する関数。
			*/
			void UpdateForward();
		public:
			/**
			 * @brief デストラクタ。
			*/
			~CEnemy();

			/**
			 * @brief Update()関数の前に一度だけ呼ばれる関数。
			 * @return 成功した？
			*/
			bool Start() override;

			
			/**
			 * @brief 更新関数。
			*/
			void Update() override;

			

			/**
			 * @brief プレイヤーとの距離を求める関数。
			 * @return プレイヤーとの距離。
			*/
			const float GetLengthToPlayer()const {

				//プレイヤーに伸びるベクトルを求める。
				CVector3 length = m_player->GetPosition() - m_position;

				//距離を返す。
				return length.Length();
			}

			
			/**
			 * @brief 描画関数。
			 * @param rc レンダーコンテキスト
			*/
			void Render(CRenderContext& rc);

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

			/**
			 * @brief 回転を設定する関数。
			 * @param rot 回転
			*/
			void SetRotation(const CQuaternion& rot) {

				m_rotation = rot;
			}

			/**
			 * @brief 回転を取得する関数。
			 * @return 回転
			*/
			const CQuaternion& GetRotation()const {

				return m_rotation;
			}

			/**
			 * @brief クールタイムを設定する関数。
			 * @param coolTime クールタイム
			*/
			void SetCoolTime(const float coolTime) {

				m_coolTime = coolTime;
			}

			/**
			 * @brief クールタイムを取得する関数。
			 * @return クールタイム
			*/
			const float GetCoolTime()const {

				return m_coolTime;
			}

			/**
			 * @brief キャラクターコントローラーを取得する関数。
			 * @return キャラクターコントローラー
			*/
			CharacterController& GetCharacterController() {

				return m_charaCon;
			}

			/**
			 * @brief 自身を削除する関数。
			*/
			void Delete() {
				DeleteGO(this);
			}

			/**
			 * @brief 自分が死んでいる？
			 * @return HPが０以下？（死んでいる？）
			*/
			const bool IsDeath()const {

				return m_status.hp <= 0;
			}

			/**
			 * @brief ダメージを受けている？
			 * @return ダメージフラグ
			*/
			const bool GetReceiveDamage()const {

				return m_receiveDamage;
			}

			/**
			 * @brief 前方向を取得する関数。
			 * @return 前方向
			*/
			const CVector3& GetForward()const {

				return m_forward;
			}

		public:
			/**
			 * @brief 各種敵固有のStart()関数。
			 * @return 成功した？
			*/
			virtual bool StartSub() = 0;

			/**
			 * @brief 各種敵固有のUpdate()関数。
			*/
			virtual void UpdateSub() = 0;

			/**
			 * @brief ステートを変化させる関数。
			 * @param stateNum ステート番号
			*/
			virtual void ChangeState(int stateNum) = 0;

			/**
			 * @brief トリガーボックスを更新する関数。
			 * @param pos 座標
			 * @param rot 回転
			 * @param forward 前方向
			*/
			virtual void UpdateTriggerBox(const CVector3& pos, const CQuaternion& rot, const CVector3& forward) = 0;

			/**
			 * @brief 移動関数。
			*/
			virtual void Move() = 0;

			/**
			 * @brief 回転関数。
			*/
			virtual void Rotate();

			/**
			 * @brief アニメーションを更新する関数。
			*/
			virtual void AnimationUpdate() = 0;

			/**
			 * @brief ダメージフラグを設定する関数。
			 * @param receiveDamageFlag ダメージフラグ
			*/
			virtual void SetReceiveDamage(const bool receiveDamageFlag) = 0;

			/**
			 * @brief ガード時間を設定する関数。
			 * @param guardTime ガード時間
			*/
			void SetGuardTime(const float guardTime) {

				m_guardTime = guardTime;
			}

			/**
			 * @brief ガード時間を取得する関数。
			 * @return ガード時間
			*/
			float GetGuardTime()const {

				return m_guardTime;
			}

			virtual bool IsGuarded() = 0;
		protected:
			bool m_receiveDamage = false;								//ダメージを受けたか？（たとえ0ダメージでも）
			SStatus m_status;											//ステータス
			CModelRender* m_modelRender = nullptr;						//モデル
			CVector3 m_position = CVector3::Zero;						//座標
			CQuaternion m_rotation = CQuaternion::Identity;				//回転
			float m_coolTime = 0.0f;									//クールタイム
			float m_guardTime = 0.0f;									//ガード時間
			CharacterController m_charaCon;								//キャラクターコントローラー
			CVector3 m_moveSpeed = CVector3::Zero;						//移動速度
			CVector3 m_forward = CVector3::Zero;						//前方向
			pybind11::module m_enemyPyModule;							//Pythonのモジュール

			nsPlayer::CPlayer* m_player = nullptr;						
		};
	}
}

