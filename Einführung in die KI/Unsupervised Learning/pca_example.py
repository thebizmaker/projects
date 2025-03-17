import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.decomposition import PCA


def load_data(source: str = 'ES') -> tuple | np.ndarray:
    filepath_es = "temperature_data_es.csv"
    filepath_can = "temperature_data_can.csv"
    df_es = pd.read_csv(filepath_es).to_numpy().transpose()
    df_can = pd.read_csv(filepath_can).to_numpy().transpose()

    if source == 'ES':
        return df_es
    elif source == 'CAN':
        return df_can
    else:
        data = np.concatenate([df_es, df_can], axis=0)
        labels = np.concatenate([np.zeros(len(df_es)), np.ones(len(df_can))])
        return data, labels


if __name__ == '__main__':
    X, y = load_data()

    model = PCA(n_components=2)
    X_proj = model.fit_transform(X)

    fig, axes = plt.subplots(1, 2)
    axes[0].plot(X.transpose())
    axes[0].set_xlabel('Days')
    axes[0].set_ylabel('Temperature in °C')
    axes[1].scatter(X_proj[:, 0], X_proj[:, 1])
    axes[1].set_xlabel("First principal component")
    axes[1].set_ylabel("Second principal component")
    plt.show()
