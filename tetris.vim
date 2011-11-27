""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"在当前窗口玩俄罗斯方块
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"启动游戏
function Cyy_start()
	if Cyy_check_feature() ==1
		echo "该vim版本无法支持此游戏"
		return
	endif
	call Cyy_save_configure() " 保存配置
	call Cyy_init()		  " 初始化操作
	
	" 绘制框架
	for i in range(1,b:height)   
		call Cyy_initline(i)
	endfor
	let b:cur_block=Cyy_next_block()
	call Cyy_show_block([2,2],b:cur_block)
	call Timer_set("Cyy_block",1000,"Cyy_move_block","d",b:cur_block)
	augroup tetris
	au!
	au CursorMoved * call Cyy_input()
	augroup end
endfunction

"检查游戏的特性
function Cyy_check_feature()
	if v:version < 700  " 版本太低了
		return 1
	elseif !has("syntax") " 没有版本高亮
		return 1
	elseif !exists("*Timer_set") " 不存在我写的timer函数
		return 1
	endif
	return 0
endfunction
"保存当前buf的配置以及内容
function Cyy_save_configure()
	if &number  " 当前是否设置行号
		let b:number=1
	else
		let b:number=0
	endif
	if exists("#Syntax") " 当前是否设置语法高亮
		let b:syntax=1
	else
		let b:syntax=0
	endif
	if has("gui_running") " 保存光标设置
		let b:cursor=&guicursor
	endif
	let b:buf=[]	" 保存当前buf内容
	for line_num in range(line("$"))	
		call add(b:buf,getline(line_num+1))
	endfor
endfunction

"删除buf内容
function Cyy_removebuf()
	for line_num in range(line("$"))	
		exec "normal dd"
	endfor
endfunction

"恢复buf的配置以及内容
function Cyy_restore_configure()
	" 恢复行号设置
	if b:number==1
		set number
	else
		set nonumber
	endif
	" 恢复语法高亮设置
	if b:syntax==1
		syntax on
	else
		syntax off
	endif
	call Cyy_removebuf()
	" 恢复旧的buf内容
	for line_num in range(len(b:buf))	
		call setline(line_num+1,b:buf[line_num])
	endfor
	if has("gui_running") " 显示光标
		exec "set guicursor=".b:cursor
	endif
endfunction
" 初始化 设置选项和变量
function Cyy_init()
	set nonumber				" 去除行号
	syntax on				" 启动语法高亮
	if has("gui_running") " 不显示光标
		set guicursor=n:Normal
	endif
	
	call Cyy_removebuf()			" 删除当前buf
	
	let b:height=winheight(0)		" 游戏高度
	let b:width=winwidth(0)			" 游戏宽度
	if b:width %2 ==1
		let b:width=b:width-1
	endif
	" 设置块的形状
	let b:block={"red":{"cur_pos":[0,0],"code":"1","cur_shape":1,"shape_num":2,"shape1":[[0,0],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7]],"transfer1":[-1,4],"shape2":[[0,0],[0,1],[1,0],[1,1],[2,0],[2,1],[3,0],[3,1]],"transfer2":[1,-4]}}
	" 当前块 下个块
	let b:cur_block="" 
	let b:next_block="" 
	
	"  设置块的编码
	syntax match band /[-|]/
	syntax match black_block /0/
	syntax match red_block /1/
	syntax match blue_block /2/
	syntax match yellow_block /3/
	syntax match cyan_block /4/
	
	"  设置块的颜色
	if has("gui_running")
		hi band guifg=Gray guibg=Black
		hi black_block guifg=Black guibg=Black
		hi red_block guifg=Red guibg=Red
		hi blue_block guifg=Blue guibg=Blue
		hi yellow_block guifg=DarkYellow guibg=DarkYellow
		hi cyan_block guifg=Cyan guibg=Cyan
	else 
		hi band ctermfg=Gray ctermbg=Black
		hi black_block ctermfg=0 ctermbg=0
		hi red_block ctermfg=1* ctermbg=1*
		hi blue_block ctermfg=4* ctermbg=4*
		hi yellow_block ctermfg=3* ctermbg=3*
		hi cyan_block ctermfg=6* ctermbg=6*
	endif
"	hi block3  guifg=Green guibg=Green
"	syntax match block4 /4444/
"	syntax match block5 /5555/
"	hi block5  guifg=Magenta guibg=Magenta
endfunction
" 初始化行
function Cyy_initline(line_num)
	if a:line_num==1 || a:line_num==b:height
		call setline(a:line_num,repeat('-',b:width))
	else
		call setline(a:line_num,"|".repeat("0",b:width-2)."|")
	endif
endfunction
" 选择下一个块
function Cyy_next_block() 
	let keys=keys(b:block)
	let index=float2nr(str2float(reltimestr(reltime()))*13) %len(keys) " 随机下标
	return keys[index]
endfunction
function Cyy_draw_floor(floor)
	let len=len(a:floor)
	if len==0
		return
	endif
	for i in range(b:height-len,b:height-1)
		call setline(a:floor[i])
	endfor
endfunction

"显示块
function Cyy_show_block(pos,blocktype)
	let block=b:block[a:blocktype]
	let cur_shape=block["cur_shape"]
	let shape=block["shape".cur_shape]
	for relate_pos in shape
		let line_num=a:pos[0]+relate_pos[0]
		let col_num=a:pos[1]+relate_pos[1]
		call cursor(line_num,col_num)
		exec "normal r".block["code"]
	endfor
	let block["cur_pos"]=a:pos
	call setpos(".",[0]+a:pos) 
endfunction

"擦除块
function Cyy_clear_block(pos,blocktype)
	let block=b:block[a:blocktype]
	let cur_shape=block["cur_shape"]
	let shape=block["shape".cur_shape]
	for relate_pos in shape
		let line_num=a:pos[0]+relate_pos[0]
		let col_num=a:pos[1]+relate_pos[1]
		call cursor(line_num,col_num)
		exec "normal r0"
	endfor
endfunction
"旋转块
function Cyy_rotate_block(blocktype)
	let block=b:block[a:blocktype]
	let cur_shape=block["cur_shape"]
	let transfer=block["transfer".cur_shape]
	let block["cur_pos"][0]+=transfer[0]
	let block["cur_pos"][1]+=transfer[1]
	let cur_shape=cur_shape+1
	if cur_shape>block["shape_num"]
		let cur_shape=1
	endif
	let block["cur_shape"]=cur_shape
endfunction
" 检查能否移动块
function Cyy_check_move_block(blocktype,old_shapenum,old_pos,new_shapenum,new_pos)
	let block=b:block[a:blocktype]
	let old_shape=[]
	let new_shape=[]
	for relate_pos in block["shape".a:old_shapenum]
		let line_num=a:old_pos[0]+relate_pos[0]
		let col_num=a:old_pos[1]+relate_pos[1]
		let old_shape+=[[line_num,col_num]]
	endfor
	for relate_pos in block["shape".a:new_shapenum]
		let line_num=a:new_pos[0]+relate_pos[0]
		let col_num=a:new_pos[1]+relate_pos[1]
		if index(old_shape,[line_num,col_num]) == -1
			
			if getline(line_num)[col_num-1] !='0' " 碰到了
				return 1
			endif
		endif
	endfor
	return 0
endfunction
"移动块
function Cyy_move_block(direction,blocktype)
	let block=b:block[a:blocktype]
	let cur_shape=block["cur_shape"]
	let new_pos=copy(block["cur_pos"])
	if a:direction=="r"  " 右移
		let new_pos[1]+=2
	elseif a:direction=="l" " 左移
		let new_pos[1]-=2
	elseif a:direction=="d" " 下降
		let new_pos[0]+=1
	elseif a:direction=="u" " 旋转
		let transfer=block["transfer".cur_shape]
		let new_pos[0]+=transfer[0]
		let new_pos[1]+=transfer[1]
	endif
	
	" 检查能否移动 可能会碰到边界或者下面到了
	if a:direction=="u" " 旋转
		let cur_shape=cur_shape+1
		if cur_shape>block["shape_num"]
			let cur_shape=1
		endif
	endif
	
	if Cyy_check_move_block(a:blocktype,cur_shape,block["cur_pos"],cur_shape,new_pos) !=0
		return
	endif
	
	echo block["cur_pos"]+new_pos
	call Cyy_clear_block(block["cur_pos"],a:blocktype) " 清除旧的块
	if a:direction=="u" " 旋转
		call Cyy_rotate_block(a:blocktype)
	else
		let block["cur_pos"]=new_pos
	endif	
	call Cyy_show_block(block["cur_pos"],a:blocktype) " 打印新的块
endfunction

"根据输入移动块
function Cyy_input()
	let cur_pos=getpos(".")[1:2]
	let last_pos=b:block[b:cur_block]["cur_pos"]
	if cur_pos[0]==last_pos[0] && cur_pos[1]==last_pos[1]+1
		let direction="r"
	elseif cur_pos[0]==last_pos[0] && cur_pos[1]==last_pos[1]-1
		let direction="l"
	elseif cur_pos[0]==last_pos[0]+1 
		let direction="d"
	elseif cur_pos[0]==last_pos[0]-1 
		let direction="u"
	else
		call setpos(".",[0]+last_pos)
		return 
	endif
	call Cyy_move_block(direction,b:cur_block)
endfunction

