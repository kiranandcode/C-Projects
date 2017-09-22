let c_word = expand("<cword>")
let len = strlen(c_word)
let i = 0
let output = []
let result = ["if("]
while i < len - 1
	let c = strpart(c_word, (i + 1), 1)
	
	call add (result, "rcp[" . i ."] == '" . c . "' ")
	call add (result, "&&")
	let i += 1
endwhile
call add (result, "!isalphanum(rcp[" . i . "])) {")
call add(output, join(result))
call add (output, "filebuffer_setcp(buffer, (char *)rcp+" . i .");")
call add(output, "return " . toupper(c_word) . ";")
call add(output, "}")


let failed = append(".+1", output)
if failed
	echo "error"
endif


