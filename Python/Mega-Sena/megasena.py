
import secrets  # mais seguro que o modulo random, não há seed
import array
import numpy as np
import matplotlib.pyplot as plt
from typing import Callable


def mega_sena_1(q_num: int = 6):

    my_array = []

    while len(my_array) < q_num:
        dezena = secrets.randbelow(60) + 1  # (0, 59) --> (1, 60)
        if dezena in my_array:
            continue
        my_array.append(dezena)

    my_array.sort()

    return my_array


def mega_sena_2(q_num: int = 6):

    my_array = set()

    while len(my_array) < q_num:
        my_array.add(secrets.randbelow(60) + 1)

    my_array = list(my_array)
    my_array.sort()

    return my_array


# comportamento mais parecido com o sorteio real, a dezena sorteada não é reposta
def mega_sena_3(q_num: int = 6):

    my_array = []
    dezena_1_60_array = list(range(1, 60 + 1))

    while len(my_array) < q_num:
        dezena = secrets.choice(dezena_1_60_array)
        dezena_1_60_array.remove(dezena)  # without replacement
        my_array.append(dezena)

    my_array.sort()

    return my_array


def mega_sena_np(q_num: int = 6):

    my_array = np.empty(shape=q_num, dtype=np.uint8)

    i = 0
    while i < q_num:
        dezena = secrets.randbelow(60) + 1
        # if np.where(my_array == dezena)[0].any():
        if dezena in my_array:  # bottleneck
            continue
        my_array[i] = dezena
        i += 1

    my_array.sort()

    return my_array


"""
def mega_sena_np_rng_integers(q_num: int = 6):

    secrets_seed = secrets.randbelow(2 ** 32 - 1)
    
    rng = np.random.default_rng(secrets_seed)
    my_array = rng.integers(low=1, high=60, size=q_num, endpoint=True, dtype=np.uint8)  # gets repeated numbers ('with replacement')

    return my_array
"""


def mega_sena_np_rng_choice(q_num: int = 6):

    secrets_seed = secrets.randbelow(2 ** 32 - 1)  # 32-bit unsigned
    rng = np.random.default_rng(secrets_seed)  # generator, .default_rng() is PRNG, providing a CSPRNG seed

    dezena_1_60_array = np.arange(1, 60 + 1, dtype=np.uint8)
    my_array = rng.choice(dezena_1_60_array, size=q_num, replace=False)

    my_array.sort()

    return my_array


def mega_sena_array(q_num: int = 6):

    my_array = array.array('B')

    while len(my_array) < q_num:
        dezena = secrets.randbelow(60) + 1  # (0, 59) --> (1, 60)
        if dezena in my_array:
            continue
        my_array.append(dezena)

    return my_array


def verify_qnum(q_num: int):

    if not isinstance(q_num, int):
        raise TypeError(f'{q_num} is not of int type.')

    valid_numbers = range(6, 15 + 1)
    if q_num not in valid_numbers:
        raise ValueError(f'Not a valid number for q_num parameter, please choose one of these numbers: {list(valid_numbers)}')

    return q_num

# wrapper for mega_sena_* functions
def mega_sena(q_num: int = 6, func: str = '3'):

    q_num = verify_qnum(q_num)

    func_dict = {'1': mega_sena_1, '2': mega_sena_2,
                 '3': mega_sena_3, 'np': mega_sena_np,
                 'np_rng_choice': mega_sena_np_rng_choice, 'array': mega_sena_array}

    if func not in func_dict.keys():
        raise ValueError(f'invalid value for func parameter, please choose one of these options: {list(func_dict.keys())}')
    else:
        mega_func = func_dict[func]

    return mega_func(q_num)


def distribution_plot(mega_func: Callable[[int], list] | Callable[[int], np.ndarray] | Callable[[int], array.array], q_num : int = 6, n_sorteios: int = 10000):

    q_num = verify_qnum(q_num)

    test_array = np.empty(shape=(n_sorteios, q_num), dtype=np.uint8)

    for num in range(n_sorteios):
        test_array[num, :] = mega_func(q_num)

    v_unique, v_counts = np.unique(test_array, return_counts=True)

    plt.figure(figsize=(15, 5))
    plt.bar(v_unique, v_counts)
    plt.xticks(v_unique)
    plt.title(f'Distribuição dos {q_num} números sorteados em {n_sorteios} jogos')
    plt.show()

    return


# distribution_plot(mega_sena, n_sorteios=100000)
