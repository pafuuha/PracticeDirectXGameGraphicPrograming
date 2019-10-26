#include "stdafx.h"
#include "Scene.h"

#include <mmsystem.h>
#include "Scene.h"

Scene::Scene()
	:m_pMyVB(0), m_pTexture(0), m_pMyBack(0), m_pTextureBack(0)
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


	
	if (FAILED(pD3DDevice->CreateVertexBuffer(4 * sizeof(MY_VERTEX),
		D3DUSAGE_WRITEONLY, //リソースの制御。頂点バッファのみ
		MY_VERTEX_FVF, // 頂点フォーマット。FVF
		D3DPOOL_MANAGED, //どのメモリを使用するか
		&m_pMyVB, NULL))) {
		return E_FAIL;
	}

	MY_VERTEX* v;


	//
	// 表示するものの作成
	//

	m_pMyVB->Lock(0, 0, (void**)&v, 0);

	// 頂点座標の設定
	v[0].p = D3DXVECTOR3(-1.5f, 1.5f, 0.0f);
	v[1].p = D3DXVECTOR3(1.5f, 1.5f, 0.0f);
	v[2].p = D3DXVECTOR3(-1.5f, -1.5f, 0.0f);
	v[3].p = D3DXVECTOR3(1.5f, -1.5f, 0.0f);

	// テクスチャ座標の設定
	v[0].t = D3DXVECTOR2(0.0f, 0.0f);
	v[1].t = D3DXVECTOR2(1.0f, 0.0f);
	v[2].t = D3DXVECTOR2(0.0f, 1.0f);
	v[3].t = D3DXVECTOR2(1.0f, 1.0f);
	// 頂点カラーの設定
	v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_pMyVB->Unlock();
	

	// 4つの頂点からなる頂点バッファを作る
	if (FAILED(pD3DDevice->CreateVertexBuffer(4 * sizeof(MY_VERTEX),
		D3DUSAGE_WRITEONLY,
		MY_VERTEX_FVF,
		D3DPOOL_MANAGED, &m_pMyBack, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	m_pMyBack->Lock(0, 0, (void**)&v, 0);
	v[0].p = D3DXVECTOR3(-10.0f, 10.0f, 12.0f);
	v[1].p = D3DXVECTOR3(10.0f, 10.0f, 12.0f);
	v[2].p = D3DXVECTOR3(-10.0f, -10.0f, 12.0f);
	v[3].p = D3DXVECTOR3(10.0f, -10.0f, 12.0f);
	v[0].t = D3DXVECTOR2(0.0f, 0.0f);
	v[1].t = D3DXVECTOR2(10.0f, 0.0f);
	v[2].t = D3DXVECTOR2(0.0f, 10.0f);
	v[3].t = D3DXVECTOR2(10.0f, 10.0f);
	v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMyBack->Unlock();


	//
	//　テクスチャ　　　　　　　　　　　　　　　画像読み込み
	//
	D3DXCreateTextureFromFileEx(pD3DDevice,
		TEXT("tex.bmp"),
		0, 0, 0, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
		D3DCOLOR_XRGB(0, 0, 0), 		// カラーキー
		NULL,
		NULL,
		&m_pTexture);

	D3DXCreateTextureFromFile(pD3DDevice, TEXT("bg.bmp"), &m_pTextureBack);


	//
	// テクスチャステージ
	//
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// 透過処理
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//
	// レンダリングのパラメーター設定
	//

	// 両面描画モードの指定
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Ｚ比較を行なう
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ディザリングを行なう（高品質描画）
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// ノーライティングモード 	
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 鏡面反射成分の有効化 	
//	pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);


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

	if (m_pMyVB == 0)
		return;


	// 座標変換（カメラの設定など）
	Transform(pD3DDevice);

	// 座標変換（モデルの配置）
	D3DXMatrixIdentity(&m_world);
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);




	// テクスチャをパイプラインにセット
	pD3DDevice->SetTexture(0, m_pTextureBack);

	// 頂点バッファの描画
	pD3DDevice->SetStreamSource(0, m_pMyBack, 0, sizeof(MY_VERTEX));
	pD3DDevice->SetFVF(MY_VERTEX_FVF);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);



	// テクスチャをパイプラインにセット
	pD3DDevice->SetTexture(0, m_pTexture);

	// 頂点バッファの描画
	pD3DDevice->SetStreamSource(0, m_pMyVB, 0, sizeof(MY_VERTEX));
	pD3DDevice->SetFVF(MY_VERTEX_FVF);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void Scene::Destroy()
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}


	if (m_pTextureBack)
	{
		m_pTextureBack->Release();
		m_pTextureBack = NULL;
	}

	if (m_pMyVB)
	{
		m_pMyVB->Release();
		m_pMyVB = NULL;
	}

	if (m_pMyBack)
	{
		m_pMyBack->Release();
		m_pMyBack = NULL;
	}
}

