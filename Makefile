database: record_keeper administrator

record_keeper: record_keeper.c
	gcc -o record_keeper record_keeper.c

administrator: administrator.c
	gcc -o administrator administrator.c