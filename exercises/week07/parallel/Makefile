CC = mpicc
CFLAGS = -O2
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg

PROJ = exercise_3
OBJS = exercise_3.o

all : simple-jpeg $(PROJ)

exercise_3 : $(OBJS)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

simple-jpeg :
	$(MAKE) -C ../simple-jpeg

obj-clean:
	$(RM) *.o

exec-clean:
	$(RM) $(PROJ)

autosave-clean:
	$(RM) *~

clean:
	$(MAKE) obj-clean
	$(MAKE) exec-clean
	$(MAKE) autosave-clean
