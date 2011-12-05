call pathogen#infect()

" // --- General --- //

syntax on
filetype on

set nu
set confirm
set modeline
set ls=2
set showcmd
set nobackup	
set hlsearch
set autoindent
set noswapfile
set smartindent
set nocompatible
"set expandtab " use spaces replace tab
set tabstop=4
set backspace=2
set shiftwidth=4
set textwidth=90
set encoding=utf-8
"set guifont=Monaco\ 12  " ubuntu
"set guifont=Monaco:h14 " windows/mac
set fileencodings=utf-8,cp950

:map<F2> a<C-R> DISP_ERR("");<CR><ESC>
:map<F7> a<C-R> DISP_INFO_IN();<CR><ESC>
:map<F8> a<C-R> DISP_INFO_OUT();<CR><ESC>
:map<F9> a<C-R> DISP_INFO_LN("");<CR><ESC>

" status line
set statusline=
set statusline +=\ %n\             "buffer number
set statusline +=%{&ff}            "file format
set statusline +=%y%*                "file type
set statusline +=\ %<%F            "full path
set statusline +=%m                "modified flag
set statusline +=%=%5l             "current line
set statusline +=/%L               "total lines
set statusline +=%4c\              "column number
set statusline +=0x%04B\           "character under cursor

" toggle cursorline/cursorcolumn or center line
nmap <F12> zz
if version >= 700 " NONE turns off underlining
	highlight CursorLine NONE ctermbg=Yellow
	highlight CursorColumn NONE ctermbg=Yellow
	let s:lico=0
	function LiCo()
	let s:lico=s:lico>2 ?0 :s:lico+1
	let &cursorline=s:lico % 2
	let &cursorcolumn=s:lico / 2
	endfun
	nmap <silent> <F12> :call LiCo()<cr>
	endif
	imap <F12> <c-o><F12>
	vmap <F12> <c-c><F12>gv

" insert blank line without into insert mode
map <S-Enter> O<Esc>
map <CR> o<ESc>k

" // --- Copy/Paste Cross Session --- //
" 不同檔案之間的複製/貼上 
" 用法：Ctrl+V選取欲複製行, Shift+Y複製, 跳到另外一個檔案Shift+P貼上

"custom copy'n'paste
"copy the current visual selection to ~/.vbuf
vmap <S-y> :w! ~/.vbuf<CR>
"copy the current line to the buffer file if no visual selection
nmap <S-y> :.w! ~/.vbuf<CR>
"paste the contents of the buffer file
nmap <S-p> :r ~/.vbuf<CR>


" // --- Ctags Plugin --- //

set tags=tags;/
" configure tags - add additional tags here
set tags+=~/.vim/tags/cpp
" build tags of your own project with Ctrl-L
nmap <C-L> :!ctags -R --sort=yes --c++-kinds=+p --fields=+iaS --extra=+q .<CR>


" // --- Taglist Plugin ---//

let Tlist_Show_One_File = 1
nnoremap <silent> <F6> :TlistToggle<CR>


" // --- CSCOPE Plugin ---//

" 讓子目錄也可以利用根目錄建構出的 cscope.out 檔案
function s:FindFile(file)
    let curdir = getcwd()
    let found = curdir
    while !filereadable(a:file) && found != "/"
        cd ..
        let found = getcwd()
    endwhile
    execute "cd " . curdir
    return found
endfunction

if has("cscope")

    " use both cscope and ctag for 'ctrl-]', ':ta', and 'vim -t'
    set cscopetag

    " check cscope for definition of a symbol before checking ctags: set to 1
    " if you want the reverse search order.
    set csto=0

    " add any cscope database in current directory
    let $CSCOPE_DIR=s:FindFile("cscope.out")
    let $CSCOPE_DB=$CSCOPE_DIR."/cscope.out"
    if filereadable($CSCOPE_DB)
        cscope add $CSCOPE_DB $CSCOPE_DIR
    endif

    command -nargs=0 Cscope !cscope -ub -R &
    " show msg when any other cscope db added
    set cscopeverbose

    " disable autocscope auto mappings
    let g:autocscope_menus = 0

    """"""""""""" cscope/vim key mappings
    "
    " The following maps all invoke one of the following cscope search types:
    "
    "   's'   symbol: find all references to the token under cursor
    "   'g'   global: find global definition(s) of the token under cursor
    "   'c'   calls:  find all calls to the function name under cursor
    "   't'   text:   find all instances of the text under cursor
    "   'e'   egrep:  egrep search for the word under cursor
    "   'f'   file:   open the filename under cursor
    "   'i'   includes: find files that include the filename under cursor
    "   'd'   called: find functions that function under cursor calls

    nmap <C-\>s :cs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>g :cs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>c :cs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>t :cs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>e :cs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
    nmap <C-\>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
    nmap <C-\>d :cs find d <C-R>=expand("<cword>")<CR><CR>

    nmap <C-a>s :scs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>g :scs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>c :scs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>t :scs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>e :scs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>f :scs find f <C-R>=expand("<cfile>")<CR><CR>
    nmap <C-a>i :scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
    nmap <C-a>d :scs find d <C-R>=expand("<cword>")<CR><CR>

    nmap <C-a><C-a>s :vert scs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a><C-a>g :vert scs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a><C-a>c :vert scs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a><C-a>t :vert scs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a><C-a>e :vert scs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a><C-a>f :vert scs find f <C-R>=expand("<cfile>")<CR><CR>
    nmap <C-a><C-a>i :vert scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
    nmap <C-a><C-a>d :vert scs find d <C-R>=expand("<cword>")<CR><CR>

endif

" // --- Color Scheme ---//
colorscheme desert

" Cursor color
"highlight Cursor ctermbg=Green
"highlight Cursor  guifg=white guibg=steelblue
"highlight iCursor guifg=white guibg=steelblue
"set guicursor=n-v-c:block-Cursor
"set guicursor+=i:ver100-iCursor
"set guicursor+=n-v-c:blinkon0
"set guicursor+=i:blinkwait10

" Cscope result color
"hi ModeMsg guifg=black guibg=#C6C5FE gui=BOLD ctermfg=black ctermbg=cyan cterm=BOLD


" // --- Mark Redundant Spaces ---//
" 用法：按F3標示出多餘空白, 持續按N向下搜尋, 按X刪除
function ShowSpaces(...)
	let @/='\v(\s+$)|( +\ze\t)'
	let oldhlsearch=&hlsearch
	if !a:0
	let &hlsearch=!&hlsearch
	else
	let &hlsearch=a:1
	end
	return oldhlsearch
endfunction

function TrimSpaces() range
	let oldhlsearch=ShowSpaces(1)
	execute a:firstline.",".a:lastline."substitute ///gec"
	let &hlsearch=oldhlsearch
endfunction

command -bar -nargs=? ShowSpaces call ShowSpaces(<args>)
command -bar -nargs=0 -range=% TrimSpaces <line1>,<line2>call TrimSpaces()
nnoremap <F3>     :ShowSpaces 1<CR>


" // --- MiniBufExplorer --- //
function! <SID>CycleBuffer(forward)

  " The following hack handles the case where we only have one
  " window open and it is too small
  let l:saveAutoUpdate = g:miniBufExplorerAutoUpdate
  if (winbufnr(2) == -1)
    resize
    let g:miniBufExplorerAutoUpdate = 0
  endif
  
  " Change buffer (keeping track of before and after buffers)
  let l:origBuf = bufnr('%')
  if (a:forward == 1)
    bn!
  else
    bp!
  endif
  let l:curBuf  = bufnr('%')

  " Skip any non-modifiable buffers, but don't cycle forever
  " This should stop us from stopping in any of the [Explorers]
  while getbufvar(l:curBuf, '&modifiable') == 0 && l:origBuf != l:curBuf
    if (a:forward == 1)
        bn!
    else
        bp!
    endif
    let l:curBuf = bufnr('%')
  endwhile

  let g:miniBufExplorerAutoUpdate = l:saveAutoUpdate
  if (l:saveAutoUpdate == 1)
    "call <SID>AutoUpdate(-1,bufnr("%"))
  endif

endfunction

let mapleader = ","
noremap <silent> <leader>n :call <SID>CycleBuffer(1)<CR>:<BS>
noremap <silent> <leader>p :call <SID>CycleBuffer(0)<CR>:<BS>

" // --- Show Function Name --- //
fun! ShowFuncName()
  let lnum = line(".")
  let col = col(".")
  echohl ModeMsg
  echo getline(search("^[^ \t#/]\\{2}.*[^:]\s*$", 'bW'))
  echohl None
  call search("\\%" . lnum . "l" . "\\%" . col . "c")
endfun
map f :call ShowFuncName() <CR>
