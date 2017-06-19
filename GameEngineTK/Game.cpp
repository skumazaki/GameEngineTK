//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <ctime>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;


Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	srand(static_cast<unsigned int>(time(nullptr)));

    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	// �������͂����ɏ���
	// �L�[�{�[�h�̐���
	m_keyboard = std::make_unique<Keyboard>();
	// �J�����̐���
	m_Camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	// �J�����ɃL�[�{�[�h���Z�b�g 
	m_Camera->SetKeyboard(m_keyboard.get());

	// 3D�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_Camera.get());

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// �ėp�X�e�[�g�ݒ�𐶐�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	// �f�o�b�O�J�����𐶐�
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// �G�t�F�N�g�t�@�N�g������
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// �e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"Resources");
	// �V�����f���̐���
	m_objSkydome.LoadModel(L"Resources/skydome.cmo");
	// �n�ʃ��f���̐���
	m_modelGround1 = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ground200m.cmo", *m_factory);

	// �����f���̐���
	//m_modelBall = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/que.cmo", *m_factory);
	//m_modelBall1 = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/que.cmo", *m_factory);
	// �����f���̐���
	//m_modelTeapot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/teapot.cmo", *m_factory);
	// �����f���̐���
	//m_modelHead = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/head.cmo", *m_factory);
	//for (int i = 0; i < 20; i++)
	//{
	//	// �X�P�[�����O
	//	Matrix scalemat = Matrix::CreateScale(1.0f);
	//	// ���s�ړ�
	//	l_x = rand() % 360;
	//	l_z = rand() % 100;
	//	m_x = cosf(l_x) * l_z;
	//	m_z = sinf(l_x) * l_z;
	//	Matrix transmat = Matrix::CreateTranslation(m_x, 0, m_z);
	//	// ���[���h�s��̍���(SRT)
	//	// m_worldBall = scalemat * rotmat * transmat;
	//	m_worldTeapot[i] = scalemat * transmat ;
	//}

	//angle = 0.0f;
	//head_rot = 0.0f;

	m_player = std::make_unique<Player>(m_keyboard.get());

	m_Camera->SetPlayer(m_player.get());

	int enemyNum = rand() % 10 + 1;
	
	m_enemy.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		m_enemy[i] = std::make_unique<Enemy>(m_keyboard.get());
	}

	//// ���@�p�[�c�̓ǂݍ���
	//m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	//m_ObjPlayer[PLAYER_PARTS_FOOT].LoadModel(L"Resources/foot.cmo");
	//m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resources/body.cmo");
	//m_ObjPlayer[PLAYER_PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	//m_ObjPlayer[PLAYER_PARTS_MARU].LoadModel(L"Resources/maru.cmo");
	//m_ObjPlayer[PLAYER_PARTS_MARU2].LoadModel(L"Resources/maru.cmo");
	//m_ObjPlayer[PLAYER_PARTS_ARM].LoadModel(L"Resources/arm.cmo");
	//m_ObjPlayer[PLAYER_PARTS_GUN].LoadModel(L"Resources/gun.cmo");
	//
	//// �e�q�֌W�̍\�z(�q�p�[�c�ɐe��ݒ�)
	//m_ObjPlayer[PLAYER_PARTS_BODY].SetObjParent(&m_ObjPlayer[PLAYER_PARTS_FOOT]);
	//m_ObjPlayer[PLAYER_PARTS_HEAD].SetObjParent(&m_ObjPlayer[PLAYER_PARTS_BODY]);
	//m_ObjPlayer[PLAYER_PARTS_MARU].SetObjParent(&m_ObjPlayer[PLAYER_PARTS_BODY]);
	//m_ObjPlayer[PLAYER_PARTS_MARU2].SetObjParent(&m_ObjPlayer[PLAYER_PARTS_BODY]);
	//m_ObjPlayer[PLAYER_PARTS_ARM].SetObjParent(&m_ObjPlayer[PLAYER_PARTS_BODY]);
	//m_ObjPlayer[PLAYER_PARTS_GUN].SetObjParent(&m_ObjPlayer[PLAYER_PARTS_ARM]);
	//
	//// �q�p�[�c�̐e����̃I�t�Z�b�g�i���W�̂���j���Z�b�g
	//m_ObjPlayer[PLAYER_PARTS_BODY].SetTranslation(Vector3(0, 0.5f, 0));
	//m_ObjPlayer[PLAYER_PARTS_HEAD].SetTranslation(Vector3(0, 0.4f, 0));
	//m_ObjPlayer[PLAYER_PARTS_ARM].SetTranslation(Vector3(0.7f, 0.3f, -0.2f));
	//m_ObjPlayer[PLAYER_PARTS_GUN].SetTranslation(Vector3(0, 0, 0));
	//
	//m_ObjPlayer[PLAYER_PARTS_ARM].SetRotation(Vector3(0, XMConvertToRadians(90), 0));
	//m_ObjPlayer[PLAYER_PARTS_GUN].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));
	//
	//m_cycle = 0.0f;

	flag = false;

	//head_pos = Vector3(0, 0, 30);
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

	// ���t���[������������
	m_debugCamera->Update();

	//for (int i = 0; i < 40000; i++)
	//{
	//	// �X�P�[�����O
	//	Matrix scalemat = Matrix::CreateScale(1.0f);
	//	Matrix transmat = Matrix::CreateTranslation(i / 200 - 100, 0, i % 200 - 100);
	//	m_worldGround2[i] = scalemat * transmat;
	//}
	// ���[���h�s����v�Z
	//for (int i = 0; i < 10; i++)
	//{
	//	// �X�P�[�����O
	//	Matrix scalemat = Matrix::CreateScale(1.0f);
	//	// ���[��								XMConvertToRadians()
	//	Matrix rotmatz = Matrix::CreateRotationZ(0.0f);
	//	// �s�b�`(�p)
	//	Matrix rotmatx = Matrix::CreateRotationX(0.0f);
	//	// ���[(���ʊp)
	//	Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(36 * i));
	//	// ��]1
	//	static float angle1 = 0.0f;
	//	angle1 += 0.002f;
	//	Matrix rotate1 = Matrix::CreateRotationY(angle1);
	//	// ��]2
	//	static float angle2 = 0.0f;
	//	angle2 -= 0.002f;
	//	Matrix rotate2 = Matrix::CreateRotationY(angle2);
	//	// ��]�s��̍���
	//	Matrix rotmat1 = rotmatz * rotmatx * rotate1 * rotmaty;
	//	Matrix rotmat2 = rotmatz * rotmatx * rotate2 * rotmaty;
	//	// ���s�ړ�
	//	Matrix transmat = Matrix::CreateTranslation(20, 0, 0);
	//	Matrix transmat1 = Matrix::CreateTranslation(40, 0, 0);
	//	// ���[���h�s��̍���(SRT)
	//	// m_worldBall = scalemat * rotmat * transmat;
	//	m_worldBall[i] = scalemat * transmat * rotmat1;
	//	m_worldBall1[i] = scalemat * transmat1 * rotmat2;
	//}
	//
	//for (int i = 0; i < 20; i++)
	//{
	//	float val = sinf(l_x);
	//	
	//	// ��]
	//	Matrix scalemat = Matrix::CreateScale(val);
	//	Matrix rotate1 = Matrix::CreateRotationY(XMConvertToRadians(4));
	//	m_worldTeapot[i] = rotate1 * m_worldTeapot[i];
	//}

	//// �p�[�c�M�~�b�N
	//{
	//	m_cycle += 0.04f;
	//	float scale = 1.0f + sinf(m_cycle);
	//	// �w�b�h�p�[�c�̏k���g��
	//	m_ObjPlayer[PLAYER_PARTS_HEAD].SetScale(Vector3(scale, scale, scale));
	//	if (flag)
	//	{
	//		static float angle1 = 0.0f;
	//		angle1 += 0.01f;
	//
	//		m_ObjPlayer[PLAYER_PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, cosf(scale) * 2, sinf(angle1) * 2));
	//		m_ObjPlayer[PLAYER_PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, cosf(angle1) * -2, sinf(angle1) * -2));
	//	}
	//	else
	//	{
	//		static float angle1 = 0.0f;
	//		angle1 += 1.0f;
	//		m_ObjPlayer[PLAYER_PARTS_MARU].SetTranslation(Vector3(cosf(angle1) * 2, tanf(scale) * 2, sinf(angle1) * 2));
	//		m_ObjPlayer[PLAYER_PARTS_MARU2].SetTranslation(Vector3(cosf(angle1) * 2, tanf(angle1) * -2, sinf(angle1) * -2));
	//	}
	//
	//	// �e�̉�]
	//	//float angle = m_ObjPlayer[PLAYER_PARTS_GUN].GetRotation().y;
	//	//m_ObjPlayer[PLAYER_PARTS_GUN].SetRotation(Vector3(0, angle + 0.03f, 0));
	//	//Vector3 pos = m_ObjPlayer[PLAYER_PARTS_GUN].GetTranslation();
	//	//m_ObjPlayer[PLAYER_PARTS_GUN].SetTranslation(pos + Vector3(0, 0.01f, 0));
	//
	//}

	// �L�[�{�[�h�̏�Ԏ擾
	Keyboard::State g_key = m_keyboard->GetState();
	m_keyboardTracker.Update(g_key);

	m_player->Update();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	{
		Enemy* enemy = it->get();

		enemy->Update();
	}

	//// C�L�[�������ꂽ��
	//if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::Z))
	//{
	//	// �J�����̃t���O�̐؂�ւ�
	//
	//	flag = !flag;
	//}

	//// ������
	//if (g_key.A)
	//{
	//	// ���@����]
	//	//head_rot += 0.05f;
	//	float angle = m_ObjPlayer[0].GetRotation().y;
	//	m_ObjPlayer[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	//}
	//
	//// �E����
	//if (g_key.D)
	//{
	//	// ���@����]
	//	//head_rot -= 0.05f;
	//	float angle = m_ObjPlayer[0].GetRotation().y;
	//	m_ObjPlayer[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	//}
	//
	//// �O�i����
	//if (g_key.W)
	//{
	//	// �ړ��ʂ̃x�N�g��
	//	Vector3 moveV(0, 0, -0.1f);
	//	// �ړ��x�N�g�������@�̊p�x����]������
	//	//moveV = Vector3::TransformNormal(moveV, head_world);
	//	float angle = m_ObjPlayer[0].GetRotation().y;
	//	Matrix rotmat = Matrix::CreateRotationY(angle);
	//	moveV = Vector3::TransformNormal(moveV, rotmat);
	//
	//	// ���@�̍��W���ړ�
	//	Vector3 pos = m_ObjPlayer[0].GetTranslation();
	//	//head_pos += moveV;
	//	m_ObjPlayer[0].SetTranslation(pos + moveV);
	//}
	//
	//// ��ޏ���
	//if (g_key.S)
	//{
	//	// �ړ��ʂ̃x�N�g��
	//	Vector3 moveV(0, 0, 0.1f);
	//	// �ړ��x�N�g�������@�̊p�x����]������
	//	float angle = m_ObjPlayer[0].GetRotation().y;
	//	Matrix rotmat = Matrix::CreateRotationY(angle);
	//	moveV = Vector3::TransformNormal(moveV, rotmat);
	//	// ���@�̍��W���ړ�
	//	Vector3 pos = m_ObjPlayer[0].GetTranslation();
	//	m_ObjPlayer[0].SetTranslation(pos + moveV);
	//}
	//
	//{// ���@�̃��[���h�s����v�Z
	//	// �p�[�c�P�̌v�Z
	//	Matrix rotmat = Matrix::CreateRotationY(head_rot);
	//	Matrix transmat = Matrix::CreateTranslation(head_pos);
	//	// ���[���h�s��̍���
	//	head_world = rotmat * transmat;
	//	// �p�[�c2�̌v�Z
	//	Matrix rotmat2 = Matrix::CreateRotationZ(XM_PIDIV2) * Matrix::CreateRotationY(0);
	//	Matrix transmat2 = Matrix::CreateTranslation(Vector3(0, 0.5f, 0));
	//	// ���[���h�s��̍���(�q���̍s�� * ��l�̍s��)
	//	head_world2 = rotmat2 * transmat2 * head_world;
	//}
	//
	//{// ���@�ɒǏ]����J����
	//	Vector3 pos = m_ObjPlayer[0].GetTranslation();
	//	float angle = m_ObjPlayer[0].GetRotation().y;
	//	m_Camera->SetTargetPos(pos);
	//	m_Camera->SetTargetAngle(angle);
	//
	//	m_Camera->Update();
	//	m_view = m_Camera->GetView();
	//	m_proj = m_Camera->GetProj();
	//
	//	//const Matrix m_view = m_Camera->GetView();
	//	//const Matrix m_proj = m_Camera->GetProj();
	//}

	//{// ���@�ɒǏ]����J����
	//	Vector3 pos = m_player->GetTrans();
	//	float angle = m_player->GetRot().y;
	//	m_Camera->SetTargetPos(pos);
	//	m_Camera->SetTargetAngle(angle);
	//
		m_Camera->Update();
		m_view = m_Camera->GetView();
		m_proj = m_Camera->GetProj();
	//
	//	//const Matrix m_view = m_Camera->GetView();
	//	//const Matrix m_proj = m_Camera->GetProj();
	//}


	m_objSkydome.Update();

	//for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	//{
	//	it->Update();
	//}
}

// Draws the scene.
void Game::Render()
{
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3
	};
	
	VertexPositionNormal vertice[] =
	{
		 {Vector3(-1,+1,0),Vector3(0,0,+1)},
		{ Vector3(-1,-1,0),Vector3(0,0,+1) },
		{ Vector3(+1,+1,0),Vector3(0,0,+1) },
		{ Vector3(+1,-1,0),Vector3(0,0,+1) },
	};
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	// �`�揈��������
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_d3dContext->RSSetState(m_states->Wireframe());

	// �r���[�s��𐶐�
	//m_view = Matrix::CreateLookAt(
	//	Vector3(0, 0, 2.f),			// �J�������_
	//	Vector3(0, 0, 0),			// �J�����Q�Ɠ_
	//	Vector3(0, 1, 0)			// ��ʏ�����x�N�g��
	//);
	// �f�o�b�O�J��������r���[�s����擾
	//m_view = m_debugCamera->GetCameraMatrix();
	//// �J�����̈ʒu(���_���W�j
	//Vector3 eyepos(0, 0, 5);
	//// �J�����̌��Ă����(�����_/�Q�Ɠ_�j
	//Vector3 refpos(0, 0, 0);
	//// �J�����̏�����x�N�g��
	//static float angle = 0.0f;
	//angle += 0.1;
	//Vector3 upvec(cosf(angle), sinf(angle), 0);
	//upvec.Normalize();
	////�r���[�s��̐���
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);
	//// �v���W�F�N�V�����s��𐶐�
	//// ������������p
	//float fovY = XMConvertToRadians(60.0f);
	//// �A�X�y�N�g��i�c���̔䗦�j
	//float aspect = (float)m_outputWidth / m_outputHeight;
	//// �j�A�X���b�v(��O�̕\�����E����)
	//float nearclip = 0.1f;
	//// �t�@�[�N���b�v(���̕\�����E����)
	//float farclip = 10.0f;
	//m_proj = Matrix::CreatePerspectiveFieldOfView(
	//	fovY,									// ����p�i�㉺�����j
	//	aspect,	// �A�X�y�N�g��
	//	nearclip,											// �j�A�N���b�v	�@ ���̐��l���猩����
	//	farclip											// �t�@�[�N���b�v�@���̐��l�܂ł�������
	//);
	//m_proj = Matrix::CreatePerspectiveFieldOfView(
	//	XM_PI / 4.f,									// ����p�i�㉺�����j
	//	float(m_outputWidth) / float(m_outputHeight),	// �A�X�y�N�g��
	//	0.1f,											// �j�A�N���b�v	�@ ���̐��l���猩����
	//	500.f											// �t�@�[�N���b�v�@���̐��l�܂ł�������
	//);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	// �V�����f���̕`��
	m_objSkydome.Drow();
	// �n�ʃ��f���̕`��
	//for (int i = 0; i < 40000; i++)
	//{
	//		m_modelGround2->Draw(m_d3dContext.Get(), *m_states, m_worldGround2[i], m_view, m_proj);
	//}

	m_modelGround1->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);

	//for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	//{
	//	it->Drow();
	//}

	m_player->Render();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	{
		Enemy* enemy = it->get();

		enemy->Render();
	}

	//// �p�[�c�P�̕`��
	//m_modelHead->Draw(m_d3dContext.Get(), *m_states, head_world, m_view, m_proj);
	//// �p�[�c�Q�̕`��
	//m_modelHead->Draw(m_d3dContext.Get(), *m_states, head_world2, m_view, m_proj);
	// �����f���̕`��
	//for (int i = 0; i < 10; i++)
	//{
	//	m_modelBall->Draw(m_d3dContext.Get(), *m_states, m_worldBall[i], m_view, m_proj);
	//	m_modelBall1->Draw(m_d3dContext.Get(), *m_states, m_worldBall1[i], m_view, m_proj);
	//}
	// �e�B�[�|�b�g�̕`��
	//for (int i = 0; i < 20; i++)
	//{
	//	m_modelTeapot->Draw(m_d3dContext.Get(), *m_states, m_worldTeapot[i], m_view, m_proj);
	//}

	m_batch->Begin();					// ��������`

	m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertice, 4);

	//m_batch->DrawLine(
	//	VertexPositionColor(
	//		SimpleMath::Vector3(0, 0, 0),
	//		SimpleMath::Color(1, 1, 1)),
	//	VertexPositionColor(
	//		SimpleMath::Vector3(800, 600, 0),
	//		SimpleMath::Color(0, 0, 1))
	//	);
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Blue);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Gold);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Green);
	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Blue);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Gold);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Green);
	//m_batch->DrawTriangle(v1, v2, v3);

	m_batch->End();						// �`�����܂ł��`��

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}