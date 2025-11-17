// #include "stdafx.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

// Kỹ thuật Include: Giả định các thư mục cha đã được cấu hình trong Project Properties
#include "SVGDocument/SVG_H/SVGDocument.h"
#include "SVGParser/SVGParser/include/SVGParser.h"
#include "SVGRenderer/SVGRenderer.h"
#include "SVGGroup_Factory/SVG_H/SVGGroup.h"
#include "SVGElement/SVG_H/Library.h" 

// Cần cho các hàm của Gdiplus
using namespace Gdiplus;
using namespace std;

#ifdef _MSC_VER
#pragma comment (lib,"Gdiplus.lib")
#endif

// Tên file SVG để tải
const std::string SVG_FILENAME = "D:\\Downloads\\sample.svg";
const LPCSTR WINDOW_CLASS_NAME = "SVGReaderWindow";
const LPCSTR WINDOW_TITLE = "SVG Reader Demo";

// Khai báo Toàn cục để sử dụng trong WndProc và OnPaint
SVGDocument* g_svgDocument = nullptr;
SVGParser g_parser;
SVGRenderer g_renderer;
ULONG_PTR gdiplusToken;


// Hàm vẽ chính (GDI+ Paint Handler)
VOID OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    
    // Khởi tạo đối tượng Graphics của GDI+
    Graphics graphics(hdc);

    // Kích hoạt Antialiasing (làm mịn)
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.Clear(Gdiplus::Color(255, 255, 255)); // Xóa nền trắng

    if (g_svgDocument && g_svgDocument->getRootGroup()) {
        // GỌI HÀM RENDER CẤP CAO NHẤT (SVGDocument sẽ điều phối)
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
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow) {
    
    // ----------------------------------------------------
    // 1. KHỞI TẠO GDI+ VÀ PHÂN TÍCH FILE SVG
    // ----------------------------------------------------
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    try {
        // Tạo Document, Parser
        g_svgDocument = new SVGDocument(SVG_FILENAME);
        
        // Tải file và xây dựng cây đối tượng (sử dụng SVGParser)
        g_svgDocument->parseSVGImage(g_parser);
        
        if (!g_svgDocument->getRootGroup()) {
            std::cerr << "Error: SVG parsing failed or root group is empty." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Parsing Exception: " << e.what() << std::endl;
        GdiplusShutdown(gdiplusToken);
        return 1; 
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
    
    if (!RegisterClassEx(&wc)) {
        GdiplusShutdown(gdiplusToken);
        return 0;
    }

    HWND hWnd = CreateWindow(
        WINDOW_CLASS_NAME,          // window class name
        WINDOW_TITLE,               // window caption
        WS_OVERLAPPEDWINDOW,        // window style
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // Kích thước mặc định
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
    
    // Explicitly cast WPARAM to INT to avoid narrowing-conversion warnings
    return static_cast<INT>(Msg.wParam);
}