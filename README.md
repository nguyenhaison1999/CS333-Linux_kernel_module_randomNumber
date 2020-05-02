# CS333 -Project 1: Linux_kernel_module_randomNumber

Project 1 on **CS333**, create a Linux Kernel module that generates random number

### How to run
1. Open terminal in the directiory or `$ cd` into it
2. Run `$ make all` to compile the module
3. To insert the module  
```
$ sudo insmod vchar_drvier.ko
```
4. To check the module, use `dmesg` for the kernel log or `lsmod | grep vchar_driver` for successful loaded modules list
5. To open and read the module:
  - Compile *user_test.c* with 
  ```
  $ cc -o user_test user_test.c
  ```
  - Now run the file with the terminal
  ```
  $ sudo ./user_test
  ```
  You should see the random number output on your terminal. The number also appears in kernel log, use `dmesg` to check if wanted. 

6. To remove the module, use 
```
$ sudo rmmod vchar_driver
```

## Contributing
Nguyễn Minh Nhật    - `1751090`

Nguyễn Hải Sơn      - `1751100`

Đỗ Vương Quốc Thịnh - `1751105`


## Acknowledgments
- Hướng dẫn project 1 - Lập trình linux kernel module | Youtuber Long Viet (**Unpublished video**)
- [Vimentor](https://vimentor.com/vi/lesson/gioi-thieu-khoa-hoc) on series of Linux Device Driver

