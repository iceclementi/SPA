@echo off
title System Testing

.\Release\AutoTester ..\Tests09\01_single_procedure_source.txt ..\Tests09\01_single_procedure_queries.txt ..\Tests09\01.xml
.\Release\AutoTester ..\Tests09\02_nested_while_if_source.txt ..\Tests09\02_nested_while_if_queries.txt ..\Tests09\02.xml
.\Release\AutoTester ..\Tests09\03_wia_source.txt ..\Tests09\03_wia_queries.txt ..\Tests09\03.xml
.\Release\AutoTester ..\Tests09\03_wia_minimal_source.txt ..\Tests09\03_wia_queries.txt ..\Tests09\03_minimal.xml
.\Release\AutoTester ..\Tests09\04_attrRef_bool_tuple_source.txt ..\Tests09\04_attrRef_bool_tuple_queries.txt ..\Tests09\04.xml
.\Release\AutoTester ..\Tests09\05_multiple_queries_source.txt ..\Tests09\05_multiple_queries_queries.txt ..\Tests09\05.xml
.\Release\AutoTester ..\Tests09\06_stress_source.txt ..\Tests09\06_stress_queries.txt ..\Tests09\06.xml
.\Release\AutoTester ..\Tests09\07_08_affects_extension_source.txt ..\Tests09\07_affects_queries.txt ..\Tests09\07.xml
.\Release\AutoTester ..\Tests09\09_stress_source_a.txt ..\Tests09\09_stress_queries_a.txt ..\Tests09\09a.xml
.\Release\AutoTester ..\Tests09\09_stress_source_b.txt ..\Tests09\09_stress_queries_b.txt ..\Tests09\09b.xml

echo FINISHED
pause
exit