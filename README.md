Small programme that sorts a CSV-file of persons by age. 
It can take a user defined max age and return every person below that age.
It can either read a specified input file and output to a specified output file (creating it if it doesn't exist already), or it can just use stdin and stdout.



## Compile and run
To compile quick and dirty with clang just

```bash
#for macOs/Linux
clang -std=c2x -g -o main src/main.c
```
or
```bash
#for Windows (I think this is how)
clang -std=c2x -g -o main.exe src/main.c
```

And then run
```bash
#for macOs/Linux
./main
```
Or
```cmd
#for Windows (I think this is how)
main.exe
```


## Flowchart

![Simplified flowchart of CSV-Filter](https://i.imgur.com/oQF2MCU.png)

