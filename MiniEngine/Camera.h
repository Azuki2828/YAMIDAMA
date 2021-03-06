/*!
* @brief	カメラ
*/

#pragma once

/// <summary>
/// カメラクラス。
/// </summary>
class CCamera  {
public:
	/// <summary>
	/// 射影行列の更新方法。
	/// </summary>
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//透視射影行列。遠近法が効いた絵を作りたいならこっち。
		enUpdateProjMatrixFunc_Ortho,			//平行投影。２Ｄ的な表現がしたいならこっち。
	};
	/// <summary>
	/// ビュー行列、プロジェクション行列を更新する。
	/// </summary>
	void Update();
	/// <summary>
	/// 注視点を原点としてカメラを回転させる。
	/// </summary>
	/// <param name="qRot">回転させるクォータニオン</param>
	void RotateOriginTarget( const CQuaternion& qRot);
	/// <summary>
	/// カメラを動かす。
	/// </summary>
	/// <param name="move">動かす量</param>
	void Move(const CVector3& move)
	{
		m_position += move;
		m_target += move;
		m_isDirty = true;
	}
	/// <summary>
	/// 注視点を動かす。
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveTarget(const CVector3& move)
	{
		m_target += move;
		m_isDirty = true;
	}
	/// <summary>
	/// 視点を動かす。
	/// </summary>
	/// <param name="move"></param>
	void MovePosition(const CVector3& move)
	{
		m_position += move;
		m_isDirty = true;
	}
	/// <summary>
	/// カメラの前方方向に移動。
	/// </summary>
	/// <param name="moveForward"></param>
	void MoveForward(float moveForward)
	{
		Move(m_forward * moveForward);
	}
	/// <summary>
	/// カメラの右方向に移動。
	/// </summary>
	/// <param name="moveRight"></param>
	void MoveRight(float moveRight)
	{
		Move(m_right * moveRight);
	}
	/// <summary>
	/// カメラの上方向に移動。
	/// </summary>
	/// <param name="moveUp"></param>
	void MoveUp(float moveUp)
	{
		Move(m_up * moveUp);
	}
	/// <summary>
	/// カメラの座標を設定する。
	/// </summary>
	void SetPosition( const CVector3& pos ) 
	{
		m_position = pos;
		m_isDirty = true;
	}
	void SetPosition(float x, float y, float z)
	{
		SetPosition({ x, y, z });
	}
	/// <summary>
	/// カメラの座標を取得。
	/// </summary>
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 注視点を設定。
	/// </summary>
	void SetTarget(float x, float y, float z)
	{
		SetTarget({x, y, z});
	}
	void SetTarget( const CVector3& target )
	{
		m_target = target;
		m_isDirty = true;
	}
	/// <summary>
	/// 注視点を取得。
	/// </summary>
	const CVector3& GetTarget() const
	{
		return m_target;
	}
	/// <summary>
	/// カメラの上方向を設定。
	/// </summary>
	void SetUp( const CVector3& up )
	{
		m_up = up;
	}
	/// <summary>
	/// カメラの上方向を取得。
	/// </summary>
	const CVector3& GetUp() const
	{
		return m_up;
	}

	/// <summary>
	/// ビュー行列の逆行列を取得。
	/// </summary>
	const CMatrix& GetViewMatrixInv() 
	{
		if (m_isDirty) {
			//更新する必要がある。
			Update();
		}
		return m_viewMatrixInv;
	}
	/// <summary>
	/// ビュー行列を取得。
	/// </summary>
	const CMatrix& GetViewMatrix() 
	{
		if (m_isDirty) {
			//更新する必要がある。
			Update();
		}
		return m_viewMatrix;
	}
	/// <summary>
	/// プロジェクション行列を取得。
	/// </summary>
	const CMatrix& GetProjectionMatrix() 
	{
		if (m_isDirty) {
			//更新する必要がある。
			Update();
		}
		return m_projectionMatrix;
	}
	/// <summary>
	/// ビュー×プロジェクション行列を取得。
	/// </summary>
	const CMatrix& GetViewProjectionMatrix() 
	{
		if (m_isDirty) {
			//更新する必要がある。
			Update();
		}
		return m_viewProjectionMatrix;
	}
	/// <summary>
	/// カメラの回転行列を取得。
	/// </summary>
	const CMatrix& GetCameraRotation() 
	{
		if (m_isDirty) {
			//更新する必要がある。
			Update();
		}
		return m_cameraRotation;
	}
	/// <summary>
	/// 遠平面までの距離を設定。
	/// </summary>
	void SetFar( float fFar )
	{
		m_far = fFar;
		m_isDirty = true;
	}
	/// <summary>
	/// 近平面までの距離を設定。
	/// </summary>
	void SetNear( float fNear )
	{
		m_near = fNear;
		m_isDirty = true;
	}
	/// <summary>
	/// 遠平面までの距離を取得。
	/// </summary>
	float GetFar() const
	{
		return m_far;
	}
	/// <summary>
	/// 近平面までの距離を取得。
	/// </summary>
	float GetNear() const
	{
		return m_near;
	}
	/// <summary>
	/// 平行投影の幅を設定。
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Orthoが設定されているときに使用される。
	/// </remarks>
	void SetWidth(float w)
	{
		m_width = w;
		m_isDirty = true;
	}
	/// <summary>
	/// 平行投影の高さを設定。
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Orthoが設定されているときに使用される。
	/// </remarks>
	void SetHeight(float h)
	{
		m_height = h;
		m_isDirty = true;
	}
	/// <summary>
	/// 平行投影の幅を取得。
	/// </summary>
	float GetWidth() const
	{
		return m_width;
	}
	/// <summary>
	/// 平行投影の高さを取得。
	/// </summary>
	float GetHeight() const
	{
		return m_height;
	}
	/// <summary>
	/// 平行投影の高さを設定。
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Orthoが設定されているときに使用される。
	/// </remarks>
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		m_updateProjMatrixFunc = func;
		m_isDirty = true;
	}
	/// <summary>
	/// 画角を設定。
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Perspectiveが設定されているときに使用される。
	/// </remarks>
	/// <param name="viewAngle">画角。単位ラジアン</param>
	void SetViewAngle(float viewAngle)
	{
		m_viewAngle = viewAngle;
		m_isDirty = true;
	}
	/// <summary>
	/// 画角を取得。
	/// </summary>
	/// <returns>画角。単位ラジアン</returns>
	float GetViewAngle() const
	{
		return m_viewAngle;
	}
	/// <summary>
	/// 注視点と視点の距離を取得。
	/// </summary>
	/// <returns></returns>
	float GetTargetToPositionLength() const
	{
		return m_targetToPositionLen;
	}
	/// <summary>
	/// カメラの前方向を取得。
	/// </summary>
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	/// <summary>
	/// カメラの右方向を取得。
	/// </summary>

	const CVector3& GetRight() const
	{
		return m_right;
	}
	/// <summary>
	/// アスペクト比を取得。
	/// </summary>
	float GetAspect() const
	{
		return m_aspect;
	}
	/// <summary>
	/// カメラのコピーを作成。
	/// </summary>
	/// <param name="dst"></param>
	void CopyTo(CCamera& dst) 
	{
		memcpy(&dst, this, sizeof(dst));
		m_isDirty = true;
	}
	/// <summary>
	/// ワールド座標からスクリーン座標を計算する。
	/// </summary>
	/// <remarks>
	/// 計算されるスクリーン座標は画面の中心を{0,0}、左上を{画面の幅*-0.5,画面の高さ*-0.5}
	/// 右下を{ 画面の幅 * 0.5,画面の高さ * 0.5 }とする座標系です。
	/// </remarks>
	/// <param name="screenPos">スクリーン座標の格納先</param>
	/// <param name="worldPos">ワールド座標</param>
	void CalcScreenPositionFromWorldPosition(CVector2& screenPos, const CVector3& worldPos) const;

	/**
	 * @brief ライトカメラを生成する関数。
	*/
	static void CreateLightCamera() {

		m_lightCamera = new CCamera;
	}

	/**
	 * @brief ライトカメラを取得する関数。
	 * @return ライトカメラ
	*/
	static CCamera* GetLightCamera() {

		return m_lightCamera;
	}
protected:
	float		m_targetToPositionLen = 1.0f;			//注視点と視点まで距離。
	CVector3		m_position = {0.0f, 0.0f, 1.0f};		//カメラ位置。
	CVector3		m_up = g_vec3Up;						//カメラの上方向。
	CVector3		m_target;								//カメラの注視点。
	CMatrix		m_viewMatrix;							//ビュー行列。
	CMatrix		m_projectionMatrix;						//プロジェクション行列。
	CMatrix		m_viewProjectionMatrix;					//ビュープロジェクション行列。
	CMatrix		m_viewMatrixInv;						//ビュー行列の逆行列。
	CMatrix		m_cameraRotation;						//カメラの回転行列。
	CVector3		m_forward = g_vec3Front;				//カメラの前方。
	CVector3		m_right = g_vec3Right;					//カメラの右。
	float		m_near = 1.0f;							//近平面。
	float		m_far = 5000.0f;						//遠平面。
	float		m_viewAngle = CMath::DegToRad(60.0f);	//画角(ラジアン)。
	float		m_aspect = 1.0f;						//アスペクト比。
	float		m_width = 1280.0f;						//平行投影行列を作成するときに使用される幅。
	float		m_height = 720.0f;						//平行投影行列を作成するときに使用される高さ。
	EnUpdateProjMatrixFunc m_updateProjMatrixFunc = enUpdateProjMatrixFunc_Perspective;	//プロジェクション行列の更新の仕方。
	bool		m_isNeedUpdateProjectionMatrix = true;
	bool		m_isDirty = false;						//ダーティフラグ。

	static CCamera* m_lightCamera;						//影描画用のライトカメラ。
};
