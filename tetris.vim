"在当前窗口玩俄罗斯方块
function Tetris_start()
	set nonumber
	call Tetris_init()
	if Tetris_test() ==0
		return 
	endif
	call Tetris_clear()
	call Tetris_draw_frame()	       
endfunction 
function Tetris_init()
	let b:height=winheight(0)
	let b:width=winwidth(0)
	syntax match block1 /1111/
	hi block1  guifg=Red guibg=Red
	syntax match block2 /2222/
	hi block2  guifg=Blue guibg=Blue
	syntax match block3 /3333/
	hi block3  guifg=Green guibg=Green
	syntax match block4 /4444/
	hi block4  guifg=Cyan guibg=Cyan
	syntax match block5 /5555/
	hi block5  guifg=Magenta guibg=Magenta
endfunction
function Tetris_test()
	if b:height<20 || b:width <100
		echo "当前窗口太小"
		return 0
	endif
	return 1
endfunction
function Tetris_clear()
	for i in range(b:height)
		call setline(i+1,repeat(' ',b:width))
	endfor
endfunction
function Tetris_draw_frame()
	call setline(1,repeat('-',b:width))
	call setline(b:height,repeat('-',b:width))
	for i in range(2,b:height-1)
		call setline(i,"|".repeat(" ",b:width-2)."|")
	endfor
endfunction
