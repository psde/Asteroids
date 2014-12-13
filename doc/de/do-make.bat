mkdir out\
del out\build.txt
cmd /C do-make_impl.bat %1 > out\build.txt
type out\build.txt