#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(int width, int height)
{
	// メンバ変数の初期化
	m_Eyepos = Vector3(0, 0, 5);
	m_Refpos = Vector3(0, 0, 0);
	m_Upvec = Vector3(0, 1, 0);

	m_FovY = XMConvertToRadians(60.0f);
	m_Aspect = (float)width / height;
	m_Nearclip = 0.1f;
	m_Farclip = 1000.0f;

	// ビュー行列の生成
	m_View = Matrix::CreateLookAt(m_Eyepos, m_Refpos, m_Upvec);

	// 射影行列の生成
	m_Proj = Matrix::CreatePerspectiveFieldOfView(m_FovY, m_Aspect,	m_Nearclip,	m_Farclip);

}

Camera::~Camera()
{
}

// 更新
void Camera::Update()
{
	// ビュー行列の生成
	m_View = Matrix::CreateLookAt(m_Eyepos, m_Refpos, m_Upvec);
	// 射影行列の生成
	m_Proj = Matrix::CreatePerspectiveFieldOfView(m_FovY, m_Aspect, m_Nearclip, m_Farclip);
}


const Matrix& Camera::GetView()
{
	return m_View;
}


const Matrix& Camera::GetProj()
{
	return m_Proj;
}


void Camera::SetEyePos(const Vector3& eyepos)
{
	m_Eyepos = eyepos;
}


void Camera::SetRefPos(const Vector3& refpos)
{
	m_Refpos = refpos;
}


void Camera::SetEUpVec(const Vector3& upvec)
{
	m_Upvec = upvec;
}

void Camera::SetFovY(float fovY)
{
	m_FovY = fovY;
}

void Camera::SetAspect(float aspect)
{
	m_Aspect = aspect;
}

void Camera::SetNearClip(float nearclip)
{
	m_Nearclip = nearclip;
}

void Camera::SetFarClip(float farclip)
{
	m_Farclip = farclip;
}
