let s:timer={}
let s:flag=0
function Timer_set(timer_id,timer_interval,timer_callback,...)
	let s:timer[a:timer_id]=[reltime(),a:timer_interval,a:timer_callback,a:000]
	if s:flag==0
		exec 'au CursorHold * call Timer_tick()'
		let s:flag=1
	endif
	exec "call ".a:timer_callback."(".Timer_arg(a:000).")"
	set updatetime=1
	call feedkeys("f\e")
endfunction
function Timer_clear(timer_id)
	if !has_key(s:timer,timer_id)
		return
	else
		call remove(s:timer,timer_id)
	endif
endfunction
function Timer_arg(args)
	let argstring=""
	for i in a:args
		if len(argstring) !=0
			let argstring.=","
		endif
		if(type(i)==type(0))
			let argstring.=i
		elseif(type(i)==type(""))
			let argstring.="'".i."'"
		endif
	endfor
	return argstring
endfunction
function Timer_tick()
	for key in keys(s:timer)
		let timer=s:timer[key]
		if !exists('*'.timer[2])
			call remove(s:timer,key)
		else
			if str2float(reltimestr(reltime(timer[0])))*1000 > timer[1]
				let timer[0]=reltime()
				exec "call ".timer[2]."(".Timer_arg(timer[3]).")"
			endif
		endif
	endfor
	call feedkeys("f\e")
endfunction
function Callback1(a,b)
echo strftime("%Y %b %d %X")
endfunctio
"call Timer_set("cyy",1000,"Callback1",1,"aa")
