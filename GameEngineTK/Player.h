#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"


// �v���C���[�N���X
class Player
{
public:

	// ���@�p�[�c
	enum PARTS
	{
		PARTS_FOOT,	// ��
		PARTS_BODY,	// ��
		PARTS_HEAD,	// ��
		PARTS_MARU,	// �ۂ���
		PARTS_MARU2,
		PARTS_ARM,	// �r
		PARTS_GUN,	// �C��

		PARTS_NUM
	};
	// �f�t�H���g�R���X�g���N�^
	Player();
	// �R���X�g���N�^
	Player(DirectX::Keyboard* key);
	// �f�X�g���N�^
	~Player();

	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Render();
	// �e�ۂ𔭎�
	void FireBullet();
	// �e�ۂ��đ��U(���Z�b�g)
	void ResetBullet();
	// ��]
	const DirectX::SimpleMath::Vector3& GetRot();
	// �ꏊ
	const DirectX::SimpleMath::Vector3& GetTrans();
	// �e�ۗp�̓����蔻����擾
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
private:
	// �L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// ���@�p�[�c
	std::vector<Obj3d> m_ObjPlayer;
	// �T�C���p�̊p�x
	float m_cycle;

	bool flag;

	bool m_FireFlag;

	// �e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;

	// �e�ۗp�̓����蔻��
	SphereNode m_CollisionNodeBullet;
};

