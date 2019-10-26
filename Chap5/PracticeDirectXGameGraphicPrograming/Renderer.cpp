#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"




Renderer::Renderer() : m_pD3D(0),m_pD3DDevice(0)
{
}


Renderer::~Renderer()
{
	Finalize();
}

/*
レンダラーの初期化
*/
HRESULT Renderer::Init(HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight)
{
	// ディスプレイモード
	D3DDISPLAYMODE d3ddm;

	//Direct3d9の作成
	if ((m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0){
		return	E_FAIL;
	}

	//現在のディスプレイモードをd3ddmへ取得しつつ、エラーチェック
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	// D3DPRESENT_PARAMETERSサイズを動的に確保。初期値は0
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));

	//バックバッファの数
	m_D3DPP.BackBufferCount = 1;
	if (isFullScreen)
	{
		m_D3DPP.Windowed = false;
		m_D3DPP.BackBufferWidth = clientWidth;
		m_D3DPP.BackBufferHeight = clientHeight;
	}
	else {
		m_D3DPP.Windowed = true;
	}

	m_D3DPP.BackBufferFormat = d3ddm.Format;
	//フロントバッファとバックバッファの切り替え方法 をディスプレイドライバが自動判断モードへ
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.EnableAutoDepthStencil = TRUE;
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D16;

	/*
	ディスプレイアダプタを表すためのデバイスを作成

	*/
	// 描画と頂点処理をハードウェアで
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&m_D3DPP, &m_pD3DDevice))) {
		//　描画はハードウェア。頂点はCPU
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&m_D3DPP, &m_pD3DDevice))) {
			// 描画と頂点をCPU
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&m_D3DPP, &m_pD3DDevice))) {
				return E_FAIL;
			}
		}
	}


	return S_OK;
}

// デバイスの取得
LPDIRECT3DDEVICE9 Renderer::GetDevice()
{
	return m_pD3DDevice;
}

void Renderer::RenderScene(Scene * pScene)
{
	if (pScene == 0)
		return;

	// ビューポートと深度バッファのクリアとステンシルバッファの削除
	if (FAILED(m_pD3DDevice->Clear(0, NULL,
		D3DCLEAR_TARGET |
		D3DCLEAR_ZBUFFER,// 「バックバッファ」＋「深度バッファ」
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f,
		0))) {
		return;
	}

	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {

		pScene->Draw(m_pD3DDevice);

		m_pD3DDevice->EndScene();
	}

	// 描画結果をデバイス側へ転送
	if (FAILED(m_pD3DDevice->Present(0, 0, 0, 0))) {
		// 復帰
		m_pD3DDevice->Reset(&m_D3DPP);
	}
}

void Renderer::Finalize()
{
	if (m_pD3DDevice) {
		m_pD3DDevice->Release(); 
		m_pD3DDevice = NULL;
	}

	if (m_pD3D) {
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}
