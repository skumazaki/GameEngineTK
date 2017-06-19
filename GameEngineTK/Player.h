#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"


// プレイヤークラス
class Player
{
public:

	// 自機パーツ
	enum PARTS
	{
		PARTS_FOOT,	// 足
		PARTS_BODY,	// 体
		PARTS_HEAD,	// 頭
		PARTS_MARU,	// 丸い球
		PARTS_MARU2,
		PARTS_ARM,	// 腕
		PARTS_GUN,	// 砲塔

		PARTS_NUM
	};
	// デフォルトコンストラクタ
	Player();
	// コンストラクタ
	Player(DirectX::Keyboard* key);
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Render();
	// 弾丸を発射
	void FireBullet();
	// 弾丸を再装填(リセット)
	void ResetBullet();
	// 回転
	const DirectX::SimpleMath::Vector3& GetRot();
	// 場所
	const DirectX::SimpleMath::Vector3& GetTrans();
	// 弾丸用の当たり判定を取得
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
private:
	// キーボード
	DirectX::Keyboard* m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// 自機パーツ
	std::vector<Obj3d> m_ObjPlayer;
	// サイン用の角度
	float m_cycle;

	bool flag;

	bool m_FireFlag;

	// 弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;

	// 弾丸用の当たり判定
	SphereNode m_CollisionNodeBullet;
};

