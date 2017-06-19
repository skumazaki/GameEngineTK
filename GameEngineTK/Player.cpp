#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Player::Player()
{
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="key"></param>
Player::Player(Keyboard* key)
{
	m_keyboard = key;

	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}

/// <summary>
/// ������
/// </summary>
void Player::Initialize()
{
	// ���@�p�[�c�̓ǂݍ���
	m_ObjPlayer.resize(PARTS_NUM);
	m_ObjPlayer[PARTS_FOOT].LoadModel(L"Resources/foot.cmo");
	m_ObjPlayer[PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_ObjPlayer[PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	m_ObjPlayer[PARTS_MARU].LoadModel(L"Resources/maru.cmo");
	m_ObjPlayer[PARTS_MARU2].LoadModel(L"Resources/maru.cmo");
	m_ObjPlayer[PARTS_ARM].LoadModel(L"Resources/arm.cmo");
	m_ObjPlayer[PARTS_GUN].LoadModel(L"Resources/gun.cmo");

	// �e�q�֌W�̍\�z(�q�p�[�c�ɐe��ݒ�)
	m_ObjPlayer[PARTS_BODY].SetObjParent(&m_ObjPlayer[PARTS_FOOT]);
	m_ObjPlayer[PARTS_HEAD].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_MARU].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_MARU2].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_ARM].SetObjParent(&m_ObjPlayer[PARTS_BODY]);
	m_ObjPlayer[PARTS_GUN].SetObjParent(&m_ObjPlayer[PARTS_ARM]);

	// �q�p�[�c�̐e����̃I�t�Z�b�g�i���W�̂���j���Z�b�g
	m_ObjPlayer[PARTS_BODY].SetTranslation(Vector3(0, 0.5f, 0));
	m_ObjPlayer[PARTS_HEAD].SetTranslation(Vector3(0, 0.4f, 0));
	m_ObjPlayer[PARTS_ARM].SetTranslation(Vector3(0.7f, 0.3f, -0.2f));
	m_ObjPlayer[PARTS_GUN].SetTranslation(Vector3(0, 0, 0));

	m_ObjPlayer[PARTS_ARM].SetRotation(Vector3(0, XMConvertToRadians(90), 0));
	m_ObjPlayer[PARTS_GUN].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));

	m_FireFlag = false;
	
	{// �e�ۗp�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBullet.Initialize();
		// �e�p�[�c��ݒ�
		m_CollisionNodeBullet.SetParent(&m_ObjPlayer[PARTS_HEAD]);
		m_CollisionNodeBullet.SetTrans(Vector3(0, 0.25f, 0));
		m_CollisionNodeBullet.SetLocalRadius(0.3f);
	}
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{
	// �p�[�c�M�~�b�N
	{
		m_cycle += 0.04f;
		float scale = 1.0f + sinf(m_cycle);
		// �w�b�h�p�[�c�̏k���g��
		m_ObjPlayer[PARTS_HEAD].SetScale(Vector3(scale, scale, scale));
		if (flag)
		{
			static float angle1 = 0.0f;
			angle1 += 0.01f;

			m_ObjPlayer[PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, cosf(scale) * 2, sinf(angle1) * 2));
			m_ObjPlayer[PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, cosf(angle1) * -2, sinf(angle1) * -2));
		}
		else
		{
			static float angle1 = 0.0f;
			angle1 += 1.0f;
			m_ObjPlayer[PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, tanf(scale) * 2, sinf(angle1) * 2));
			m_ObjPlayer[PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, tanf(angle1) * -2, sinf(angle1) * -2));
		}

	}

	// �L�[�{�[�h�̏�Ԏ擾
	Keyboard::State g_key = m_keyboard->GetState();
	m_KeyboardTracker.Update(g_key);

	// C�L�[�������ꂽ��
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keyboard::Z))
	{
		// �J�����̃t���O�̐؂�ւ�

		flag = !flag;
	}

	// ������
	if (g_key.A)
	{
		// ���@����]
		//head_rot += 0.05f;
		float angle = m_ObjPlayer[0].GetRotation().y;
		m_ObjPlayer[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	// �E����
	if (g_key.D)
	{
		// ���@����]
		//head_rot -= 0.05f;
		float angle = m_ObjPlayer[0].GetRotation().y;
		m_ObjPlayer[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	// �O�i����
	if (g_key.W)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.1f);
		// �ړ��x�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_ObjPlayer[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		// ���@�̍��W���ړ�
		Vector3 pos = m_ObjPlayer[0].GetTranslation();
		//head_pos += moveV;
		m_ObjPlayer[0].SetTranslation(pos + moveV);
	}

	// ��ޏ���
	if (g_key.S)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, 0.1f);
		// �ړ��x�N�g�������@�̊p�x����]������
		float angle = m_ObjPlayer[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_ObjPlayer[0].GetTranslation();
		m_ObjPlayer[0].SetTranslation(pos + moveV);
	}

	// �e�ۃp�[�c�̑O�i����
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keyboard::Space))
	{
		if (m_FireFlag)
		{
			m_FireFlag = false;
			//FireBullet();
			//// �e�ۃp�[�c�̍��W���ړ�
			//Vector3 pos = m_ObjPlayer[PARTS_HEAD].GetTranslation();
			//m_ObjPlayer[PARTS_HEAD].SetTranslation(pos + m_BulletVel);
		}
		else
		{
			m_FireFlag = true;
			//ResetBullet();
		}
	}
	if (m_FireFlag)
	{
		FireBullet();
		// �e�ۃp�[�c�̍��W���ړ�
		Vector3 pos = m_ObjPlayer[PARTS_HEAD].GetTranslation();
		m_ObjPlayer[PARTS_HEAD].SetTranslation(pos + m_BulletVel);

	}
	else
	{
		ResetBullet();

	}
	


	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Update();
	}

	m_CollisionNodeBullet.Update();
}

/// <summary>
/// �`��
/// </summary>
void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Drow();
	}

	m_CollisionNodeBullet.Draw();
}

/// <summary>
/// �e�̔���
/// </summary>
void Player::FireBullet()
{
	// ���˂���p�[�c�̃��[���h�s����擾
	Matrix worldm = m_ObjPlayer[PARTS_HEAD].GetWorld();

	// ���o�����������܂��Ă����ϐ�
	Vector3 scale;			// ���[���h�X�P�[�����O
	Quaternion rotation;	// ���[���h��]
	Vector3 translation;	// ���[���h���W

	// ���[���h�s�񂩂�e�v�f�𒊏o 
	worldm.Decompose(scale, rotation, translation);

	// �e�p�[�c���番���A�Ɨ�������
	m_ObjPlayer[PARTS_HEAD].SetObjParent(nullptr);
	m_ObjPlayer[PARTS_HEAD].SetScale(scale);
	m_ObjPlayer[PARTS_HEAD].SetRotationQ(rotation);
	m_ObjPlayer[PARTS_HEAD].SetTranslation(translation);

	// �e�ۃp�[�c�̑��x��ݒ�
	m_BulletVel = Vector3(0, 0, -0.1f);
	// �p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	m_FireFlag = true;
}

/// <summary>
/// �e�̃��Z�b�g
/// </summary>
void Player::ResetBullet()
{
	m_ObjPlayer[PARTS_HEAD].SetObjParent(&m_ObjPlayer[PARTS_BODY]);

	m_ObjPlayer[PARTS_HEAD].SetScale(Vector3(1, 1, 1));
	m_ObjPlayer[PARTS_HEAD].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[PARTS_HEAD].SetTranslation(Vector3(0, 0.4f, 0));

	m_FireFlag = false;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const DirectX::SimpleMath::Vector3& Player::GetRot()
{
	return  m_ObjPlayer[0].GetRotation();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	return  m_ObjPlayer[0].GetTranslation();
}
