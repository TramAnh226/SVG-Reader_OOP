//#ifndef NOMINMAX
//#define NOMINMAX 
//#endif
//
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0600
//#endif
//
//// Sắp xếp thứ tự include để tránh xung đột GDI+/Windows API
//#include <WinSock2.h>
//#include <Windows.h>
//#include <wingdi.h> // Cần thiết để đảm bảo các hằng số GDI được định nghĩa
//#include <gdiplus.h>
//
//#include <iostream>
//#include <string>
//
//// Kỹ thuật Include: Giả định các thư mục cha đã được cấu hình trong Project Properties
//#include "SVGDocument.h"
//#include "SVGParser.h"
//#include "SVGRenderer.h"
//#include "SVGGroup.h"
//#include "SVGFactoryPattern.h" // Thêm Factory để đảm bảo nó được nhìn thấy
////#include "SVGDocumentContext.h"
//
//// Cần cho các hàm của Gdiplus
//using namespace Gdiplus;
//using namespace std;
//
//// BẮT BUỘC: Đảm bảo các thư viện Windows được liên kết chính xác
//#ifdef _MSC_VER
//#pragma comment (lib,"Gdiplus.lib")
//#pragma comment (lib,"user32.lib")
//#pragma comment (lib,"gdi32.lib")
//#pragma comment (lib,"kernel32.lib")
//#endif
//
//// Tên file SVG để tải
////const std::string SVG_FILENAME = "D:\\Downloads\\sample.svg";
//const std::string SVG_FILENAME = "D:\\Downloads\\TestCases-20251205T140549Z-3-001\\TestCases\\svg-01.svg";
////const std::string SVG_FILENAME = "D:\\Downloads\\csc10003-svg-reader-main\\csc10003-svg-reader-main\\external\\samples\\TestCases\\TikTok_logo.svg";
//
//
//// Đã chuyển sang Unicode (LPCWSTR)
//const LPCWSTR WINDOW_CLASS_NAME = L"SVGReaderWindow";
//const LPCWSTR WINDOW_TITLE = L"SVG Reader Demo";
//
//// Khai báo Hàm xử lý Thông điệp Cửa sổ (cần thiết cho WndProc)
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//// Khai báo Toàn cục
//SVGDocument* g_svgDocument = nullptr;
//SVGParser g_parser;
//SVGRenderer g_renderer;
//SVGFactoryPattern g_factory; // Factory để phân tích cú pháp
//ULONG_PTR gdiplusToken;
//
//
//
//// Hàm vẽ chính (GDI+ Paint Handler)
////VOID OnPaint(HWND hWnd) {
////	PAINTSTRUCT ps;
////	HDC hdc = BeginPaint(hWnd, &ps);
////
////	Graphics graphics(hdc);
////
////	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
////	graphics.Clear(Gdiplus::Color(255, 255, 255));
////
////	if (g_svgDocument && g_svgDocument->getRootGroup()) {
////		// Gọi hàm render chính
////		g_svgDocument->renderSVGImage(g_renderer, graphics);
////	}
////
////	EndPaint(hWnd, &ps);
////}
//
//// Trong main.cpp
////VOID OnPaint(HWND hWnd) {
////	PAINTSTRUCT ps;
////	HDC hdc = BeginPaint(hWnd, &ps);	Graphics graphics(hdc);
////
////	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
////	graphics.Clear(Gdiplus::Color(255, 255, 255)); // Tô nền cửa sổ màu trắng
////
////	if (g_svgDocument && g_svgDocument->getRootGroup()) {
////		// --- BỔ SUNG LOGIC VIEWPORT & TRANSFORMATION Ở ĐÂY ---
////		// 1. Lấy kích thước cửa sổ hiện tại (Viewport)
////		RECT clientRect;
////		GetClientRect(hWnd, &clientRect);
////		float viewportW = (float)clientRect.right;
////		float viewportH = (float)clientRect.bottom;
////		// 2. Lấy ViewBox (giả sử SVGDocument có các hàm này)
////		// Ví dụ: ViewBox viewBox = g_svgDocument->getViewBox();
////		// Giả định ViewBox là [0, 0, 800, 400] dựa trên file SVG (height="400", width="800")
////		float viewBoxW = 1500.0f; // Lấy từ thuộc tính SVG hoặc ViewBox parser
////		float viewBoxH = 750.0f; // Lấy từ thuộc tính SVG hoặc ViewBox parser
////		// 3. Tính toán tỷ lệ (Scale) để vừa vặn
////		float scaleX = viewportW / viewBoxW;
////		float scaleY = viewportH / viewBoxH;
////		float scale = std::min(scaleX, scaleY);
////		// 4. Áp dụng phép biến đổi
////		graphics.ScaleTransform(scale, scale);
////		// Cần thêm Translate Transform nếu bạn muốn căn giữa (chúng ta bỏ qua bước này để đơn giản)
////		// ----------------------------------------------------
////		g_svgDocument->renderSVGImage(g_renderer, graphics, g_svgDocument->context);
////	}
////	EndPaint(hWnd, &ps);
////}
//
//// Trong main.cpp
//VOID OnPaint(HWND hWnd) {
//	PAINTSTRUCT ps;
//	HDC hdc = BeginPaint(hWnd, &ps);
//	Graphics graphics(hdc);
//
//	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//	graphics.Clear(Gdiplus::Color(255, 255, 255)); // Tô nền cửa sổ màu trắng
//
//	if (g_svgDocument && g_svgDocument->getRootGroup()) {
//		// --- BỔ SUNG LOGIC VIEWPORT & TRANSFORMATION Ở ĐÂY ---
//
//		// 1. Lấy kích thước cửa sổ hiện tại (Viewport)
//		RECT clientRect;
//		GetClientRect(hWnd, &clientRect);
//		float viewportW = (float)clientRect.right;
//		float viewportH = (float)clientRect.bottom;
//
//		// 2. Lấy ViewBox
//		// Giả định ViewBox là [0, 0, 1500, 750] (dựa trên giả định của bạn)
//		//float viewBoxW = 1500.0f;
//		//float viewBoxH = 750.0f;
//		float viewBoxH = g_svgDocument->getRootGroup()->getHeight();
//		float viewBoxW = g_svgDocument->getRootGroup()->getWidth();
//
//		// 3. Tính toán tỷ lệ (Scale) để vừa vặn
//		float scaleX = viewportW / viewBoxW;
//		float scaleY = viewportH / viewBoxH;
//		float scale = std::min(scaleX, scaleY);
//
//		// --- LOGIC CĂN GIỮA (CENTERING LOGIC) ---
//
//		// 4. Tính toán kích thước SVG sau khi scale
//		float scaledW = viewBoxW * scale;
//		float scaledH = viewBoxH * scale;
//
//		// 5. Tính toán khoảng trống (Padding)
//		float paddingX = viewportW - scaledW;
//		float paddingY = viewportH - scaledH;
//
//		// 6. Tính toán Offset (dịch chuyển)
//		float offsetX = paddingX / 2.0f;
//		float offsetY = paddingY / 2.0f;
//
//		// 7. Áp dụng Biến đổi cho Graphics Context
//
//		// THỨ TỰ QUAN TRỌNG: Translate phải thực hiện SAU Scale để áp dụng dịch chuyển trên màn hình
//		// A. Áp dụng Tịnh tiến (để căn giữa)
//		graphics.TranslateTransform(offsetX, offsetY);
//
//		// B. Áp dụng Tỉ lệ (để vừa vặn)
//		//graphics.ScaleTransform(scale, scale);
//
//		// ----------------------------------------------------
//		//g_svgDocument->resolveReferences();
//		g_svgDocument->renderSVGImage(g_renderer, graphics, g_svgDocument->context);
//	}
//	EndPaint(hWnd, &ps);
//}
//// Xử lý Thông điệp Cửa sổ (Windows Message Handler)
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//	switch (message) {
//	case WM_PAINT:
//		OnPaint(hWnd);
//		return 0;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	case WM_SIZE:
//		InvalidateRect(hWnd, NULL, TRUE);
//		return 0;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//}
//
//// Hàm Main (Điểm Khởi chạy Ứng dụng) - Đã đổi thành wWinMain cho Unicode
//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT iCmdShow) { // Đã sửa tham số thứ 2 và 3
//
//	// ----------------------------------------------------
//	// 1. KHỞI TẠO GDI+ VÀ PHÂN TÍCH FILE SVG
//	// ----------------------------------------------------
//	GdiplusStartupInput gdiplusStartupInput;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//
//	//try {
//	//	// Tạo Document
//	//	g_svgDocument = new SVGDocument(SVG_FILENAME);
//
//	//	// Tải file và xây dựng cây đối tượng (truyền g_factory)
//	//	g_svgDocument->parseSVGImage(g_parser, g_factory);
//	//	if (!g_svgDocument->getRootGroup()) {
//	//		std::cerr << "Error: SVG parsing failed or root group is empty." << std::endl;
//	//	}
//	//	//if (g_svgDocument && g_svgDocument->getRootGroup()) {
//	//	//	g_svgDocument->getRootGroup()->printDebugAttributes();
//	//	//}
//	//}
//	//catch (const std::exception& e) {
//	//	std::cerr << "Parsing Exception: " << e.what() << std::endl;
//	//	GdiplusShutdown(gdiplusToken);
//	//	return 1;
//	//}
//
//	try {
//		// Tạo Document
//		g_svgDocument = new SVGDocument(SVG_FILENAME);
//
//		// Tải file và xây dựng cây đối tượng (truyền g_parser và g_factory)
//		g_svgDocument->parseSVGImage(g_parser, g_factory);
//
//		// -----------------------------------------------------------------
//		// <<< ĐIỂM GỌI HÀM DEBUG CHÍNH XÁC >>>
//		// -----------------------------------------------------------------
//		//if (g_svgDocument && g_svgDocument->getRootGroup()) {
//
//		//	std::cerr << "\n========== START PARSING DEBUG ==========" << std::endl;
//
//		//	// Gọi hàm debug đệ quy của đối tượng Group gốc, truyền std::cerr làm output
//		//	g_svgDocument->getRootGroup()->printDebugAttributes(std::cerr, 0);
//
//		//	std::cerr << "========== END PARSING DEBUG ==========\n" << std::endl;
//		//}
//		// -----------------------------------------------------------------
//
//		// print context
//		//SVGDocumentContext context = g_svgDocument->context;
//		//std::unordered_map<std::string, SVGGradient*> gradDef = context.gradientDefinitions;
//		//for (auto it : gradDef) {
//		//	cout << it.first << ": " << it.second << '\n';
//		//}
//		// print resolve
//		//cout << "AFTER RESOLVE\n";
//		g_svgDocument->resolveReferences();
//		if (g_svgDocument && g_svgDocument->getRootGroup()) {
//
//			std::cerr << "\n========== START PARSING DEBUG ==========" << std::endl;
//
//			// Gọi hàm debug đệ quy của đối tượng Group gốc, truyền std::cerr làm output
//			g_svgDocument->getRootGroup()->printDebugAttributes(std::cerr, 0);
//
//			std::cerr << "========== END PARSING DEBUG ==========\n" << std::endl;
//		}
//	}
//	catch (const std::exception& e) {
//		// ...
//	}
//
//
//	// ----------------------------------------------------
//	// 2. KHỞI TẠO CỬA SỔ WINDOWS
//	// ----------------------------------------------------
//	WNDCLASSEX wc = {};
//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	wc.lpfnWndProc = WndProc; // <<< BỎ COMMENT/GÁN
//	wc.hInstance = hInstance;
//	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	wc.lpszClassName = WINDOW_CLASS_NAME; // <<< BỎ COMMENT/GÁN
//	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//
//	// Đăng ký lớp cửa sổ
//	if (!RegisterClassEx(&wc)) {
//		GdiplusShutdown(gdiplusToken);
//		return 0;
//	}
//
//	// Tạo cửa sổ
//	HWND hWnd = CreateWindow(
//		WINDOW_CLASS_NAME,
//		WINDOW_TITLE,
//		WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
//		NULL, NULL, hInstance, NULL
//	);
//
//	if (hWnd == NULL) {
//		GdiplusShutdown(gdiplusToken);
//		return 0;
//	}
//
//	ShowWindow(hWnd, iCmdShow);
//	UpdateWindow(hWnd);
//
//	// ----------------------------------------------------
//	// 3. VÒNG LẶP THÔNG ĐIỆP (MAIN LOOP)
//	// ----------------------------------------------------
//	MSG Msg;
//	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
//		TranslateMessage(&Msg);
//		DispatchMessage(&Msg);
//	}
//
//	// ----------------------------------------------------
//	// 4. DỌN DẸP
//	// ----------------------------------------------------
//	delete g_svgDocument;
//	GdiplusShutdown(gdiplusToken);
//
//	return static_cast<INT>(Msg.wParam);
//}

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
const std::string SVG_FILENAME = "C:\\Users\\LENOVO\\Downloads\\TestCases-20251204T035050Z-1-001\\TestCases\\svg-21.svg";

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
VOID OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Gdiplus::Graphics graphics(hdc);

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.Clear(Gdiplus::Color(255, 255, 255)); // Tô nền cửa sổ màu trắng

	if (g_svgDocument && g_svgDocument->getRootGroup()) {

		// Lấy đối tượng Group gốc (chứa Viewport và ViewBox)
		SVGGroup* rootGroup = g_svgDocument->getRootGroup();

		// 1. Lấy kích thước cửa sổ hiện tại (Viewport Rendering)
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);/*
		float viewportW = (float)clientRect.right;
		float viewportH = (float)clientRect.bottom;*/

		float viewportW = rootGroup->getWidth();
		float viewportH = rootGroup->getHeight();

		// --- LẤY THÔNG SỐ SVG VIEWBOX ---
		float viewBoxX = rootGroup->getViewBoxX();
		float viewBoxY = rootGroup->getViewBoxY();
		float viewBoxW = rootGroup->getViewBoxW();
		float viewBoxH = rootGroup->getViewBoxH();

		// Kiểm tra ViewBox hợp lệ
		if (viewBoxW <= 0.0f || viewBoxH <= 0.0f) {
			// Nếu ViewBox không hợp lệ, dùng Viewport làm ViewBox (Tỉ lệ 1:1)
			viewBoxW = viewportW;
			viewBoxH = viewportH;
			viewBoxX = 0.0f;
			viewBoxY = 0.0f;
		}

		// 2. TÍNH TOÁN TỶ LỆ (Scale Factor - Preserve Aspect Ratio: meet)

		// Tỷ lệ cần thiết trên trục X và Y
		float scaleX = viewportW / viewBoxW;
		float scaleY = viewportH / viewBoxH;

		// Chọn tỷ lệ nhỏ hơn để đảm bảo toàn bộ ViewBox nằm gọn trong Viewport ("meet")
		float scale = std::min(scaleX, scaleY);

		// 3. TÍNH TOÁN DỊCH CHUYỂN (Centering Logic)

		// Kích thước ViewBox sau khi Scale
		float scaledW = viewBoxW * scale;
		float scaledH = viewBoxH * scale;

		// Khoảng trống còn lại (Padding)
		float paddingX = viewportW - scaledW;
		float paddingY = viewportH - scaledH;

		// Offset (Dịch chuyển để căn giữa)
		float offsetX = paddingX / 2.0f;
		float offsetY = paddingY / 2.0f;


		// 4. ÁP DỤNG BIẾN ĐỔI GDI+

		// Tái tạo ma trận biến đổi (Gdiplus::Matrix)
		Gdiplus::Matrix transformMatrix;

		// Bước A: Tịnh tiến để căn giữa trong Viewport
		transformMatrix.Translate(offsetX, offsetY);

		// Bước B: Áp dụng tỉ lệ
		transformMatrix.Scale(scale, scale);

		// Bước C: Dịch chuyển nội dung ViewBox để căn chỉnh gốc (0, 0)
		// Dịch chuyển nội dung ViewBox sao cho (viewBoxX, viewBoxY) trở thành (0, 0)
		transformMatrix.Translate(-viewBoxX, -viewBoxY);

		// Áp dụng Ma trận tổng hợp lên Graphics Context
		graphics.MultiplyTransform(&transformMatrix);

		// 5. GỌI HÀM RENDER CHÍNH
		g_svgDocument->renderSVGImage(g_renderer, graphics, g_svgDocument->context);
	}
	EndPaint(hWnd, &ps);
}
//VOID OnPaint(HWND hWnd) {
//	PAINTSTRUCT ps;
//	// Bắt đầu quá trình vẽ và lấy Graphics Context (HDC)
//	HDC hdc = BeginPaint(hWnd, &ps);
//
//	// Tạo đối tượng Graphics từ HDC
//	Gdiplus::Graphics graphics(hdc);
//
//	// Bật chế độ làm mịn (Anti-aliasing)
//	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//
//	// Xóa nền cửa sổ bằng màu trắng
//	graphics.Clear(Gdiplus::Color(255, 255, 255));
//
//	// Kiểm tra xem tài liệu SVG đã được tải và có Group gốc chưa
//	if (g_svgDocument && g_svgDocument->getRootGroup()) {
//
//		// 1. Lấy kích thước cửa sổ hiện tại (Viewport)
//		RECT clientRect;
//		GetClientRect(hWnd, &clientRect);
//		float viewportW = (float)clientRect.right;
//		float viewportH = (float)clientRect.bottom;
//
//		// 2. Lấy kích thước cơ sở của SVG (ViewBox hoặc W/H của thẻ <svg>)
//		// Tùy chọn 1: Lấy từ thuộc tính ViewBox của SVGDocument (Tốt nhất)
//		// Tùy chọn 2: Sử dụng các giá trị mặc định/giả định như trong code của bạn
//		// Giả định ViewBox là [0, 0, 1500, 750] (hoặc lấy từ g_svgDocument->getRootGroup())
//		float viewBoxW = g_svgDocument->getRootGroup()->getWidth() > 0 ? g_svgDocument->getRootGroup()->getWidth() : 1500.0f;
//		float viewBoxH = g_svgDocument->getRootGroup()->getHeight() > 0 ? g_svgDocument->getRootGroup()->getHeight() : 750.0f;
//
//		// 3. Tính toán tỷ lệ (Scale) để vừa vặn (Aspect Ratio Preservation - meet)
//		float scaleX = viewportW / viewBoxW;
//		float scaleY = viewportH / viewBoxH;
//		float scale = std::min(scaleX, scaleY); // Lấy tỷ lệ nhỏ hơn để đảm bảo toàn bộ hình ảnh hiển thị
//
//		// --- LOGIC CĂN GIỮA (CENTERING LOGIC) ---
//
//		// 4. Tính toán kích thước SVG sau khi scale
//		float scaledW = viewBoxW * scale;
//		float scaledH = viewBoxH * scale;
//
//		// 5. Tính toán khoảng trống (Padding)
//		float paddingX = viewportW - scaledW;
//		float paddingY = viewportH - scaledH;
//
//		// 6. Tính toán Offset (dịch chuyển)
//		float offsetX = paddingX / 2.0f;
//		float offsetY = paddingY / 2.0f;
//
//		// 7. Áp dụng Biến đổi cho Graphics Context
//
//		// GDI+ sử dụng MatrixOrderPrepend (nhân ma trận mới vào bên trái) theo mặc định.
//		// Để đạt được hiệu ứng: [Tịnh tiến Căn giữa] * [Tỷ lệ] * [Tọa độ SVG]
//		// Cần gọi Tỷ lệ TRƯỚC (nhân bên phải nhất), sau đó gọi Tịnh tiến.
//
//		// A. Áp dụng Tỷ lệ (Scale)
//		// Đây là phép biến đổi đầu tiên tác động lên tọa độ SVG.
//		graphics.ScaleTransform(scale, scale);
//
//		// B. Áp dụng Tịnh tiến (Translate)
//		// Phép tịnh tiến này áp dụng cho kết quả đã được scale, dịch chuyển trên màn hình.
//		// Chúng ta muốn tịnh tiến theo đơn vị Pixel của màn hình, không phải đơn vị SVG đã scale.
//		// Vì đã dùng ScaleTransform(scale, scale) trước đó, TranslateTransform(offsetX, offsetY) 
//		// sẽ bị nhân với nghịch đảo của scale nếu dùng Prepend.
//		// CÁCH KHẮC PHỤC: Sử dụng MatrixOrderAppend hoặc chia cho scale.
//
//		// Dùng MatrixOrderAppend để đảm bảo Translate hoạt động trên tọa độ màn hình (Pixel)
//		graphics.TranslateTransform(offsetX, offsetY, Gdiplus::MatrixOrderAppend);
//
//		//graphics.TranslateTransform(-viewBoxX, -viewBoxY, Gdiplus::MatrixOrderPrepend);
//
//		// 8. Gọi hàm render chính
//		g_svgDocument->renderSVGImage(g_renderer, graphics, g_svgDocument->context);
//	}
//
//	// Kết thúc quá trình vẽ
//	EndPaint(hWnd, &ps);
//}
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
		//if (g_svgDocument && g_svgDocument->getRootGroup()) {

		//	std::cerr << "\n========== START PARSING DEBUG ==========" << std::endl;

		//	// Gọi hàm debug đệ quy của đối tượng Group gốc, truyền std::cerr làm output
		//	g_svgDocument->getRootGroup()->printDebugAttributes(std::cerr, 0);

		//	std::cerr << "========== END PARSING DEBUG ==========\n" << std::endl;
		//}
		// -----------------------------------------------------------------

		// print context
		SVGDocumentContext context = g_svgDocument->context;
		std::unordered_map<std::string, SVGGradient*> gradDef = context.gradientDefinitions;
		for (auto it : gradDef) {
			cout << it.first << ": " << it.second << '\n';
		}
		// print resolve
		cout << "AFTER RESOLVE\n";
		g_svgDocument->resolveReferences();
		if (g_svgDocument && g_svgDocument->getRootGroup()) {

			std::cerr << "\n========== START PARSING DEBUG ==========" << std::endl;

			// Gọi hàm debug đệ quy của đối tượng Group gốc, truyền std::cerr làm output
			g_svgDocument->getRootGroup()->printDebugAttributes(std::cerr, 0);

			std::cerr << "========== END PARSING DEBUG ==========\n" << std::endl;
		}
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