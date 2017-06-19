#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̒�`
// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// �f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// �J����
Camera*											Obj3d::m_Camera;
// �ėp�X�e�[�g�ݒ�
std::unique_ptr<DirectX::CommonStates>			Obj3d::m_states;
// �G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory>			Obj3d::m_factory;


void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_Camera = camera;

	// �ėp�X�e�[�g�ݒ�𐶐�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	// �G�t�F�N�g�t�@�N�g������
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// �e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{
	// �����o�ϐ���������
	m_scale = Vector3(1, 1, 1);
	m_ObjParent = nullptr;

	// �f�t�H���g�ł̓I�C���[�p���g�p
	m_UseQuaternion = false;
}

void Obj3d::LoadModel(const wchar_t * FileName)
{
	// CMO���烂�f����ǂݍ���
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), FileName, *m_factory);
}

void Obj3d::Update()
{
	// ���[���h�s��̌v�Z
	Matrix scalemat = Matrix::CreateScale(m_scale);
	Matrix rotmat;
	if (m_UseQuaternion)
	{// �N�H�[�^�j�I���ŉ�]���v�Z
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{// �I�C���[�p�ŉ�]���v�Z
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}
	Matrix transmat = Matrix::CreateTranslation(m_translation);
	// ���[���h�s��̍���
	m_world = scalemat * rotmat * transmat;
	// �e�̍s����|����
	if (m_ObjParent)
	{
		m_world *= m_ObjParent->GetWorld();
	}
}


void Obj3d::Drow()
{
	// �`��
	if(m_model)
	{
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_Camera->GetView(), m_Camera->GetProj());
	}
}