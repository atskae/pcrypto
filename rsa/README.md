# Sequential and Parallel RSA
To build, first install the [GMP library](https://gmplib.org/manual/Installing-GMP.html), then run:
```
make
```

The following executables will be build:
* `rsa`
* `test`: This runs both the sequential and parallel versions of RSA and prints out the timing measurements
* `memcheck`: This was a debug executable to find memory leaks

To measure and compare sequential and parallel RSA, run:
```
./test <message.txt>
```
where `<message.txt>` is a text file of the data to encrypt/decrypt. Sample text files can be found in the `text_files` directory.
