#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Enemy::Enemy()
{
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="key"></param>
Enemy::Enemy(Keyboard* key)
{
	m_keyboard = key;

	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Enemy::~Enemy()
{
}

/// <summary>
/// ������
/// </summary>
void Enemy::Initialize()
{
	// ���@�p�[�c�̓ǂݍ���
	m_ObjEnemy.resize(PARTS_NUM);
	m_ObjEnemy[ENEMY_PARTS_FOOT].LoadModel(L"Resources/foot.cmo");
	m_ObjEnemy[ENEMY_PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_ObjEnemy[ENEMY_PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	m_ObjEnemy[ENEMY_PARTS_MARU].LoadModel(L"Resources/maru.cmo");
	m_ObjEnemy[ENEMY_PARTS_MARU2].LoadModel(L"Resources/maru.cmo");
	m_ObjEnemy[ENEMY_PARTS_ARM].LoadModel(L"Resources/arm.cmo");
	m_ObjEnemy[ENEMY_PARTS_GUN].LoadModel(L"Resources/gun.cmo");

	// �e�q�֌W�̍\�z(�q�p�[�c�ɐe��ݒ�)
	m_ObjEnemy[ENEMY_PARTS_BODY].SetObjParent(&m_ObjEnemy[ENEMY_PARTS_FOOT]);
	m_ObjEnemy[ENEMY_PARTS_HEAD].SetObjParent(&m_ObjEnemy[ENEMY_PARTS_BODY]);
	m_ObjEnemy[ENEMY_PARTS_MARU].SetObjParent(&m_ObjEnemy[ENEMY_PARTS_BODY]);
	m_ObjEnemy[ENEMY_PARTS_MARU2].SetObjParent(&m_ObjEnemy[ENEMY_PARTS_BODY]);
	m_ObjEnemy[ENEMY_PARTS_ARM].SetObjParent(&m_ObjEnemy[ENEMY_PARTS_BODY]);
	m_ObjEnemy[ENEMY_PARTS_GUN].SetObjParent(&m_ObjEnemy[ENEMY_PARTS_ARM]);

	// �q�p�[�c�̐e����̃I�t�Z�b�g�i���W�̂���j���Z�b�g
	m_ObjEnemy[ENEMY_PARTS_BODY].SetTranslation(Vector3(0, 0.5f, 0));
	m_ObjEnemy[ENEMY_PARTS_HEAD].SetTranslation(Vector3(0, 0.4f, 0));
	m_ObjEnemy[ENEMY_PARTS_ARM].SetTranslation(Vector3(0.7f, 0.3f, -0.2f));
	m_ObjEnemy[ENEMY_PARTS_GUN].SetTranslation(Vector3(0, 0, 0));

	m_ObjEnemy[ENEMY_PARTS_ARM].SetRotation(Vector3(0, XMConvertToRadians(90), 0));
	m_ObjEnemy[ENEMY_PARTS_GUN].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));

	// �����ʒu������
	Vector3 pos;

	float l_x = rand() % 360;
	float l_z = rand() % 100;
	float m_x = cosf(l_x) * l_z;
	float m_z = sinf(l_x) * l_z;

	pos.x = m_x;
	pos.z = m_z;

	m_ObjEnemy[ENEMY_PARTS_FOOT].SetTranslation(Vector3(pos.x, 0, pos.z));

	m_timer = 0;

	m_distAngle = 0;
}

/// <summary>
/// �X�V
/// </summary>
void Enemy::Update()
{
	// �p�[�c�M�~�b�N
	{
		m_cycle += 0.04f;
		float scale = 1.0f + sinf(m_cycle);
		// �w�b�h�p�[�c�̏k���g��
		m_ObjEnemy[ENEMY_PARTS_HEAD].SetScale(Vector3(scale, scale, scale));
		if (flag)
		{
			static float angle1 = 0.0f;
			angle1 += 0.01f;

			m_ObjEnemy[ENEMY_PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, cosf(scale) * 2, sinf(angle1) * 2));
			m_ObjEnemy[ENEMY_PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, cosf(angle1) * -2, sinf(angle1) * -2));
		}
		else
		{
			static float angle1 = 0.0f;
			angle1 += 1.0f;
			m_ObjEnemy[ENEMY_PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, tanf(scale) * 2, sinf(angle1) * 2));
			m_ObjEnemy[ENEMY_PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, tanf(angle1) * -2, sinf(angle1) * -2));
		}

	}

	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;

		// -0.5�`0.5�̗���
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90�`+90�̗���
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);

		m_distAngle += rnd;
	}

	{
		// ���@�̊p�x�̉�]
		Vector3 rot = m_ObjEnemy[0].GetRotation();
		
		// ���̊p�x�ƖڕW�p�x�̍�
		float angle = m_distAngle - rot.y;
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < XM_PI)
		{
			angle += XM_2PI;
		}

		// ���
		rot.y += angle * 0.01f;

		m_ObjEnemy[0].SetRotation(rot);
	}

	// �L�[�{�[�h�̏�Ԏ擾
	Keyboard::State g_key = m_keyboard->GetState();
	m_KeyboardTracker.Update(g_key);


	{
		Vector3 trans = m_ObjEnemy[0].GetTranslation();

		Vector3 move(0, 0, -0.02f);
		Vector3 rotv = m_ObjEnemy[0].GetRotation();
		Matrix rotmat = Matrix::CreateRotationY(rotv.y);
		move = Vector3::TransformNormal(move, rotmat);

		trans += move;

		m_ObjEnemy[0].SetTranslation(trans);
	}

	// �s��X�V
	for (std::vector<Obj3d>::iterator it = m_ObjEnemy.begin(); it != m_ObjEnemy.end(); it++)
	{
		it->Update();
	}
}

/// <summary>
/// �`��
/// </summary>
void Enemy::Render()
{
	for (std::vector<Obj3d>::iterator it = m_ObjEnemy.begin(); it != m_ObjEnemy.end(); it++)
	{
		it->Drow();
	}
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const DirectX::SimpleMath::Vector3& Enemy::GetRot()
{
	return  m_ObjEnemy[0].GetRotation();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	return  m_ObjEnemy[0].GetTranslation();
}

