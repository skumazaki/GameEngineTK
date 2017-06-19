#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3d.h"


// プレイヤークラス
class Enemy
{
public:

	// 自機パーツ
	enum PARTS
	{
		ENEMY_PARTS_FOOT,	// 足
		ENEMY_PARTS_BODY,	// 体
		ENEMY_PARTS_HEAD,	// 頭
		ENEMY_PARTS_MARU,	// 丸い球
		ENEMY_PARTS_MARU2,
		ENEMY_PARTS_ARM,	// 腕
		ENEMY_PARTS_GUN,	// 砲塔

		PARTS_NUM
	};
	// デフォルトコンストラクタ
	Enemy();
	// コンストラクタ
	Enemy(DirectX::Keyboard* key);
	// デストラクタ
	~Enemy();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Render();
	// 回転
	const DirectX::SimpleMath::Vector3& GetRot();
	// 場所
	const DirectX::SimpleMath::Vector3& GetTrans();

private:
	// キーボード
	DirectX::Keyboard* m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// 自機パーツ
	std::vector<Obj3d> m_ObjEnemy;
	// サイン用の角度
	float m_cycle;

	bool flag;

	// タイマー
	int m_timer;

	//
	float m_distAngle;
};

