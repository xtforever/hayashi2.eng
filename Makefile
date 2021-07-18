
all:
	for subdir in ch* ; \
	do \
	    echo "Making $$subdir" ; \
	    cd $$subdir ; make all ; cd .. ; \
	done

clean:
	for subdir in ch* ; \
	do \
	    echo "Cleaning $$subdir" ; \
	    cd $$subdir ; make clean ; cd .. ; \
	done

