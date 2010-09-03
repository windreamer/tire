#!/bin/bash
for FILE in $(find . -name bakefile.xml)
do 
    bakefile -f gnu -o $(dirname $FILE)/Makefile $FILE
done
