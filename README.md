# motor_shield_driver
This is the PC motor driver client zu communicate with the acutal motor shields.

## build and install

```
git clone git@github.com:mx-car/motor_shield_driver.git
mkdir  motor_shield_driver/build
cd motor_shield_driver/build

# you can install the lib into your home folder 
# but you have to set an enviroment varible CarCom_DIR afterwards
cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt/CarCom ..

make install 
export CarCom_DIR=$HOME/opt/CarCom/lib/CMake/CarCom/  
```

Only need if you installed the lib into your home folder
```
echo "export CarCom_DIR=$HOME/opt/CarCom/lib/CMake/CarCom/" >> ~/.bashrc
source ~/.bashrc
```

## Demo
```
demo_car_com -r 100 # to run the motor with 100 rps
```
