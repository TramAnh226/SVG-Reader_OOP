#ifndef NOMINMAX
#define NOMINMAX 
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

// Include
#include <WinSock2.h>
#include <Windows.h>
#include <wingdi.h> 
#include <gdiplus.h>
#include <shellapi.h> // Cho Command Line

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 

// SVG Project Headers
#include "SVGDocument.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "SVGGroup.h"
#include "SVGFactoryPattern.h" 

using namespace Gdiplus;
using namespace std;

#ifdef _MSC_VER
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"gdi32.lib")
#pragma comment (lib,"kernel32.lib")
#pragma comment (lib,"shell32.lib")
#endif

// Config
const std::string DEFAULT_FILENAME = "D:/Download/HCMUS.svg";
const LPCWSTR WINDOW_CLASS_NAME = L"SVGReaderWindow";
const LPCWSTR WINDOW_TITLE = L"SVG Reader Demo";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Globals
SVGDocument* g_svgDocument = nullptr;
SVGParser g_parser;
SVGRenderer g_renderer;
SVGFactoryPattern g_factory;
ULONG_PTR gdiplusToken;

int g_scrollX = 0;
int g_scrollY = 0;
float g_userZoom = 1.0f;
float g_rotation = 0.0f;

// Chuyển đổi chuỗi
std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// --- HÀM MỚI: VẼ BẢNG HƯỚNG DẪN LÊN GÓC PHẢI DƯỚI ---
void RenderHelpOverlay(Gdiplus::Graphics& graphics, int viewW, int viewH) {
    // 1. Nội dung hướng dẫn
    std::wstring helpText =
        L"Controls:\n"
        L"------------------\n"
        L"Mouse Wheel : Zoom\n"
        L"L / R       : Rotate\n"
        L"Arrows/WASD : Pan\n"
        L"X           : Reset";

    // 2. Cấu hình Font và Brush
    Gdiplus::Font font(L"Consolas", 10, Gdiplus::FontStyleBold); // Font đơn cách cho dễ nhìn
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255)); // Chữ trắng
    Gdiplus::SolidBrush bgBrush(Gdiplus::Color(180, 0, 0, 0));    // Nền đen bán trong suốt (Alpha=180)

    // 3. Tính toán kích thước khung chứa chữ
    Gdiplus::RectF layoutRect(0, 0, 250, 150); // Kích thước ước lượng tối đa
    Gdiplus::RectF boundingBox;
    graphics.MeasureString(helpText.c_str(), -1, &font, layoutRect, NULL, &boundingBox);

    // Thêm padding cho đẹp
    float padding = 10.0f;
    float boxW = boundingBox.Width + padding * 2;
    float boxH = boundingBox.Height + padding * 2;

    // 4. Tính toán vị trí (Góc dưới bên phải)
    float x = (float)viewW - boxW - 20.0f; // Cách lề phải 20px
    float y = (float)viewH - boxH - 20.0f; // Cách lề dưới 20px

    // 5. Vẽ nền và vẽ chữ
    graphics.FillRectangle(&bgBrush, x, y, boxW, boxH);
    graphics.DrawString(helpText.c_str(), -1, &font, PointF(x + padding, y + padding), &textBrush);
}

// Cập nhật Scrollbar
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

    if (viewBoxW <= 0.0f || viewBoxH <= 0.0f) {
        if (svgWidth > 0.0f && svgHeight > 0.0f) { viewBoxW = svgWidth; viewBoxH = svgHeight; }
        else { viewBoxW = clientW > 0 ? (float)clientW : 1.0f; viewBoxH = clientH > 0 ? (float)clientH : 1.0f; }
    }

    float scaleX = (float)clientW / viewBoxW;
    float scaleY = (float)clientH / viewBoxH;
    float final_scale = std::min(scaleX, scaleY);
    if (final_scale <= 1.0f) final_scale = std::min(final_scale, 0.8f);

    final_scale *= g_userZoom;

    int contentW = (int)(viewBoxW * final_scale);
    int contentH = (int)(viewBoxH * final_scale);

    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;

    si.nMin = 0; si.nMax = contentW; si.nPage = clientW;
    g_scrollX = std::min(g_scrollX, std::max(0, contentW - clientW));
    si.nPos = g_scrollX;
    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

    si.nMax = contentH; si.nPage = clientH;
    g_scrollY = std::min(g_scrollY, std::max(0, contentH - clientH));
    si.nPos = g_scrollY;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

    InvalidateRect(hWnd, NULL, TRUE);
}

VOID OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    Gdiplus::Graphics graphics(hdc);

    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.Clear(Gdiplus::Color(255, 255, 255)); // Xóa màn hình màu trắng

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    float viewportW = (float)clientRect.right;
    float viewportH = (float)clientRect.bottom;

    // --- PHẦN 1: VẼ SVG (CÓ BIẾN ĐỔI XOAY/ZOOM) ---
    if (g_svgDocument && g_svgDocument->getRootGroup()) {
        SVGGroup* rootGroup = g_svgDocument->getRootGroup();

        float viewBoxX = rootGroup->getViewBoxX();
        float viewBoxY = rootGroup->getViewBoxY();
        float viewBoxW = rootGroup->getViewBoxW();
        float viewBoxH = rootGroup->getViewBoxH();
        float svgWidth = rootGroup->getWidth();
        float svgHeight = rootGroup->getHeight();

        if (viewBoxW <= 0.0f || viewBoxH <= 0.0f) {
            if (svgWidth > 0.0f && svgHeight > 0.0f) { viewBoxW = svgWidth; viewBoxH = svgHeight; }
            else { viewBoxW = viewportW > 0 ? viewportW : 1.0f; viewBoxH = viewportH > 0 ? viewportH : 1.0f; }
            viewBoxX = 0.0f; viewBoxY = 0.0f;
        }

        float scaleX_base = viewportW / viewBoxW;
        float scaleY_base = viewportH / viewBoxH;
        float final_scale = std::min(scaleX_base, scaleY_base);
        if (final_scale <= 1.0f) final_scale = std::min(final_scale, 0.8f);

        final_scale *= g_userZoom;

        float scaledW = viewBoxW * final_scale;
        float scaledH = viewBoxH * final_scale;
        float offsetX = (viewportW - scaledW) / 2.0f;
        float offsetY = (viewportH - scaledH) / 2.0f;
        if (offsetX < 0) offsetX = 0;
        if (offsetY < 0) offsetY = 0;

        int scrollX = GetScrollPos(hWnd, SB_HORZ);
        int scrollY = GetScrollPos(hWnd, SB_VERT);

        // Áp dụng Transform cho SVG
        graphics.TranslateTransform(-viewBoxX, -viewBoxY);
        graphics.ScaleTransform(final_scale, final_scale, Gdiplus::MatrixOrderAppend);
        graphics.TranslateTransform(offsetX - (float)scrollX, offsetY - (float)scrollY, Gdiplus::MatrixOrderAppend);

        if (g_rotation != 0.0f) {
            float centerX = viewportW / 2.0f;
            float centerY = viewportH / 2.0f;
            graphics.TranslateTransform(-centerX, -centerY, Gdiplus::MatrixOrderAppend);
            graphics.RotateTransform(g_rotation, Gdiplus::MatrixOrderAppend);
            graphics.TranslateTransform(centerX, centerY, Gdiplus::MatrixOrderAppend);
        }

        rootGroup->render(g_renderer, graphics, g_svgDocument->context);
    }

    // --- PHẦN 2: VẼ UI HƯỚNG DẪN (KHÔNG BIẾN ĐỔI) ---
    // Reset ma trận biến đổi về mặc định để vẽ UI đè lên trên và đứng yên
    graphics.ResetTransform();
    RenderHelpOverlay(graphics, (int)viewportW, (int)viewportH);

    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int bar, newPos, maxPos;
    SCROLLINFO si = { 0 };

    switch (message) {
    case WM_PAINT:
        OnPaint(hWnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        UpdateScrollBars(hWnd);
        break;

    case WM_MOUSEWHEEL:
    {
        float delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (delta > 0) g_userZoom *= 1.1f;
        else           g_userZoom /= 1.1f;

        if (g_userZoom < 0.1f) g_userZoom = 0.1f;
        UpdateScrollBars(hWnd);
        return 0;
    }

    case WM_KEYDOWN:
    {
        bool changed = false;
        switch (wParam) {
        case 'L': g_rotation -= 5.0f; changed = true; break;
        case 'R': g_rotation += 5.0f; changed = true; break;
        case 'X':
            g_rotation = 0.0f; g_userZoom = 1.0f;
            g_scrollX = 0; g_scrollY = 0;
            UpdateScrollBars(hWnd);
            changed = true;
            break;
        }
        if (changed) InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_HSCROLL:
    case WM_VSCROLL:
        bar = (message == WM_HSCROLL) ? SB_HORZ : SB_VERT;
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hWnd, bar, &si);
        newPos = si.nPos;

        switch (LOWORD(wParam)) {
        case SB_LINELEFT:      newPos -= 20; break;
        case SB_LINERIGHT:     newPos += 20; break;
        case SB_PAGELEFT:      newPos -= si.nPage; break;
        case SB_PAGERIGHT:     newPos += si.nPage; break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION: newPos = HIWORD(wParam); break;
        }

        maxPos = (int)si.nMax - (int)si.nPage;
        maxPos = std::max(0, maxPos);
        si.fMask = SIF_POS;
        si.nPos = std::max(si.nMin, std::min(newPos, maxPos));

        if (si.nPos != GetScrollPos(hWnd, bar)) {
            SetScrollInfo(hWnd, bar, &si, TRUE);
            if (bar == SB_HORZ) g_scrollX = si.nPos;
            else                g_scrollY = si.nPos;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT iCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    std::string svgFileToOpen = DEFAULT_FILENAME;
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv && argc > 1) {
        std::wstring wArg = argv[1];
        svgFileToOpen = WStringToString(wArg);
    }
    if (argv) LocalFree(argv);

    try {
        g_svgDocument = new SVGDocument(svgFileToOpen);
        g_svgDocument->parseSVGImage(g_parser, g_factory);
        g_svgDocument->resolveReferences();
    }
    catch (...) {}

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

    HWND hWnd = CreateWindowEx(
        0, WINDOW_CLASS_NAME, WINDOW_TITLE,
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

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    delete g_svgDocument;
    GdiplusShutdown(gdiplusToken);

    return static_cast<INT>(Msg.wParam);
}
