#!/bin/bash
​
# MUST BE CALLED with the NAME of the MODULE YOU ARE INSERTING  ./insrem.sh jiffies  or ./insrem.sh seconds
​
# clear the message queue, then
# insert $1.ko into the kernel and remove it again, then
# display the messages as a result of doing so
​
if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters -- Usage: ./insrem.sh modulename"
    exit
fi
​
sudo -p "osc"
sudo dmesg -c 
clear 
​
printf "beginning script...\n"
printf "Module name is: %s\n\n" $1".ko"
​
sudo insmod $1".ko"
​
for time in {1..4}
do
  printf "\tsleeping for %s second" $time
  if [ "$time" -gt "1" ]; then 
    echo "s"
  fi
  echo "..."
​
  sleep $time
  cat "/proc/"$1
done
printf "\n"
​
sudo rmmod $1 
sudo dmesg 
​
printf "\n...end of script\n\n"





