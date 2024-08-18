# linux_sample

#socket commands:-
ls -l /proc/3222/fd/ | grep socket
#example
ubuntu1604@ubuntu:~/gokul_space/rbus$ ls -l /proc/3222/fd/ | grep socket
lrwx------ 1 ubuntu1604 ubuntu1604 64 Aug 18 16:03 4 -> socket:[53672]
lrwx------ 1 ubuntu1604 ubuntu1604 64 Aug 18 16:03 5 -> socket:[56409]
