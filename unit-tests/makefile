
test : main.cpp event_generator_test.h
	g++ -pthread -I../streamer/src/main/jni -std=c++11 -g -ggdb3 $< -o $@

clean:
	@rm -f test

run: test
	@./test
