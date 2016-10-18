DIRS = barrier chase-lev-deque dekker-fences linuxrwlocks mcs-lock mpmc-queue ms-queue spsc-queue

all :
	-for d in $(DIRS); do (cd $$d; $(MAKE) ); done

clean :
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done
