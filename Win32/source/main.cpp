#include <cstdio>
#include <string>

#include <Windows.h>
#include <gdiplus.h>


//ウィンドウ名
constexpr TCHAR* D_WIN_NAME = TEXT("GDISample");

//ウィンドウ位置
constexpr int D_WIN_POSX = 0;
constexpr int D_WIN_POSY = 0;

//ウィンドウ幅高さ
constexpr int D_WIN_WIDTH = 800;
constexpr int D_WIN_HEIGHT = 400;

std::string convert_UTF8_to_SJIS(const std::string& utf8)
{
	//UTF8からUnicodeへ変換
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size() + 1, NULL, 0);
	wchar_t* unicode = new wchar_t[unicodeLen];
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size() + 1, unicode, unicodeLen);
	
	//UnicodeからShiftJISへ変換
	int sjisLen = WideCharToMultiByte(CP_THREAD_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);
	char* sjis = new char[sjisLen];
	WideCharToMultiByte(CP_THREAD_ACP, 0, unicode, unicodeLen + 1, sjis, sjisLen, NULL, NULL);

	//戻り値へ格納
	std::string retSJIS(sjis);

	//メモリ解放
	delete unicode;
	delete sjis;

	return retSJIS;
}

//WM_CREATEイベント処理
void winproc_create(HWND hWnd)
{
	printf("<winproc_create> hWnd:0x%p\n", hWnd);
}

//WM_DESTROYイベント処理
void winproc_destroy()
{
	::PostQuitMessage(0);
}

//WM_PAINTイベント処理
void winproc_paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	::BeginPaint(hWnd, &ps);
	::EndPaint(hWnd, &ps);

	//デバイスコンテキスト取得
	HDC hDC = ::GetDC(hWnd);

	//GDI+描画用
	Gdiplus::Color white(255, 255, 255, 255);
	Gdiplus::Color black(255, 0, 0, 0);
	Gdiplus::Graphics graphics(hDC);

	//GDI+による画面クリア
	graphics.Clear(white);

	//テキスト描画1
	std::string text1 = convert_UTF8_to_SJIS(u8"赤色サンプル");
	SetTextColor(hDC, RGB(255, 0, 0));
	TextOut(hDC, 0, 0, text1.c_str(), int(text1.size()));

	//テキスト描画2
	std::string text2 = convert_UTF8_to_SJIS(u8"青色サンプル");
	SetTextColor(hDC, RGB(0, 0, 255));
	TextOut(hDC, 0, 20, text2.c_str(), int(text2.size()));

	//テキスト描画3
	std::string text3 = convert_UTF8_to_SJIS(u8"フォントサンプル");
	HFONT hFont = CreateFont(
		40, 0, 0, 0, FW_BOLD, TRUE, TRUE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		VARIABLE_PITCH | FF_ROMAN, NULL
	);
	SelectObject(hDC, hFont);
	TextOut(hDC, 0, 40, text3.c_str(), int(text3.size()));
	SelectObject(hDC, GetStockObject(SYSTEM_FONT));
	DeleteObject(hFont);

	//線描画
	MoveToEx(hDC, 0, 100, NULL);
	LineTo(hDC, 300, 120);

	//線描画(GDI+)
	Gdiplus::Pen pen(black, 1);
	graphics.DrawLine(&pen, 0, 110, 300, 130);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.DrawLine(&pen, 0, 120, 300, 140);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.DrawLine(&pen, 0, 130, 300, 150);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeDefault);
	graphics.DrawLine(&pen, 0, 140, 300, 160);

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.DrawLine(&pen, 0, 150, 200, 170);

	//デバイスコンテキスト解放
	ReleaseDC(hWnd, hDC);

	//次フレーム描画のため、WM_PAINTイベントを発行
	RECT rect;
	::GetClientRect(hWnd, &rect);
	//::InvalidateRect(hWnd, &rect, false);
}

//ウィンドウプロシージャ
LRESULT CALLBACK winproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0L;

	switch (msg) {
	case WM_CREATE:
		winproc_create(hWnd);
		break;

	case WM_DESTROY:
		winproc_destroy();
		break;

	case WM_PAINT:
		winproc_paint(hWnd);
		break;

	default:
		//デフォルト処理
		ret = ::DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return ret;
}

int main()
{
	//コンソールウィンドウ生成
	::AllocConsole();
	FILE* fConsole = nullptr;
	freopen_s(&fConsole, "CONOUT$", "w", stdout);

	//GDI+初期化
	Gdiplus::GdiplusStartupInput gpSI;
	ULONG_PTR lpToken;
	Gdiplus::GdiplusStartup(&lpToken, &gpSI, NULL);

	//インスタンスハンドル取得
	HINSTANCE hInstance = ::GetModuleHandle(nullptr);
	printf("GetModuleHandle %p\n", hInstance);

	//クラス登録情報設定
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = winproc;
	wcex.lpszClassName = D_WIN_NAME;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.hIconSm = ::LoadIcon(nullptr, IDI_APPLICATION);

	//クラス登録
	ATOM ret = ::RegisterClassEx(&wcex);
	if (ret == 0) {
		printf("[ERROR] %s:%d RegisterClassEx\n", __FUNCTION__, __LINE__);
		return -1;
	}
	printf("RegisterClassEx\n");

	//ウィンドウ作成
	HWND hWnd = ::CreateWindowEx(
		0,
		D_WIN_NAME,
		D_WIN_NAME,
		WS_OVERLAPPEDWINDOW,
		D_WIN_POSX,
		D_WIN_POSY,
		D_WIN_WIDTH,
		D_WIN_HEIGHT,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);
	if (hWnd == nullptr) {
		printf("[ERROR] %s:%d CreateWindowEx\n", __FUNCTION__, __LINE__);
		return -1;
	}
	printf("CreateWindowEx %p\n", hWnd);

	// ウィンドウ表示
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// イベント処理開始
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//GDI+終了
	Gdiplus::GdiplusShutdown(lpToken);

	//コンソールウィンドウ破棄
	fclose(fConsole);
	//::FreeConsole();

	return 0;
}