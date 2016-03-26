genetics: genetics.o biology.o	
	g++ genetics.o biology.o -o darwin
	rm *.o
genetics.o: chromo.h util.h genetics.cpp
	g++ -c genetics.cpp
biology.o: biology.h chromo.h util.h biology.cpp
	g++ -c biology.cpp