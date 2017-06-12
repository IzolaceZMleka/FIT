int foo( int p1 ) {
	return p1;
}

int main() {
	int a;
	a = foo( 0 );

	int b;
	b = foo( a );

	return 0;
}