Hana Park and Joshua Robinson Submission of Clisp Interpretor for CS403

To compile code, run make file by typing make into terminal.
1. Read top of "TestMe.lisp"

2.Format to run code
    2.2 Format to run files: ./lisp <file.lisp>
        --> ./lisp.exe testMe.lisp
    3.2 Format to run script: ./lisp

3. Input seen in testMe.lisp
4. Results can be seen below

GUIDE FOR RESULTS BELOW
"lisp>expresses actions"
"-result of action/write below"
"-if defining variable/list, result of write below"
    "-if defining function, no result"
    "-if evaluating, result of evaluation below"
    ""

-------------------------------------------------
"Test Cases"


"--------------------DEFINING/SETTING VARIABLES-------------------------------------"

"lisp>define a 10"

10

"lisp>set a 50"

50

"lisp>define pi 3.14"

3.140000

"lisp>define str sdf"

"sdf"

"lisp>set str please_change"

"please_change"

"--------------------------DEFINING LISTS-------------------------------------------"

"lisp>define list_num_1 (list 1 2 3)"

(1 2 3)

"lisp>define list_num_2 (list 23 4 521 56 7 8 52 432 67)"

(23 4 521 56 7 8 52 432 67)

"lisp>define list_num_3 (list 23.234 4.1 521.4 561.0 7.3 8.24 52.53 432.2 62.7)"

(23.234000 4.100000 521.400000 561.000000 7.300000 8.240000 52.530000 432.200000 62.700000)

"lisp>define list_string_1 hello, world, !"

("hello" "world" "!")

"lisp>define list_string_2 Roll, Tide, !, I, am, so, sad,that, the, United States of America did, not advance far, in the World Cup"

("Roll" "Tide" "!" "I" "am" "so" "sad" "that" "the" "United States of America did" "not advance far" "in the World Cup")

"lisp>list of numbers and strings in list_numstr_combo_1 s24, 34gs,5sd3,afd342,23 , 8, sd"

("s24" "34gs" "5sd3" "afd342" 23 8 "sd")

"--------------------------CONS LISTS, CDR LIST, CAR LIST-------------------------------------------"

"lisp>cons 1 2"

(1 . 2)

"lisp>cons 1 (cons 2 (cons 3 4))"

(1 2 3 . 4)

"lisp>cons 9 (cons 7 (cons 4 ()))"

(9 7 4)

"lisp>cons live (cons laugh (cons love ()))"

("live" "laugh" "love")

"lisp>write define words (cons live (cons laugh (cons love ())))"

"lisp>write words"

("live" "laugh" "love")



"REFERENCING list_num_2:"(23 4 521 56 7 8 52 432 67)

"lisp>car list_num_2"

23

"lisp>cdr list_num_2"

(4 521 56 7 8 52 432 67)

"lisp>(car (cdr list_num_2))"

4

"lisp>car (cdr(cdr list_num_2))"

521

"lisp>def ref cons 1 ()"



"lisp> cdr ref"

"lisp>NULL? cdr ref"

TRUE

"lisp>NULL? car ref"

FALSE

"--------------------------DEFINING FUNCTIONS-------------------------------------------"

"lisp>Defining circle-area"

"lisp>circle-area 3"

28.260000

"lisp>Defining factorial fact"

"lisp>fact 10"

3628800

"lisp>Defining fibonnaci fib"

"lisp>fib 10"

89

"lisp>circle-area(fact 10)"

41348114841600.000000

"lisp>Defining twice"

"lisp>twice 5"

10

"lisp>Defining repeat"

"lisp>((repeat twice )10)"

40

"lisp>(repeat (repeat twice)) 10)"

160

"lisp>(repeat (repeat (repeat twice))) 10)"

2560

"lisp>Defining Range"

"lisp>range 0 10"

(0 1 2 3 4 5 6 7 8 9)

"lisp>(map fib (range 0 10))"

(1 1 2 3 5 8 13 21 34 55)

"lisp>Defining cube"

"lisp> cube 5"

125

"lisp>Defining combine"

"lisp>Defining zip"

"lisp>zip (list 1 2 3 4) (list 5 6 7 8)"

((1 5) (2 6) (3 7) (4 8))

"lisp>Defining riff-shuffle"



"lisp>riff-shuffle (list 1 2 3 4 5 6 7 8)"

(1 5 2 6 3 7 4 8)

"lisp>riff-shuffle (riff-shuffle (riff-shuffle (list 1 2 3 4 5 6 7 8)))"

(1 2 3 4 5 6 7 8)

"lisp>let ((a 1) (b 2)) (+ a b)"

"--------------------------TESTING >, <, or?, and?, = -------------------------------------------"

"b = 11, c=3, d=50"

"lisp>let (and? (> b 2)(< c 10))"

TRUE

"lisp>let (or? (< b 2)(< c 10))"

TRUE

"lisp>let (and?(or? (< b 2)(< c 10))(= 50 d))"

TRUE

"lisp>let (and?(and? (< b 2)(< c 10))(= 50 d))"

FALSE

"b_list =(1 2 3), c_list =(5 6 7), d_list=(4 4 4)""lisp>(and?(and? (< (car b_list) (car c_list))(< c 10))(= 50 d))"

"lisp>let (and?(and? (< (car (cdr b_list)) (car (cdr c_list)))(< c 10))(= 50 d))"

FALSE"--------------------------ARITHMETIC(+, -, *, /)-------------------------------------------"

"b = 11, c=3, d=50"

"b_list =(1 2 3), c_list =(5 6 7), d_list=(4 4 4)"

"lisp> define e (+ b c)"

14

"lisp>define f (- (+ b c) (car b_list) )"

13

"lisp>define g (- (+ b c) (car (cdr b_list)) )"

12

"lisp>define h (- (+ b c) (car (cdr b_list)) )"

7

"define i (* (- b c) (car(cdr(cdr b_list)) ) )"

24

"--------------------------COND Function-------------------------------------------"

"var1=10, var2=11, var3 12, var4 13, var5 14"

"b_list =(1 2 3), c_list =(5 6 7), d_list=(4 4 4)"



"lisp>Testing Cond. Set 1"

"STRING_OUTPUT_FROM FIRST_CONDITION"

"lisp>Testing Cond. Set 2"

"STRING_OUTPUT_FROM FIRST_CONDITION"

"lisp>Testing Cond. Set 3"



"var6 ="100

"--------------------------TESTING NUMBER? -------------------------------------------"

"lisp>NUMBER? 5"

TRUE

"lisp>NUMBER? (list 1 34 5 2)"

FALSE

"In the test below, 4 and 34 are strings"

"lisp>NUMBER? (car (list 4 34 5 2))"

FALSE

"lisp>NUMBER? zip"

FALSE

"lisp>NUMBER? ()"

FALSE

"--------------------------TESTING SYMBOL? -------------------------------------------"



"REFERENCE list: "("five" 123 "ad234" "fd" "s" 434 "1ds" 3 "ss")

"lisp>SYMBOL? list_sym_test"

FALSE

"lisp>SYMBOL? 5"

FALSE

"lisp>SYMBOL? IF"

TRUE

"lisp>SYMBOL? zip function"

TRUE

"lisp>SYMBOL? (cdr(cons 1 2))"

FALSE

"lisp>SYMBOL? pi"

TRUE

"--------------------------TESTING LIST? -------------------------------------------"

"b_list =(1 2 3), c_list =(5 1 7), d_list=(4 4 4)"

"lisp>LIST? b_list"

TRUE

"lisp>LIST? (cdr(cons 1 2))"

TRUE

"lisp>LIST? fact function"

FALSE

"lisp>LIST? string somanytest"

FALSE

"lisp>LIST? number var1"

FALSE

"--------------------------TESTING EQ? -------------------------------------------"

"b_list =(1 2 3), c_list =(5 1 7), d_list=(4 4 4), e_list(4 4 4)"

"lisp>EQ? e_list d_list"

TRUE

"lisp>EQ? (car e_list) (car( cdr d_list))"

TRUE

"NOTE: second 5 is a string"

"lisp>EQ? 5 5"

FALSE

"lisp>define var7 5"

"lisp>EQ? var7 5"

TRUE

"lisp>EQ? (car (cdr (list 14 5 6))) 5"

TRUE

TRUE
