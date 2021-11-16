# burrOS

## About
This project was made as an assignment for a course called 'Sistemas operativos' (Operating systems).

## To run the proyect:

1- Install the following packages before building the Toolchain and Kernel:

```
sudo apt-get install nasm qemu gcc make
```

2- Build the Toolchain

Run the following commands from the repository directory:
```
cd Toolchain
make all
```

3- Build the Kernel

From the repository directory:
```
make all
```

4- Run the Kernel

From the directory repository:
```
./run.sh
```
