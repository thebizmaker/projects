import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import numpy as np
from sklearn.cluster import KMeans

from pca_example import load_data

COLOR_MAP = LinearSegmentedColormap.from_list('tab_blue_to_orange',
                                              ['tab:blue', 'tab:orange'], N=10)

if __name__ == '__main__':
    data, labels = load_data('both')
    data_2d = np.stack([np.min(data, axis=-1), np.max(data, axis=-1)], axis=-1)

    fig, axes = plt.subplots(3, sharex=True, sharey=True)
    for k, ax in zip([2, 3, 4], axes):
        model = KMeans(n_clusters=k)
        model.fit(data_2d)
        pred = model.predict(data_2d)
        ax.scatter(data_2d[:, 0], data_2d[:, 1], c=pred, cmap=COLOR_MAP)

    plt.show()
