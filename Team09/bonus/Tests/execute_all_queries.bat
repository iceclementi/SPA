@echo off
title System Testing

..\Release\AutoTester 01_single_procedure_source.txt 01_single_procedure_queries.txt 01.xml
..\Release\AutoTester 02_nested_while_if_source.txt 02_nested_while_if_queries.txt 02.xml
..\Release\AutoTester 03_wia_source.txt 03_wia_queries.txt 03.xml
..\Release\AutoTester 03_wia_minimal_source.txt 03_wia_queries.txt 03_minimal.xml
..\Release\AutoTester 04_attrRef_bool_tuple_source.txt 04_attrRef_bool_tuple_queries.txt 04.xml
..\Release\AutoTester 05_multiple_queries_source.txt 05_multiple_queries_queries.txt .05.xml
..\Release\AutoTester 06_stress_source.txt 06_stress_queries.txt 06.xml
..\Release\AutoTester 07_08_affects_extension_source.txt 07_affects_queries.txt 07.xml
..\Release\AutoTester 07_08_affects_extension_source.txt 08_extension_queries.txt 08.xml
..\Release\AutoTester 09_stress_source_a.txt 09_stress_queries_a.txt 09a.xml
..\Release\AutoTester 09_stress_source_b.txt 09_stress_queries_b.txt 09b.xml

echo FINISHED
pause
exit
