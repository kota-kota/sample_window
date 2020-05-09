#include <cstdio>
#include <string>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

//ウィンドウ名
constexpr char* D_WIN_NAME = "X11";

//ウィンドウ位置
constexpr int D_WIN_POSX = 0;
constexpr int D_WIN_POSY = 0;

//ウィンドウ幅高さ
constexpr int D_WIN_WIDTH = 800;
constexpr int D_WIN_HEIGHT = 400;

int main()
{
	//X11をスレッドセーフにする
	XInitThreads();

	//Xサーバ接続
	Display* dpy = XOpenDisplay(nullptr);
	if (dpy == nullptr) {
		//失敗
		printf("[ERROR] %s:%d XOpenDisplay\n", __FUNCTION__, __LINE__);
		return -1;
	}
	printf("XOpenDisplay %p\n", dpy);

	//Xウィンドウ作成
	Window win = XCreateSimpleWindow(
		dpy,
		DefaultRootWindow(dpy),
		0,
		0,
		300,
		300,
		0,
		BlackPixel(dpy, 0),
		WhitePixel(dpy, 0)
	);
	if(win == 0) {
		//失敗
		printf("[ERROR] %s:%d XCreateWindow\n", __FUNCTION__, __LINE__);
		return -1;
	}
	printf("XCreateWindow %d\n", (int)win);

	//サーバに通知するイベントを選択
	const long eventMask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask;
	XSelectInput(dpy, win, eventMask);
	//ウィンドウマップ
	XMapWindow(dpy, win);
	//ウィンドウを左上に移動
	XMoveWindow(dpy, win, 0, 0);
	//ウィンドウを最前面に移動
	XRaiseWindow(dpy, win);
	//ウィンドウ名更新
	XStoreName(dpy, win, D_WIN_NAME);

	printf("<WindowCreaterX::start>\n");
	bool running = true;

	//アトム名設定(閉じるボタン処理用)
	Atom atom1 = XInternAtom(dpy, "WM_PROTOCOLS", False);
	Atom atom2 = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &atom2, 1);

	while (running) {
		XEvent ev;
		XNextEvent(dpy, &ev);

		switch (ev.type) {
		case Expose:
		{
			printf("EVENT: Expose\n");
			break;
		}
		case ButtonPress:
		{
			//押下された画面座標(左上基準)
			std::int32_t x = ev.xbutton.x;
			std::int32_t y = ev.xbutton.y;

			if (ev.xbutton.button == 1) {
				//左
				printf("EVENT: ButtonPress <Left>(%d %d)\n", x, y);
			}
			else if (ev.xbutton.button == 2) {
				//ホイール
				printf("EVENT: ButtonPress <Wheel>(%d %d)\n", x, y);
			}
			else if (ev.xbutton.button == 3) {
				//右
				printf("EVENT: ButtonPress <Right>(%d %d)\n", x, y);
			}
			else if (ev.xbutton.button == 4) {
				//ホイールアップ
				printf("EVENT: ButtonPress <WheelUp>(%d %d)\n", x, y);
			}
			else if (ev.xbutton.button == 5) {
				//ホイールダウン
				printf("EVENT: ButtonPress <WheelDown>(%d %d)\n", x, y);
			}
			else {
				//未サポート
			}

			break;
		}
		case ButtonRelease:
		{
			printf("EVENT: ButtonRelease\n");
			break;
		}
		case MotionNotify:
		{
			//printf("EVENT: MotionNotify\n");
			break;
		}
		case KeyPress:
		{
			KeySym keySym = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, (ev.xkey.state & ShiftMask ? 1 : 0));
			if (keySym == XK_Up) {
				//↑キー
				printf("EVENT: KeyPress <XK_Up>\n");
			}
			else if (keySym == XK_Down) {
				//↓キー
				printf("EVENT: KeyPress <XK_Down>\n");
			}
			else if (keySym == XK_Escape) {
				//ESCキー
				printf("EVENT: KeyPress <XK_Escape>\n");
			}
			else if ((keySym == XK_Shift_L) || (keySym == XK_Shift_R)) {
				//Shiftキー
				printf("EVENT: KeyPress <Shift>\n");
			}
			else if ((keySym == XK_Control_L) || (keySym == XK_Control_R)) {
				//Ctrlキー
				printf("EVENT: KeyPress <Ctrl>\n");
			}
			else {
				//未サポート
			}
			break;
		}
		case KeyRelease:
		{
			printf("EVENT: KeyRelease\n");
			break;
		}
		case ClientMessage:
		{
			//閉じる釦イベント
			if ((atom1 == ev.xclient.message_type) &&
				(atom2 == static_cast<Atom>(ev.xclient.data.l[0]))) {
				//終了
				printf("EVENT: ClientMessage terminate\n");
				running = false;
			}
			break;
		}
		default:
			break;
		};
	}

	return 0;
}