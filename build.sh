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
sudo apt-get install build-essential libssl-dev libdb++-dev libdb-dev libboost-all-dev libqrencode-dev libminiupnpc-dev
sudo apt-get update

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

