#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <windows.h>
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <cctype> // 添加头文件用于字符处理

namespace fs = std::filesystem;

class VimConfigurator {
public:
    VimConfigurator() {
        // 修复：使用正确的类型初始化配置
        config["line_numbers"] = "true";
        config["cursor_line"] = "true";
        config["ruler"] = "true";
        config["incsearch"] = "true";
        config["hlsearch"] = "true";
        config["autoindent"] = "true";
        config["expandtab"] = "true";
        config["showcmd"] = "true";
        config["showmatch"] = "true";
        config["swapfile"] = "true";
        config["backup"] = "false";
        config["syntax"] = "true";
        config["tabstop"] = "4";
        config["shiftwidth"] = "4";
        config["softtabstop"] = "4";
        config["colorscheme"] = "desert";
        config["nerdtree"] = "false";
        config["auto_pairs"] = "false";
        config["airline"] = "false";
        config["ycm"] = "false";
    }

    void run() {
        showWelcome();
        mainMenu();
    }

private:
    std::map<std::string, std::string> config; // 全部使用string类型存储配置

    void showWelcome() {
        std::cout << "=============================================\n";
        std::cout << "        Vim 配置生成工具 (C++版) v1.0\n";
        std::cout << "=============================================\n\n";
    }

    void mainMenu() {
        while (true) {
            std::cout << "\n主菜单:\n";
            std::cout << "1. 基本设置\n";
            std::cout << "2. 缩进设置\n";
            std::cout << "3. 主题与外观\n";
            std::cout << "4. 插件管理\n";
            std::cout << "5. 生成配置文件\n";
            std::cout << "6. 退出\n";
            std::cout << "选择: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "无效输入，请重新选择!\n";
                continue;
            }

            switch (choice) {
                case 1: basicSettings(); break;
                case 2: indentationSettings(); break;
                case 3: themeSettings(); break;
                case 4: pluginManagement(); break;
                case 5: generateConfig(); break;
                case 6: return;
                default: std::cout << "无效选择!\n";
            }
        }
    }

    void basicSettings() {
        std::cout << "\n===== 基本设置 =====\n";
        toggleSetting("显示行号", "line_numbers");
        toggleSetting("高亮当前行", "cursor_line");
        toggleSetting("显示光标位置", "ruler");
        toggleSetting("增量搜索", "incsearch");
        toggleSetting("高亮搜索结果", "hlsearch");
        toggleSetting("自动缩进", "autoindent");
        toggleSetting("使用空格代替Tab", "expandtab");
        toggleSetting("显示命令", "showcmd");
        toggleSetting("显示匹配括号", "showmatch");
        toggleSetting("启用交换文件", "swapfile");
        toggleSetting("创建备份文件", "backup");
        toggleSetting("语法高亮", "syntax");
    }

    void indentationSettings() {
        std::cout << "\n===== 缩进设置 =====\n";
        numericSetting("Tab宽度", "tabstop", 2, 8);
        numericSetting("自动缩进宽度", "shiftwidth", 2, 8);
        numericSetting("编辑模式缩进", "softtabstop", 2, 8);
    }

    void themeSettings() {
        std::cout << "\n===== 主题与外观 =====\n";
        std::cout << "可用主题: desert, slate, murphy, ron, koehler\n";
        std::cout << "当前主题: " << config["colorscheme"] << "\n";
        std::cout << "输入新主题名 (或按回车保持当前): ";
        
        std::cin.ignore();
        std::string newTheme;
        std::getline(std::cin, newTheme);
        
        if (!newTheme.empty()) {
            config["colorscheme"] = newTheme;
            std::cout << "主题已设置为: " << newTheme << "\n";
        }
    }

    void pluginManagement() {
        while (true) {
            std::cout << "\n===== 插件管理 =====\n";
            std::cout << "1. " << statusStr("NERDTree (文件浏览器)", config["nerdtree"]) << "\n";
            std::cout << "2. " << statusStr("Auto-Pairs (自动括号)", config["auto_pairs"]) << "\n";
            std::cout << "3. " << statusStr("Airline (状态栏美化)", config["airline"]) << "\n";
            std::cout << "4. " << statusStr("YouCompleteMe (自动补全)", config["ycm"]) << "\n";
            std::cout << "5. 返回主菜单\n";
            std::cout << "选择: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "无效输入，请重新选择!\n";
                continue;
            }

            if (choice == 5) break;

            switch (choice) {
                case 1: togglePlugin("nerdtree"); break;
                case 2: togglePlugin("auto_pairs"); break;
                case 3: togglePlugin("airline"); break;
                case 4: togglePlugin("ycm"); break;
                default: std::cout << "无效选择!\n";
            }
        }
    }

    void generateConfig() {
        std::string homeDir = getHomeDir();
        if (homeDir.empty()) {
            std::cerr << "错误: 无法确定家目录!\n";
            return;
        }

        fs::path vimrcPath = fs::path(homeDir) / ".vimrc";
        fs::path plugDir = fs::path(homeDir) / ".vim" / "plugged";

        std::ofstream vimrc(vimrcPath);
        if (!vimrc) {
            std::cerr << "错误: 无法创建 " << vimrcPath << "!\n";
            return;
        }

        // 生成配置头部
        vimrc << "\" ==========================================\n";
        vimrc << "\" 自动生成的Vim配置文件\n";
        vimrc << "\" 由Vim配置工具生成\n";
        vimrc << "\" ==========================================\n\n";

        // 基本设置
        vimrc << "\" 基本设置\n";
        vimrc << "set nocompatible\n";
        vimrc << "filetype plugin indent on\n";
        vimrc << "syntax " << (config["syntax"] == "true" ? "enable" : "off") << "\n";
        vimrc << "set encoding=utf-8\n";
        vimrc << "set " << (config["line_numbers"] == "true" ? "" : "no") << "number\n";
        vimrc << "set " << (config["ruler"] == "true" ? "" : "no") << "ruler\n";
        vimrc << "set " << (config["cursor_line"] == "true" ? "" : "no") << "cursorline\n";
        vimrc << "set " << (config["incsearch"] == "true" ? "" : "no") << "incsearch\n";
        vimrc << "set " << (config["hlsearch"] == "true" ? "" : "no") << "hlsearch\n";
        vimrc << "set " << (config["showcmd"] == "true" ? "" : "no") << "showcmd\n";
        vimrc << "set " << (config["showmatch"] == "true" ? "" : "no") << "showmatch\n";
        vimrc << "set " << (config["swapfile"] == "true" ? "" : "no") << "swapfile\n";
        vimrc << "set " << (config["backup"] == "true" ? "" : "no") << "backup\n";
        vimrc << "set laststatus=2\n\n";

        // 缩进设置
        vimrc << "\" 缩进设置\n";
        vimrc << "set tabstop=" << config["tabstop"] << "\n";
        vimrc << "set shiftwidth=" << config["shiftwidth"] << "\n";
        vimrc << "set softtabstop=" << config["softtabstop"] << "\n";
        vimrc << "set " << (config["autoindent"] == "true" ? "" : "no") << "autoindent\n";
        vimrc << "set " << (config["expandtab"] == "true" ? "" : "no") << "expandtab\n\n";

        // 主题设置
        vimrc << "\" 主题设置\n";
        vimrc << "colorscheme " << config["colorscheme"] << "\n";
        vimrc << "set t_Co=256\n\n";

        // 插件配置
        if (config["nerdtree"] == "true" || config["auto_pairs"] == "true" || 
            config["airline"] == "true" || config["ycm"] == "true") {
            
            vimrc << "\" ==========================================\n";
            vimrc << "\" 插件配置\n";
            vimrc << "\" ==========================================\n";
            vimrc << "call plug#begin('" << plugDir.string() << "')\n";
            
            if (config["nerdtree"] == "true") {
                vimrc << "\" NERDTree 文件浏览器\n";
                vimrc << "Plug 'preservim/nerdtree'\n";
            }
            
            if (config["auto_pairs"] == "true") {
                vimrc << "\" 自动括号补全\n";
                vimrc << "Plug 'jiangmiao/auto-pairs'\n";
            }
            
            if (config["airline"] == "true") {
                vimrc << "\" 状态栏美化\n";
                vimrc << "Plug 'vim-airline/vim-airline'\n";
                vimrc << "Plug 'vim-airline/vim-airline-themes'\n";
            }
            
            if (config["ycm"] == "true") {
                vimrc << "\" 代码自动补全\n";
                vimrc << "Plug 'ycm-core/YouCompleteMe'\n";
            }
            
            vimrc << "call plug#end()\n\n";
            
            // 插件特定配置
            if (config["nerdtree"] == "true") {
                vimrc << "\" NERDTree 配置\n";
                vimrc << "let g:NERDTreeShowHidden=1\n";
                vimrc << "let g:NERDTreeWinSize=30\n";
                vimrc << "nmap <F3> :NERDTreeToggle<CR>\n";
                vimrc << "autocmd VimEnter * if argc() == 0 && !exists(\"s:std_in\") | NERDTree | endif\n";
                vimrc << "autocmd BufEnter * if (winnr(\"$\") == 1 && exists(\"b:NERDTree\") && b:NERDTree.isTabTree()) | q | endif\n\n";
            }
            
            if (config["ycm"] == "true") {
                vimrc << "\" YouCompleteMe 配置\n";
                vimrc << "let g:ycm_auto_trigger = 1\n";
                vimrc << "let g:ycm_min_num_of_chars_for_completion = 2\n";
                vimrc << "let g:ycm_seed_identifiers_with_syntax = 1\n";
                vimrc << "let g:ycm_clangd_binary_path = \"" << findClangd() << "\"\n\n";
            }
        }

        vimrc << "\" ==========================================\n";
        vimrc << "\" 自定义配置结束\n";
        vimrc << "\" ==========================================\n";

        vimrc.close();
        std::cout << "\n配置文件已生成: " << vimrcPath << "\n";

        // 插件安装
        if (config["nerdtree"] == "true" || config["auto_pairs"] == "true" || 
            config["airline"] == "true" || config["ycm"] == "true") {
            
            std::cout << "\n需要安装插件。是否现在安装? (y/n): ";
            char choice;
            std::cin >> choice;
            
            if (choice == 'y' || choice == 'Y') {
                installPlugins();
            }
        }
    }

    void installPlugins() {
        std::cout << "\n正在安装插件，这可能需要几分钟...\n";
        
        #ifdef _WIN32
            system(("vim -c \"PlugInstall\" -c \"qa\" > NUL 2>&1"));
        #else
            system("vim -c 'PlugInstall' -c 'qa' > /dev/null 2>&1");
        #endif
        
        if (config["ycm"] == "true") {
            std::cout << "\nYouCompleteMe 需要额外编译步骤\n";
            std::cout << "请运行: cd ~/.vim/plugged/YouCompleteMe && python3 install.py\n";
        }
        
        std::cout << "\n插件安装完成!\n";
    }

private:
    void toggleSetting(const std::string& name, const std::string& key) {
        bool current = config[key] == "true";
        std::cout << (current ? "[X] " : "[ ] ") << name;
        std::cout << " - 切换? (y/n): ";
        
        char choice;
        std::cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            config[key] = current ? "false" : "true";
        }
    }

    void numericSetting(const std::string& name, const std::string& key, int min, int max) {
        std::cout << name << " (当前: " << config[key] << ")\n";
        std::cout << "输入新值 (" << min << "-" << max << "): ";
        
        int value;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "无效输入! 保持为 " << config[key] << "\n";
            return;
        }
        
        if (value >= min && value <= max) {
            config[key] = std::to_string(value);
            std::cout << name << " 已设置为 " << value << "\n";
        } else {
            std::cout << "无效值! 保持为 " << config[key] << "\n";
        }
    }

    void togglePlugin(const std::string& plugin) {
        bool current = config[plugin] == "true";
        config[plugin] = current ? "false" : "true";
        std::cout << plugin << " " << (current ? "已禁用" : "已启用") << "\n";
    }

    std::string statusStr(const std::string& name, const std::string& status) {
        return name + " [" + (status == "true" ? "启用" : "禁用") + "]";
    }

    std::string getHomeDir() {
        #ifdef _WIN32
            const char* home = std::getenv("USERPROFILE");
        #else
            const char* home = std::getenv("HOME");
        #endif
        return home ? std::string(home) : "";
    }

    std::string findClangd() {
        // 在常见路径中查找clangd
        std::vector<std::string> paths = {
            #ifdef _WIN32
                "C:\\Program Files\\LLVM\\bin\\clangd.exe",
                "C:\\LLVM\\bin\\clangd.exe",
                "C:\\msys64\\mingw64\\bin\\clangd.exe"
            #else
                "/usr/bin/clangd",
                "/usr/local/bin/clangd",
                "/opt/homebrew/bin/clangd",
                "/usr/lib/llvm/bin/clangd"
            #endif
        };
        
        for (const auto& path : paths) {
            if (fs::exists(path)) {
                return path;
            }
        }
        
        return "clangd"; // 如果找不到，使用PATH环境变量中的版本
    }
};

int main() {
    SetConsoleOutputCP(65001);
    VimConfigurator configurator;
    configurator.run();
    return 0;
}