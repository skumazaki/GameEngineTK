#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int heigth)
	:Camera(width ,heigth)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}

void FollowCamera::Update()
{
	// �J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;

	// �Q�Ɠ_���W���v�Z
	refpos = m_targetPos + Vector3(0, 2, 0);
	// ���@����J�������W�ւ̍���
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	// ���@�̌��ɉ�荞�ވׂ̉�]�s��
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	// �J�����ւ̃x�N�g������]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	// �J�������W���v�Z
	eyepos = refpos + cameraV;

	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	// ���N���X�̍X�V
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
