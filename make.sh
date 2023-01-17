#!/bin/bash
gcc -g -o db db.c $(mariadb_config --include --libs)
gcc -g -o main main.c
