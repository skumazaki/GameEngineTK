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
	m_keyborad = nullptr;
	cameraflag = false;
}

void FollowCamera::Update()
{
	// �L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State keystate = m_keyborad->GetState();
	m_keyboardTracker.Update(keystate);

	// C�L�[�������ꂽ��
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		// �J�����̃t���O�̐؂�ւ�
		
			cameraflag = !cameraflag;
	}

	// �Ǐ]�Ώۂ̍��W�����Z�b�g
	if (m_player)
	{
		SetTargetPos(m_player->GetTrans());
		SetTargetAngle(m_player->GetRot().y);
	}

	// �J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;

	if(cameraflag)
	{	
		//FPS�J�����̏���
		Vector3 position;
		// �Q�Ɠ_���W���v�Z
		position = m_targetPos + Vector3(0, 0.2f, 0);
		// ���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		// ���@�̌��ɉ�荞�ވׂ̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// �J�����ւ̃x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// �J�������W���v�Z
		// �����i�񂾈ʒu�����_���W
		eyepos = position + cameraV * 0.1f;
		// �i�񂾈ʒu�����_���W
		refpos = position + cameraV;
	}
	else
	{	
		//TPS�J�����̏���
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

		// ���_�����݈ʒu�����Ԃ���
		eyepos = m_Eyepos + (eyepos - m_Eyepos) * 0.05f;
		// �Q�Ɠ_�����݈ʒu�����Ԃ���
		refpos = m_Refpos + (refpos - m_Refpos) * 0.2f;
	}


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

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyborad)
{
	m_keyborad = keyborad;
}
