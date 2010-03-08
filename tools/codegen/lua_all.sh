#!/bin/bash

for f in idl/*.idl;
do
	echo $f
	./luagen $f
done
