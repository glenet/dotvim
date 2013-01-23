call pathogen#infect()

"// --- General --- //
syntax on
filetype on
filetype plugin indent on

set nu
set hidden
set confirm
set modeline
set showcmd
set nobackup
set hlsearch
set splitright
set autoindent
set noswapfile
set smartindent
set nocompatible
set ignorecase
set smartcase

set ls=2
set history=1000
set tabstop=4
set backspace=2
set shiftwidth=4
set scrolloff=999 " keep cursor at center when page up/down
set encoding=utf-8
set fileencodings=utf-8,cp950

"// --- Appearance --- //
set t_Co=256
colorscheme ir_black
set cursorline 			"cursor highlight
"set textwidth=90
"set expandtab
let python_highlight_all = 1

" *** status line appearance ***
set statusline=%1*%F\ %*                             " filepath
set statusline+=%h                                   " help file flag
set statusline+=%m                                   " modified flag
set statusline+=%r                                   " read only flag
"set statusline+=%y                                   " filetype
set statusline+=%=                                   " left/right separator
set statusline+=%c,\                                 " cursor column
set statusline+=%l/%L\                               " cursor line/total lines
set statusline+=%2*\ %{GitBranchInfoTokens()[0]}\ %* " git branch
"set statusline+=[%{strlen(&fenc)?&fenc:'none'},      " file encoding
"set statusline+=%{&ff}]                              " file format

hi User1 ctermfg=blue ctermbg=black
hi User2 ctermfg=red  ctermbg=black


"// ---  Keys Mapping --- //
:map<F4> a<C-R> <pre><code class="prettyprint"><CR><ESC>
:map<F8> a<C-R> ALOGD("[BBB] %s: ", __FUNCTION__);<CR><ESC>
:map<F9> a<C-R> printk(KERN_DEBUG "[BR] %s: "\n, __func__);<CR><ESC>

" *** keep line in center ***
nmap <space> zz
nmap n nzz
nmap N Nzz

" *** split window ***
nnoremap <silent> vv <C-w>v
nnoremap <silent> ss <C-w>s

" *** insert blank line without into insert mode ***
map <S-Enter> O<Esc> 
map <CR> o<ESc>k

" *** copy/paste cross session ***
" How: 'Ctrl+V' select the rows you want to copy, 'Shift+Y' copy, jump to anthoer buffer, 'Shift+P' paste
" *** ------------------------ ***  
" copy the current visual selection to ~/.vbuf
vmap <S-y> :w! ~/.vbuf<CR>
" copy the current line to the buffer file if no visual selection
nmap <S-y> :.w! ~/.vbuf<CR>
" paste the contents of the buffer file
nmap <S-p> :r ~/.vbuf<CR>

" *** mark redundant spaces ***
" How: 'F3' mark redundant spaces, 'N' to search next, 'X' to delete
" *** --------------------- ***  
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

" *** show function name ***
" How: '<leader>+,' shows function name
" *** ------------------ ***  
fun! ShowFuncName()
  let lnum = line(".")
  let col = col(".")
  echohl ModeMsg
  echo getline(search("^[^ \t#/]\\{2}.*[^:]\s*$", 'bW'))
  echohl None
  call search("\\%" . lnum . "l" . "\\%" . col . "c")
endfun
map f :call ShowFuncName() <CR>

" *** QUICKFIX WINDOW ***
" How: '<leader>+q' shows quickfix window
" *** --------------- ***  
command -bang -nargs=? QFix call QFixToggle(<bang>0)
function! QFixToggle(forced)
	if exists("g:qfix_win") && a:forced == 0
	cclose
	unlet g:qfix_win
	else
	copen 10
	let g:qfix_win = bufnr("$")
	endif
endfunction
nnoremap <leader>q :QFix<CR>


"// --- Ctags plugin --- //
set tags=tags;/
" configure tags - add additional tags here
set tags+=~/.vim/tags/cpp
" build tags of your own project with Ctrl-L
nmap <C-L> :!ctags -R --sort=yes --c++-kinds=+p --fields=+iaS --extra=+q .<CR>


"// --- Taglist plugin ---//
let Tlist_Show_One_File = 1
let Tlist_Use_Right_Window = 0
nnoremap <silent> <F6> :TlistToggle<CR>


"// --- NERDTree plugin ---//
nnoremap <silent> <F5> :NERDTree<CR>
let g:NERDTreeWinPos = "right"


"// --- fugitive plugin --- //
nnoremap <silent> <leader>gs :Gstatus<CR>
nnoremap <silent> <leader>gd :Gdiff<CR>
nnoremap <silent> <leader>gc :Gcommit<CR>
nnoremap <silent> <leader>gl :Glog<CR>
nnoremap <silent> <leader>gp :Git push<CR>

"// --- ack plugin ---//
let g:ackprg="ack-grep -H --nocolor --nogroup --column"

"// --- MiniBufExplorer plugin --- //
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

" switching to buffer 1 - 9 is mapped to ,[nOfBuffer]
let mapleader = ","
for buffer_no in range(1, 9)
  execute "nmap <Leader>" . buffer_no . " :b" . buffer_no . "\<CR>"
endfor

noremap <silent> <leader>n :call <SID>CycleBuffer(1)<CR>:<BS>
noremap <silent> <leader>p :call <SID>CycleBuffer(0)<CR>:<BS>


"// --- FuzzyFinder plugin --- //
map ff <esc>:FufFile<cr>
map fba <esc>:FufBookmarkFileAdd<cr>
map fb <esc>:FufBookmarkFile<cr>
map fu <esc>:FufBuffer<cr>


"// --- ctrlp plugin --- //
let g:ctrlp_map = '<c-p>'
let g:ctrlp_cmd = 'CtrlP'
let g:ctrlp_working_path_mode = 'ra'
let g:ctrlp_custom_ignore = {
	\ 'dir':  '\v[\/]\.(git|hg|svn)$',
	\ 'file': '\v\.(exe|so|dll)$',
	\ 'link': 'SOME_BAD_SYMBOLIC_LINKS',
	\ }


"// --- CSCOPE plugin ---//
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

    nmap <C-w>s :scs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w>g :scs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w>c :scs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w>t :scs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w>e :scs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w>f :scs find f <C-R>=expand("<cfile>")<CR><CR>
    nmap <C-w>i :scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
    nmap <C-w>d :scs find d <C-R>=expand("<cword>")<CR><CR>

    nmap <C-w><C-w>s :vert scs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w><C-w>g :vert scs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w><C-w>c :vert scs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w><C-w>t :vert scs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w><C-w>e :vert scs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-w><C-w>f :vert scs find f <C-R>=expand("<cfile>")<CR><CR>
    nmap <C-w><C-w>i :vert scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
    nmap <C-w><C-w>d :vert scs find d <C-R>=expand("<cword>")<CR><CR>

endif

" Cscope result color
"hi ModeMsg guifg=black guibg=#C6C5FE gui=BOLD ctermfg=black ctermbg=cyan cterm=BOLD
