#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int heigth)
	:Camera(width ,heigth)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
	m_keyborad = nullptr;
	cameraflag = false;
}

void FollowCamera::Update()
{
	// キーボードの状態を取得
	Keyboard::State keystate = m_keyborad->GetState();
	m_keyboardTracker.Update(keystate);

	// Cキーが押されたら
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		// カメラのフラグの切り替え
		
			cameraflag = !cameraflag;
	}

	// 追従対象の座標等をセット
	if (m_player)
	{
		SetTargetPos(m_player->GetTrans());
		SetTargetAngle(m_player->GetRot().y);
	}

	// カメラ視点座標、参照点座標
	Vector3 eyepos, refpos;

	if(cameraflag)
	{	
		//FPSカメラの処理
		Vector3 position;
		// 参照点座標を計算
		position = m_targetPos + Vector3(0, 0.2f, 0);
		// 自機からカメラ座標への差分
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		// 自機の後ろに回り込む為の回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// カメラへのベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// カメラ座標を計算
		// 少し進んだ位置が視点座標
		eyepos = position + cameraV * 0.1f;
		// 進んだ位置が視点座標
		refpos = position + cameraV;
	}
	else
	{	
		//TPSカメラの処理
		// 参照点座標を計算
		refpos = m_targetPos + Vector3(0, 2, 0);
		// 自機からカメラ座標への差分
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
		// 自機の後ろに回り込む為の回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// カメラへのベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// カメラ座標を計算
		eyepos = refpos + cameraV;

		// 視点を現在位置から補間する
		eyepos = m_Eyepos + (eyepos - m_Eyepos) * 0.05f;
		// 参照点を現在位置から補間する
		refpos = m_Refpos + (refpos - m_Refpos) * 0.2f;
	}


	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	// 基底クラスの更新
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyborad)
{
	m_keyborad = keyborad;
}
