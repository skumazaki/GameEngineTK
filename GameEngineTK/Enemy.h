#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3d.h"


// �v���C���[�N���X
class Enemy
{
public:

	// ���@�p�[�c
	enum PARTS
	{
		ENEMY_PARTS_FOOT,	// ��
		ENEMY_PARTS_BODY,	// ��
		ENEMY_PARTS_HEAD,	// ��
		ENEMY_PARTS_MARU,	// �ۂ���
		ENEMY_PARTS_MARU2,
		ENEMY_PARTS_ARM,	// �r
		ENEMY_PARTS_GUN,	// �C��

		PARTS_NUM
	};
	// �f�t�H���g�R���X�g���N�^
	Enemy();
	// �R���X�g���N�^
	Enemy(DirectX::Keyboard* key);
	// �f�X�g���N�^
	~Enemy();

	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Render();
	// ��]
	const DirectX::SimpleMath::Vector3& GetRot();
	// �ꏊ
	const DirectX::SimpleMath::Vector3& GetTrans();

private:
	// �L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// ���@�p�[�c
	std::vector<Obj3d> m_ObjEnemy;
	// �T�C���p�̊p�x
	float m_cycle;

	bool flag;

	// �^�C�}�[
	int m_timer;

	//
	float m_distAngle;
};

