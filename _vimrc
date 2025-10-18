" 语法显示
filetype plugin indent on  " 初始化文件类型检测
syntax enable              " 启用语法高亮

" 适配优化
set nocompatible     " 禁用 Vi 兼容模式
set encoding=utf-8   " 设置编码

" 使用优化
set showmatch        " 匹配符号（括号等）
set number           " 显示行号
set ruler            " 右下角显示光标位置
set incsearch        " 找要匹配的单词
set autowrite        " 在每次修改后自动保存并退出
set cursorline       " 当前行高亮
set linebreak        " 不可拆词，除非遇到空格或连词符等
set laststatus=2     " 是否显示状态栏。0 表示不显示，1 表示只在多窗口时显示，2 表示显示
set ttimeoutlen=70   " ..某种..刷新时间？防止ecs会使终端失去焦点
set noundofile       " 保存撤销历史
filetype indent on   " 文件类型检测并载入缩进规则
set t_Co=256         " 256色模式
set showcmd          " 命令提示
"set spell spelllang=en_us,jck   " 拼写检测

" 防丢失策略
set nobackup         " 不在保存时自动创建备份
set swapfile         " 使用交换文件（防崩溃）

" 缩进设置
set tabstop=4        " 设置TAB=4（默认值是8）
set softtabstop=4    " 设置编辑模式时按退格键时退回缩进的长度
set autoindent     " 禁用自动缩进格式对齐
set expandtab        " 当设置成expandtab时，缩进用空格来表示
set shiftwidth=4     " 自动缩进空白字符个数(<<,>>,==)

" 搜索
set hlsearch         " 高亮显示搜索
set noincsearch      " 搜索模式输入自动跳转

" 字体与大小
set guifont=Courier_New:h30 " 注：本设置仅对GUI生效，终端字体仅能默认跟随系统终端设置

" 配色方案
colorscheme desert

" 设置帮助文本为中文（插件实现）
set helplang=cn

" 可选的选项
"
" set vb t_vb=   "取消命令错误的响声
" set cursorline " 设置游标
" set noswapfile " 不创建交换文件



" 插件选项
" ==================================
"set rtp+=~/.vim/bundle/Vundle.vim
" 启用插件管理器
call plug#begin('~/.vim/plugged')
" ----------------------------------


" 文件树插件
Plug 'preservim/nerdtree'

" Ctrl+f 定位当前文件
map <C-f> :NERDTreeFind<CR>

" 禁用netrw以避免冲突
let g:loaded_netrw = 1
let g:loaded_netrwPlugin = 1

" 核心配置：始终打开NERDTree但避免重复
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * 
    \ if argc() == 0 || (argc() == 1 && !isdirectory(argv()[0])) | 
    \   NERDTree | 
    \   wincmd p | 
    \ endif

" 处理目录参数的特殊情况
autocmd VimEnter * 
    \ if argc() == 1 && isdirectory(argv()[0]) | 
    \   execute 'NERDTree' fnameescape(argv()[0]) | 
    \   wincmd p | 
    \ endif

" 处理多个文件参数的情况
autocmd VimEnter * 
    \ if argc() > 1 | 
    \   NERDTree | 
    \   wincmd p | 
    \ endif

" 自动退出优化
autocmd BufEnter * 
    \ if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | 
    \   q | 
    \ endif

" 优化设置
let g:NERDTreeWinSize = 30
let g:NERDTreeShowHidden = 1
let g:NERDTreeChDirMode = 2  " 总是跟随当前文件目录


" ----------------------------------


"自动括号补全
Plug 'jiangmiao/auto-pairs'


" ----------------------------------


" 自动补全插件
" Plug 'ycm-core/YouCompleteMe'  "不会装....


" ----------------------------------


" 状态栏美化
Plug 'vim-airline/vim-airline'
" 主题设置
Plug 'vim-airline/vim-airline-themes'
let g:airline_theme = 'dark'
"let g:airline_theme = 'solarized'
"let g:airline_theme = 'onedark'

" 显示标签栏
let g:airline#extensions#tabline#enabled = 1


" ----------------------------------


" 中文文档插件
Plug 'vimcn/vimcdoc'


" ----------------------------------


" Vundle插件管理
" Plugin 'gmarik/Vundle.vim'


" ----------------------------------


" Git集成
Plug 'tpope/vim-fugitive'


" ----------------------------------
" 语法检查
Plug 'dense-analysis/ale'

" === ALE 异步语法检查配置 ===

" 基本设置
let g:ale_enabled = 1
let g:ale_lint_on_enter = 0           " 打开文件时不立即检查（提升启动速度）
let g:ale_lint_on_save = 1            " 保存时检查
let g:ale_lint_on_text_changed = 'never'  " 输入时不实时检查（避免卡顿）
let g:ale_lint_on_insert_leave = 0    " 离开插入模式时不检查

" 界面设置
let g:ale_sign_error = '✗'            " 错误符号
let g:ale_sign_warning = '⚠'          " 警告符号
let g:ale_sign_info = 'ℹ'             " 信息符号
let g:ale_sign_style_error = '➤'      " 风格错误符号
let g:ale_sign_style_warning = '➤'    " 风格警告符号

" 位置列表设置
let g:ale_open_list = 0               " 不自动打开位置列表
let g:ale_keep_list_window_open = 0   " 检查后关闭列表窗口
let g:ale_list_window_size = 5        " 列表窗口大小

" 检查器配置
let g:ale_linters = {
\   'python': ['flake8'],
\   'cpp': ['gcc', 'cppcheck'],
\   'c': ['gcc', 'cppcheck'],
\   'javascript': ['eslint'],
\   'typescript': ['tsserver', 'eslint'],
\   'html': ['tidy'],
\   'css': ['stylelint'],
\   'markdown': ['markdownlint'],
\   'yaml': ['yamllint'],
\   'json': ['jq'],
\   'sh': ['shellcheck'],
\   'vim': ['vint'],
\   'dockerfile': ['hadolint'],
\}

" 检查器参数
let g:ale_python_flake8_options = '--ignore=E501,W503 --max-line-length=120'
let g:ale_cpp_gcc_options = '-std=c++17 -Wall -Wextra -I./include'
let g:ale_c_gcc_options = '-std=c11 -Wall -Wextra -I./include'

" 文件类型特定设置
let g:ale_pattern_options = {
\   '\.min\.js$': {'ale_linters': [], 'ale_fixers': []},
\   '\.min\.css$': {'ale_linters': [], 'ale_fixers': []},
\}

" 自动修复
let g:ale_fix_on_save = 0             " 保存时不自动修复（可选）
let g:ale_fixers = {
\   '*': ['remove_trailing_lines', 'trim_whitespace'],
\   'python': ['black', 'isort'],
\   'javascript': ['prettier'],
\   'typescript': ['prettier'],
\   'css': ['prettier'],
\   'html': ['prettier'],
\   'json': ['jq'],
\   'markdown': ['prettier'],
\}

" 虚拟文本显示（需要 Vim 8.2+ 或 Neovim）
let g:ale_virtualtext_cursor = 0      " 不在光标位置显示虚拟文本

" 状态栏集成
let g:ale_statusline_format = ['⨉ %d', '⚠ %d', '⬥ ok']
set statusline=%{ale#statusline#Status()}
" ----------------------------------


Plug 'tpope/vim-surround' " 快速编辑包围符号


" ----------------------------------


Plug 'junegunn/fzf', { 'do': { -> fzf#install() } } " 模糊查找


" ----------------------------------


Plug 'junegunn/fzf.vim' " fzf 集成


" ----------------------------------


" 代码结构浏览
Plug 'majutsushi/tagbar'
" 快捷键
nmap <F8> :TagbarToggle<CR>" F8 打开/关闭标签栏
" 配置
let g:tagbar_autofocus = 1" 打开后光标聚焦到tagbar

" 自动设置 tags 路径，递归向上查找 tags 文件
set tags=./tags,tags,./.tags,.tags
" 自动切换工作目录到当前文件所在目录
set autochdir
" F12 在当前目录生成 tags 文件
map <F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>
imap <F12> <ESC>:!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>


" ------------------------------------


Plug 'airblade/vim-gitgutter' " Git 差异显示


" ------------------------------------


Plug 'preservim/nerdcommenter' " 快速注释


Plug 'sheerun/vim-polyglot' " 多语言语法高亮


" CMake 支持
" Plug 'cdelledonne/vim-cmake'


" YAML 特定支持
Plug 'pedrohdz/vim-yaml-folds'
" ==================================
call plug#end()
" NERDTree 配置（按 F3 开关文件树）
map <F3> :NERDTreeToggle<CR>
let NERDTreeShowHidden=1  " 显示隐藏文件
" YouCompleteMe 基础设置
let g:ycm_auto_trigger = 1
let g:ycm_min_num_of_chars_for_completion = 2
let g:ycm_seed_identifiers_with_syntax = 1
