#include "guirenderer.h"
#include <string>

// GUI渲染器实现
GUIRenderer::GUIRenderer(VimConfig* cfg, HWND parent) : config(cfg), parentWindow(parent), scrollContainer(NULL), scrollPosition(0), contentHeight(0) {
    // 创建默认字体
    defaultFont = CreateFont(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH | FF_SWISS, TEXT("宋体"));
}

GUIRenderer::~GUIRenderer() {
    DeleteObject(defaultFont);
}

void GUIRenderer::Initialize() {
    // 修改父窗口样式，添加垂直滚动条
    LONG style = GetWindowLong(parentWindow, GWL_STYLE);
    style |= WS_VSCROLL;
    SetWindowLong(parentWindow, GWL_STYLE, style);
    
    CreateControls();
}

void GUIRenderer::CreateControls() {
    // 获取父窗口的客户区大小
    RECT clientRect;
    GetClientRect(parentWindow, &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;
    
    // 创建标题标签
    HWND titleLabel = CreateWindow(
        TEXT("STATIC"), TEXT("Vim配置项 - 类手机设置风格界面"),
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        10, 10, clientWidth - 20, 40, parentWindow, NULL, 
        (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);
    
    // 设置标题字体
    HFONT titleFont = CreateFont(
        24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH | FF_SWISS, TEXT("宋体"));
    SendMessage(titleLabel, WM_SETFONT, (WPARAM)titleFont, TRUE);
    
    // 创建配置项列表
    int yPos = 60;
    const auto& configItems = config->GetConfigItems();
    
    for (size_t i = 0; i < configItems.size(); ++i) {
        // 创建标签 - 现在name已经是宽字符串，直接使用
        HWND label = CreateWindow(
            TEXT("STATIC"), configItems[i].name.c_str(),
            WS_CHILD | WS_VISIBLE,
            20, yPos, 200, 30, parentWindow, NULL, 
            (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);
        SendMessage(label, WM_SETFONT, (WPARAM)defaultFont, TRUE);
        
        // 创建开关按钮
        HWND toggle = CreateWindow(
            TEXT("BUTTON"), configItems[i].enabled ? TEXT("开启") : TEXT("关闭"),
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            240, yPos, 100, 30, parentWindow, (HMENU)(1000 + i), 
            (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);
        SendMessage(toggle, WM_SETFONT, (WPARAM)defaultFont, TRUE);
        SendMessage(toggle, BM_SETCHECK, configItems[i].enabled ? BST_CHECKED : BST_UNCHECKED, 0);
        
        // 创建帮助按钮（问号标记）
        HWND helpBtn = CreateWindow(
            TEXT("BUTTON"), TEXT("?"),
            WS_CHILD | WS_VISIBLE,
            360, yPos, 30, 30, parentWindow, (HMENU)(2000 + i), 
            (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);
        SendMessage(helpBtn, WM_SETFONT, (WPARAM)defaultFont, TRUE);
        
        yPos += 40;
    }
    
    // 创建保存按钮
    HWND saveBtn = CreateWindow(
        TEXT("BUTTON"), TEXT("保存配置"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        20, yPos + 20, 150, 40, parentWindow, (HMENU)3000, 
        (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);
    SendMessage(saveBtn, WM_SETFONT, (WPARAM)defaultFont, TRUE);
    
    // 创建取消按钮
    HWND cancelBtn = CreateWindow(
        TEXT("BUTTON"), TEXT("取消"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        200, yPos + 20, 150, 40, parentWindow, (HMENU)3001, 
        (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);
    SendMessage(cancelBtn, WM_SETFONT, (WPARAM)defaultFont, TRUE);
    
    // 计算内容总高度
    contentHeight = yPos + 80;
    
    // 设置滚动条范围
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = contentHeight - clientHeight + 10;  // 10是额外的边距
    si.nPage = clientHeight;
    SetScrollInfo(parentWindow, SB_VERT, &si, TRUE);
}

void GUIRenderer::Render(HDC hdc, RECT rect) {
    // 渲染背景
    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW+1));
}

void GUIRenderer::HandleCommand(WPARAM wParam, LPARAM lParam) {
    int cmd = LOWORD(wParam);
    
    // 处理配置项开关
    if (cmd >= 1000 && cmd < 2000) {
        size_t index = cmd - 1000;
        const auto& configItems = config->GetConfigItems();
        
        if (index < configItems.size()) {
            HWND checkbox = (HWND)lParam;
            BOOL checked = SendMessage(checkbox, BM_GETCHECK, 0, 0);
            
            config->UpdateConfigItem(configItems[index].name, checked == BST_CHECKED);
            
            // 更新按钮文本
            SetWindowText(checkbox, checked == BST_CHECKED ? TEXT("开启") : TEXT("关闭"));
        }
    }
    // 处理帮助按钮（问号标记）
    else if (cmd >= 2000 && cmd < 3000) {
        size_t index = cmd - 2000;
        const auto& configItems = config->GetConfigItems();
        
        if (index < configItems.size()) {
            ShowHelpDialog(configItems[index].name, configItems[index].description);
        }
    }
    // 处理保存按钮
    else if (cmd == 3000) {
        // 保存配置到文件
        bool success = config->SaveToFile(config->GetOriginalContent().empty() ? ".\\_vimrc" : "");
        
        if (success) {
            MessageBox(parentWindow, TEXT("配置保存成功！"), TEXT("提示"), MB_ICONINFORMATION);
        } else {
            MessageBox(parentWindow, TEXT("配置保存失败！"), TEXT("错误"), MB_ICONERROR);
        }
    }
    // 处理取消按钮
    else if (cmd == 3001) {
        SendMessage(parentWindow, WM_CLOSE, 0, 0);
    }
}

void GUIRenderer::ShowHelpDialog(const std::wstring& title, const std::wstring& content) {
    // 创建帮助弹窗，显示详细用法说明
    MessageBox(parentWindow, 
               content.c_str(),
               title.c_str(),
               MB_ICONINFORMATION | MB_OK);
}

bool GUIRenderer::HandleScrollMessage(HWND scrollWindow, WPARAM wParam, LPARAM lParam) {
    // 检查是否是父窗口的滚动条消息
    if (scrollWindow == parentWindow) {
        // 获取滚动条信息
        SCROLLINFO si;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;
        GetScrollInfo(parentWindow, SB_VERT, &si);
        
        // 保存当前位置
        int oldPos = si.nPos;
        
        // 处理滚动条消息
        switch (LOWORD(wParam)) {
        case SB_TOP:
            si.nPos = si.nMin;
            break;
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;
        case SB_LINEUP:
            si.nPos -= 10;
            break;
        case SB_LINEDOWN:
            si.nPos += 10;
            break;
        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;
        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            si.nPos = HIWORD(wParam);
            break;
        default:
            break;
        }
        
        // 限制位置范围
        si.nPos = std::max(si.nMin, std::min(si.nMax, si.nPos));
        
        // 设置新的滚动位置
        si.fMask = SIF_POS;
        SetScrollInfo(parentWindow, SB_VERT, &si, TRUE);
        
        // 获取实际设置的位置
        GetScrollInfo(parentWindow, SB_VERT, &si);
        
        // 如果位置发生变化，滚动窗口
        if (si.nPos != oldPos) {
            ScrollWindow(parentWindow, 0, oldPos - si.nPos, NULL, NULL);
            UpdateWindow(parentWindow);
            scrollPosition = si.nPos;
        }
        
        return true;
    }
    
    return false;
}

bool GUIRenderer::HandleMouseWheel(WPARAM wParam) {
    // 获取鼠标滚轮滚动的距离
    int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    
    // 根据滚动方向发送滚动消息
    if (zDelta > 0) {
        // 向上滚动
        SendMessage(parentWindow, WM_VSCROLL, SB_LINEUP, 0);
    } else {
        // 向下滚动
        SendMessage(parentWindow, WM_VSCROLL, SB_LINEDOWN, 0);
    }
    
    return true;
}