Build AutoTester in solution:
1. Open StartupSPASolution.sln file in Visual Studio 2019
2. Build AutoTester in `Release` mode
3. Upon successful build, the path to `AutoTester` binary will be `Team09\Code09\Release\AutoTester.exe`

Autotester (Powershell):
1. Ensure that code has been built successfully
2. Run Windows Powershell as Administrator
3. Run `cd Team09\Code09`
4. Run `execute_all_queries.bat`
5. Results of the tests can be found in `Team09\Tests09` directory

If the abovementioned .bat failed to run/generate output, you may manually run Autotester with the following command in directory `Team09\Code09`:
```
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
```

Windows Explorer (Alternative):
1. Locate the folder `Team09\Code09`
2. Run `execute_all_queries.bat`
3. Results of the tests can be found in `Team09\Tests09` directory