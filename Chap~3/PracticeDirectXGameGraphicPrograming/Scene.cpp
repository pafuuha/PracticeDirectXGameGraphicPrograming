#include "stdafx.h"
#include "Scene.h"

#include <mmsystem.h>
#include "Scene.h"

Scene::Scene() : m_pMesh(0), m_numMtrl(0L), m_pD3DXMtrlBuffer(0)
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

	// Xファイル
	if (FAILED(D3DXLoadMeshFromX(TEXT("robo0.x"),// Ｘファイルのファイル名
		D3DXMESH_SYSTEMMEM,	// メッシュをシステムメモリに展開する
		pD3DDevice,			// D3Dデバイスオブジェクト
		NULL,
		&m_pD3DXMtrlBuffer,	// 属性バッファの設定
		NULL,
		&m_numMtrl,			// 属性の数
		&m_pMesh))) {		// 格納先のメッシュ
		return E_FAIL;
	}

	/*
	// 球体
	if (FAILED(FAILED(D3DXCreateSphere(pD3DDevice,
		1.0f,
		36,
		36,
		&m_pMesh,
		NULL)))) {
		return E_FAIL;
	}
	*/

	/*
	頂点
	if (FAILED(pD3DDevice->CreateVertexBuffer(3 * sizeof(MY_VERTEX), // MyVerTex3つ分確保
		D3DUSAGE_WRITEONLY, //リソースの制御。頂点バッファのみ
		MY_VERTEXT_FVF, // 頂点フォーマット。FVF
		D3DPOOL_MANAGED, //どのメモリを使用するか
		&m_pMyVB, NULL))) {
		return E_FAIL;
	}

	MY_VERTEX* v;


	//
	// 表示するものの作成
	//

	m_pMyVB->Lock(0, 0, (void**)&v, 0);

	//頂点データ：頂点の座標値の設定
	v[0].p = D3DXVECTOR3(-1.5f, 1.5f, 0.0f);	// 頂点①
	v[1].p = D3DXVECTOR3(1.5f, 1.5f, 0.0f);	// 頂点②
	v[2].p = D3DXVECTOR3(-1.5f, -1.5f, 0.0f);	// 頂点③
	v[3].p = D3DXVECTOR3(1.5f, -1.5f, 0.0f);	// 頂点④

	//頂点データ：頂点の基本的な色の設定（赤色に設定）
	v[0].diffuse = v[1].diffuse = v[2].diffuse = v[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//頂点データ：頂点の鏡面反射成分の設定（1つ目の頂点にだけ白のスペキュラ成分を設定）
	v[0].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	v[1].specular = v[2].specular = v[3].specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_pMyVB->Unlock();
	*/

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
//	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 鏡面反射成分の有効化 	
//	pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	D3DLIGHT9 mLight;
	ZeroMemory(&mLight, sizeof(D3DLIGHT9));

	//ライトのタイプ
	mLight.Type = D3DLIGHT_DIRECTIONAL;

	//拡散光
	mLight.Diffuse.r = 1.0f;
	mLight.Diffuse.g = 1.0f;
	mLight.Diffuse.b = 1.0f;
	/*
	// 環境光
	m_light.Ambient.r = 0.5f;
	m_light.Ambient.g = 0.5f;
	m_light.Ambient.b = 0.5f;

	// 鏡面反射光
	m_light.Specular.r = 1.0f;
	m_light.Specular.g = 1.0f;
	m_light.Specular.b = 1.0f;
	*/


	//ライトの方向
	D3DXVECTOR3 mVecDir;
	mVecDir = D3DXVECTOR3(-5.0f, -5.0f, 10.0f);
	// 正規化
	D3DXVec3Normalize((D3DXVECTOR3*)&mLight.Direction, &mVecDir);

	// カメラを設定
	pD3DDevice->SetLight(0, &mLight);

	//有効か
	pD3DDevice->LightEnable(0, TRUE);

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// D3DMATERIAL9構造体を0でクリア
//	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));

	// モデルの色を設定
	/*
	// 拡散光
	m_material.Diffuse.r = 0.5f;
	m_material.Diffuse.g = 1.0f;
	m_material.Diffuse.b = 0.5f;
	m_material.Diffuse.a = 1.0f;
	// 環境光
	m_material.Ambient.r = 0.2f;
	m_material.Ambient.g = 0.5f;
	m_material.Ambient.b = 0.2f;
	m_material.Ambient.a = 1.0f;

	// 鏡面反射光
	m_material.Specular.r = 0.5f;
	m_material.Specular.g = 1.0f;
	m_material.Specular.b = 0.5f;
	m_material.Specular.a = 1.0f;
	m_material.Power = 4000.0f;
	// 拡散光
	m_material.Emissive.r = 0.5f;
	m_material.Emissive.g = 1.0f;
	m_material.Emissive.b = 0.5f;
	m_material.Emissive.a = 1.0f;
	*/
	//
	// ビューイングの行列を初期化。カメラ
	//
	
	// 単位行列を作成
	// D3DXMATRIXは4*4の行列
	D3DXMatrixIdentity(&m_view);
	// 向き先を設定
	// パラメーター：(出力、視点、注視点、上方向)
	D3DXMatrixLookAtLH(&m_view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	
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

	Transform(pD3DDevice);
	// 座標変換（モデルの配置）
	D3DXMATRIX	mScale;
	D3DXMATRIX	mRot;
	D3DXMATRIX	mMove;
	D3DXMatrixIdentity(&m_world);

	// 拡大縮小
	D3DXMatrixScaling(&mScale, 2.0f, 0.5f, 1.0f);

	// 回転
	D3DXMatrixRotationY(&mRot, D3DXToRadian(45.0f));

	// 移動
	D3DXMatrixTranslation(&mMove, 4.0f, 3.0f, 0.0f);

	// 合成１　拡大縮小と回転
	D3DXMATRIX	mMult;
	D3DXMatrixMultiply(&mMult, &mScale, &mRot);

	// 合成２　合成１と移動　
	D3DXMatrixMultiply(&m_world, &mMult, &mMove);

	// マトリックスをレンダリングパイプラインに設定
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);


	D3DMATERIAL9	mMaterial;
	// マテリアル情報を取り出す
	D3DXMATERIAL*	mD3DXMaterials = (D3DXMATERIAL*)m_pD3DXMtrlBuffer->GetBufferPointer();
	for (DWORD i = 0; i < m_numMtrl; i++) {
		// マテリアルのコピー
		mMaterial = mD3DXMaterials[i].MatD3D;
		// マテリアルのセット
		pD3DDevice->SetMaterial(&mMaterial);
		// 分割されたメッシュの描画
		m_pMesh->DrawSubset(i);
	}

	// マテリアルをレンダリングパイプラインに設定
//	pD3DDevice->SetMaterial(&m_material);

	// モデルの描画
//	m_pMesh->DrawSubset(0);

	/*
	pD3DDevice->SetStreamSource(0, m_pMyVB, 0, sizeof(MY_VERTEX));

	pD3DDevice->SetFVF(MY_VERTEXT_FVF);

	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	*/
}

void Scene::Destroy()
{
	if (m_pD3DXMtrlBuffer)
	{
		m_pD3DXMtrlBuffer->Release();
		m_pD3DXMtrlBuffer = NULL;
	}


	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

