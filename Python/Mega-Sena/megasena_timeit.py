
import timeit


def run_timeit(mycode_snippet: str, func_str: str, num: int = 10000):
    print(f"{func_str}: \t", timeit.timeit(setup=mysetup, stmt=mycode_snippet, number=num), 'sec')


file_py_name = "megasena.py"

with open(file_py_name, mode='r') as file:
    mysetup = file.read()

mysetup = mysetup.replace('plt.show()', '')  # remove to time it properly


func_list = ['1', '2', '3', 'np', 'np_rng_choice', 'array']

for f in func_list:
    mycode_param = f"func='{f}'"
    mycode = f"distribution_plot({mycode_param}, n_sorteios=100000)"
    run_timeit(mycode, mycode_param, 1)

# func='1':        0.7723654999863356 sec
# func='2':        0.7283532000146806 sec
# func='3':        0.904986800160259 sec
# func='np':       3.874983600107953 sec
# func='np_rng_choice':    5.769865799928084 sec
# func='array':    0.789967100135982 sec


print()
mycode_param = "func='np'"
mycode = f"mega_sena({mycode_param})"
run_timeit(mycode, mycode_param, 10000)
mysetup = mysetup.replace('if dezena in my_array:  # bottleneck\n            continue', '')
run_timeit(mycode, mycode_param, 10000)

# func='np':       0.36388760013505816 sec
# func='np':       0.05405810009688139 sec


print()
mycode_param = "func='np_rng_choice'"
mycode = f"mega_sena({mycode_param})"
run_timeit(mycode, mycode_param, 10000)  # bottleneck when creating the .default_rng() generator and using .choice()

# func='np_rng_choice':    0.5480897000525147 sec


print()
