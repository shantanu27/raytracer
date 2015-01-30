CC = g++
DEPS = Camera.h Color.h Plane.h Object.h Sphere.h Triangle.h Ray.h Vector.h Light.h 
OBJ = Raytracer.o
VEC = Vector.o

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(INC)

Raytracer: $(OBJ) $(VEC) $(DEPS)
	$(CC) $(VEC) $(OBJ) -o Raytracer
	rm -f $(VEC) $(OBJ)

Vector.o: 
	$(CC) -c Vector.cpp

Raytracer.o: 
	$(CC) -c Raytracer.cpp 

clean:
	rm -f $(OBJ) Raytracer
	rm -f $(VEC) 