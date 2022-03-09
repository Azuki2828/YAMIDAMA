#pragma once

namespace nsMyGame {

	namespace nsItem {

		constexpr float c_needToDistanceForGetItem = 100.0f;							//アイテムを回収するのに必要な距離
		constexpr const char16_t* c_itemEffectFilePath = u"Assets/effect/item.efk";		//アイテムエフェクトのファイルパス
		const CVector3 c_itemEffectSize = { 10.0f,10.0f,10.0f };						//アイテムエフェクトの拡大率
		constexpr float c_itemEffectReplayTime = 1.0f;									//エフェクトを再生成する間隔
	}
}