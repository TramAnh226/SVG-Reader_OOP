/*#include <windows.h>
#include <gdiplus.h>
#include "SVGRender.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

ULONG_PTR gdiToken;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics graphics(hdc);

        //SolidBrush backgroundBrush(Color(255, 255, 255, 255)); // màu trắng
        //graphics.FillRectangle(&backgroundBrush, 0, 0, 800, 600);

        SVGRenderer renderer(&graphics);
        renderer.DrawCircle(400, 200, 60, Color(255, 0, 0, 255), Color(255, 0, 0, 0), 3);
        renderer.DrawRect(100, 100, 200, 120, Color(255, 0, 255, 0), Color(255, 0, 0, 0), 2);
       
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SVGRenderTest";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// nền trắng 
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, L"SVGRenderTest", L"GDI+ Renderer",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiToken);
    return 0;
}*/

#include <windows.h>
#include <gdiplus.h>
#include "Shape.h"
#include "SvgParser.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

ULONG_PTR gdiToken;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics graphics(hdc);

        SVGParser parser;
        if (parser.loadFromFile("example.svg")) {
            for (auto shape : parser.getShapes())
                shape->draw(graphics);
        }

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SVGParserDemo";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, L"SVGParserDemo", L"SVG Reader with RapidXML",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiToken);
    return 0;
}
