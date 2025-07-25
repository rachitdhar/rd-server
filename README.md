# rd-server

## Running the server

For first time compilation:

```
make compile-libs
```

This allows the creation of the sqlite3.o and mongoose.o object files, so that they need not have to be compiled each time, and the .o files can be used directly as is.

For compile and run:

```
make
```

or, for linux

```
make build-linux
make run
```

The definitions for these are given in the Makefile.

## Resources

I wanted to go for as simple libraries as possible. So basically I have taken reasonably robust, single file libraries (i.e., to use them, you just have to include their respective .c and .h file).

- For HTTP server: Mongoose (https://github.com/cesanta/mongoose)
- For Database: SQLite - C Source code as amalgamation (https://www.sqlite.org/download.html)

## References

For building with MinGW on Windows, there is the requirement for two linker tags with the gcc command. One is -lw2\_32.
However, the second one (-D\_POSIX\_C\_SOURCE=200000L) is not so obvious, and was found eventually from the following references that also were facing a similar issue:

- https://github.com/cesanta/mongoose/discussions/1408
- https://github.com/cesanta/mongoose/commit/d311e967fa4383ebcabb146fa826de87b91bb3a7
