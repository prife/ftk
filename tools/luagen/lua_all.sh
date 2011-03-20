#!/bin/bash

for f in idl/*.idl;
do
	echo $f
	./luagen $f
done

./luagen idl/ftk_typedef.idl 1

cp -fv lua_ftk*.h  ../../script_binding/lua/
cp -fv lua_ftk*.c  ../../script_binding/lua/

