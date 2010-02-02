Tests to be run (order does not matter)

1) . ./zshtests_auto1.sh
2) . ./zshtests_auto2.sh
3) . ./greptest2.sh > greptest2.txt
4) . ./findtest1.sh > findtest1.txt

5) . ./findtest2.sh > findtest2.txt
6) . ./findtest3.sh > findtest3.txt

7) . ./greptest1.sh > greptest1.txt


Run Manually the following tests ( Do not redirect the output )

1. readtest_manual.sh
2. ls.sh
3) . ./externalcmd_test.sh - This test will not have any output, but should not hang the shell
4) zshtests_manual.sh
5) Enter atleast 10 commands at the zsh prompt and then execute . ./fctest_manual.sh
