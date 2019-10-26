#include "stdafx.h"
#include "Scene.h"

#include <windows.h>
#include <mmsystem.h>

#include "Scene.h"
#include "AnimationLinear.h"

Scene::Scene()
	: m_pMesh(0), m_numMesh(0L), m_pAnim(0)
{
}


Scene::~Scene()
{
	Destroy();
}

HRESULT Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	Destroy();

	if (pD3DDevice == 0) {
		return E_FAIL;
	}


	


	//
	// 表示するものの作成
	//


	//--------------------------------------
	// Ｘファイルの読み込み
	//--------------------------------------
	if (FAILED(D3DXLoadMeshFromX(TEXT("sphere.x"),
		D3DXMESH_SYSTEMMEM,
		pD3DDevice,
		NULL,
		NULL,
		NULL,
		&m_numMesh,
		&m_pMesh))) {
		return E_FAIL;
	}

	//
	// レンダリングのパラメーター設定
	//

	// 両面描画モードの指定
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Ｚ比較を行なう
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ディザリングを行なう（高品質描画）
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);

	///
	/// ライト
	///
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	// ライトの色設定
	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;

	// ライトの方向の設定
	D3DXVECTOR3		mVecDir;
	mVecDir = D3DXVECTOR3(-0.5f, -0.5f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &mVecDir);

	// 設定
	pD3DDevice->SetLight(0, &m_light);
	pD3DDevice->LightEnable(0, TRUE);


	//
	// マテリアル
	//

	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));

	// モデルの色を設定
	m_material.Diffuse.r = 1.0f;
	m_material.Diffuse.g = 0.0f;
	m_material.Diffuse.b = 0.0f;
	m_material.Diffuse.a = 1.0f;


	// 
	// アニメーション
	//
	m_pAnim = new AnimationLinear;

	// キーフレームを作成
	m_pAnim->CreateKey(3);

	// キーフレームの時間（割合で指定）と値の設定	
	m_pAnim->SetKeyValue(0, 0.0f, 0.0f);
	m_pAnim->SetKeyValue(1, 0.5f, 1.0f);
	m_pAnim->SetKeyValue(2, 1.0f, 0.0f);

	// アニメーションの継続時間	
	m_pAnim->SetDuration(5000);

	// アニメーションをループ設定	
	m_pAnim->SetLoopFlag(TRUE); // TRUE＝ループさせる

	// アニメーションのスタート時間の設定	
	DWORD time = timeGetTime(); // 現在時刻をミリ単位で取得
	m_pAnim->SetStarTime(time);




	//
	// ビューイングの行列を初期化。カメラ
	//
	// 単位行列を作成
	// D3DXMATRIXは4*4の行列
	D3DXMatrixIdentity(&m_view);
	// 向き先を設定
	// パラメーター：(出力、視点、注視点、上方向)
	D3DXMatrixLookAtLH(&m_view, &D3DXVECTOR3(0.0f, 0.0f, -6.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	
	return S_OK;
}


// 座標変換を行う
void Scene::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == 0)
		return;

	// ビューポートの設定
	// ビューポート最終的に見る画面の範囲
	/*
	DirecXの場合 初期値
		D3DVIEWPORT9 vp;
		vp.X = 0;
		vp.Y = 0;
		vp.Width = RenderTarget.Width;
		vp.Height = RenderTarget.Height;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;
		// vp.X, vp.Y が描写する左上の座標
		// vp.Width, vp.Height が描写幅
		// vp.MinZ, vp.MaxZ が深度値の変換です
	*/
	D3DVIEWPORT9 vp;
	if (FAILED(pD3DDevice->GetViewport(&vp)))
		return;

	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	//
	// 射影行列
	// 
	// 単位行列
	D3DXMatrixIdentity(&m_proj);
	// 作成
	//パラメーター: (出力、FOV（カメラの画角）、アスペクト、どこから先を写すか（Nearプレーン）、どこまでを写すか（Farプレーン）
	D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 1.0f, 1000.0f);

	//
	//  デバイスにそれぞれの行列を設定
	// 
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

}

// 描画
void Scene::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{

	if (pD3DDevice == 0)
		return;

	if (m_pMesh == 0)
		return;


	// 座標変換（カメラの設定など）
	Transform(pD3DDevice);

	// 座標変換（モデルの配置）
	D3DXMatrixIdentity(&m_world);
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);


	//
	// マテリアルのアニメ
	//
	float value;
	float red = 0.0f;
	bool isEnd;

	DWORD time = timeGetTime();

	if (m_pAnim->GetValue(time, &value, &isEnd)) {
		red = value;
	}


	// マテリアル
	m_material.Diffuse.r = red;
	m_material.Diffuse.g = 0.0f;
	m_material.Diffuse.b = 0.0f;
	pD3DDevice->SetMaterial(&m_material);

	// モデル

	for (DWORD i = 0; i < m_numMesh; i++) {
		m_pMesh->DrawSubset(i);
	}
}

void Scene::Destroy()
{

	if (m_pAnim != 0) {
		delete m_pAnim;
	}

	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

}

