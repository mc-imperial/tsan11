DIRS = barrier chase-lev-deque dekker-fences linuxrwlocks mcs-lock mpmc-queue ms-queue spsc-queue

all :
	set -e; set -u; for d in $(DIRS); do (cd $$d; $(MAKE) ); done

clean :
	set -e; set -u; for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done
