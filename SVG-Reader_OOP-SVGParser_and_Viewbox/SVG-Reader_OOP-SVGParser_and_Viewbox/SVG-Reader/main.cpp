#ifndef NOMINMAX
#define NOMINMAX 
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

// Sắp xếp thứ tự include để tránh xung đột GDI+/Windows API
#include <WinSock2.h>
#include <Windows.h>
#include <wingdi.h> // Cần thiết để đảm bảo các hằng số GDI được định nghĩa
#include <gdiplus.h>

#include <iostream>
#include <string>

// Kỹ thuật Include: Giả định các thư mục cha đã được cấu hình trong Project Properties
#include "Library.h"
#include "SVGDocument.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "SVGFactoryPattern.h" // Thêm Factory để đảm bảo nó được nhìn thấy
#include "SVGDocumentContext.h"

// Cần cho các hàm của Gdiplus
using namespace Gdiplus;
using namespace std;

// BẮT BUỘC: Đảm bảo các thư viện Windows được liên kết chính xác
#ifdef _MSC_VER
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"gdi32.lib")
#pragma comment (lib,"kernel32.lib")
#endif

// Tên file SVG để tải
const std::string SVG_FILENAME = "D:\\Downloads\\sample.svg";
// Đã chuyển sang Unicode (LPCWSTR)
const LPCWSTR WINDOW_CLASS_NAME = L"SVGReaderWindow";
const LPCWSTR WINDOW_TITLE = L"SVG Reader Demo";

// Khai báo Hàm xử lý Thông điệp Cửa sổ (cần thiết cho WndProc)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Khai báo Toàn cục
SVGDocument* g_svgDocument = nullptr;
SVGParser g_parser;
SVGRenderer g_renderer;
SVGFactoryPattern g_factory; // Factory để phân tích cú pháp
ULONG_PTR gdiplusToken;



// Hàm vẽ chính (GDI+ Paint Handler)
//VOID OnPaint(HWND hWnd) {
//	PAINTSTRUCT ps;
//	HDC hdc = BeginPaint(hWnd, &ps);
//
//	Graphics graphics(hdc);
//
//	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
//	graphics.Clear(Gdiplus::Color(255, 255, 255));
//
//	if (g_svgDocument && g_svgDocument->getRootGroup()) {
//		// Gọi hàm render chính
//		g_svgDocument->renderSVGImage(g_renderer, graphics);
//	}
//
//	EndPaint(hWnd, &ps);
//}
// Trong main.cpp
VOID OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);	Graphics graphics(hdc);

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.Clear(Gdiplus::Color(255, 255, 255)); // Tô nền cửa sổ màu trắng

	if (g_svgDocument && g_svgDocument->getRootGroup()) {
		// --- BỔ SUNG LOGIC VIEWPORT & TRANSFORMATION Ở ĐÂY ---
		// 1. Lấy kích thước cửa sổ hiện tại (Viewport)
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		float viewportW = (float)clientRect.right;
		float viewportH = (float)clientRect.bottom;
		// 2. Lấy ViewBox (giả sử SVGDocument có các hàm này)
		// Ví dụ: ViewBox viewBox = g_svgDocument->getViewBox();
		// Giả định ViewBox là [0, 0, 800, 400] dựa trên file SVG (height="400", width="800")
		float viewBoxW = 1500.0f; // Lấy từ thuộc tính SVG hoặc ViewBox parser
		float viewBoxH = 750.0f; // Lấy từ thuộc tính SVG hoặc ViewBox parser
		// 3. Tính toán tỷ lệ (Scale) để vừa vặn
		float scaleX = viewportW / viewBoxW;
		float scaleY = viewportH / viewBoxH;
		float scale = std::min(scaleX, scaleY);
		// 4. Áp dụng phép biến đổi
		graphics.ScaleTransform(scale, scale);
		// Cần thêm Translate Transform nếu bạn muốn căn giữa (chúng ta bỏ qua bước này để đơn giản)
		// ----------------------------------------------------
		g_svgDocument->renderSVGImage(g_renderer, graphics, g_svgDocument->context);
	}
	EndPaint(hWnd, &ps);
}

// Xử lý Thông điệp Cửa sổ (Windows Message Handler)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_PAINT:
		OnPaint(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

// Hàm Main (Điểm Khởi chạy Ứng dụng) - Đã đổi thành wWinMain cho Unicode
INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT iCmdShow) { // Đã sửa tham số thứ 2 và 3

	// ----------------------------------------------------
	// 1. KHỞI TẠO GDI+ VÀ PHÂN TÍCH FILE SVG
	// ----------------------------------------------------
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//try {
	//	// Tạo Document
	//	g_svgDocument = new SVGDocument(SVG_FILENAME);

	//	// Tải file và xây dựng cây đối tượng (truyền g_factory)
	//	g_svgDocument->parseSVGImage(g_parser, g_factory);
	//	if (!g_svgDocument->getRootGroup()) {
	//		std::cerr << "Error: SVG parsing failed or root group is empty." << std::endl;
	//	}
	//	//if (g_svgDocument && g_svgDocument->getRootGroup()) {
	//	//	g_svgDocument->getRootGroup()->printDebugAttributes();
	//	//}
	//}
	//catch (const std::exception& e) {
	//	std::cerr << "Parsing Exception: " << e.what() << std::endl;
	//	GdiplusShutdown(gdiplusToken);
	//	return 1;
	//}

	try {
		// Tạo Document
		g_svgDocument = new SVGDocument(SVG_FILENAME);

		// Tải file và xây dựng cây đối tượng (truyền g_parser và g_factory)
		g_svgDocument->parseSVGImage(g_parser, g_factory);

		// -----------------------------------------------------------------
		// <<< ĐIỂM GỌI HÀM DEBUG CHÍNH XÁC >>>
		// -----------------------------------------------------------------
		if (g_svgDocument && g_svgDocument->getRootGroup()) {

			std::cerr << "\n========== START PARSING DEBUG ==========" << std::endl;

			// Gọi hàm debug đệ quy của đối tượng Group gốc, truyền std::cerr làm output
			g_svgDocument->getRootGroup()->printDebugAttributes(std::cerr);

			std::cerr << "========== END PARSING DEBUG ==========\n" << std::endl;
		}
		// -----------------------------------------------------------------

		// ... (Phần khởi tạo cửa sổ và vòng lặp thông điệp)
	}
	catch (const std::exception& e) {
		// ...
	}


	// ----------------------------------------------------
	// 2. KHỞI TẠO CỬA SỔ WINDOWS
	// ----------------------------------------------------
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc; // <<< BỎ COMMENT/GÁN
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = WINDOW_CLASS_NAME; // <<< BỎ COMMENT/GÁN
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Đăng ký lớp cửa sổ
	if (!RegisterClassEx(&wc)) {
		GdiplusShutdown(gdiplusToken);
		return 0;
	}

	// Tạo cửa sổ
	HWND hWnd = CreateWindow(
		WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL
	);

	if (hWnd == NULL) {
		GdiplusShutdown(gdiplusToken);
		return 0;
	}

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	// ----------------------------------------------------
	// 3. VÒNG LẶP THÔNG ĐIỆP (MAIN LOOP)
	// ----------------------------------------------------
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	// ----------------------------------------------------
	// 4. DỌN DẸP
	// ----------------------------------------------------
	delete g_svgDocument;
	GdiplusShutdown(gdiplusToken);

	return static_cast<INT>(Msg.wParam);
}