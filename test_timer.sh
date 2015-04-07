make
sudo insmod sys_timer.ko

echo 3000 >> /sys/kernel/timer/period
sleep 10
dmesg | tail -n 10

sudo rmmod sys_timer
make clean