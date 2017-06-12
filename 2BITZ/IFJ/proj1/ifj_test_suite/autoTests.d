import std.file;
import std.process;
import std.regex;
import std.conv;
import std.stdio;
import std.path;

int main( string[] args ) {
	foreach( string filename; dirEntries( "tests", "*.c", SpanMode.shallow ) ) {
		writeln( "\n==== Testing using '", filename, "' ====" );

		string expectedReturnCode = filename.replaceFirst( "^.*_R([0-9]+)_.*$".regex, "$1" );
		if( expectedReturnCode == filename ) {
			writeln( "## ERROR: Wrong filename format" );
			return -1;
		}

		string filenameWithoutExtension = filename.stripExtension;
		Pid pid;

		if( exists( filenameWithoutExtension ~ "_cin.txt" ) )
			pid = spawnProcess( [ "bin/ifj_win_debug", filename ], File( filenameWithoutExtension ~ "_cin.txt", "r" ) );
		else
			pid = spawnProcess( [ "bin/ifj_win_debug", filename ] );

		int returnCode = pid.wait;

		if( returnCode != expectedReturnCode.to!int ) {
			writeln( "#### ERROR: Expected return code was ", expectedReturnCode, " but the actual one was ", returnCode, " ####" );
			return -1;
		}
		else {
			writeln( "==== OK (return code ", returnCode, ") ====" );
		}
	}

	writeln( "\n========================================\n=== Done. Everything OK, congrats :) ===\n========================================" );

	return 0;
}