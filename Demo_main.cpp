#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <string>

#include "SVGDocument/SVG_H/SVGDocument.h"
#include "SVGParser/SVGParser/include/SVGParser.h" 
#include "SVGRenderer/SVGRenderer.h"

using namespace Gdiplus;

SVGDocument* g_svgDocument = nullptr;
SVGParser g_parser;
SVGRenderer g_renderer; 
ULONG_PTR gdiplusToken;

LPCWSTR g_szClassName = L"SVGReaderApp"; 
const std::string SVG_FILENAME = "sample.svg"; 


// Hàm vẽ chính (GDI+ Paint Handler)
void OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    
    // Khởi tạo đối tượng Graphics của GDI+
    Graphics graphics(hdc);

    // Kích hoạt Antialiasing (làm mịn) cho chất lượng tốt hơn
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // Xóa nền (Tùy chọn: màu trắng)
    graphics.Clear(Gdiplus::Color(255, 255, 255));

    if (g_svgDocument && g_svgDocument->getRootGroup()) {
        // GỌI HÀM RENDER CẤP CAO NHẤT
        // renderSVGImage gọi render.renderFigure, truyền Graphics& g.
        g_svgDocument->renderSVGImage(g_renderer, graphics); 
    }

    EndPaint(hWnd, &ps);
}

// Xử lý Thông điệp Cửa sổ (Windows Message Handler)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT:
        OnPaint(hWnd); // Gọi hàm vẽ khi cửa sổ yêu cầu
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0); // Đóng ứng dụng
        return 0;
    case WM_SIZE:
        // Yêu cầu vẽ lại khi cửa sổ thay đổi kích thước
        InvalidateRect(hWnd, NULL, TRUE); 
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

// Hàm Main (Điểm Khởi chạy Ứng dụng)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    // ----------------------------------------------------
    // 1. KHỞI TẠO GDI+
    // ----------------------------------------------------
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // ----------------------------------------------------
    // 2. TẢI FILE SVG VÀ PARSE
    // ----------------------------------------------------
    try {
        g_svgDocument = new SVGDocument(SVG_FILENAME);
        g_svgDocument->parseSVGImage(g_parser);
        
        if (!g_svgDocument->getRootGroup()) {
            std::cerr << "Error: SVG parsing failed or root group is empty." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Parsing Exception: " << e.what() << std::endl;
        // Xử lý lỗi và thoát
        return 1; 
    }


    // ----------------------------------------------------
    // 3. KHỞI TẠO CỬA SỔ WINDOWS
    // ----------------------------------------------------
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc; // Đặt hàm xử lý thông điệp
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wc)) {
        return 0;
    }

    HWND hWnd = CreateWindowEx(
        0,
        g_szClassName,
        L"SVG Reader", // Tiêu đề cửa sổ
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // Kích thước mặc định
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hWnd == NULL) {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // ----------------------------------------------------
    // 4. VÒNG LẶP THÔNG ĐIỆP (MAIN LOOP)
    // ----------------------------------------------------
    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    
    // ----------------------------------------------------
    // 5. DỌN DẸP
    // ----------------------------------------------------
    delete g_svgDocument;
    GdiplusShutdown(gdiplusToken);
    
    return Msg.wParam;
}