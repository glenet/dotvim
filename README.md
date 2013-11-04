## The dotvim for vim/bash/tmux/git settings

I use the dotvim repository to sync the `vim`, `bash`, `tmux` and `git` settings. 
The vim plugins are managed by [vundle](https://github.com/gmarik/vundle)

### Installation
	
	cd ~
	git clone git@github.com:browny/dotvim.git ~/.vim

### Create symlinks

	ln -s ~/.vim/vimrc ~/.vimrc
	ln -s ~/.vim/gitsetting/gitconfig ~/.gitconfig
	ln -s ~/.vim/gitsetting/gitignore ~/.gitignore
	ln -s ~/.vim/tmux.conf ~/.tmux.conf
	cat bashrc_append >> ~/.bashrc

### Mount submodules managed by vundle

	git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle
	vim
	:BundleInstall

### Screenshot

![Image](https://raw.github.com/browny/dotvim/master/screenshot/vim-screenshot-20131104.png)

### Plugins

* [vim-tmux-navigator](https://github.com/christoomey/vim-tmux-navigator)
  This makes seamlessly navigate vim and tmux splits, refer to [Seamlessly Navigate Vim And Tmux Splits](http://robots.thoughtbot.com/seamlessly-navigate-vim-and-tmux-splits)

* [vim-json](https://github.com/elzr/vim-json)
  A better JSON for Vim: distinct highlighting of keywords vs values, JSON-specific (non-JS) warnings, quote concealing

* [supertab](https://github.com/ervandew/supertab)
  Perform all your vim insert mode completions with Tab

* [minibufexpl.vim](https://github.com/fholgado/minibufexpl.vim)
  Elegant buffer explorer - takes very little screen space

* [tabular](https://github.com/godlygeek/tabular)
  Text filtering and alignment

* [ctrlp.vim](https://github.com/kien/ctrlp.vim)
  Fuzzy file, buffer, mru, tag, etc finder

* [vim-easymotion](https://github.com/Lokaltog/vim-easymotion)
  Vim motions on speed!
  
* [ack.vim](https://github.com/mileszs/ack.vim)
  Vim plugin for the Perl module / CLI script 'ack'

* [vim-markdown](https://github.com/plasticboy/vim-markdown)
  Markdown Vim Mode http://plasticboy.com/markdown-vim-mode/

* [nerdtree](https://github.com/scrooloose/nerdtree)
  A tree explorer plugin for vim

* [tcomment](https://github.com/tomtom/tcomment_vim)
  An extensible and universal comment vim-plugin

* [vim-autoclose](https://github.com/Townk/vim-autoclose)
  This plugin for Vim enable an auto-close chars feature for you 
  
* [vim-fugitive](https://github.com/tpope/vim-fugitive)
  A Git wrapper so awesome, it should be illegal

* [vim-repeat](https://github.com/tpope/vim-repeat)
  Enable repeating supported plugin maps with "."

* [vim-surround](https://github.com/tpope/vim-surround)
  Quoting/parenthesizing made simple

* [dbext.vim](http://www.vim.org/scripts/script.php?script_id=356)
  This plugin contains functions/mappings/commands to enable Vim to access several databases

* [omincompletion](http://vim.wikia.com/wiki/Omni_completion)
  Omni completion provides smart autocompletion for programs

* [seoul256.vim](https://github.com/junegunn/seoul256.vim)
  A low-contrast Vim color scheme based on Seoul Colors
  
