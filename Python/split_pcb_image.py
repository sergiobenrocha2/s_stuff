
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
# from skimage import data

def mask(image_np, color, tol):
  mask_list = list()
  for i in range(3):
    mask_list.append((my_image_np[:,:,i] > color[i]*(1-tol[i]/100)) & (my_image_np[:,:,i] < color[i]*(1+tol[i]/100)))
  return mask_list[0] & mask_list[1] & mask_list[2]

# Rodar 1 vez somente, nossa referencia
def print_mean(image_np):
  mean_val = list()
  for i in range(3):  # image_np.shape[2]
    mean_val.append(np.sum(image_np[:,:,i]) / np.count_nonzero(image_np[:,:,i]))
  print(f'\nColor mean (RGB): {mean_val[0]:.0f} {mean_val[1]:.0f} {mean_val[2]:.0f}')
  return;


# BMP files to open:
full_path = "D:/PCB_tmp/"
# file_name = "pcb00_1200dpi"
file_name = "tmp"
input_extension = ".bmp"
output_extension = ".png"
my_image = Image.open(full_path + file_name + input_extension)  # RGB 24-bits

# Import as NumPy array:
my_image_np = np.array(my_image)  # .max(): 255 (8-bit), .dtype: uint8, .shape: (6600, 3088, 3)

# Our PCB layers, it's a map key/layer
PCB_layers = dict()

# amarelo: [180 175 77], [196 192 116] (alguns ptos)
# marron das trilhas: [110, 85, 70]
# verde_musgo (remoção no layer copper): [86, 96, 69]

# marron das trilhas (aprox.):
#  [89, 71, 56] <- mais escuro
#  [104, 85, 69] <- media borrando
#  [110, 85, 70] <- media borrando

# verde (removendo):
#  [85, 90, 75] <--
#  [86, 96, 69]

# verde do PCB:
#  [72, 104, 63]
#  [73, 80, 63]
#  [96, 118, 69]

# estrutura desse dict: ["layer", [RGB values], [tolerances], "create layer or remove / add values from existing layer"]
mask_list = [["silkscreen", [180, 175, 77],  [20, 20, 30], "layer" ], # tol_ em %, tolerancias p/ cada camada
             ["copper",     [110,  85, 70],  [25, 20, 20], "layer" ],
             ["copper",     [ 86,  96, 69],  [ 7, 12, 25], "remove"],
             ["lead",       [130, 130, 130], [15, 15, 15], "layer" ],
             ["lead",       [178, 178, 178], [15, 15, 15], "add"   ],
             ["lead",       [216, 216, 216], [5,   5,  5], "add"   ]]

for value in mask_list:
  if value[3] == "layer":
    PCB_layers[value[0]] = my_image_np.copy()
    PCB_layers[value[0]][~mask(my_image_np, value[1], value[2])] = [0,0,0]
  elif value[3] == "remove":
    PCB_layers[value[0]][mask(PCB_layers[value[0]], value[1], value[2])] = [0,0,0]
  else:
    tmp = my_image_np.copy()
    tmp[~mask(my_image_np, value[1], value[2])] = [0,0,0]
    PCB_layers[value[0]] += tmp
  # print_mean(PCB_layers[value[0]])

PCB_layers["copper+lead"] = PCB_layers["copper"] + PCB_layers["lead"]

for key in PCB_layers.keys():
  plt.imshow(PCB_layers[key])
  plt.show()
  Image.fromarray(PCB_layers[key]).save(full_path + file_name + "__" + key + output_extension)

