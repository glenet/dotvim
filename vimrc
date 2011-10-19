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
set textwidth=80
set encoding=utf-8
"set guifont=Monaco\ 12  " ubuntu
set guifont=Monaco:h14 " windows/mac
set fileencodings=utf-8,cp950


" // --- Copy/Paste Cross Session --- //

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
    "
    " Below are three sets of the maps: one set that just jumps to your
    " search result, one that splits the existing vim window horizontally and
    " diplays your search result in the new window, and one that does the same
    " thing, but does a vertical split instead (vim 6 only).
    "
    " I've used CTRL-\ and CTRL-@ as the starting keys for these maps, as it's
    " unlikely that you need their default mappings (CTRL-\'s default use is
    " as part of CTRL-\ CTRL-N typemap, which basically just does the same
    " thing as hitting 'escape': CTRL-@ doesn't seem to have any default use).
    " If you don't like using 'CTRL-@' or CTRL-\, , you can change some or all
    " of these maps to use other keys.  One likely candidate is 'CTRL-_'
    " (which also maps to CTRL-/, which is easier to type).  By default it is
    " used to switch between Hebrew and English keyboard mode.
    "
    " All of the maps involving the <cfile> macro use '^<cfile>$': this is so
    " that searches over '#include <time.h>" return only references to
    " 'time.h', and not 'sys/time.h', etc. (by default cscope will return all
    " files that contain 'time.h' as part of their name).


    " To do the first type of search, hit 'CTRL-\', followed by one of the
    " cscope search types above (s,g,c,t,e,f,i,d).  The result of your cscope
    " search will be displayed in the current window.  You can use CTRL-T to
    " go back to where you were before the search.
    "

    nmap <C-\>s :cs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>g :cs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>c :cs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>t :cs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>e :cs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
    nmap <C-\>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
    nmap <C-\>d :cs find d <C-R>=expand("<cword>")<CR><CR>


    " Using 'CTRL-spacebar' (intepreted as CTRL-@ by vim) then a search type
    " makes the vim window split horizontally, with search result displayed in
    " the new window.
    "
    " (Note: earlier versions of vim may not have the :scs command, but it
    " can be simulated roughly via:
    "    nmap <C-@>s <C-W><C-S> :cs find s <C-R>=expand("<cword>")<CR><CR>

    nmap <C-a>s :scs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>g :scs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>c :scs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>t :scs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>e :scs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-a>f :scs find f <C-R>=expand("<cfile>")<CR><CR>
    nmap <C-a>i :scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
    nmap <C-a>d :scs find d <C-R>=expand("<cword>")<CR><CR>


    " Hitting CTRL-space *twice* before the search type does a vertical
    " split instead of a horizontal one (vim 6 and up only)
    "
    " (Note: you may wish to put a 'set splitright' in your .vimrc
    " if you prefer the new window on the right instead of the left

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


" Highlight Class and Function names
"syn match    cCustomParen    "(" contains=cParen
"syn match    cCustomFunc     "\w\+\s*(" contains=cCustomParen
"syn match    cCustomScope    "::"
"syn match    cCustomClass    "\w\+\s*::" contains=cCustomScope

"hi cCustomFunc	guifg=yellowgreen
"hi cCustomClass guifg=#00FF00


