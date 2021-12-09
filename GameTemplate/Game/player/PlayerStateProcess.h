#pragma once
#include "Player.h"

////////////////////////////////////////////////////
//プレイヤーのステートによる更新を定義するヘッダー//
////////////////////////////////////////////////////

namespace nsMyGame {

	namespace nsPlayer {

		void CPlayer::CommonStateProcess() {

			//移動処理。
			m_playerAction.Move(m_position, m_forward, m_playerState);

			//回転処理。
			m_playerAction.Rotate(m_rotation);

			//前方向を更新。
			UpdateForward();

			//アクション処理。
			m_playerAction.Action(m_playerState, m_isSelect);

			//ダメージ判定。
			JudgeDamage();

			//アニメーション処理。
			m_playerAnimation.Update(*m_modelRender, m_playerState);

			//クールタイムを更新。
			m_playerAction.Update(m_position, m_rotation, m_forward, m_playerState);

			//死んでいる状態かどうか判定。
			if (IsDeath()){ m_playerState = enState_Death; }

			//座標を設定。
			m_modelRender->SetPosition(m_position);

			//回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//ライトカメラを更新。
			LightCameraUpdate();

			m_isSelect = false;
		}

		void CPlayer::IsRollingStateProcess() {

			//移動処理。
			m_playerAction.Move(m_position, m_forward, m_playerState);

			//前方向を更新。
			UpdateForward();

			//アクション処理。
			m_playerAction.Action(m_playerState, m_isSelect);

			//アニメーション処理。
			m_playerAnimation.Update(*m_modelRender, m_playerState);

			//クールタイムを更新。
			m_playerAction.Update(m_position, m_rotation, m_forward, m_playerState);

			//死んでいる状態かどうか判定。
			if (IsDeath()) { m_playerState = enState_Death; }

			//座標を設定。
			m_modelRender->SetPosition(m_position);

			//回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//ライトカメラを更新。
			LightCameraUpdate();

			m_isSelect = false;
		}

		void CPlayer::IsDeathStateProcess() {

			//死んでいる状態にする。
			m_playerState = enState_Death;

			//アニメーション処理。
			m_playerAnimation.Update(*m_modelRender, m_playerState);

			//ライトカメラを更新。
			LightCameraUpdate();
		}

		void CPlayer::IsDamagedStateProcess() {

			//移動処理。
			m_playerAction.Move(m_position, m_forward, m_playerState);

			//回転処理。
			m_playerAction.Rotate(m_rotation);

			//前方向を更新。
			UpdateForward();

			//アクション処理。
			m_playerAction.Action(m_playerState, m_isSelect);

			//アニメーション処理。
			m_playerAnimation.Update(*m_modelRender, m_playerState);

			//クールタイムを更新。
			m_playerAction.Update(m_position, m_rotation, m_forward, m_playerState);

			//死んでいる状態かどうか判定。
			if (IsDeath()) { m_playerState = enState_Death; }

			//座標を設定。
			m_modelRender->SetPosition(m_position);

			//回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//ライトカメラを更新。
			LightCameraUpdate();

			m_isSelect = false;
		}
	}
}