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


int APIENTRY WinMain( // �ü������ �ʿ��� �� ȣ��Ǵ� �Լ�
	HINSTANCE hInstance, // �������� â�� ��ü�� ���Ѵ�. â�� ���� ��ü�� ���� �ĺ��� �� ���Ѵ�.
	HINSTANCE prevInstace, // ������ ���� ������.
	LPSTR lpszCmdParam, // ���ڿ�. ����. �밡���� ǥ��� �� ����Ѵ�.
	int nCmdShow // cmd : ���������Ʈ.[ command window ] ȭ�鿡 ������� ��Ÿ�� ���ΰ�. ���ڿ����·� ��Ÿ�����ΰ� �� ǥ���Ѱ�.
	// �� �׸���� �ۼ����� �ʾƵ� �ڵ����� ����. nullptr ���� ����
)

{
	WNDCLASSA wnd_class; // A : ASCII �����쿡 �����Ұ� �̶�� �ǹ�.

	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0; // Ȯ��� ������ ����� �� �ΰ�.
	wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // ��׶��� ������ ��� ����� �� �ΰ�.
	wnd_class.hCursor = LoadCursor(nullptr, IDC_CROSS); // Ŀ�� ����� ���ڰ� ������� ����ϰڴ�. ��� �ǹ�.
	wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR); // ���α׷� �������� �� �۾�ǥ���ٿ� ��Ÿ���� ������ ����� ERROR�� ǥ���Ұ� �̴�.
	wnd_class.hInstance = hInstance;
	wnd_class.lpfnWndProc = WndProc; // �������� �����Ϳ� �Լ������ �������ִ� ����
	wnd_class.lpszClassName = "First Window"; // ����ȭ ������ ��, Ŭ���� �̸��� �������ִ°�.
	wnd_class.lpszMenuName = nullptr; // �޴��� �� �̾߱��Ѵ�.
	wnd_class.style = CS_HREDRAW | CS_VREDRAW; // ���� �Ǵ� ������ �ٲ���� �� �ٽñ׷��شٴ� �ǹ�.

	RegisterClassA(&wnd_class); // �� Ŭ���� �׸���� �������Ϳ� ����Ͽ� �̸����� ã�ƾ� �� �ִ�.

	HWND hwnd = CreateWindowA
	(
		"First Window",
		"Hello, Window!",
		WS_OVERLAPPEDWINDOW, // ��Ÿ�� �� �ϳ�. �������� �ϳ� �����Ͽ� ��밡��.
		0,
		0,
		1080,
		720,
		nullptr, // �������� �ΰ�â�� ����� ��.
		nullptr, // �޴�
		hInstance,
		nullptr
	); // ȭ���� �����Ͽ���.

	assert(hwnd != nullptr);

	ShowWindow(hwnd, nCmdShow);
	ShowCursor(TRUE);

	MSG message;
	ZeroMemory(&message, sizeof(MSG)); // ������ �޸� �ʱ�ȭ ���.

	while (GetMessage(&message, nullptr, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	DestroyWindow(hwnd); // ������ â ������
	UnregisterClassA("First Window", hInstance); // ������ â�� �������ش�.

	return 0;
}

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParm, LPARAM lParam)
{
	static POINT position;
	static POINT start;
	static POINT end;
	static BOOL is_clicked = FALSE; // ���콺 Ŭ�� ���� Ȯ��.

	static RECT rect1 = { 100, 100, 200, 200 };
	static RECT rect2 = { 300, 300, 400, 400 };
	static BOOL is_intersect = FALSE;

	switch (message) // ���� �޼��� ó���ϴ� �κ�.
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
			HPEN old_pen = static_cast<HPEN>(SelectObject(hdc, cur_pen)); // ���� ������ �ٽ� ����ش�.

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
		PostQuitMessage(0); // �޼����� 0 �� �����鼭 �����츦 �������ش�. ������ ������ 0 ������ 1�� ���ϵȴ�.
		break;
	default:
		return DefWindowProcA(handle, message, wParm, lParam); // ó�� ���Ѱ� �ִ� ���¶�� �̰����� ���Ͻ�Ų��.
	}

	return 0;
}
