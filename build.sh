#!/bin/bash

set -e

date

#################################################################
# Update Ubuntu and install prerequisites for running altcommunitycoin   #
#################################################################
sudo apt-get update
#################################################################
# Build altcommunitycoin from source                                     #
#################################################################
NPROC=$(nproc)
echo "nproc: $NPROC"
#################################################################
# Install all necessary packages for building altcommunitycoin           #
#################################################################
sudo apt-get install -y qt4-qmake libqt4-dev libminiupnpc-dev libdb++-dev libdb-dev libcrypto++-dev libqrencode-dev libboost-all-dev build-essential libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libssl-dev ufw git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

# By default, assume running within repo
repo=$(pwd)
file=$repo/src/altcommunitycoind
if [ ! -e "$file" ]; then
	# Now assume running outside and repo has been downloaded and named altcommunitycoin
	if [ ! -e "$repo/altcommunitycoin/build.sh" ]; then
		# if not, download the repo and name it altcommunitycoin
		git clone https://github.com/altcommunitycoind/source altcommunitycoin
	fi
	repo=$repo/altcommunitycoin
	file=$repo/src/altcommunitycoind
	cd $repo/src/
fi
make -j$NPROC -f makefile.unix

cp $repo/src/altcommunitycoind /usr/bin/altcommunitycoind

################################################################
# Configure to auto start at boot                                      #
################################################################
file=$HOME/.altcommunitycoin
if [ ! -e "$file" ]
then
        mkdir $HOME/.altcommunitycoin
fi
printf '%s\n%s\n%s\n%s\n' 'daemon=1' 'server=1' 'rpcuser=u' 'rpcpassword=p' | tee $HOME/.altcommunitycoin/altcommunitycoin.conf
file=/etc/init.d/altcommunitycoin
if [ ! -e "$file" ]
then
        printf '%s\n%s\n' '#!/bin/sh' 'sudo altcommunitycoind' | sudo tee /etc/init.d/altcommunitycoin
        sudo chmod +x /etc/init.d/altcommunitycoin
        sudo update-rc.d altcommunitycoin defaults
fi

/usr/bin/altcommunitycoind
echo "altcommunitycoin has been setup successfully and is running..."

