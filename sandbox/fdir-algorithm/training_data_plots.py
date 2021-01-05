import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import math

pd_df = pd.read_csv("data/webmust_labeled/perfect_training_set.csv")
quat_euler_df = pd.read_csv("data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv")


#########################################
# 2 dimension using PD3 and PD6 values. #
#########################################

# Round values.
x = round(pd_df['PD3'], 1)
y = round(pd_df['PD6'], 1)

# The camera state: ON/OFF.
hd_camera_state = pd_df['HD_CAMERA_STATE']

# Color ON states blue and OFF states red.
col = np.where(hd_camera_state == 1, 'b', 'r')

# Plot.
plt.scatter(x, y, c=col)

# Show.
plt.title("HD Camera State - 2D")
plt.show()

##################################
# 3 dimension with Euler angles. #
##################################

x_euler = round(quat_euler_df['x_euler'], 0)
y_euler = round(quat_euler_df['y_euler'], 0)
z_euler = round(quat_euler_df['z_euler'], 0)

# The camera state: ON/OFF.
hd_camera_state = quat_euler_df['HD_CAMERA_STATE']

# Color ON states blue and OFF states red.
col3d = np.where(hd_camera_state == 1, 'b', 'r')

# Plot.
fig = plt.figure()
ax = Axes3D(fig)
ax.scatter(x_euler, y_euler, z_euler, c=col3d)

# Show.
plt.title("HD Camera State - 3D")
plt.show()


#########################################################
# 2 dimension using polynomial transformation function. #
#########################################################

def kernel_linear(x, y):
    return x*y

def kernel_polynomial(x, y):
    return pow(x * y + 1, 2)

def kernel_rbf(x, y):
    gamma = 1
    return pow(math.e, -gamma * pow(abs(x-y), 2)) 


kernel_tricks = [kernel_linear, kernel_polynomial, kernel_rbf]


for kernel_function in kernel_tricks:

    # Round values.
    x = round(pd_df['PD3'], 1)
    y = round(pd_df['PD6'], 1)

    # Apply kernel trick.
    y = kernel_function(x, y)

    # The camera state: ON/OFF.
    hd_camera_state = pd_df['HD_CAMERA_STATE']

    # Color ON states blue and OFF states red.
    col = np.where(hd_camera_state == 1, 'b', 'r')

    # Plot.
    plt.scatter(x, y, c=col)

    # Show.
    plt.title("Kernel Trick")
    plt.show()