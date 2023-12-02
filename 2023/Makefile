COMPILE = kotlinc -include-runtime -d
RUN = kotlin

.PHONY = clean none day%
.DEFAULT_GOAL := none 

day%: day%.jar
	$(RUN) $^

%.jar: utils.kt %.kt
	$(COMPILE) $@ $^

clean: 
	rm -f *.jar

none:
	echo "make day{n}"
