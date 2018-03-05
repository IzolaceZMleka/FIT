# Simple MAKEFILE
compile:
	mvn compile

run-wildfly:
	sh ./server/wildfly-11.0.0.Final/bin/standalone.sh &

all: compile run-wildfly
	mvn wildfly:deploy
