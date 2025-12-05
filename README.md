# ReadMe
### Authors:
David Gonçalves Bermúdez, Mikael Rudenvald and Linnea Rydberg

### How to run:
1. open a terminal on project root
2. to compile type `make`
```sh
make 
```
3. to run type `make run < N > < M >` (with N being the number of pages and M being the checker sleep time in microseconds)

Example for 64 pages and 20 microseconds:
```sh
make run 64 20
```
4. in case you want to remove the executable file, simply run:
```sh
make clean
```