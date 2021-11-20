all : clean grasix

grasix : grasix.o network.o worker.o dll.o loader.o command.o base.o attribute.o node.o writer.o relation.o
	gcc grasix.o network.o worker.o dll.o loader.o command.o base.o attribute.o node.o writer.o relation.o -pthread -ggdb -o grasix

grasix.o : grasix.c grasix.h
	gcc -c -ggdb grasix.c

network.o : network.c network.h
	gcc -c -ggdb network.c -pthread -o network.o

worker.o : worker.c worker.h
	gcc -c -ggdb worker.c -pthread -o worker.o

dll.o : dll.c dll.h
	gcc -c -ggdb dll.c -o dll.o

loader.o : loader.h loader.h
	gcc -c -ggdb loader.c -o loader.o

command.o : command.c command.h
	gcc -c -ggdb command.c -o command.o

base.o : base.c base.h
	gcc -c -ggdb base.c -o base.o

attribute.o : attribute.c attribute.h
	gcc -c -ggdb attribute.c -o attribute.o

node.o : node.c node.h
	gcc -c -ggdb node.c -o node.o

writer.o : writer.c writer.h
	gcc -c -ggdb writer.c -o writer.o

relation.o : relation.c relation.h
	gcc -c -ggdb relation.c -o relation.o

clean :
	rm *.o

clean_all :
	rm *.o
	rm *~
