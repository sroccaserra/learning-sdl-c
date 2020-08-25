augroup project
  autocmd!
  autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END

let g:ale_c_cc_options = '-std=c18 -Wall -Wextra -Wpedantic -Werror -Iinclude -I/usr/local/include/SDL2'
