### Usage

```
make
```

```
sudo insmod barmap.ko
```

```
sudo mount -t debugfs nodev /sys/kernel/debug/
```

To map BAR into userspace, refer to the following example:
```
int configfd;
configfd = open("/sys/kernel/debug/barmap", O_RDWR);
if(configfd < 0) {
    perror("open");
return -1;
}

char * address = NULL;
address = mmap(NULL, 16*1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, 0);
if (address == MAP_FAILED) {
    perror("mmap");
return -1;
}
```
