
echo "Installing dependencies"
mkdir -p deps
cd deps

if [ ! -f bcm2835-1.50.tar.gz ]
then
    wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.50.tar.gz
fi

if [ ! -d bcm2835-1.50 ]
then
    tar zxvf bcm2835-1.50.tar.gz
fi

cd bcm2835-1.50
./configure
make
sudo make check
sudo make install
cd ../