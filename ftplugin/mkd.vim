" markdown filetype file
if exists("did\_load\_filetypes")
 finish
endif
augroup markdown
 au! BufRead,BufNewFile *.mkd   setfiletype mkd
augroup END

autocmd BufNewFile,BufReadPost *.mkd,*.markdown,*.mdown set filetype=mkd
