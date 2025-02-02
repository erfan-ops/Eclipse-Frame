#include <Windows.h>
#include <gl/GL.h>
#include <chrono>
#include <fstream>
#include <thread>
#include <random>
#include <cmath>

#include "settings.h"
#include "trayUtils.h"
#include "BackGround.h"
#include "DesktopUtils.h"
#include "star.h"
#include "rendering.h"


constinit bool running = true;

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_TRAYICON) {
		if (lParam == WM_RBUTTONUP) {
			// Create a popup menu
			HMENU menu = CreatePopupMenu();
			AppendMenuW(menu, MF_STRING, 1, L"Quit");

			// Get the cursor position
			POINT cursorPos;
			GetCursorPos(&cursorPos);

			// Show the menu
			SetForegroundWindow(hwnd);
			// Example with TPM_NONOTIFY to avoid blocking
			int selection = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY, cursorPos.x - 120, cursorPos.y - 22, 0, hwnd, nullptr);
			DestroyMenu(menu);

			// Handle the menu selection
			if (selection == 1) {
				OnQuit(hwnd, running);
			}
		}
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static float randomUniform(float start, float end) {
	// Create a random device and a random engine
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(start, end); // Range [start, end)

	return dist(gen); // Generate the random number
}

static inline void checkEvents(MSG& msg, bool& running) {
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			running = false;
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

static inline void gameTick(float& frameTime, const float& stepInterval, float& fractionalTime) {
	if (frameTime < stepInterval) {
		// Calculate total sleep time including any leftover fractional time
		float totalSleepTime = (stepInterval - frameTime) + fractionalTime;

		// Truncate to whole milliseconds
		int sleepMilliseconds = static_cast<int>(totalSleepTime * 1e+3f);

		// Calculate remaining fractional time and ensure it’s within 0.0f to 1.0f
		fractionalTime = (totalSleepTime - sleepMilliseconds * 1e-3f);

		// Sleep for the calculated milliseconds
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepMilliseconds));
	}
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	const wchar_t* className = L"HypercubeClass";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className;
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	const int Width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	const int Height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	const int leftMost = GetSystemMetrics(SM_XVIRTUALSCREEN);
	const int topMost = GetSystemMetrics(SM_YVIRTUALSCREEN);

	HWND hwnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		className,
		L"Hypercube Harmony",
		WS_POPUP,
		0, 0,
		Width, Height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hwnd) return -1;

	const Settings settings = loadSettings("settings.json");

	const float stepInterval = 1.0f / settings.targetFPS;
	const float MOUSE_RADIUS_SQR = settings.mouseRadius * settings.mouseRadius;
	const float STARS_LINE_RADIUS_SQR = settings.starsLineRadius * settings.starsLineRadius;
	const float DLINE_WIDTH = settings.stars.lineMaxWidth - settings.stars.lineMinWidth;

	// Load the icon from resources
	HICON hIcon = LoadIconFromResource();

	// Add the tray icon
	AddTrayIcon(hwnd, hIcon, L"Just a Simple Icon");

	wchar_t* originalWallpaper = GetCurrentWallpaper();

	SetAsDesktop(hwnd);

	ShowWindow(hwnd, SW_SHOW);

	HDC hdc = GetDC(hwnd); // Get device context

	// Step 1: Set up the pixel format
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	// Step 2: Create and set the rendering context
	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);

	// OpenGL is now set up for this window!

	float dt{0};
	float frameTime{0};
	float fractionalTime{0};

	POINT mousePos;
	
	Star* stars = new Star[settings.stars.count];

	const float offsetBounds = settings.starsLineRadius;
	const float roffsetBounds = -settings.starsLineRadius;
	const float woffsetBounds = offsetBounds + Width;
	const float hoffsetBounds = offsetBounds + Height;

	// Fill the array with Star objects
	for (int i = 0; i < settings.stars.count; ++i) {
		stars[i] = Star(
			randomUniform(roffsetBounds, woffsetBounds),
			randomUniform(roffsetBounds, hoffsetBounds),
			randomUniform(-settings.stars.maxSpeed, settings.stars.maxSpeed),
			randomUniform(-settings.stars.maxSpeed, settings.stars.maxSpeed)
		);
	}

	auto newF = std::chrono::high_resolution_clock::now();
	auto oldF = std::chrono::high_resolution_clock::now();
	auto endF = std::chrono::high_resolution_clock::now();

	BG bg(settings.backGroundColors, Width, Height);

	// Message loop
	MSG msg = {};
	while (running) {
		// Check for messages
		checkEvents(msg, running);

		oldF = newF;
		newF = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float>(newF - oldF).count();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//BG::fillGradient(settings.backGroundColors, Width, Height);
		bg.fillGradient();
		glLoadIdentity();
		glOrtho(0, Width, Height, 0, -1, 1);

		GetCursorPos(&mousePos);
		mousePos.x -= leftMost;
		mousePos.y -= topMost;

		for (int starIdx = 0; starIdx < settings.stars.count; ++starIdx) {
			Star& star = stars[starIdx];
			star.move(dt);

			if (star.orgx < roffsetBounds) {
				star.speedx = std::abs(star.speedx);
			}
			else if (star.orgx > woffsetBounds) {
				star.speedx = -std::abs(star.speedx);
			}

			if (star.orgy < roffsetBounds) {
				star.speedy = std::abs(star.speedy);
			}
			else if (star.orgy > hoffsetBounds) {
				star.speedy = -std::abs(star.speedy);
			}

			float mouse_dx = mousePos.x - star.x;
			float mouse_dy = mousePos.y - star.y;
			float distance_from_mouse_sqr = mouse_dx * mouse_dx + mouse_dy * mouse_dy;
			if (distance_from_mouse_sqr && distance_from_mouse_sqr < MOUSE_RADIUS_SQR) {
				float distance_from_mouse = std::sqrtf(distance_from_mouse_sqr);
				float ratio = settings.mouseRadius / distance_from_mouse;
				float newX = mouse_dx * ratio;
				float newY = mouse_dy * ratio;
				star.x = mousePos.x - newX;
				star.y = mousePos.y - newY;
			}

			for (int otherIdx = starIdx + 1; otherIdx < settings.stars.count; ++otherIdx) {
				Star& other = stars[otherIdx];
				float dx = other.x - star.x;
				float dy = other.y - star.y;
				float starsDisSqr = dx * dx + dy * dy;
				if (starsDisSqr && starsDisSqr < STARS_LINE_RADIUS_SQR) {
					float starsDis = std::sqrtf(starsDisSqr);
					float starsDisRatio = (1 - (starsDis / settings.starsLineRadius));
					float alpha = settings.stars.lineColor[3] * starsDisRatio;
					float lineWidth = starsDisRatio * DLINE_WIDTH + settings.stars.lineMinWidth;
					aaline(settings.stars.lineColor[0], settings.stars.lineColor[1], settings.stars.lineColor[2], alpha, star.x, star.y, other.x, other.y, lineWidth);
				}
			}
		}


		SwapBuffers(hdc);

		endF = std::chrono::high_resolution_clock::now();
		frameTime = std::chrono::duration<float>(endF - newF).count();

		gameTick(frameTime, stepInterval, fractionalTime);
	}

	SetParent(hwnd, nullptr);
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)originalWallpaper, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	RemoveTrayIcon(hwnd);

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hglrc);
	ReleaseDC(hwnd, hdc);
	
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	delete[] originalWallpaper;
	delete[] stars;
	DestroyIcon(hIcon);

	return 0;
}
