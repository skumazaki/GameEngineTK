#include "CollisionNode.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// CollisionNodeのメンバ関数
/// </summary>

void CollisionNode::SetParent(Obj3d * parent)
{
	m_obj.SetObjParent(parent);
}

/// <summary>
/// SphereNodeのメンバ関数
/// </summary>
/// 

SphereNode::SphereNode()
{
	// デフォルトの半径
	m_LocalRadius = 1.0f;
}

void SphereNode::Initialize()
{
	// デバッグ表示用モデル読み込み
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

