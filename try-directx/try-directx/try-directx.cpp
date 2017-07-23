// try-directx.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "try-directx.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X
WCHAR szTitle[MAX_LOADSTRING];                  // �^�C�g�� �o�[�̃e�L�X�g
WCHAR szWindowClass[MAX_LOADSTRING];            // ���C�� �E�B���h�E �N���X��

												// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




#include <iostream>
using namespace std;

//#�� _��� .�S�[�� o�u���b�N p�l
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

//�֐��v���g�^�C�v
void initialize(Object* state, int w, int h, const char* stageData);
// void draw(const Object* state, int w, int h);
// void update(Object* state, char input, int w, int h);
// bool checkClear(const Object* state, int w, int h);

Object* getStage() {
	//�ꎟ���z��ł��闝�R�͖{���Q��
	Object* state = new Object[gStageWidth * gStageHeight]; //��Ԕz��m��

	initialize(state, gStageWidth, gStageHeight, gStageData); //�X�e�[�W������
															  //���C�����[�v

	return state;
	// while (true) {
	// �܂��`��
	//draw(state, gStageWidth, gStageHeight);
	//�N���A�`�F�b�N
	// if (checkClear(state, gStageWidth, gStageHeight)) {
	// 	break; //�N���A�`�F�b�N
	// }
	// //���͎擾
	// cout << "a:left s:right w:up z:down. command?" << endl; //�������
	// char input;
	// cin >> input;
	// //�X�V
	// update(state, input, gStageWidth, gStageHeight);
	// }
	//�j���̃��b�Z�[�W
	// cout << "Congratulation's! you won." << endl;
	//��n��
	// delete[] state;
	// state = 0;

	//Visual Studio������s����l�̂��߂ɖ������[�v�B�R�}���h���C�������Ctrl-C�ŏI���Ă��������B
	// while (true) {
	// 	;
	// }
	// return 0;
}

//---------------------�ȉ��֐���`------------------------------------------


//�����g���������邾�낤�ƍ������n���d�l�ɂ������A����g���Ă��Ȃ��̂Ŗ��O����(height)�R�����g�A�E�g���Ă���B
void initialize(Object* state, int width, int /* height */, const char* stageData) {
	const char* d = stageData; //�ǂݍ��݃|�C���^
	int x = 0;
	int y = 0;
	while (*d != '\0') { //NULL�����łȂ���
		Object t; //���ɈӖ��͂Ȃ����g���񐔂������ϐ��Ɏ��͗ǂ�t���g���Btemporary�̗��B���Ԃ�悭�Ȃ��K�������A���ʂɒ������O�ɂ��ēǂ݂ɂ����̂����蕨���낤�B
		switch (*d) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //���s����
		default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN) { //�m��Ȃ������Ȃ疳������̂ł���if��������
			state[y*width + x] = t; //��������
			++x;
		}
	}
}

void draw(const Object* state, int width, int height) {
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; //Object�񋓂̏�

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Object o = state[y*width + x];
			cout << font[o];
		}
		cout << endl;
	}
}

//�������͂ق��̊֐��ł�state�Ƃ��Ă��邪�A���܂�ɕp�ɂɎg���̂�
//�Z��s�ōς܂��Ă���Bw,h�����ꂼ��width,height�ł���B
void update(Object* s, char input, int w, int h) {
	//�ړ������ɕϊ�(d��difference�ł�delta�ł����D���ȕ��̗����Ǝv���ė~����)
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //��
	case 's': dx = 1; break; //�E
	case 'w': dy = -1; break; //��BY�͉����v���X
	case 'z': dy = 1; break; //���B
	}
	//�l���W������
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w; //x�͕��Ŋ��������܂�
	int y = i / w; //y�͕��Ŋ�������

				   //�ړ�
				   //�ړ�����W(t�ɈӖ��͂Ȃ��B���߂�Ȃ���)
	int tx = x + dx;
	int ty = y + dy;
	//���W�̍ő�ŏ��`�F�b�N�B�O��Ă���Εs����
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//A.���̕������󔒂܂��̓S�[���B�l���ړ��B
	int p = y*w + x; //�l�ʒu
	int tp = ty*w + tx; //�^�[�Q�b�g�ʒu(TargetPosition)
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //�S�[���Ȃ�S�[����̐l��
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //���Ƃ��ƃS�[����Ȃ�S�[����
																 //B.���̕��������B���̕����̎��̃}�X���󔒂܂��̓S�[���ł���Έړ��B
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		//2�}�X�悪�͈͓����`�F�b�N
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //�����Ȃ�
			return;
		}

		int tp2 = (ty + dy)*w + (tx + dx); //2�}�X��
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			//��������ւ�
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

//�u���b�N�݂̂��Ȃ���΃N���A���Ă���B
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

	// TODO: �����ɃR�[�h��}�����Ă��������B

	// �O���[�o������������������Ă��܂��B
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TRYDIRECTX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRYDIRECTX));

	MSG msg;

	// ���C�� ���b�Z�[�W ���[�v:
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
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

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
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E�̕`��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
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

		const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; //Object�񋓂̏�

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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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
