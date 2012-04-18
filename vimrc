call pathogen#infect()

"// --- General --- //

syntax on
filetype on

set nu
set confirm
set modeline
set showcmd
set nobackup
set hlsearch
set autoindent
set noswapfile
set smartindent
set nocompatible

set ls=2
set tabstop=4
set backspace=2
set shiftwidth=4
set encoding=utf-8
set fileencodings=utf-8,cp950

" cursor highlight
set cursorline
"highlight CursorLine cterm=NONE ctermbg=Black
"autocmd InsertEnter * set nocursorline
"autocmd InsertLeave * set cursorline

"// --- Appearance --- //

"set textwidth=90
"set expandtab
colorscheme desert

" status line appearance
set statusline=
"set statusline +=\ %n\             "buffer number
"set statusline +=%{&ff}            "file format
"set statusline +=%y%*              "file type
set statusline +=\ %<%F            "full path
set statusline +=%m                "modified flag
set statusline +=%=%5l             "current line
set statusline +=/%L               "total lines
"set statusline +=%4c\              "column number
"set statusline +=0x%04B\           "character under cursor

"// ---  Keys Mapping --- //
:map<F9> a<C-R> DISP_INFO_LN("[BBB]\n");<CR><ESC>

" move up/down quickly by using Ctrl-j, Ctrl-k which will move us around by functions
nnoremap <silent> <C-j> }
nnoremap <silent> <C-k> {

" map K for :GitGrep
nnoremap <silent> K :GitGrep <cword><CR>

" split window
nnoremap <silent> vv <C-w>v
nnoremap <silent> ss <C-w>s

" insert blank line without into insert mode
" 按enter新增一個空行
map <S-Enter> O<Esc>
map <CR> o<ESc>k

" Copy/Paste Cross Session
" 不同檔案之間的複製/貼上
" 用法：Ctrl+V選取欲複製行, Shift+Y複製, 跳到另外一個檔案Shift+P貼上

"custom copy'n'paste
"copy the current visual selection to ~/.vbuf
vmap <S-y> :w! ~/.vbuf<CR>
"copy the current line to the buffer file if no visual selection
nmap <S-y> :.w! ~/.vbuf<CR>
"paste the contents of the buffer file
nmap <S-p> :r ~/.vbuf<CR>

" Mark Redundant Spaces
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


"// --- Ctags Plugin --- //

set tags=tags;/
" configure tags - add additional tags here
set tags+=~/.vim/tags/cpp
" build tags of your own project with Ctrl-L
nmap <C-L> :!ctags -R --sort=yes --c++-kinds=+p --fields=+iaS --extra=+q .<CR>


"// --- Taglist Plugin ---//

let Tlist_Show_One_File = 1
nnoremap <silent> <F6> :TlistToggle<CR>


"// --- CSCOPE Plugin ---//

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

"// --- fugitive --- //
nnoremap <silent> <leader>gs :Gstatus<CR>
nnoremap <silent> <leader>gd :Gdiff<CR>
nnoremap <silent> <leader>gc :Gcommit<CR>
nnoremap <silent> <leader>gl :Glog<CR>
nnoremap <silent> <leader>gp :Git push<CR>


"// --- Source Explorer --- //
" // The switch of the Source Explorer
nmap <F8> :SrcExplToggle<CR>

" // Set the height of Source Explorer window
let g:SrcExpl_winHeight = 8

" // Set 100 ms for refreshing the Source Explorer
let g:SrcExpl_refreshTime = 100

" // Set "Enter" key to jump into the exact definition context
" let g:SrcExpl_jumpKey = "<ENTER>"

" // Set "Space" key for back from the definition context
let g:SrcExpl_gobackKey = "<SPACE>"

" // In order to Avoid conflicts, the Source Explorer should know what plugins
" // are using buffers. And you need add their bufname into the list below
" // according to the command ":buffers!"
let g:SrcExpl_pluginList = [
\ "__Tag_List__",
\ "_NERD_tree_",
\ "-MiniBufExplorer-",
\ "Source_Explorer"
\ ]

" // Enable/Disable the local definition searching, and note that this is not
" // guaranteed to work, the Source Explorer doesn't check the syntax for now.
" // It only searches for a match with the keyword according to command 'gd'
let g:SrcExpl_searchLocalDef = 1

" // Do not let the Source Explorer update the tags file when opening
let g:SrcExpl_isUpdateTags = 0

" // Use 'Exuberant Ctags' with '--sort=foldcase -R .' or '-L cscope.files' to
" //  create/update a tags file
let g:SrcExpl_updateTagsCmd = "ctags --sort=foldcase -R ."

" // Set "<F12>" key for updating the tags file artificially
let g:SrcExpl_updateTagsKey = "<F12>"


"// --- minibufexpl --- //
" switching to buffer 1 - 9 is mapped to ,[nOfBuffer]
let mapleader = ","
for buffer_no in range(1, 9)
  execute "nmap <Leader>" . buffer_no . " :b" . buffer_no . "\<CR>"
endfor
