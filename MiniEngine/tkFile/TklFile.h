#pragma once

class CTklFile
{
public:
	/// <summary>
	/// ボーン。
	/// </summary>
	struct SBone {
		std::unique_ptr<char[]> name;	//骨の名前。
		int parentNo;					//親の番号。
		float bindPose[4][3];			//バインドポーズ。
		float invBindPose[4][3];		//バインドポーズの逆数。
		char ShadowCasterFlag;
		char ShadowReceiverFlag;
		int no;							//ボーンの番号。
	};
	/// <summary>
	/// TKSファイルをロードする。
	/// </summary>
	/// <param name="filePath"></param>
	void Load(const char* filePath);
	/// <summary>
	/// ボーンに対してクエリを行う。
	/// </summary>
	/// <param name="query">クエリ関数</param>
	void QueryBone(std::function<void(SBone& bone)> query)
	{
		for (auto& bone : m_bones) {
			query(bone);
		}
	}
	void BuildBoneMatrices();
	void CalcWorldTRS(CVector3& trans, CQuaternion& rot, CVector3& scale);

	 int GetBone(int boneNo) const
	{
		return m_bones2[boneNo]->GetParentBoneNo();
	}

	 const wchar_t* GetName(int boneNo) const { 
		return m_bones2[boneNo]->GetName();
	 }

	 Bone* GetBonePtr(int num) { return m_bones2[num].get(); }

	 int GetBoneNum() { return m_numBone; }
private:
	int m_numBone = 0;			//骨の数。
	std::vector<SBone> m_bones;	//骨のリスト。
	using BonePtr = std::unique_ptr<Bone>;
	std::vector<BonePtr>	m_bones2;				//ボーンの配列。
	std::unique_ptr<CMatrix[]>	m_boneMatrixs;	//ボーン行列。
	bool m_isInited = false;								//初期化済み？

	CMatrix			m_worldMatrix;			//ワールド行列。
	CVector3			m_positoin;				//このボーンのワールド空間での位置。最後にCalcWorldTRSを実行したときの結果が格納されている。
	CVector3			m_scale;				//このボーンの拡大率。最後にCalcWorldTRSを実行したときの結果が格納されている。
	CQuaternion		m_rotation;
};

