NOVAPROVA_CFLAGS= -Inovaprova/install_dir/include/novaprova
NOVAPROVA_LIBS= -Lnovaprova/install_dir/lib -lnovaprova -lstdc++ -lbfd -ldl -lrt -lxml2
CFLAGS= $(NOVAPROVA_CFLAGS)

novaprova:
	git clone https://github.com/novaprova/novaprova.git
	cd novaprova && autoreconf -iv && ./configure --prefix=$(pwd)/install_dir --disable-docs && make -j8 && make install

testrunner: test.o
	$(LINK.c) -o $@ $^ $(NOVAPROVA_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
