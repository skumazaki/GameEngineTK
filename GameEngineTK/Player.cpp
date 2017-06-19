#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Player::Player()
{
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="key"></param>
Player::Player(Keyboard* key)
{
	m_keyboard = key;

	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
	// 自機パーツの読み込み
	m_ObjPlayer.resize(PARTS_NUM);
	m_ObjPlayer[PARTS_FOOT].LoadModel(L"Resources/foot.cmo");
	m_ObjPlayer[PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_ObjPlayer[PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	m_ObjPlayer[PARTS_MARU].LoadModel(L"Resources/maru.cmo");
	m_ObjPlayer[PARTS_MARU2].LoadModel(L"Resources/maru.cmo");
	m_ObjPlayer[PARTS_ARM].LoadModel(L"Resources/arm.cmo");
	m_ObjPlayer[PARTS_GUN].LoadModel(L"Resources/gun.cmo");

	// 親子関係の構築(子パーツに親を設定)
	m_ObjPlayer[PARTS_BODY].SetObjParent(&m_ObjPlayer[PARTS_FOOT]);
	m_ObjPlayer[PARTS_HEAD].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_MARU].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_MARU2].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_ARM].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_GUN].SetObjParent(&m_ObjPlayer[PARTS_ARM]);

	// 子パーツの親からのオフセット（座標のずれ）をセット
	m_ObjPlayer[PARTS_BODY].SetTranslation(Vector3(0, 0.5f, 0));
	m_ObjPlayer[PARTS_HEAD].SetTranslation(Vector3(0, 0.4f, 0));
	m_ObjPlayer[PARTS_ARM].SetTranslation(Vector3(0.7f, 0.3f, -0.2f));
	m_ObjPlayer[PARTS_GUN].SetTranslation(Vector3(0, 0, 0));

	m_ObjPlayer[PARTS_ARM].SetRotation(Vector3(0, XMConvertToRadians(90), 0));
	m_ObjPlayer[PARTS_GUN].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));

	m_FireFlag = false;
	
	{// 弾丸用の当たり判定ノードの設定
		m_CollisionNodeBullet.Initialize();
		// 親パーツを設定
		m_CollisionNodeBullet.SetParent(&m_ObjPlayer[PARTS_HEAD]);
		m_CollisionNodeBullet.SetTrans(Vector3(0, 0.25f, 0));
		m_CollisionNodeBullet.SetLocalRadius(0.3f);
	}
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	// パーツギミック
	{
		m_cycle += 0.04f;
		float scale = 1.0f + sinf(m_cycle);
		// ヘッドパーツの縮小拡大
		m_ObjPlayer[PARTS_HEAD].SetScale(Vector3(scale, scale, scale));
		if (flag)
		{
			static float angle1 = 0.0f;
			angle1 += 0.01f;

			m_ObjPlayer[PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, cosf(scale) * 2, sinf(angle1) * 2));
			m_ObjPlayer[PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, cosf(angle1) * -2, sinf(angle1) * -2));
		}
		else
		{
			static float angle1 = 0.0f;
			angle1 += 1.0f;
			m_ObjPlayer[PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, tanf(scale) * 2, sinf(angle1) * 2));
			m_ObjPlayer[PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, tanf(angle1) * -2, sinf(angle1) * -2));
		}

	}

	// キーボードの状態取得
	Keyboard::State g_key = m_keyboard->GetState();
	m_KeyboardTracker.Update(g_key);

	// Cキーが押されたら
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keyboard::Z))
	{
		// カメラのフラグの切り替え

		flag = !flag;
	}

	// 左旋回
	if (g_key.A)
	{
		// 自機を回転
		//head_rot += 0.05f;
		float angle = m_ObjPlayer[0].GetRotation().y;
		m_ObjPlayer[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	// 右旋回
	if (g_key.D)
	{
		// 自機を回転
		//head_rot -= 0.05f;
		float angle = m_ObjPlayer[0].GetRotation().y;
		m_ObjPlayer[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	// 前進処理
	if (g_key.W)
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 移動ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_ObjPlayer[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		// 自機の座標を移動
		Vector3 pos = m_ObjPlayer[0].GetTranslation();
		//head_pos += moveV;
		m_ObjPlayer[0].SetTranslation(pos + moveV);
	}

	// 後退処理
	if (g_key.S)
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, 0.1f);
		// 移動ベクトルを自機の角度分回転させる
		float angle = m_ObjPlayer[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_ObjPlayer[0].GetTranslation();
		m_ObjPlayer[0].SetTranslation(pos + moveV);
	}

	// 弾丸パーツの前進処理
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keyboard::Space))
	{
		if (m_FireFlag)
		{
			m_FireFlag = false;
			//FireBullet();
			//// 弾丸パーツの座標を移動
			//Vector3 pos = m_ObjPlayer[PARTS_HEAD].GetTranslation();
			//m_ObjPlayer[PARTS_HEAD].SetTranslation(pos + m_BulletVel);
		}
		else
		{
			m_FireFlag = true;
			//ResetBullet();
		}
	}
	if (m_FireFlag)
	{
		FireBullet();
		// 弾丸パーツの座標を移動
		Vector3 pos = m_ObjPlayer[PARTS_HEAD].GetTranslation();
		m_ObjPlayer[PARTS_HEAD].SetTranslation(pos + m_BulletVel);

	}
	else
	{
		ResetBullet();

	}
	


	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Update();
	}

	m_CollisionNodeBullet.Update();
}

/// <summary>
/// 描画
/// </summary>
void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Drow();
	}

	m_CollisionNodeBullet.Draw();
}

/// <summary>
/// 弾の発射
/// </summary>
void Player::FireBullet()
{
	// 発射するパーツのワールド行列を取得
	Matrix worldm = m_ObjPlayer[PARTS_HEAD].GetWorld();

	// 抽出した情報をしまっておく変数
	Vector3 scale;			// ワールドスケーリング
	Quaternion rotation;	// ワールド回転
	Vector3 translation;	// ワールド座標

	// ワールド行列から各要素を抽出 
	worldm.Decompose(scale, rotation, translation);

	// 親パーツから分離、独立させる
	m_ObjPlayer[PARTS_HEAD].SetObjParent(nullptr);
	m_ObjPlayer[PARTS_HEAD].SetScale(scale);
	m_ObjPlayer[PARTS_HEAD].SetRotationQ(rotation);
	m_ObjPlayer[PARTS_HEAD].SetTranslation(translation);

	// 弾丸パーツの速度を設定
	m_BulletVel = Vector3(0, 0, -0.1f);
	// パーツの向きに合わせて速度ベクトルを回転
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	m_FireFlag = true;
}

/// <summary>
/// 弾のリセット
/// </summary>
void Player::ResetBullet()
{
	m_ObjPlayer[PARTS_HEAD].SetObjParent(&m_ObjPlayer[PARTS_BODY]);

	m_ObjPlayer[PARTS_HEAD].SetScale(Vector3(1, 1, 1));
	m_ObjPlayer[PARTS_HEAD].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[PARTS_HEAD].SetTranslation(Vector3(0, 0.4f, 0));

	m_FireFlag = false;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const DirectX::SimpleMath::Vector3& Player::GetRot()
{
	return  m_ObjPlayer[0].GetRotation();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	return  m_ObjPlayer[0].GetTranslation();
}
