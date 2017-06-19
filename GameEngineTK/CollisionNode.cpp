#include "CollisionNode.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// CollisionNode�̃����o�֐�
/// </summary>

void CollisionNode::SetParent(Obj3d * parent)
{
	m_obj.SetObjParent(parent);
}

/// <summary>
/// SphereNode�̃����o�֐�
/// </summary>
/// 

SphereNode::SphereNode()
{
	// �f�t�H���g�̔��a
	m_LocalRadius = 1.0f;
}

void SphereNode::Initialize()
{
	// �f�o�b�O�\���p���f���ǂݍ���
	m_obj.LoadModel(L"Resources/SphereNode.cmo");
}

void SphereNode::Update()
{
	m_obj.SetTranslation(m_Trans);
	m_obj.SetScale(Vector3(m_LocalRadius));

	m_obj.Update();
}

void SphereNode::Draw()
{
	m_obj.Drow();
}

