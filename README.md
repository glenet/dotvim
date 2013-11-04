
### Installation
	
	cd ~
	git clone git@github.com:browny/dotvim.git ~/.vim

### Create symlinks

	ln -s ~/.vim/vimrc ~/.vimrc
	ln -s ~/.vim/gitsetting/gitconfig ~/.gitconfig
	ln -s ~/.vim/gitsetting/gitignore ~/.gitignore
	ln -s ~/.vim/tmux.conf ~/.tmux.conf
	cat bashrc_append >> ~/.bashrc

### Mount submodules

	cd ~/.vim
	git submodule init
	git submodule update

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
  This plugin for Vim enable an auto-close chars feature for you. For instance if you type an '(', ``autoclose`` will automatically insert a ')' and put the cursor between them
  
* [vim-fugitive](https://github.com/tpope/vim-fugitive)
  A Git wrapper so awesome, it should be illegal

* [vim-repeat](https://github.com/tpope/vim-repeat)
  Enable repeating supported plugin maps with "."

* [vim-surround](https://github.com/tpope/vim-surround)
  Quoting/parenthesizing made simple

