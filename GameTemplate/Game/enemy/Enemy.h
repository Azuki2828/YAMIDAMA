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

			//敵のステータス
			struct SEnemyStatus {
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

			void UpdateForward();
		public:
			/**
			 * @brief Update()関数の前に一度だけ呼ばれる関数。
			 * @return 成功した？
			*/
			bool Start() override;

			/**
			 * @brief 派生クラス用のStart()関数。
			 * @return 成功した？
			*/
			virtual bool StartSub() = 0;

			/**
			 * @brief 更新関数。
			*/
			void Update() override;

			/**
			 * @brief 派生クラス用のUpdate()関数。
			*/
			virtual void UpdateSub() = 0;

			/**
			 * @brief ステートを変化させる関数。
			 * @param stateNum ステート番号
			*/
			virtual void ChangeState(int stateNum) = 0;


			float GetLengthToPlayer()const {

				

				CVector3 length = m_player->GetPosition() - m_position;

				return length.Length();
			}

			virtual void Move() = 0;

			virtual void Rotate();
			/**
			 * @brief 描画関数。
			 * @param rc レンダーコンテキスト
			*/
			void Render(CRenderContext& rc);

			void SetPosition(const CVector3& pos) {

				m_position = pos;
			}

			void SetCoolTime(const float coolTime) {

				m_coolTime = coolTime;
			}

			float GetCoolTime()const {

				return m_coolTime;
			}


		protected:
			CModelRender* m_modelRender = nullptr;			//モデル
			CVector3 m_position = {1200.0f,500.0f,-1200.0f};			//座標
			CQuaternion m_rotation = CQuaternion::Identity;

			float m_coolTime = 0.0f;

			CharacterController m_charaCon;
			CVector3 m_moveSpeed = CVector3::Zero;
			CVector3 m_forward = CVector3::Zero;

			nsPlayer::CPlayer* m_player = nullptr;
			pybind11::module m_enemyPyModule;				//Pythonのモジュール
		};
	}
}

