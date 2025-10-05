#ifndef GUIRenderer_H
#define GUIRenderer_H

#include <windows.h>
#include "vimconfig.h"

// GUI渲染类
class GUIRenderer {
private:
    VimConfig* config;
    HWND parentWindow;
    HFONT defaultFont;
    HWND scrollContainer;  // 滚动容器窗口
    int scrollPosition;    // 当前滚动位置
    int contentHeight;     // 内容总高度
    
public:
    GUIRenderer(VimConfig* cfg, HWND parent);
    ~GUIRenderer();
    
    void Initialize();
    void Render(HDC hdc, RECT rect);
    void HandleCommand(WPARAM wParam, LPARAM lParam);
    bool HandleScrollMessage(HWND scrollWindow, WPARAM wParam, LPARAM lParam);
    bool HandleMouseWheel(WPARAM wParam);
    
private:
    void CreateControls();
    void ShowHelpDialog(const std::wstring& title, const std::wstring& content);
};

#endif // GUIRenderer_H