#include "vimconfig.h"
#include <fstream>
#include <cstdio>
#include <ctime>
#include <windows.h>  // 添加Windows.h头文件以使用WideCharToMultiByte等函数

// VimConfig类实现
VimConfig::VimConfig() {
    // 初始化一些默认的配置项，参考vimcdoc项目中的配置帮助
    configItems = {
        {L"语法高亮", L"启用Vim的语法高亮功能，使代码更易读", "syntax enable", true, true},
        {L"行号显示", L"在编辑器左侧显示行号，便于导航和引用", "set number", true, true},
        {L"自动缩进", L"根据上一行的缩进自动调整当前行的缩进", "set autoindent", true, true},
        {L"显示匹配括号", L"当光标位于括号上时，高亮显示匹配的括号", "set showmatch", true, true},
        {L"光标行高亮", L"高亮显示当前光标所在的行，便于定位", "set cursorline", false, true},
        {L"搜索高亮", L"高亮显示搜索匹配的结果，便于查找", "set hlsearch", true, true},
        {L"256色模式", L"启用终端的256色支持，提供更多颜色", "set t_Co=256", true, true},
        {L"智能缩进", L"根据文件类型设置智能缩进，提高代码可读性", "filetype plugin indent on", true, true},
        {L"制表符宽度", L"设置制表符的宽度为4个空格", "set tabstop=4", true, true},
        {L"使用空格代替制表符", L"将制表符转换为空格，避免不同编辑器显示差异", "set expandtab", true, true},
        {L"禁用Vi兼容模式", L"启用Vim的增强功能，提高编辑效率", "set nocompatible", true, true},
        {L"设置编码", L"设置文件编码为UTF-8，支持多语言", "set encoding=utf-8", true, true},
        {L"右下角显示光标位置", L"在状态栏显示当前光标位置信息", "set ruler", true, true},
        {L"自动保存", L"在每次修改后自动保存并退出", "set autowrite", true, true},
        {L"不可拆词", L"在换行时保持单词完整，除非遇到空格或连词符", "set linebreak", true, true},
        {L"显示状态栏", L"始终显示状态栏，提供更多信息", "set laststatus=2", true, true},
        {L"文件类型检测并载入缩进规则", L"根据文件类型自动设置缩进规则", "filetype indent on", true, true},
        {L"命令提示", L"在底部显示当前输入的命令", "set showcmd", true, true},
        {L"渐进式搜索", L"在输入搜索模式时自动跳转", "set incsearch", true, true}
    };
}

VimConfig::~VimConfig() {
}

bool VimConfig::LoadFromFile(const std::string& path) {
    vimrcPath = path;
    
    // 尝试打开文件
    FILE* file = fopen(path.c_str(), "r");
    if (!file) {
        // 文件不存在，使用默认配置
        originalContent = "";
        currentContent = GenerateConfigContent();
        return false;
    }
    
    // 读取文件内容
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = new char[length + 1];
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    
    originalContent = buffer;
    currentContent = originalContent;
    
    delete[] buffer;
    fclose(file);
    
    // 解析配置文件，更新配置项状态
    // 这里实现简单的解析逻辑，实际应用中可能需要更复杂的解析
    for (auto& item : configItems) {
        size_t pos = originalContent.find(item.option);
        if (pos != std::string::npos) {
            // 检查是否被注释掉
            size_t commentPos = originalContent.rfind('"', pos);
            size_t newlinePos = originalContent.rfind('\n', pos);
            if (commentPos == std::string::npos || commentPos < newlinePos) {
                item.enabled = true;
            } else {
                item.enabled = false;
            }
        } else {
            // 未找到该选项，保持默认值
            item.enabled = true; // 默认为开启
        }
    }
    
    return true;
}

bool VimConfig::SaveToFile(const std::string& path) {
    std::string savePath = path.empty() ? vimrcPath : path;
    
    // 如果路径为空，使用默认路径
    if (savePath.empty()) {
        savePath = ".\\_vimrc";
    }
    
    FILE* file = fopen(savePath.c_str(), "w");
    if (!file) {
        return false;
    }
    
    std::string content = GenerateConfigContent();
    fwrite(content.c_str(), 1, content.length(), file);
    fclose(file);
    
    originalContent = content;
    currentContent = content;
    vimrcPath = savePath;
    
    return true;
}

std::vector<ConfigItem>& VimConfig::GetConfigItems() {
    return configItems;
}

std::string VimConfig::GetOriginalContent() const {
    return originalContent;
}

std::string VimConfig::GenerateConfigContent() const {
    std::string content;
    
    // 添加配置文件头部信息
    content += "\" Vim配置文件 - 由Vim配置工具生成\n";
    content += "\" 本配置工具基于vimcdoc项目的配置帮助文档\n";
    content += "\" 最后修改时间: ";
    
    // 添加当前时间
    time_t now = time(0);
    char* dt = ctime(&now);
    if (dt) {
        // 移除末尾的换行符
        std::string timeStr(dt);
        size_t pos = timeStr.find_last_of('\n');
        if (pos != std::string::npos) {
            timeStr.erase(pos);
        }
        content += timeStr;
    }
    
    content += "\n\n";
    
    // 添加配置项分组
    content += "\" ========== 基本设置 ==========\n\n";
    
    // 添加配置项
    for (const auto& item : configItems) {
        // 将宽字符串转换为窄字符串
        content += "\" ";
        int len = WideCharToMultiByte(CP_UTF8, 0, item.description.c_str(), -1, NULL, 0, NULL, NULL);
        if (len > 0) {
            std::string narrowDesc(len, 0);
            WideCharToMultiByte(CP_UTF8, 0, item.description.c_str(), -1, &narrowDesc[0], len, NULL, NULL);
            content += narrowDesc;
        }
        
        content += "\n";
        
        if (item.enabled) {
            content += item.option;
        } else {
            content += "\" ";
            content += item.option;
        }
        content += "\n\n";
    }
    
    // 添加尾部注释
    content += "\" ========== 配置结束 ==========\n";
    content += "\" 如需更多高级配置，请参考vimcdoc文档\n";
    
    return content;
}

void VimConfig::UpdateConfigItem(const std::wstring& name, bool enabled) {
    for (auto& item : configItems) {
        if (item.name == name) {
            item.enabled = enabled;
            break;
        }
    }
}