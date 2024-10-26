blah: blah.o
	g++ blah.o -o blah # 3

blah.o: blah.cpp
	g++ -c blah.cpp -o blah.o # 2

# Обычно уже существует, но так проще показать
blah.c:
	echo "int main() { return 0; }" > blah.cpp # Runs first