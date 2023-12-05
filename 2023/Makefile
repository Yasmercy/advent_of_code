COMPILE = kotlinc -include-runtime -d
RUN = kotlin

.PRECIOUS: day%.jar
.PHONY: day% clean none
.DEFAULT_GOAL := none

day%: day%.jar
	$(RUN) $^

day%.jar: utils.kt day%.kt
	$(COMPILE) $@ $^

clean:
	rm -f *.jar

none:
	echo "make day{n}"

