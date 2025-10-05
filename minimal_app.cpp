#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

// 最小版本的Vim配置应用
class VimConfigApp {
private:
    HINSTANCE hInstance;
    HWND mainWindow;
    HFONT defaultFont;
    std::vector<HWND> checkboxes;
    std::vector<HWND> labels;
    std::vector<bool> configValues;
    std::vector<std::wstring> configNames;
    
public:
    VimConfigApp(HINSTANCE hInst);
    ~VimConfigApp();
    
    bool Initialize();
    int Run();
    
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    void CreateControls();
    void HandleCommand(WPARAM wParam, LPARAM lParam);
};

VimConfigApp::VimConfigApp(HINSTANCE hInst) : hInstance(hInst), mainWindow(nullptr), defaultFont(nullptr) {
    // 初始化一些示例配置项
    configNames = {L"行号显示", L"语法高亮", L"自动缩进", L"鼠标支持", L"搜索高亮", L"括号匹配", L"自动补全", L"拼写检查"};
    configValues.resize(configNames.size(), true);
}

VimConfigApp::~VimConfigApp() {
    DeleteObject(defaultFont);
}

bool VimConfigApp::Initialize() {
    // 创建默认字体
    defaultFont = CreateFont(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH | FF_SWISS, TEXT("宋体"));
    
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
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500, 
        NULL, NULL, hInstance, this);
    
    if (!mainWindow) {
        MessageBox(NULL, TEXT("创建窗口失败"), TEXT("错误"), MB_ICONERROR);
        return false;
    }
    
    CreateControls();
    
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
        if (app) {
            app->HandleCommand(wParam, lParam);
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

void VimConfigApp::CreateControls() {
    // 创建标题标签
    HWND titleLabel = CreateWindow(
        TEXT("STATIC"), TEXT("Vim配置项 - 类手机设置风格界面"),
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        10, 10, 580, 40, mainWindow, NULL, 
        hInstance, NULL);
    
    // 设置标题字体
    HFONT titleFont = CreateFont(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH | FF_SWISS, TEXT("宋体"));
    SendMessage(titleLabel, WM_SETFONT, (WPARAM)titleFont, TRUE);
    
    // 创建配置项列表
    int yPos = 60;
    
    for (size_t i = 0; i < configNames.size(); ++i) {
        // 创建标签
        HWND label = CreateWindow(
            TEXT("STATIC"), configNames[i].c_str(),
            WS_CHILD | WS_VISIBLE,
            20, yPos, 150, 30, mainWindow, NULL, 
            hInstance, NULL);
        SendMessage(label, WM_SETFONT, (WPARAM)defaultFont, TRUE);
        labels.push_back(label);
        
        // 创建开关按钮
        HWND toggle = CreateWindow(
            TEXT("BUTTON"), configValues[i] ? TEXT("开启") : TEXT("关闭"),
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            180, yPos, 100, 30, mainWindow, (HMENU)(1000 + i), 
            hInstance, NULL);
        SendMessage(toggle, WM_SETFONT, (WPARAM)defaultFont, TRUE);
        SendMessage(toggle, BM_SETCHECK, configValues[i] ? BST_CHECKED : BST_UNCHECKED, 0);
        checkboxes.push_back(toggle);
        
        yPos += 40;
    }
    
    // 创建保存按钮
    HWND saveBtn = CreateWindow(
        TEXT("BUTTON"), TEXT("保存配置"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        20, yPos + 20, 150, 40, mainWindow, (HMENU)3000, 
        hInstance, NULL);
    SendMessage(saveBtn, WM_SETFONT, (WPARAM)defaultFont, TRUE);
    
    // 创建取消按钮
    HWND cancelBtn = CreateWindow(
        TEXT("BUTTON"), TEXT("取消"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        180, yPos + 20, 150, 40, mainWindow, (HMENU)3001, 
        hInstance, NULL);
    SendMessage(cancelBtn, WM_SETFONT, (WPARAM)defaultFont, TRUE);
}

void VimConfigApp::HandleCommand(WPARAM wParam, LPARAM lParam) {
    int cmd = LOWORD(wParam);
    
    // 处理配置项开关
    if (cmd >= 1000 && cmd < 2000) {
        size_t index = cmd - 1000;
        
        if (index < configValues.size()) {
            HWND checkbox = (HWND)lParam;
            BOOL checked = SendMessage(checkbox, BM_GETCHECK, 0, 0);
            
            configValues[index] = (checked == BST_CHECKED);
            
            // 更新按钮文本
            SetWindowText(checkbox, checked == BST_CHECKED ? TEXT("开启") : TEXT("关闭"));
        }
    }
    // 处理保存按钮
    else if (cmd == 3000) {
        MessageBox(mainWindow, TEXT("配置保存成功！"), TEXT("提示"), MB_ICONINFORMATION);
    }
    // 处理取消按钮
    else if (cmd == 3001) {
        SendMessage(mainWindow, WM_CLOSE, 0, 0);
    }
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