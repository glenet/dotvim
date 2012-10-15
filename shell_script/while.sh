
#!/bin/sh

c=1
while [ $c -le 1000 ]
do
	adb shell "echo 4 > /sys/devices/tegradc.0/graphics/fb0/blank"
	sleep 0.5
	adb shell "echo 0 > /sys/devices/tegradc.0/graphics/fb0/blank"
	sleep 0.5
	(( c++ ))
done


