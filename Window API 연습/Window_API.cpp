#include <Windows.h>
#include <cassert>
#include <string>


LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wparam, LPARAM lParam);

class Square
{
	int x;
	int y;
	int size;
public:
	Square(int x,int y, int size) : x(x), y(y), size(size) {}
	auto Getleft() { return x; }
	auto Getright() { return x + size; }
	auto Getup() { return y; }
	auto Getdown() { return y + size; }

	auto Print_Text(HDC hdc, std::string str) {
		TextOutA(hdc, x, y, str.c_str(), str.length());
	}
	auto Make_Square(HDC hdc) {
		Rectangle(hdc, x, y, x+size, y+size);
	}

	auto Chack_object_collision(HDC hdc, std::string str, Square sq) {
		if (this->Getleft() < sq.Getright() && this->Getright() > sq.Getleft() &&
			this->Getup() < sq.Getdown() && this->Getdown() > sq.Getup())
		{
			sq.Print_Text(hdc, str);
		}
	}
};


int APIENTRY WinMain( // 운영체제에서 필요할 때 호출되는 함수
	HINSTANCE hInstance, // 윈도우의 창의 실체를 말한다. 창을 띄우는 자체에 대한 식별자 를 말한다.
	HINSTANCE prevInstace, // 이전에 대한 정보값.
	LPSTR lpszCmdParam, // 문자열. 형식. 헝가리안 표기법 을 사용한다.
	int nCmdShow // cmd : 명령프롬프트.[ command window ] 화면에 어떤식으로 나타낼 것인가. 문자열형태로 나타낼것인가 를 표시한것.
	// 위 항목들은 작성하지 않아도 자동으로 들어간다. nullptr 같은 느낌
)

{
	WNDCLASSA wnd_class; // A : ASCII 윈도우에 지정할것 이라는 의미.

	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0; // 확장된 공간을 사용할 것 인가.
	wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // 백그라운드 색상을 어떻게 사용할 것 인가.
	wnd_class.hCursor = LoadCursor(nullptr, IDC_CROSS); // 커서 모양을 십자가 모양으로 사용하겠다. 라는 의미.
	wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR); // 프로그램 실행했을 때 작업표시줄에 나타나는 아이콘 모양을 ERROR로 표시할것 이다.
	wnd_class.hInstance = hInstance;
	wnd_class.lpfnWndProc = WndProc; // 윈도우의 포인터와 함수펑션을 연결해주는 역할
	wnd_class.lpszClassName = "First Window"; // 파일화 시켰을 때, 클래스 이름을 정의해주는것.
	wnd_class.lpszMenuName = nullptr; // 메뉴바 를 이야기한다.
	wnd_class.style = CS_HREDRAW | CS_VREDRAW; // 수평 또는 수직이 바뀌었을 때 다시그려준다는 의미.

	RegisterClassA(&wnd_class); // 위 클래스 항목들을 레지스터에 등록하여 이름으로 찾아쓸 수 있다.

	HWND hwnd = CreateWindowA
	(
		"First Window",
		"Hello, Window!",
		WS_OVERLAPPEDWINDOW, // 스타일 중 하나. 여러개중 하나 선택하여 사용가능.
		0,
		0,
		1080,
		720,
		nullptr, // 윈도우의 부가창을 만드는 것.
		nullptr, // 메뉴
		hInstance,
		nullptr
	); // 화면을 구성하였다.

	assert(hwnd != nullptr);

	ShowWindow(hwnd, nCmdShow);
	ShowCursor(TRUE);

	MSG message;
	ZeroMemory(&message, sizeof(MSG)); // 윈도우 메모리 초기화 방식.

	while (GetMessage(&message, nullptr, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	DestroyWindow(hwnd); // 윈도우 창 날리기
	UnregisterClassA("First Window", hInstance); // 윈도우 창을 해제해준다.

	return 0;
}

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParm, LPARAM lParam)
{
	static POINT position;
	static POINT start;
	static POINT end;
	static BOOL is_clicked = FALSE; // 마우스 클릭 여부 확인.

	static RECT rect1 = { 100, 100, 200, 200 };
	static RECT rect2 = { 300, 300, 400, 400 };
	static BOOL is_intersect = FALSE;

	switch (message) // 들어온 메세지 처리하는 부분.
	{
	case WM_MOUSEMOVE:
		position.x = LOWORD(lParam);
		position.y = HIWORD(lParam);
		InvalidateRect(handle, nullptr, TRUE);
		break;

	case WM_PAINT:
		{
			std::string str = "";
			str += std::to_string(position.x);
			str += ", ";
			str += std::to_string(position.y);
			int size;
			size = 100;

			Square sq1(position.x, position.y, size);
			Square sq2(400, 400, size);

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(handle, &ps);

			HPEN cur_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			HPEN old_pen = static_cast<HPEN>(SelectObject(hdc, cur_pen)); // 생성 정보를 다시 담아준다.

			sq1.Make_Square(hdc);
			TextOutA(hdc, position.x + 20, position.y - 20, str.c_str(), str.length());
			sq2.Make_Square(hdc);

			sq1.Chack_object_collision(hdc, "Collision!!", sq2);

			SelectObject(hdc, old_pen);
			DeleteObject(cur_pen);

			EndPaint(handle, &ps);
		}
	break;

	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0); // 메세지에 0 이 들어오면서 윈도우를 종료해준다. 문제가 없으면 0 있으면 1이 리턴된다.
		break;
	default:
		return DefWindowProcA(handle, message, wParm, lParam); // 처리 안한게 있는 상태라면 이곳으로 리턴시킨다.
	}

	return 0;
}
