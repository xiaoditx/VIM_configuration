#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "vimconfig.h"
#include "guirenderer.h"

// 程序主类
class VimConfigApp {
private:
    HINSTANCE hInstance;
    VimConfig* config;
    GUIRenderer* gui;
    HWND mainWindow;
    
public:
    VimConfigApp(HINSTANCE hInst);
    ~VimConfigApp();
    
    bool Initialize();
    int Run();
    
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

// 实现部分
VimConfigApp::VimConfigApp(HINSTANCE hInst) : hInstance(hInst), config(nullptr), gui(nullptr), mainWindow(nullptr) {
}

VimConfigApp::~VimConfigApp() {
    delete gui;
    delete config;
}

bool VimConfigApp::Initialize() {
    // 创建配置处理对象
    config = new VimConfig();
    
    // 尝试加载默认vimrc文件
    std::string vimrcPath = "c:\\Users\\" + 
        std::string(getenv("USERNAME")) + "\\_vimrc";
    
    // 如果默认路径不存在，使用当前目录下的_vimrc
    if (!config->LoadFromFile(vimrcPath)) {
        config->LoadFromFile(".\\_vimrc");
    }
    
    // 注册窗口类
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TEXT("VimConfigApp");
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, TEXT("注册窗口类失败"), TEXT("错误"), MB_ICONERROR);
        return false;
    }
    
    // 创建主窗口
    mainWindow = CreateWindowEx(
        0, 
        TEXT("VimConfigApp"), 
        TEXT("Vim配置工具 - 类手机设置风格"), 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 
        NULL, NULL, hInstance, this);
    
    if (!mainWindow) {
        MessageBox(NULL, TEXT("创建窗口失败"), TEXT("错误"), MB_ICONERROR);
        return false;
    }
    
    // 创建GUI渲染器
    gui = new GUIRenderer(config, mainWindow);
    gui->Initialize();
    
    // 显示窗口
    ShowWindow(mainWindow, SW_SHOW);
    UpdateWindow(mainWindow);
    
    return true;
}

int VimConfigApp::Run() {
    MSG msg;
    
    // 主消息循环
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}

LRESULT CALLBACK VimConfigApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    VimConfigApp* app = nullptr;
    
    if (message == WM_NCCREATE) {
        // 存储app指针到窗口数据
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        app = (VimConfigApp*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
    } else {
        // 从窗口数据获取app指针
        app = (VimConfigApp*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }
    
    switch (message) {
    case WM_COMMAND:
        if (app && app->gui) {
            app->gui->HandleCommand(wParam, lParam);
        }
        break;
    case WM_VSCROLL:
        if (app && app->gui) {
            // 获取消息来自哪个窗口
            HWND scrollWindow = (HWND)lParam;
            if (app->gui->HandleScrollMessage(scrollWindow, wParam, lParam)) {
                return 0;
            }
        }
        break;
    case WM_MOUSEWHEEL:
        if (app && app->gui) {
            if (app->gui->HandleMouseWheel(wParam)) {
                return 0;
            }
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW+1));
        EndPaint(hWnd, &ps);
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    VimConfigApp app(hInstance);
    if (!app.Initialize()) {
        return 1;
    }
    
    return app.Run();
}