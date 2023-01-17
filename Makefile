make:
        @echo "Compiling file"
        ./make.sh

rundb:
        ./db
run:
        ./main

clean:
        rm ./main
        rm ./db


makeclean:
        rm ./main
        rm ./db
        ./make.sh

say_hello:
        @echo "Hello World"
