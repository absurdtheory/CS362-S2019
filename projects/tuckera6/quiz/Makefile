default: testme

testme: testme.c
	gcc -o testme testme.c -coverage
	./testme

# run after testme
results:
	gcov testme.c -b

clean:
	rm testme

cleanall:
	rm testme *.gcda *.gcno *.gcov
