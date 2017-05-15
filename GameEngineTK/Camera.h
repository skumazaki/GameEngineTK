/// <summary>
/// �J�����𐧌䂷��N���X
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class Camera
{
public:
	Camera(int width, int height);

	virtual ~Camera();

	// �X�V
	virtual void Update();

	// �r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetView();

	// �ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& GetProj();

	// ���_���W�̃Z�b�g
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);

	// �Q�ƍ��W�̃Z�b�g
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);

	// ������x�N�g���̃Z�b�g
	void SetEUpVec(const DirectX::SimpleMath::Vector3& upvec);

	// ������������p�̃Z�b�g
	void SetFovY(float fovY);

	// �A�X�y�N�g��̃Z�b�g
	void SetAspect(float aspect);

	// �j�A�N���b�v�̃Z�b�g
	void SetNearClip(float nearclip);

	// �t�@�[�N���b�v�̃Z�b�g
	void SetFarClip(float farclip);

protected:
	// �J�����̈ʒu(���_���W)
	DirectX::SimpleMath::Vector3 m_Eyepos;
	// �J�����̌��Ă����(�����_/�Q�Ɠ_�j
	DirectX::SimpleMath::Vector3 m_Refpos;
	// �J�����̏�����x�N�g��
	DirectX::SimpleMath::Vector3 m_Upvec;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_View;
	// ������������p
	float m_FovY;
	// �A�X�y�N�g��i�c���̔䗦�j
	float m_Aspect;
	// �j�A�X���b�v(��O�̕\�����E����)
	float m_Nearclip;
	// �t�@�[�N���b�v(���̕\�����E����)
	float m_Farclip;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_Proj;
};