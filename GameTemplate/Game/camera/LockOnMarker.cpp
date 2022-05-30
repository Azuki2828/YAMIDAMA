#include "stdafx.h"
#include "LockOnMarker.h"

namespace {

	constexpr const char* c_lockOnMarkerFilePath = "Assets/Image/lockOnMarker_white.dds";		//アイコンのファイルパス
	const CVector2 c_lockOnMarkerWH = { 200.0f,180.0f };										//アイコンの幅と高さ
	constexpr short c_markerSpeedMul = 15;														//アイコン速度
	constexpr float c_scaleBase = 0.2f;															//開始時の拡大率
}

namespace nsMyGame {

	namespace nsCamera {

		//目標となる拡大率の定義
		const float CLockOnMarker::m_stepScale[] = {

			0.50f,
			0.20f,
			0.30f
		};

		void CLockOnMarker::Init() {

			//ロックオンアイコンを初期化。
			m_lockOnMarker = NewGO<CSpriteRender>(enPriority_Fifth);
			m_lockOnMarker->Init(c_lockOnMarkerFilePath, c_lockOnMarkerWH.x, c_lockOnMarkerWH.y);

			//パラメータを初期化。
			m_sizeStep = 0;										//ステップ
			m_position = CVector3::Zero;						//座標
			m_scale = c_scaleBase;										//拡大
			m_scaleDiff = m_stepScale[m_sizeStep] - m_scale;	//拡大率変動の値
			m_updateNum = 0;									//拡大率更新回数
		}

		void CLockOnMarker::Release() {

			//解放。
			if (m_lockOnMarker != nullptr) {
				DeleteGO(m_lockOnMarker);
				m_lockOnMarker = nullptr;
			}
		}

		void CLockOnMarker::UpdateMarker(const CVector2& position) {

			//拡大率を更新。
			UpdateScale();

			//座標を更新。
			m_lockOnMarker->SetPosition(position);
		}

		void CLockOnMarker::UpdateScale() {

			//現在のステップが累計ステップ数を超えていないなら(1次元配列なので0番目を参照)
			if (std::extent<decltype(m_stepScale), 0>::value > m_sizeStep) {

				//1フレーム当たりの拡大率の動きを求めて加算。
				m_scale += m_scaleDiff / static_cast<float>(c_markerSpeedMul);
				m_updateNum++;

				//一定回数に到達したら次のステップに。
				if (m_updateNum == c_markerSpeedMul) {

					//ステップ、拡大率変動の値、回数を変更。
					m_sizeStep++;
					m_scaleDiff = m_stepScale[m_sizeStep] - m_scale;
					m_updateNum = 0;
				}

				//拡大率を更新。
				m_lockOnMarker->SetScale({ m_scale ,m_scale ,m_scale });
			}
		}
	}
}