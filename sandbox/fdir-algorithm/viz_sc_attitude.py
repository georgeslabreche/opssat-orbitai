from math import cos, sin, radians

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import numpy as np

from scipy.spatial.transform import Rotation as ro


def draw_axis(ax):
    # x, y, z
    ax.quiver(0, 0, 0, 1, 0, 0, length=0.08, arrow_length_ratio=0.1, normalize=True, color="red", label="X")
    ax.quiver(0, 0, 0, 0, 1, 0, length=0.08, arrow_length_ratio=0.1, normalize=True, color="green", label="Y")
    ax.quiver(0, 0, 0, 0, 0, 1, length=0.08, arrow_length_ratio=0.1, normalize=True, color="blue", label="Z")


if __name__ == '__main__':
    # prepare
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_axis_off()

    draw_axis(ax)

    # read attitude
    df = pd.read_csv("data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv", index_col=0)
    df.index = pd.to_datetime(df.index, format="%Y-%m-%d %H:%M:%S.%f")

    def animate(i):
        ax.clear()
        ax.set_axis_off()

        draw_axis(ax)

        # compute spacecraft attitude
        sc = np.array([[1, 0, 0], [0, 1, 0], [0, 0, 1]])
        rotation = ro.from_quat([df.iloc[i, 1], df.iloc[i, 2], df.iloc[i, 3], df.iloc[i, 0]])
        sc = rotation.apply(sc)

        # draw spacecraft attitude
        # draw x
        axis=0
        ax.quiver(0, 0, 0, sc[axis][0], sc[axis][1], sc[axis][2], length=0.08, arrow_length_ratio=0.1,
                  normalize=True, color="lightcoral", label="SC_x")
        # draw y
        axis=1
        ax.quiver(0, 0, 0, sc[axis][0], sc[axis][1], sc[axis][2], length=0.08, arrow_length_ratio=0.1,
                  normalize=True, color="yellowgreen", label="SC_y")
        # draw z
        axis=2
        ax.quiver(0, 0, 0, sc[axis][0], sc[axis][1], sc[axis][2], length=0.08, arrow_length_ratio=0.1,
                  normalize=True, color="skyblue", label="SC_z")
        plt.legend()

    # show
    ani = animation.FuncAnimation(fig, animate, interval=500)
    plt.show()
