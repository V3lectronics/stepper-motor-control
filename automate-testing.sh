echo "pulling from repo..." &&
git pull &&

echo "compiling..." &&
g++ main.cpp ramp.cpp linked-list.cpp -o stepper-motor-control -lwiringPi &&

echo "executing..." &&
echo "--------------------" &&
./stepper-motor-control
