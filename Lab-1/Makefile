a_gen: a_gen.cpp
	g++ -Wall -o a_gen a_gen.cpp

a: a.cpp
	g++ -Wall -o a a.cpp

b_gen: b_gen.cpp
	g++ -Wall -o b_gen b_gen.cpp

b_test: b_test.cpp
	g++ -Wall -o b_test b_test.cpp -Wno-sign-compare

b: b.cpp
	g++ -Wall -o b b.cpp -Wno-sign-compare

clean:
	rm a a_test a_gen
	rm b b_test b_gen
	rm *.in
	rm *.out