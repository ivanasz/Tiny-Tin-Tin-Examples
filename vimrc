set hlsearch
syntax on
set spell spelllang=en_us
set mouse=a
source ~/.vim/a.vim

" turn on file-type detection and language-dependent indentation
filetype plugin indent on

" cindent options
set cino=(0,W2s,:0,l1,g0,t0,m1

" shiftwidth
set sw=4

" softtabstop
set sts=4

" expand tabs into space
set et

" autoindent
set ai

" turn on  line numbers
set number

" Eric's code
set ts=8
set sw=8
set tw=82
set noet

"delete trailing whitespace on write
"autocmd BufWritePre * :%s/\s\+$//e

if has("wildmenu")
    set wildmenu
    set wildmode=longest,list
    set wildignore+=*.a,*.o,*.jpg,*.png,.git,*~,*.swp,*.pyc
endif

let g:Whitespace_toggle=1

function! Show_whitespace()
    if g:Whitespace_toggle
        hi WhiteSpace cterm=NONE ctermbg=NONE guibg=NONE gui=NONE
        let g:Whitespace_toggle=0
    else
        hi WhiteSpace cterm=BOLD ctermbg=RED guibg=#cbcdd4
        match WhiteSpace '\s\+$'
        let g:Whitespace_toggle=1
    endif
endfunction

" un-comment below if you want on by default
call Show_whitespace()

" for toggling whitespace
nnoremap <leader>2 :call Show_whitespace()<cr>

" for deleting all trailing whitespace in a file
nnoremap <leader>w :%s/\s\+$//e<cr>

autocmd FileType h,hpp,c,cpp,python autocmd BufWritePre <buffer> :%s/\s\+$//e

" OmniCppComplete
let g:acp_enableAtStartup = 1
setlocal omnifunc=omni#cpp#complete#Main
let OmniCpp_NamespaceSearch = 1
let OmniCpp_GlobalScopeSearch = 1
let OmniCpp_ShowAccess = 1
let OmniCpp_ShowPrototypeInAbbr = 1 " show function parameters
let OmniCpp_MayCompleteDot = 1      " autocomplete after .
let OmniCpp_MayCompleteArrow = 1    " autocomplete after ->
let OmniCpp_MayCompleteScope = 1    " autocomplete after ::
let OmniCpp_DefaultNamespaces = ["std", "_GLIBCXX_STD"]
au CursorMovedI,InsertLeave * if pumvisible() == 0|silent! pclose|endif
setlocal completeopt=menuone,menu,longest,preview

" look first for the tags file in the directory of the edited file
" (and recursively in its parent directory, etc… until the root of the filesystem),
" and then in the current folder.
setlocal    tags=./tags;tags
cscope add /home/ivanas/here/lidar/libpano/Code/cscope.out

" set the right build command
command Copenmake make! -u -j8 --prefix $VIRTUAL_ENV | cope

" pathogen
" execute pathogen#infect()

" syntastic
" let g:syntastic_cpp_compiler_options=' -std=c++11'
" let g:syntastic_debug=1
" let g:synsastic_aggregate_errors=1
" let g:syntastic_cpp_check_header=1
" let g:syntastic_cpp_no_include_search=1
" let g:syntastic_cpp_no_default_include_dirs=1
" let g:syntastic_cpp_auto_refresh_includes=1
" let g:syntastic_cpp_include_dirs=['/home/ivanas/here/wenv/include','/home/ivanas/here/wenv/include/pcl-1.8','/home/ivanas/here/wenv/include/eigen3']

" Vim Doxygen Comment Continuation "
set comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,bO:///,O://
