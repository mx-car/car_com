# carcom
This is the PC motor driver client zu communicate with the acutal motor shields.

## build and install

```
git clone https://github.com/mx-car/carcom.git
mkdir  carcom/build
cd carcom/build

# you can install the lib into your home folder 
# but you have to set an enviroment varible CarCom_DIR afterwards
export CARCOM_INSTALL_PREFIX=/opt/carcom
cmake -DCMAKE_INSTALL_PREFIX=$CARCOM_INSTALL_PREFIX ..

make
sudo make install 

# extend the ld search path
sudo sh -c "echo $CARCOM_INSTALL_PREFIX/lib > /etc/ld.so.conf.d/carcom.conf"
sudo ldconfig

# make cmake config avaliable
echo "export CarCom_DIR=$CARCOM_INSTALL_PREFIX/lib/CMake/CarCom" >> ~/.bashrc
```

## Demo
```
/opt/carcom/bin/demo_car_com -r 100 # to run the motor with 100 rps
```
## Troubleshooting
* If you are compiling it on a RPI then you will have some problems compiling. To remedy them all, type in the following command under carcom/ :#

`find . -type f | xargs sed -i  's/if defined(__amd64__)/if defined(__amd64__) || defined(__aarch64__)/g'`

Doing so will prevent the compiler from compiling Arduino code for the RPI.
* You might have an issue with some Boost functionality cannot be found alltough you have Boost properly installed. Try adding
`add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)`
to the top level CMakeLists.txt
* The default comport is dev/ttyACM0 and this might not always be the case. If you have trouble communicating with the device, try changing the com port.
  ```
  demo_car_com -m /dev/ttyACM1 -r 100 # to run the motor with 100 rps
  ```
  Try with --help for more options.
