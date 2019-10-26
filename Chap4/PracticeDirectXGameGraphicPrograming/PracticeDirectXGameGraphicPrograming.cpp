//
// Window.cpp
// アプリケーション(ウィンドウ)の実装
//
// コマンドラインパラメータに/fをつけるとフルスクリーン表示
//

#include <windows.h>
#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"
#pragma comment(lib, "d3d9.lib")

#define CLIENT_WIDTH	640	// クライアント領域のデフォルトの幅, フルスクリーンの場合は水平方向解像度
#define	CLIENT_HEIGHT	480	// クライアント領域のデフォルトの高さ, フルスクリーンの場合は垂直方向解像度
TCHAR  clsName[] = TEXT("D3DFWSampleClass");	// ウィンドウ名
TCHAR  winName[] = TEXT("D3DFWSample");			// ウィンドウ名


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------
// アプリケーションのエントリポイント
// 引数
//		hInstance     : 現在のインスタンスのハンドル
//		hPrevInstance : 以前のインスタンスのハンドル
//		lpCmdLine	  : コマンドラインパラメータ
//		nCmdShow	  : ウィンドウの表示状態
// 戻り値
//		成功したら0以外の値
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// フルスクリーンにするかどうかの判定
	// コマンドラインに/fか/Fが設定されていたらフルスクリーンにする
	BOOL isFullScreen = FALSE;
	for (int i = 0; i < nCmdShow; i++) {
		if (_stricmp((char*)&lpCmdLine[i], "/f") == 0) {	// コマンドラインに/fを発見
			isFullScreen = TRUE;	 // フラグをTRUEに設定
			break;
		}
	}

	HWND		hWnd;
	MSG			msg;

	// ウィンドウクラスの初期化
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),				// この構造体のサイズ
		NULL,							// ウインドウのスタイル(default)
		WindowProc,						// メッセージ処理関数の登録
		0,								// 通常は使わないので常に0
		0,								// 通常は使わないので常に0
		hInstance,						// インスタンスへのハンドル
		NULL,							// アイコン（なし）
		LoadCursor(NULL, IDC_ARROW),	// カーソルの形
		NULL, NULL,						// 背景なし、メニューなし
		clsName,						// クラス名の指定
		NULL							// 小アイコン（なし）
	};

	// ウィンドウクラスの登録
	if (RegisterClassEx(&wcex) == 0) {
		return 0;	// 登録失敗
	}

	// ウィンドウの作成
	if (isFullScreen) { // フルスクリーン
		int sw;
		int sh;
		// 画面全体の幅と高さを取得
		sw = GetSystemMetrics(SM_CXSCREEN);
		sh = GetSystemMetrics(SM_CYSCREEN);

		hWnd = CreateWindow(
			clsName, 			// 登録されているクラス名
			winName, 			// ウインドウ名
			WS_POPUP,			// ウインドウスタイル（ポップアップウインドウを作成）
			0, 					// ウインドウの横方向の位置
			0, 					// ウインドウの縦方向の位置
			CLIENT_WIDTH, 		// ウインドウの幅
			CLIENT_HEIGHT,		// ウインドウの高さ
			NULL,				// 親ウインドウのハンドル（省略）
			NULL,				// メニューや子ウインドウのハンドル
			hInstance, 			// アプリケーションインスタンスのハンドル
			NULL				// ウインドウの作成データ
		);
	}
	else {
		hWnd = CreateWindow(clsName,
			winName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL);

		// ウィンドウサイズを再設定する
		RECT rect;
		int ww, wh;
		int cw, ch;
		// クライアント領域の外の幅を計算
		GetClientRect(hWnd, &rect);		// クライアント部分のサイズの取得
		cw = rect.right - rect.left;	// クライアント領域外の横幅を計算
		ch = rect.bottom - rect.top;	// クライアント領域外の縦幅を計算

		// ウインドウ全体の横幅の幅を計算
		GetWindowRect(hWnd, &rect);		// ウインドウ全体のサイズ取得
		ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
		wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算
		ww = ww - cw;					// クライアント領域以外に必要な幅
		wh = wh - ch;					// クライアント領域以外に必要な高さ

		// ウィンドウサイズの再計算
		ww = CLIENT_WIDTH + ww;			// 必要なウインドウの幅
		wh = CLIENT_HEIGHT + wh;		// 必要なウインドウの高さ

		// ウインドウサイズの再設定
		SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	}

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);

	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);

	// レンダラーオブジェクト
	Renderer renderer;

	// レンダラーの初期化
	HRESULT hr;
	hr = renderer.Init(hWnd, isFullScreen, CLIENT_WIDTH, CLIENT_HEIGHT);

	if (FAILED(hr)) {
		return 0;	// 初期化失敗
	}

	// シーンオブジェクト
	Scene	 scene;

	// シーンの生成
	if (FAILED(scene.Create(renderer.GetDevice()))) {
		return 0;
	}

	// メッセージ処理および描画ループ
	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {	// PostQuitMessage()が呼ばれた
				break;	//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {	// 処理するメッセージが無いときは描画を行う

			// ウィンドウが見えている時だけ描画するための処理
			WINDOWPLACEMENT wndpl;
			GetWindowPlacement(hWnd, &wndpl);	// ウインドウの状態を取得
			if ((wndpl.showCmd != SW_HIDE) &&
				(wndpl.showCmd != SW_MINIMIZE) &&
				(wndpl.showCmd != SW_SHOWMINIMIZED) &&
				(wndpl.showCmd != SW_SHOWMINNOACTIVE)) {

				// 描画処理の実行
				renderer.RenderScene(&scene);
			}
		}

	}

	return (int)msg.wParam;
}

//-------------------------------------------------------------
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CLOSE:				// ウインドウが閉じられた
		PostQuitMessage(0);		// アプリケーションを終了する
		break;
	case WM_KEYDOWN:				// キーが押された
		if (wParam == VK_ESCAPE) {	// 押されたのはESCキーだ
			PostQuitMessage(0);		// アプリケーションを終了する
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
