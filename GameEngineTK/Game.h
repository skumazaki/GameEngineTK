//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include <vector>
#include <Keyboard.h>
#include "DebugCamera.h"
#include "FollowCamera.h"
#include "Obj3d.h"
#include "Player.h"
#include "Enemy.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;



private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 汎用ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugCamera;

	// エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	// 天球モデルデータ
	Obj3d m_objSkydome;
	// 地面モデル
	std::unique_ptr<DirectX::Model> m_modelGround1;
	//std::unique_ptr<DirectX::Model> m_modelGround2;
	// 球モデル
	std::unique_ptr<DirectX::Model> m_modelBall;
	std::unique_ptr<DirectX::Model> m_modelBall1;
	//// ティーポットモデル
	//std::unique_ptr<DirectX::Model> m_modelTeapot;
	// 球のワールド行列
	DirectX::SimpleMath::Matrix m_worldBall[10];
	DirectX::SimpleMath::Matrix m_worldBall1[10];
	// 地面のワールド行列
	DirectX::SimpleMath::Matrix m_worldGround2[40000];
	//// ティーポットのワールド行列
	//DirectX::SimpleMath::Matrix m_worldTeapot[20];
	//// 頭モデル
	//std::unique_ptr<DirectX::Model> m_modelHead;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//// 自機の座標
	//DirectX::SimpleMath::Vector3 head_pos;
	//// 自機の回転
	//float head_rot;
	//// 自機のワールド行列
	//DirectX::SimpleMath::Matrix head_world;
	//// 自機のワールド行列2
	//DirectX::SimpleMath::Matrix head_world2;

	//// 自機パーツ
	//std::vector<Obj3d> m_ObjPlayer;
	std::unique_ptr<Player> m_player;

	std::vector<std::unique_ptr<Enemy>> m_enemy;

	//// サイン用の角度
	//float m_cycle;

	// カメラ
	std::unique_ptr<FollowCamera> m_Camera;

	// 自機パーツ
	
	bool flag;

	//// 球の角度
	//float angle;
	//float m_x;
	//float m_z;
	//float l_x;
	//float l_z;
};