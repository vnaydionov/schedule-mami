
BUILT_SOURCES=LoginSession.cpp schedule_schema.sql

LoginSession.cpp: $(top_srcdir)/src/schedule_schema.xml
	@YBORM_GEN@ --domain $^ ./

schedule_schema.sql: $(top_srcdir)/src/schedule_schema.xml
	@YBORM_GEN@ --ddl $^ ${YBORM_DBTYPE} $@

export YBORM_DBTYPE=@YBORM_DBTYPE@

clean-local:
	-rm -f *.cpp *.h *.sql

