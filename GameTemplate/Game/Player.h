#pragma once


namespace nsMyGame {

	namespace nsPlayer {

		enum EnAnimationList {
			enAnim_Walk,
			enAnim_Idle,
			enAnim_Death,

			enAnimNum

		};
		class Player : public IGameObject
		{
			//プレイヤーの状態
			enum class EnPlayerState {
				enState_Normal,
				enState_Yoi,
				enState_Death,

				enStateNum
			};

			//酔いフォントのカラー状態
			enum EnColor {
				en1,
				en2,
				en3,
				en4,
				en5,
				en6,
			};


			/**
			 * @brief モジュールをインポートする関数。
			 * @param moduleName モジュール名(Pythonファイル名)
			*/
			void ImportModule(const char* moduleName) {
				// モジュールをインポート
				try {
					m_playerPyModule = pybind11::module::import(moduleName);
				}
				catch (pybind11::error_already_set& e) {
					MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
				}
			}

			/**
			 * @brief ライトカメラを更新する関数。
			*/
		public:
			void LightCameraUpdate();
			bool Start()override final;
			void Update()override final;

			/**
			 * @brief ステートを更新する関数。
			*/
			void StateUpdate();

			/**
			 * @brief 座標を取得する関数。
			 * @return
			*/
			const Vector3& GetPosition()const {

				return m_pos;
			}

			/**
			 * @brief モデルレンダーのインスタンスを取得する関数。
			 * @return モデルレンダーのインスタンス
			*/
			ModelRender* GetModelRender() {

				return m_modelRender;
			}

			/**
			 * @brief 酔いパラメーターを設定する関数。
			 * @param param  酔いパラメーター
			*/
			void SetYoiParam(const int param) {

				m_yoiParam = param;
			}

			/**
			 * @brief 酔いパラメーターを取得する関数。
			 * @return 酔いパラメーター
			*/
			int GetYoiParam() {

				return m_yoiParam;
			}

			/**
			 * @brief 移動処理を行う関数。
			*/
			void Move();

			/**
			 * @brief フォントを更新する関数。
			*/
			void FontUpdate();

			/**
			 * @brief ステートを変化させる関数。
			 * @param state
			*/
			void SetState(int state) {
				switch (state) {
				case 0:
					m_playerState = EnPlayerState::enState_Normal;
					break;
				case 1:
					m_playerState = EnPlayerState::enState_Yoi;
					break;
				case 2:
					m_playerState = EnPlayerState::enState_Death;
					break;
				}

			}
		private:
			int m_yoiParam = 0;
			Vector4 m_color = Vector4::White;
			EnColor m_colorState = en1;
			int m_count = 0;
			EnPlayerState m_playerState = EnPlayerState::enState_Normal;
			ModelRender* m_modelRender = nullptr;
			AnimationClip m_animationClip[enAnimNum];

			Vector3 m_pos = { 0.0f,0.0f,0.0f };
			pybind11::module m_playerPyModule;

			nsFont::FontRender* m_fontRender = nullptr;
			nsFont::FontRender* m_fontRender2 = nullptr;
		};
	}
}

