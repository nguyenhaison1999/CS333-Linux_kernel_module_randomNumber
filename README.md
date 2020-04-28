# CS333-Linux_kernel_module_randomNumber

Project 1 on **CS333**, create a Linux Kernel module that generates random number

### How to run
1. Open terminal in the directiory or `cd` into it
2. Run `make all` to compile the module
3. To insert the module  
```
sudo insmod vchar_drvier.ko
```
4. To check the module, use `dmesg` for the kernel log or `lsmod | grep vchar_drvier` for successful loaded
5. To open and read the module:
  - Compile *user_test.c* with 
  ```
  cc -o user_test user_test.c
  ```
  - Now run the file with the cmd
  ```
  sudo ./user_test
  ```
  You should see the random number output on the screen

6. To remove the module, use 
```
sudo rmmod vchar_drvier
```

## Contributing
Nguyễn Minh Nhật    - `1751090`

Nguyễn Hải Sơn      - `1751100`

Đỗ Vương Quốc Thịnh - `1751105`


## Acknowledgments

- [Vimentor](https://vimentor.com/vi/lesson/gioi-thieu-khoa-hoc) on series of Linux Device Driver

