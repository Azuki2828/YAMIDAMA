#pragma once

namespace nsMyGame {

	namespace nsPlayer {

		//プレイヤーアクションクラス
		class CPlayerAction
		{
		public:
			/**
			 * @brief 初期化関数。
			 * @param position プレイヤーの座標。
			*/
			void Init(const CVector3& position, const CQuaternion& rotation, const CVector3& forward, const int swordBoneNum);

			/**
			 * @brief 移動処理を行う関数。
			 * @param position 座標
			 * @param forward 前方向
			 * @param playerState プレイヤーステート
			*/
			void Move(CVector3& position, CVector3& forward, EnPlayerState& playerState);

			/**
			 * @brief 回転処理を行う関数。
			 * @param rotation 回転
			*/
			void Rotate(CQuaternion& rotation);

			/**
			 * @brief アクションを行う関数。
			 * @param playerState プレイヤーステート
			 * @param selectFlag 何かを選んでいるかどうかのフラグ
			*/
			void Action(EnPlayerState& playerState, const bool selectFlag);

			/**
			 * @brief 更新関数。クールタイムの更新などを行う。
			*/
			void Update(const CVector3& pos, const CQuaternion& rot,const CVector3& forward, EnPlayerState& playerState);

			/**
			 * @brief キャラクターコントローラーを取得する関数。
			 * @return キャラクターコントローラー
			*/
			CharacterController& GetCharacterController() {

				return m_charaCon;
			}

			void ReceiveDamage() {

				m_coolTime = 0.5f;
			}

			/**
			 * @brief ガード成功時のクールタイムを設定。
			*/
			void GuardSuccess() {

				m_guardSccessCoolTime = 0.3f;
			}
			/**
			 * @brief 攻撃をガードされた時のクールタイムを設定。
			*/
			void AttackBreak() {

				m_coolTime = 1.0f;
			}

			/**
			 * @brief ガード中？
			 * @return ガードしているかどうかのフラグ
			*/
			const bool IsGuard()const {

				return m_isGuard;
			}

			/**
			 * @brief アニメーションイベント用の関数。
			 * @param clipName アニメーションの名前
			 * @param eventName アニメーションイベントのキーの名前
			*/
			void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
		private:
			/**
			 * @brief クールタイム中かどうか判定する関数。
			 * @return クールタイム中？
			*/
			const bool IsCoolTime()const {

				return m_coolTime > 0.0f;
			}

			/**
			 * @brief ガード中成功時のクールタイム中かどうか判定する関数。
			 * @return ガード中成功時のクールタイム中？
			*/
			const bool IsGuardSccessCoolTime()const {

				return m_guardSccessCoolTime > 0.0f;
			}

			/**
			 * @brief ダッシュ中かどうか判定する関数。
			 * @return ダッシュ中？
			*/
			const bool IsDash()const {

				return m_isDash;
			}

			const bool GetHitAttackFlag()const {

				return m_hitAttack;
			}

			void SetHitAttackFlag(bool hitAttackFlag) {

				m_hitAttack = hitAttackFlag;
			}

			/**
			 * @brief 攻撃用の当たり判定を作成する関数。
			*/
			void CreateAttackCollision();

			
		private:
			bool m_isAttack = false;							//アタック中？
			bool m_hitAttack = false;							//攻撃が当たった？
			bool m_isDash = false;								//ダッシュ中？
			bool m_isGuard = false;								//ガード中？
			float m_coolTime = 0.0f;							//クールタイム
			float m_guardSccessCoolTime = 0.0f;					//ガード成功時のクールタイム
			int m_swordBoneNum = 0;								//剣に取り付けられたボーンの番号
			CVector3 m_position = CVector3::Zero;				//座標
			CQuaternion m_rotation = CQuaternion::Identity;		//回転
			CVector3 m_moveSpeed = CVector3::Zero;				//移動速度
			CVector3 m_rollingSpeed = CVector3::Zero;			//回転速度
			CharacterController m_charaCon;						//キャラクターコントローラー
		};
	}
}
