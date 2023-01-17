#!/bin/bash
gcc -g -o db db.c $(mariadb_config --include --libs)
