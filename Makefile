make:
        @echo "Compiling file"
        @./make.sh

run:
        ./main

clean:
        rm ./main


makeclean:
        @rm ./main
        @./make.sh

say_hello:
        @echo "Hello World"
