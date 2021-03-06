
" // --- Vundle --- //
set nocompatible               " be iMproved
filetype off                   " required!

set rtp+=~/.vim/bundle/vundle/
call vundle#rc()

" let Vundle manage Vundle
" required!
Bundle 'gmarik/vundle'

" My Bundles here:
"
" original repos on github
Bundle 'christoomey/vim-tmux-navigator'
Bundle 'elzr/vim-json'
Bundle 'ervandew/supertab'
Bundle 'fholgado/minibufexpl.vim'
Bundle 'godlygeek/tabular'
Bundle 'jnwhiteh/vim-golang'
Bundle 'junegunn/seoul256.vim'
Bundle 'kien/ctrlp.vim'
Bundle 'klen/python-mode'
Bundle 'Lokaltog/vim-easymotion'
Bundle 'mileszs/ack.vim'
Bundle 'plasticboy/vim-markdown'
Bundle 'scrooloose/nerdtree'
Bundle 'tomtom/tcomment_vim'
Bundle 'Townk/vim-autoclose'
Bundle 'tpope/vim-fugitive'
Bundle 'tpope/vim-repeat'
Bundle 'tpope/vim-surround'
Bundle 'goldfeld/vim-seek'
Bundle 'rking/ag.vim'
Bundle 'junegunn/goyo.vim'

" vim-scripts repos
Bundle 'dbext.vim'
Bundle 'L9'
Bundle 'OmniCppComplete'
" non github repos

" git repos on your local machine (ie. when working on your own plugin)

" ...


"// --- General --- //
syntax on
filetype plugin indent on

set autoindent
set autoread
set confirm
set hidden
set hlsearch
set ignorecase
set modeline
set nobackup
set nocompatible
set nofoldenable
set noswapfile
set showcmd
set smartcase
set smartindent
set splitright

set backspace=2
set clipboard=unnamedplus
set completeopt+=longest
set encoding=utf-8
set fileencodings=utf-8,cp950
set history=1000
set ls=2
set pastetoggle=<F2>
set scrolloff=999 " keep cursor at center when page up/down
set shiftwidth=4
set tabstop=4
"set expandtab

autocmd Filetype gitcommit setlocal spell textwidth=72 " git commit format check


"// --- Appearance --- //
colorscheme seoul256
let g:seoul256_background = 233
let python_highlight_all=1
set cursorline			"cursor highlight
set t_Co=256


" status line appearance
set statusline=%1*%F\ %*                        " filepath
set statusline+=[%{strlen(&fenc)?&fenc:'none'}, " file encoding
set statusline+=%{&ff}]                         " file format
set statusline+=%h                              " help file flag
set statusline+=%m                              " modified flag
set statusline+=%r                              " read only flag
set statusline+=%y                              " filetype
set statusline+=%=                              " left/right separator
set statusline+=%c,                             " cursor column
set statusline+=%l/%L                           " cursor line/total lines
set statusline+=\ %P                            " percent through file

hi User1 ctermfg=blue ctermbg=black
hi User2 ctermfg=red  ctermbg=black


"// ---  Keys Mapping --- //
let mapleader = ","

:map<F8> a<C-R> <pre><code class="prettyprint"><CR><ESC>
" nmap <leader>a :Ack<cr>
nmap <leader>a :Ag<cr>

" switch window faster
"nnoremap <C-h> <C-w>h
"nnoremap <C-j> <C-w>j
"nnoremap <C-k> <C-w>k
"nnoremap <C-l> <C-w>l

" let pointer stay after visual block yanking
:vmap y ygv<Esc>

" Disable direction key
:imap jk   <Esc>
:noremap   <up>     <nop>
:noremap   <down>   <nop>
:noremap   <left>   <nop>
:noremap   <right>  <nop>

" keep line in center
nmap <space> zz
nmap n nzz
nmap N Nzz

" split window
nnoremap <silent> vv <C-w>v
nnoremap <silent> ss <C-w>s

" insert blank line without into insert mode
map <S-Enter> O<Esc>
map <CR> o<Esc>

" copy/paste cross session
" How: 'Ctrl+V' select the rows you want to copy, 'Shift+Y' copy, jump to anthoer buffer, 'Shift+P' paste
" *** ------------------------ ***
" copy the current visual selection to ~/.vbuf
vmap <S-y> :w! ~/.vbuf<CR>
" copy the current line to the buffer file if no visual selection
nmap <S-y> :.w! ~/.vbuf<CR>
" paste the contents of the buffer file
nmap <S-p> :r ~/.vbuf<CR>

" mark redundant spaces
" How: 'F3' mark redundant spaces, 'N' to search next
" <leader>+w to remove all trailing space
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

command -bar -nargs=? ShowSpaces call ShowSpaces(<args>)
nnoremap <F3>     :ShowSpaces 1<CR>

func! DeleteTrailingWS()
  exe "normal mz"
  %s/\s\+$//ge
  exe "normal `z"
endfunc
noremap <leader>w :call DeleteTrailingWS()<CR>

" show function name
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

" quickfix window
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


" // === Plugins Start === //

" // --- Omni-Completion --- //
" If you prefer the Omni-Completion tip window to close when a selection is
" made, these lines close it on movement in insert mode or when leaving insert mode
autocmd CursorMovedI * if pumvisible() == 0|pclose|endif
autocmd InsertLeave * if pumvisible() == 0|pclose|endif


" // --- Python-mode plugin --- //
" Disable pylint checking every save
let g:pymode_lint_write = 0
let g:pymode_folding = 0


"// --- Vim indent guide plugin --- //
let g:indent_guides_start_level = 2
let g:indent_guides_guide_size = 1


"// --- Ctags plugin --- //
set tags=tags;/
" configure tags - add additional tags here
set tags+=~/.vim/tags/cpp
" build tags of your own project with Ctrl-L
function! UpdateTags()
    execute ":!ctags -R --languages=C++ --c++-kinds=+p --fields=+iaS --extra=+q ./"
    echohl StatusLine | echo "C/C++ tag updated" | echohl None
endfunction
"nnoremap <C-L> :call UpdateTags()))


"// --- Taglist plugin ---//
let Tlist_Show_One_File = 1
let Tlist_Use_Right_Window   = 1
nnoremap <silent> <F6> :TlistToggle<CR>


"// --- NERDTree plugin ---//
map <leader>r :NERDTreeFind<cr>
nmap <silent> <C-N> :NERDTreeToggle<CR>
let g:NERDTreeWinPos = "left"


"// --- fugitive plugin --- //
nnoremap <silent> <leader>gs :Gstatus<CR>
nnoremap <silent> <leader>gd :Gdiff<CR>
nnoremap <silent> <leader>gc :Gcommit<CR>
nnoremap <silent> <leader>gl :Glog<CR>
nnoremap <silent> <leader>gp :Git push<CR>


"// --- Ack plugin ---//
let g:ackprg="ack-grep -H --nocolor --nogroup --column"


"// --- MiniBufExplorer plugin --- //
function! <SID>CycleBuffer(forward)

" The following hack handles the case where we only have one
" window open and it is too small
if (winbufnr(2) == -1)
resize
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

endfunction

" switching to buffer 1 - 9 is mapped to ,[nOfBuffer]
for buffer_no in range(1, 9)
  execute "nmap <Leader>" . buffer_no . " :b" . buffer_no . "\<CR>"
endfor

noremap <silent> <leader>n :call <SID>CycleBuffer(1)<CR>:<BS>
noremap <silent> <leader>p :call <SID>CycleBuffer(0)<CR>:<BS>


"// --- Ctrlp plugin --- //
let g:ctrlp_map = '<c-p>'
let g:ctrlp_cmd = 'CtrlP'
let g:ctrlp_working_path_mode = 'ra'
set wildignore+=*/tmp/*,*/bin/*,*/target/*,*.so,*.swp,*.zip     " MacOSX/Linux
set wildignore+=*\\tmp\\*,*.swp,*.zip,*.exe  " Windows  "
let g:ctrlp_custom_ignore = {
	\ 'dir':  '\v[\/]\.(git|hg|svn)$',
	\ 'file': '\v\.(exe|so|dll|class)$',
	\ 'link': 'SOME_BAD_SYMBOLIC_LINKS',
	\ }

let g:ctrlp_max_files = 1000000 " 加大 cache 索引的檔案數, 否則會漏找檔案


" // --- Vimwiki plugin --- //
let g:vimwiki_list = [{'path': '~/Dropbox/vimwiki/',
			\'template_path': '~/Dropbox/vimwiki/template/',
			\'template_default': 'default',
			\'template_ext': '.html',
			\'path_html': '~/Dropbox/github/vimwiki/'}]

" 對中文用戶來說，我們並不怎麼需要駝峰英文成為維基詞條
let g:vimwiki_camel_case = 0
" 標記為完成的 checklist 項目會有特別的顏色
let g:vimwiki_hl_cb_checked = 1
" 我的 vim 是沒有菜單的，加一個 vimwiki 菜單項也沒有意義
let g:vimwiki_menu = ''
" 是否開啟按語法折疊  會讓文件比較慢
let g:vimwiki_folding = 1
" 是否在計算字串長度時用特別考慮中文字符
let g:vimwiki_CJK_length = 1
" 支援html標記符
let g:vimwiki_valid_html_tags='b,i,s,u,sub,sup,kbd,del,br,hr,div,code,h1,pre'

map <F4> :VimwikiAll2HTML<cr>
"map <F4> :Vimwiki2HTML<cr>


" // --- Markdown to HTML --- //
nmap <leader>q :%!/Users/brownylin/Dropbox/Markdown.pl --html4tags <cr>


" // --- Supertab plugin --- //
let g:SuperTabMappingForward="<tab>"
let g:SuperTabLongestEnhanced = 1
let g:SuperTabLongestHighlight = 1
let g:SuperTabDefaultCompletionType = "context"


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

