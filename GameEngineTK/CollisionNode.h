/// <summary>
/// �����蔻��m�[�h
/// </summary>
#pragma once

#include "Obj3d.h"
#include "Collision.h"

/// <summary>
/// �����蔻��m�[�h
/// </summary>
class CollisionNode
{
public:
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Draw() = 0;
	
	void SetParent(Obj3d* parent);

	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans; }
protected:
	// �f�o�b�O�\���p�I�u�W�F�N�g
	Obj3d m_obj;
	// �e����̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_Trans;
};

/// <summary>
/// �������蔻��m�[�h
/// </summary>
class SphereNode : public CollisionNode, public Sphere
{
public:
	// �f�t�H���g�R���X�g���N�^
	SphereNode();
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw();

	void SetLocalRadius(float radius) { m_LocalRadius = radius; }
protected:
	// ���[�J�����a
	float m_LocalRadius;
};