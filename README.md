###Installation
	
	cd ~
	git clone git@github.com:browny/dotvim.git ~/.vim

###Create symlinks

	ln -s ~/.vim/vimrc ~/.vimrc
	ln -s ~/.vim/gitsetting/gitconfig ~/.gitconfig
	ln -s ~/.vim/gitsetting/gitignore ~/.gitignore
	ln -s ~/.vim/tmux.conf ~/.tmux.conf
	cat bashrc_append >> ~/.bashrc

###Mount submodules

	cd ~/.vim
	git submodule init
	git submodule update
