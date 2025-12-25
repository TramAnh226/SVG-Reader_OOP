

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
#include "SVGDocument.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "SVGGroup.h"
#include "SVGFactoryPattern.h" // Thêm Factory để đảm bảo nó được nhìn thấy
//#include "SVGDocumentContext.h"

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
//const std::string SVG_FILENAME = "D:\\Downloads\\sample.svg";
const std::string SVG_FILENAME = "D:\\DoAnOOP2025\\SVG-Reader_OOP-SVGParser_and_Viewbox\\TestCases-20251225T142325Z-1-001\\TestCases\\svg-19.svg";

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

int g_scrollX = 0;
int g_scrollY = 0;

// --- THÊM MỚI: Biến Zoom và Xoay ---
float g_userZoom = 1.0f;    // 1.0 = 100%
float g_rotation = 0.0f;    // Góc xoay (độ)
// ------------------------------------

// --- HÀM HỖ TRỢ: Hiển thị bảng hướng dẫn ---
void ShowHelp(HWND hWnd) {
	MessageBox(hWnd,
		L"=== HƯỚNG DẪN ===\n\n"
		L"[ Chuột ]\n"
		L"- Lăn chuột giữa: Phóng to / Thu nhỏ\n\n"
		L"[ Bàn phím ]\n"
		L"- L: Xoay Trái\n"
		L"- R: Xoay Phải\n"
		L"- X: Reset về mặc định\n"
		L"- H: Xem lại hướng dẫn này",
		L"Hướng dẫn sử dụng", MB_OK | MB_ICONINFORMATION);
}

// --- HÀM HỖ TRỢ: Cập nhật thanh cuộn (Tách logic cũ từ WM_SIZE ra để tái sử dụng) ---
void UpdateScrollBars(HWND hWnd) {
	if (!g_svgDocument || !g_svgDocument->getRootGroup()) return;

	SVGGroup* rootGroup = g_svgDocument->getRootGroup();
	RECT rect;
	GetClientRect(hWnd, &rect);
	int clientW = rect.right;
	int clientH = rect.bottom;

	float viewBoxW = rootGroup->getViewBoxW();
	float viewBoxH = rootGroup->getViewBoxH();
	float svgWidth = rootGroup->getWidth();
	float svgHeight = rootGroup->getHeight();

	// Logic xử lý ViewBox thiếu (giữ nguyên logic cũ của bạn)
	if (viewBoxW <= 0.0f || viewBoxH <= 0.0f) {
		if (svgWidth > 0.0f && svgHeight > 0.0f) { viewBoxW = svgWidth; viewBoxH = svgHeight; }
		else { viewBoxW = clientW > 0 ? (float)clientW : 1.0f; viewBoxH = clientH > 0 ? (float)clientH : 1.0f; }
	}

	// Tính toán tỷ lệ render
	float scaleX = (float)clientW / viewBoxW;
	float scaleY = (float)clientH / viewBoxH;
	float final_scale = std::min(scaleX, scaleY);

	// Logic cũ của bạn: limit scale
	if (final_scale <= 1.0f) final_scale = std::min(final_scale, 0.8f);

	// --- ÁP DỤNG ZOOM CỦA NGƯỜI DÙNG ---
	final_scale *= g_userZoom;
	// ------------------------------------

	// Kích thước nội dung VẬT LÝ sau khi render
	int contentW = (int)(viewBoxW * final_scale);
	int contentH = (int)(viewBoxH * final_scale);

	SCROLLINFO si = { 0 };
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;

	// Thanh cuộn Ngang
	si.nMin = 0;
	si.nMax = contentW;
	si.nPage = clientW;
	int maxScrollX = contentW > clientW ? contentW - clientW : 0;
	g_scrollX = std::min(g_scrollX, maxScrollX);
	si.nPos = g_scrollX;
	SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

	// Thanh cuộn Dọc
	si.nMax = contentH;
	si.nPage = clientH;
	int maxScrollY = contentH > clientH ? contentH - clientH : 0;
	g_scrollY = std::min(g_scrollY, maxScrollY);
	si.nPos = g_scrollY;
	SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

	InvalidateRect(hWnd, NULL, TRUE);
}



//Hàm vẽ chính (GDI+ Paint Handler)
VOID OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Gdiplus::Graphics graphics(hdc);

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.Clear(Gdiplus::Color(255, 255, 255));

	if (g_svgDocument && g_svgDocument->getRootGroup()) {

		SVGGroup* rootGroup = g_svgDocument->getRootGroup();

		// 1. Lấy kích thước cửa sổ hiện tại (Viewport Rendering)
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		float actualViewportW = (float)clientRect.right;
		float actualViewportH = (float)clientRect.bottom;

		// Lấy thông số ViewBox/Kích thước SVG
		float viewBoxX = rootGroup->getViewBoxX();
		float viewBoxY = rootGroup->getViewBoxY();
		float viewBoxW = rootGroup->getViewBoxW();
		float viewBoxH = rootGroup->getViewBoxH();

		float svgWidth = rootGroup->getWidth();
		float svgHeight = rootGroup->getHeight();

		// Xử lý ViewBox bị thiếu (Dùng Kích thước SVG hoặc Kích thước cửa sổ)
		if (viewBoxW <= 0.0f || viewBoxH <= 0.0f) {
			// Ưu tiên dùng kích thước width/height của thẻ <svg>
			if (svgWidth > 0.0f && svgHeight > 0.0f) {
				viewBoxW = svgWidth;
				viewBoxH = svgHeight;
			}
			else {
				// Nếu không có gì, dùng kích thước cửa sổ làm ViewBox (hành vi mặc định)
				viewBoxW = actualViewportW > 0.0f ? actualViewportW : 1.0f;
				viewBoxH = actualViewportH > 0.0f ? actualViewportH : 1.0f;
			}
			viewBoxX = 0.0f;
			viewBoxY = 0.0f;
		}


		// 2. TÍNH TOÁN TỶ LỆ RENDER (render_scale)
		float scaleX_base = actualViewportW / viewBoxW;
		float scaleY_base = actualViewportH / viewBoxH;
		float final_scale = std::min(scaleX_base, scaleY_base);
		// cout << final_scale;
		if (final_scale <= 1.0f) final_scale = std::min(final_scale, 0.8f);
		//final_scale = 1.0f;

		// --- THÊM: ÁP DỤNG USER ZOOM ---
		final_scale *= g_userZoom;
		// -------------------------------

		// Tính toán kích thước sau khi render (dùng render_scale)
		float scaledW = viewBoxW * final_scale;
		float scaledH = viewBoxH * final_scale;

		// Tính toán Offset để căn giữa (chỉ xảy ra khi render_scale = final_scale)
		float offsetX = (actualViewportW - scaledW) / 2.0f;
		float offsetY = (actualViewportH - scaledH) / 2.0f;

		// Logic cũ: nếu scroll thì không căn giữa kiểu này nữa, reset về 0 để scroll hoạt động
		if (offsetX < 0) offsetX = 0;
		if (offsetY < 0) offsetY = 0;
		//offsetX = 0.0f;
		//offsetY = 0.0f; 

		// 3. LẤY VỊ TRÍ CUỘN HIỆN TẠI
		int scrollX = GetScrollPos(hWnd, SB_HORZ);
		int scrollY = GetScrollPos(hWnd, SB_VERT);

		// 4. ÁP DỤNG BIẾN ĐỔI GDI+

		// B1: Dịch chuyển ngược ViewBox Origin (áp dụng đầu tiên)
		graphics.TranslateTransform(-viewBoxX, -viewBoxY);

		// B2: Áp dụng Tỷ lệ RENDER
		graphics.ScaleTransform(final_scale, final_scale, Gdiplus::MatrixOrderAppend);

		// B3: Áp dụng Tịnh tiến (Centering và Scroll)
		// Dịch chuyển theo đơn vị Viewport (Pixel)
		// Offset (Căn giữa) - Vị trí Cuộn
		graphics.TranslateTransform(offsetX - (float)scrollX, offsetY - (float)scrollY, Gdiplus::MatrixOrderAppend);

		// --- THÊM MỚI: Xử lý Rotate (Xoay quanh tâm cửa sổ) ---
		if (g_rotation != 0.0f) {
			float centerX = actualViewportW / 2.0f;
			float centerY = actualViewportH / 2.0f;
			// Dịch về tâm cửa sổ -> Xoay -> Dịch ngược lại
			graphics.TranslateTransform(-centerX, -centerY, Gdiplus::MatrixOrderAppend);
			graphics.RotateTransform(g_rotation, Gdiplus::MatrixOrderAppend);
			graphics.TranslateTransform(centerX, centerY, Gdiplus::MatrixOrderAppend);
		}
		// -----------------------------------------------------

		// 5. GỌI HÀM RENDER CHÍNH
		rootGroup->render(g_renderer, graphics, g_svgDocument->context);
	}
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Khai báo biến trên đỉnh switch 
	int bar;
	int newPos;
	SCROLLINFO si = { 0 };
	int maxPos = 0;

	switch (message) {
	case WM_PAINT:
		OnPaint(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		// Thay vì viết code tính toán ở đây, ta gọi hàm UpdateScrollBars
		UpdateScrollBars(hWnd);
		break;

		// --- THÊM MỚI: Xử lý Lăn chuột (ZOOM) ---
	case WM_MOUSEWHEEL:
	{
		float delta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (delta > 0) g_userZoom *= 1.1f; // Zoom in
		else           g_userZoom /= 1.1f; // Zoom out

		if (g_userZoom < 0.1f) g_userZoom = 0.1f;

		// Cập nhật lại thanh cuộn và vẽ lại
		UpdateScrollBars(hWnd);
		return 0;
	}
	// ----------------------------------------

	// --- THÊM MỚI: Xử lý Bàn phím (ROTATE & HELP) ---
	case WM_KEYDOWN:
	{
		bool changed = false;
		switch (wParam) {
		case 'L': // Xoay Trái
			g_rotation -= 5.0f;
			changed = true;
			break;
		case 'R': // Xoay Phải
			g_rotation += 5.0f;
			changed = true;
			break;
		case 'X': // Reset
			g_rotation = 0.0f;
			g_userZoom = 1.0f;
			g_scrollX = 0; g_scrollY = 0;
			UpdateScrollBars(hWnd);
			changed = true;
			break;
		case 'H': // Help
			ShowHelp(hWnd);
			break;
		}
		if (changed) InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	// ------------------------------------------------

	case WM_HSCROLL:
	case WM_VSCROLL:
		bar = (message == WM_HSCROLL) ? SB_HORZ : SB_VERT;
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, bar, &si);
		newPos = si.nPos;

		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			newPos -= 20;
			break;
		case SB_LINERIGHT:
			newPos += 20;
			break;
		case SB_PAGELEFT:
			newPos -= si.nPage;
			break;
		case SB_PAGERIGHT:
			newPos += si.nPage;
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			newPos = HIWORD(wParam);
			break;
		}

		maxPos = (int)si.nMax - (int)si.nPage;
		maxPos = std::max(0, maxPos);

		si.fMask = SIF_POS;
		si.nPos = std::max(si.nMin, std::min(newPos, maxPos));

		if (si.nPos != GetScrollPos(hWnd, bar)) {
			SetScrollInfo(hWnd, bar, &si, TRUE);

			if (bar == SB_HORZ) {
				g_scrollX = si.nPos;
			}
			else {
				g_scrollY = si.nPos;
			}

			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Hàm Main (Điểm Khởi chạy Ứng dụng) - Đã đổi thành wWinMain cho Unicode
INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT iCmdShow) { // Đã sửa tham số thứ 2 và 3

	// ----------------------------------------------------
	// 1. KHỞI TẠO GDI+ VÀ PHÂN TÍCH FILE SVG
	// ----------------------------------------------------
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	try {
		// Tạo Document
		g_svgDocument = new SVGDocument(SVG_FILENAME);

		// Tải file và xây dựng cây đối tượng (truyền g_parser và g_factory)
		g_svgDocument->parseSVGImage(g_parser, g_factory);

		g_svgDocument->resolveReferences();
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
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Đăng ký lớp cửa sổ
	if (!RegisterClassEx(&wc)) {
		GdiplusShutdown(gdiplusToken);
		return 0;
	}

	// Tạo cửa sổ
	//HWND hWnd = CreateWindow(
	//	WINDOW_CLASS_NAME,
	//	WINDOW_TITLE,
	//	WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
	//	NULL, NULL, hInstance, NULL
	//);
	HWND hWnd = CreateWindowEx(
		0,
		WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL
	);

	if (hWnd == NULL) {
		GdiplusShutdown(gdiplusToken);
		return 0;
	}

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	// --- HIỂN THỊ BẢNG HƯỚNG DẪN LẦN ĐẦU ---
	ShowHelp(hWnd);
	// ---------------------------------------

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