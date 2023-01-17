#!/bin/bash
gcc -g -o main main.c $(mariadb_config --include --libs)
