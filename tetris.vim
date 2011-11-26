""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"在当前窗口玩俄罗斯方块
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"启动游戏
function Cyy_start()
   if Cyy_check_feature() ==1
      echo "该vim版本无法支持此游戏"
      return
   endif
   call Cyy_save_configure()
   call Cyy_init()
endfunction

"检查游戏的特性
function Cyy_check_feature()
   if !has("syntax")
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
   if b:number==1
      set number
   else
      set nonumber
   endif
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
endfunction


function Cyy_init()
   set nonumber			" 去除行号
   syntax on				" 启动语法高亮
	call Cyy_removebuf()
   let b:height=winheight(0)  		" 游戏高度
   let b:width=winwidth(0)		" 游戏宽度
   
   "  设置块的编码
   syntax match band /[-|]/
   syntax match black_block /0/   
   syntax match red_block /1/
   syntax match blue_block /2/
   syntax match yellow_block /3/
   syntax match cyan_block /4/
   
   " 设置块的形状
   let b:red_block={"code":"1","cur_shape":1,"shape_num":2,"shape1":[[0,0],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7]],"transfer1":[4,-1],"shape2":[[0,0],[0,1],[1,0],[1,1],[2,0],[2,1],[3,0],[3,1]],"transfer2":[-4,1]}
   
   "  设置块的颜色
   if has("gui_running")
      hi band    guifg=Gray  guibg=Black
      hi black_block   guifg=Black guibg=Black
      hi red_block  guifg=Red guibg=Red
      hi  blue_block guifg=Blue guibg=Blue
      hi  yellow_block guifg=DarkYellow guibg=DarkYellow
      hi cyan_block guifg=Cyan guibg=Cyan
   else 
      hi band    ctermfg=Gray ctermbg=Black
      hi black_block ctermfg=0 ctermbg=0
      hi  red_block ctermfg=1* ctermbg=1*
      hi  blue_block ctermfg=4* ctermbg=4*
      hi  yellow_block ctermfg=3* ctermbg=3*
      hi  cyan_block ctermfg=6* ctermbg=6*
   endif
   "	hi block3  guifg=Green guibg=Green
   "	syntax match block4 /4444/
   "	syntax match block5 /5555/
   "	hi block5  guifg=Magenta guibg=Magenta
   for i in range(1,b:height)
      call Cyy_clearline(i)
   endfor
endfunction
function Cyy_test()
   if b:height<20 || b:width <100
      echo "当前窗口太小"
      return 0
   endif
   return 1
endfunction
function Cyy_clearline(line_num)
   if a:line_num==1 || a:line_num==b:height
      call setline(a:line_num,repeat('-',b:width))
   else
      call setline(a:line_num,"|".repeat("0",b:width-2)."|")
   endif
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
function Cyy_position_block(position)
   let cur_shape= b:red_block["cur_shape"]
   let shape= b:red_block["shape".cur_shape]
   for relate_position in shape
      let line_num=a:position[0]+relate_position[0]
      let col_num=a:position[1]+relate_position[1]
      let line=getline(line_num)
      let line=strpart(line,0,col_num-1).b:red_block["code"].strpart(line,col_num)
      echo line
      call setline(line_num,line)
   endfor
endfunction
function Cyy_move_block(block,direction)
   if a:direction=="r"
      let block=[a:block[0],[a:block[1][0]+1,a:block[1][1]]
   elseif  a:direction="l"
      let block=[a:block[0],[a:block[1][0]-1,a:block[1][1]]
   elseif  a:direction="d"
      let block=[a:block[0],[a:block[1][0],a:block[1][1]+1]
   endif
   call Cyy_position_block(block)
   return block
endfunction
