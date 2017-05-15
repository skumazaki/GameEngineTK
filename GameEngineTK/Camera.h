/// <summary>
/// カメラを制御するクラス
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class Camera
{
public:
	Camera(int width, int height);

	virtual ~Camera();

	// 更新
	virtual void Update();

	// ビュー行列の取得
	const DirectX::SimpleMath::Matrix& GetView();

	// 射影行列の取得
	const DirectX::SimpleMath::Matrix& GetProj();

	// 視点座標のセット
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);

	// 参照座標のセット
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);

	// 上方向ベクトルのセット
	void SetEUpVec(const DirectX::SimpleMath::Vector3& upvec);

	// 垂直方向視野角のセット
	void SetFovY(float fovY);

	// アスペクト比のセット
	void SetAspect(float aspect);

	// ニアクリップのセット
	void SetNearClip(float nearclip);

	// ファークリップのセット
	void SetFarClip(float farclip);

protected:
	// カメラの位置(視点座標)
	DirectX::SimpleMath::Vector3 m_Eyepos;
	// カメラの見ている先(注視点/参照点）
	DirectX::SimpleMath::Vector3 m_Refpos;
	// カメラの上方向ベクトル
	DirectX::SimpleMath::Vector3 m_Upvec;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_View;
	// 垂直方向視野角
	float m_FovY;
	// アスペクト比（縦横の比率）
	float m_Aspect;
	// ニアスリップ(手前の表示限界距離)
	float m_Nearclip;
	// ファークリップ(奥の表示限界距離)
	float m_Farclip;
	// 射影行列
	DirectX::SimpleMath::Matrix m_Proj;
};