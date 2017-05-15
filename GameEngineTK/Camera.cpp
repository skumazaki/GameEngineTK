#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(int width, int height)
{
	// �����o�ϐ��̏�����
	m_Eyepos = Vector3(0, 0, 5);
	m_Refpos = Vector3(0, 0, 0);
	m_Upvec = Vector3(0, 1, 0);

	m_FovY = XMConvertToRadians(60.0f);
	m_Aspect = (float)width / height;
	m_Nearclip = 0.1f;
	m_Farclip = 1000.0f;

	// �r���[�s��̐���
	m_View = Matrix::CreateLookAt(m_Eyepos, m_Refpos, m_Upvec);

	// �ˉe�s��̐���
	m_Proj = Matrix::CreatePerspectiveFieldOfView(m_FovY, m_Aspect,	m_Nearclip,	m_Farclip);

}

Camera::~Camera()
{
}

// �X�V
void Camera::Update()
{
	// �r���[�s��̐���
	m_View = Matrix::CreateLookAt(m_Eyepos, m_Refpos, m_Upvec);
	// �ˉe�s��̐���
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
