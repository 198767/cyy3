"备份文件
set backup
"使用文件类型插件
filetype plugin on
"查找
map <space> /
"设置编码
set enc=utf8
"设置文件编码
set fenc=utf8
"设置写入文件编码
set fencs=utf8
if has("gui_win32")
	"去除gvim菜单乱码
	source $VIMRUNTIME/delmenu.vim
	source $VIMRUNTIME/menu.vim
	"去除gvim消息乱码
	language messages zh_CN.utf-8
endif

"递增查询
set incsearch
"设置页号
set nu
"打开文件跳转到上次阅读地方
autocmd BufReadPost * call cursor(line("'\""),1)
"检索高亮
set hlsearch
"映射到切换大小写
nmap . :call Switch_case()<cr>

map <F12> :echo "尚未绑定按键"<CR>

"去掉X命令
cmap X x

