string func( string p1, int p2, bool p3, double p4 ) {
	string a;
	a = concat( p1, "asd" );
	return a;
}

int main() {
	string x;
	x = func( "lola", 3.4, 0-84, 5.2 );

	if( x == "lolasd" )
		return 0;
}