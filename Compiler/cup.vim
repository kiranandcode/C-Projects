" creates keyword ERROR and puts it into highlight group called logError
" :syn keyword logError ERROR
"
" this creates a match on date and puts it in the highlight group called
" logDate
" :syn match logDate /^\d\{4}-\d\{2}-\d\{2},\d\{3}/
"
"
" now we just need to make them appear
" links logerror group to error highlight group, inheriting their styles
" hi link logerror Error
"
"
" hi def logDate guibg=yellow guifg=blue
" hi def logDate guiby=green guifg=white
"
"
"





" quit when a syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

" Read the Java syntax to start with
let s:javaPath= fnameescape(expand("<sfile>:p:h").("/java.vim"))
if !filereadable(s:javaPath)
 for s:javaPath in split(globpath(&rtp,("syntax/java.vim")),"\n")
  if filereadable(fnameescape(s:javaPath))
   let s:javaPath= fnameescape(s:javaPath)
   break
  endif
 endfor
endif
exe "syn include @lexJavacode ".s:javaPath


syn match CupDeclaration /^\(terminal\|non terminal\).*;/ contains=CupTerminal,CupTypes,CupNonTerminal
syn match CupNonTerminal /^\<[a-zA-Z_]*\>/
syn match CupTerminal /\<[A-Z_]\+\>/
syn match CupTypes /\(non\|terminal\)/
syn match CupOr /|/
syn match CupAssign /::=/
syn region CupCodeBlock start="{:" end=":};\?" contains=@lexJavacode
syn region CupStartDeclaration start="^start\s\+with\s*" end=";$"
syn match CupComment /\/\/.*/
syn match CupStatement /\<[a-zA-Z_]\+\s*::=\s*\(.\|.\+\n\)*;/ contains=CupTerminal,CupAssign,CupOr,CupCodeBlock,CupNonTerminal
syn region CupPrecedenceDeclaration start="^precedence" end=";" contains=CupTypes,CupTerminal
syn keyword CupKeywords scan parser code 

hi def link CupKeywords Special
hi def link CupTypes Type
hi def link CupOr Statement
hi def link CupAssign Macro
hi def link CupDeclaration Declaration
hi def link CupPrecedenceDeclaration Statement
hi def link CupStartDeclaration Statement
hi def link CupTerminal Constant
hi def link CupComment Comment
hi def link CupNonTerminal Statement

