import matplotlib.pyplot as plt
import numpy as np

def draw_alternating_circle(n_segments=60, radius=1.0):
    fig, ax = plt.subplots(figsize=(12, 12), subplot_kw={'aspect': 'equal'})

    for i in range(n_segments):
        theta1 = (360 / n_segments) * i
        theta2 = theta1 + (360 / n_segments)
        color = 'black' if i % 2 == 0 else 'white'
        wedge = plt.matplotlib.patches.Wedge(center=(0, 0), r=radius,
                                              theta1=theta1, theta2=theta2,
                                              facecolor=color, edgecolor='gray')
        ax.add_patch(wedge)

    ax.set_xlim(-radius, radius)
    ax.set_ylim(-radius, radius)
    ax.axis('off')
    plt.tight_layout()
    fig.savefig("design.png", dpi=900, bbox_inches='tight', pad_inches=0)
    plt.close(fig)

draw_alternating_circle()

