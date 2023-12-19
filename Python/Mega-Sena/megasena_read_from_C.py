
import numpy as np
import matplotlib.pyplot as plt

FIG_SIZE = (15, 5)

def distribution_plot(array: np.ndarray):

    v_unique, v_counts = np.unique(array, return_counts=True)

    plt.figure(figsize=FIG_SIZE)
    plt.bar(v_unique, v_counts)
    plt.xticks(v_unique)
    plt.title(f'Dados da implementação em C\nDistribuição dos {array.shape[1]} números sorteados em {array.shape[0]} jogos')
    plt.show()

    return


def plot_fourier_graph(array: np.ndarray):

    num_samples = array.shape[0]
    samples_rate = 1. / 1.  # 6 sample / 1 jogo

    fourier = np.fft.rfft(array)  # numpy.ndarray
    freq = np.fft.rfftfreq(num_samples, d=1. / samples_rate)

    fourier[freq < 0.001] = 0  # Better visualization / p/ visualizar melhor o gráfico
    # zero in frequency domain represents the signal mean / DC signal in time domain

    plt.figure(figsize=FIG_SIZE)
    plt.plot(freq, fourier.real)
    # plt.plot(freq, fourier.imag)
    # plt.legend(["real", "imag"])
    plt.show()


filepath = "dados/megasena_C_test.csv"
# filepath_2 = "dados/megasena_C_test_2_sorted.csv"


# Carregando os números sorteados, o .csv veio da implementação em C (/dev/urandom)

array = np.genfromtxt(filepath, delimiter=',', dtype=np.uint8)
print('\n', array)

distribution_plot(array)


# Aplicando Fourier nos dados, será que há algum tipo de repetição / padrão se repetindo?

# array = array.sum(axis=1)  # soma os 6 numeros sorteados
# array = array.T.flatten()
array = array.flatten()
print('\n', array)

plot_fourier_graph(array)

