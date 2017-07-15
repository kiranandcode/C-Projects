objects   = main.o countdown.o boom.o end.o
includes  = countdown.h boom.h end.h
build_dir = ./build
src_dir   = ./src
obj_dir   = ./src/temp
includes_dir = ./includes

main : $(addprefix $(obj_dir)/, $(objects))
	cc $(addprefix $(obj_dir)/, $(objects)) -o $(build_dir)/main 

$(obj_dir)/main.o : $(src_dir)/main.c $(addprefix $(includes_dir)/, $(includes))
	cc -c $(src_dir)/main.c -o $(obj_dir)/main.o

$(obj_dir)/countdown.o : $(src_dir)/countdown.c $(includes_dir)/countdown.h
	cc -c $(src_dir)/countdown.c -o $(obj_dir)/countdown.o


$(obj_dir)/boom.o : $(src_dir)/boom.c $(includes_dir)/boom.h
	cc -c $(src_dir)/boom.c -o $(obj_dir)/boom.o


$(obj_dir)/end.o : $(src_dir)/end.c $(includes_dir)/end.h
	cc -c $(src_dir)/end.c -o $(obj_dir)/end.o
