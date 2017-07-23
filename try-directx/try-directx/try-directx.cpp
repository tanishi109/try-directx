// try-directx.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "try-directx.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

												// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




#include <iostream>
using namespace std;

//#壁 _空間 .ゴール oブロック p人
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object {
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

//関数プロトタイプ
void initialize(Object* state, int w, int h, const char* stageData);
// void draw(const Object* state, int w, int h);
// void update(Object* state, char input, int w, int h);
// bool checkClear(const Object* state, int w, int h);

Object* getStage() {
	//一次元配列である理由は本文参照
	Object* state = new Object[gStageWidth * gStageHeight]; //状態配列確保

	initialize(state, gStageWidth, gStageHeight, gStageData); //ステージ初期化
															  //メインループ

	return state;
	// while (true) {
	// まず描画
	//draw(state, gStageWidth, gStageHeight);
	//クリアチェック
	// if (checkClear(state, gStageWidth, gStageHeight)) {
	// 	break; //クリアチェック
	// }
	// //入力取得
	// cout << "a:left s:right w:up z:down. command?" << endl; //操作説明
	// char input;
	// cin >> input;
	// //更新
	// update(state, input, gStageWidth, gStageHeight);
	// }
	//祝いのメッセージ
	// cout << "Congratulation's! you won." << endl;
	//後始末
	// delete[] state;
	// state = 0;

	//Visual Studioから実行する人のために無限ループ。コマンドラインからはCtrl-Cで終えてください。
	// while (true) {
	// 	;
	// }
	// return 0;
}

//---------------------以下関数定義------------------------------------------


//いつか使う日も来るだろうと高さも渡す仕様にしたが、現状使っていないので名前だけ(height)コメントアウトしてある。
void initialize(Object* state, int width, int /* height */, const char* stageData) {
	const char* d = stageData; //読み込みポインタ
	int x = 0;
	int y = 0;
	while (*d != '\0') { //NULL文字でない間
		Object t; //特に意味はないが使う回数が多い変数に私は良くtを使う。temporaryの略。たぶんよくない習慣だが、無駄に長い名前にして読みにくいのも困り物だろう。
		switch (*d) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //改行処理
		default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN) { //知らない文字なら無視するのでこのif文がある
			state[y*width + x] = t; //書き込み
			++x;
		}
	}
}

void draw(const Object* state, int width, int height) {
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; //Object列挙の順

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Object o = state[y*width + x];
			cout << font[o];
		}
		cout << endl;
	}
}

//第一引数はほかの関数ではstateとしているが、あまりに頻繁に使うので
//短いsで済ませている。w,hもそれぞれwidth,heightである。
void update(Object* s, char input, int w, int h) {
	//移動差分に変換(dはdifferenceでもdeltaでもお好きな方の略だと思って欲しい)
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //左
	case 's': dx = 1; break; //右
	case 'w': dy = -1; break; //上。Yは下がプラス
	case 'z': dy = 1; break; //下。
	}
	//人座標を検索
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w; //xは幅で割ったあまり
	int y = i / w; //yは幅で割った商

				   //移動
				   //移動後座標(tに意味はない。ごめんなさい)
	int tx = x + dx;
	int ty = y + dy;
	//座標の最大最小チェック。外れていれば不許可
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//A.その方向が空白またはゴール。人が移動。
	int p = y*w + x; //人位置
	int tp = ty*w + tx; //ターゲット位置(TargetPosition)
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //ゴールならゴール上の人に
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //もともとゴール上ならゴールに
																 //B.その方向が箱。その方向の次のマスが空白またはゴールであれば移動。
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		//2マス先が範囲内かチェック
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //押せない
			return;
		}

		int tp2 = (ty + dy)*w + (tx + dx); //2マス先
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			//順次入れ替え
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

//ブロックのみがなければクリアしている。
bool checkClear(const Object* s, int width, int height) {
	for (int i = 0; i < width*height; ++i) {
		if (s[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。

	// グローバル文字列を初期化しています。
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TRYDIRECTX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRYDIRECTX));

	MSG msg;

	// メイン メッセージ ループ:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRYDIRECTX));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TRYDIRECTX);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if (wParam == VK_LEFT) {
			OutputDebugString(_T("left!"));
		}
		else if (wParam == VK_RIGHT) {
			OutputDebugString(_T("right!"));
		}
		else if (wParam == VK_UP) {
			OutputDebugString(_T("up!"));
		}
		else if (wParam == VK_DOWN) {
			OutputDebugString(_T("down!"));
		}
	}
	break;
	case WM_PAINT:
	{
		Object* stage = getStage();

		const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; //Object列挙の順

		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);

		// Obtain the size of the drawing area.
		RECT rc;
		GetClientRect(
			hWnd,
			&rc
		);
		// Save the original object
		HGDIOBJ original = NULL;
		original = SelectObject(
			ps.hdc,
			GetStockObject(DC_PEN)
		);
		// Create a pen.            
		HPEN grayPen = CreatePen(PS_SOLID, 3, 0xe0e0e0);
		HPEN blackPen = CreatePen(PS_SOLID, 3, 0x333333);
		HPEN paleGreenPen = CreatePen(PS_SOLID, 3, 0xa5c9b5);
		HPEN greenPen = CreatePen(PS_SOLID, 3, 0x219653);
		HPEN yellowPen = CreatePen(PS_SOLID, 3, 0xffd450);

		int baseX = rc.left + 100;
		int baseY = rc.top + 100;
		int size = 32;

		for (int y = 0; y < gStageHeight; ++y) {
			for (int x = 0; x < gStageWidth; ++x) {
				Object o = stage[y * gStageWidth + x];
				if (font[o] == '#') {
					// Select the pen.
					SelectObject(ps.hdc, blackPen);
				}
				else if (font[o] == '.') {
					// Select the pen.
					SelectObject(ps.hdc, paleGreenPen);
				}
				else if (font[o] == 'o') {
					// Select the pen.
					SelectObject(ps.hdc, greenPen);
				}
				else if (font[o] == 'p') {
					// Select the pen.
					SelectObject(ps.hdc, yellowPen);
				}
				else {
					// Select the pen.
					SelectObject(ps.hdc, grayPen);
				}

				int topLeftX = x * size + baseX;
				int topLeftY = y * size + baseY;

				// Draw a rectangle.
				Rectangle(
					ps.hdc,
					topLeftX,
					topLeftY,
					topLeftX + size,
					topLeftY + size);
			}
		}

		DeleteObject(blackPen);

		// Restore the original object
		SelectObject(ps.hdc, original);

		EndPaint(hWnd, &ps);

	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
