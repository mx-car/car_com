# carcom
This is the PC motor driver client zu communicate with the acutal motor shields.

## build and install

```
git clone https://github.com/mx-car/carcom.git
mkdir  carcom/build
cd carcom/build

# you can install the lib into your home folder 
# but you have to set an enviroment varible CarCom_DIR afterwards
export CARCOM_INSTALL_PREFIX=/opt/CarCom
cmake -DCMAKE_INSTALL_PREFIX=$CARCOM_INSTALL_PREFIX ..

make
sudo make install 

# extend the ld search path
sudo sh -c "echo $CARCOM_INSTALL_PREFIX/lib > /etc/ld.so.conf.d/carcom.conf"
sudo ldconfig

# make cmake config avaliable
echo "export CarCom_DIR=$CARCOM_INSTALL_PREFIX/lib/cmake/realsense2" >> ~/.bashrc
```

## Demo
```
demo_car_com -r 100 # to run the motor with 100 rps
```
