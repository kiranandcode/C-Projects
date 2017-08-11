" C-Specific VIMRC 
set tabstop=4
set softtabstop=4
set shiftwidth=4
set noexpandtab


set colorcolumn=110
highlight ColorColumn ctermbg=darkgray

augroup project
	autocmd!
	autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END


set nocompatible
filetype off

set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()


Plugin 'VundleVim/Vundle.vim'
Plugin 'Valloric/YouCompleteMe'

call vundle#end()
filetype plugin indent on

syntax on

set exrc
set secure
set encoding=utf-8
