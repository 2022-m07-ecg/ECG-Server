#!/bin/bash
gcc -g -o db src/db.c $(mariadb_config --include --libs)
#gcc -g -o main main.c
