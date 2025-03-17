import matplotlib.pyplot as plt
import pandas as pd
from sklearn.cluster import KMeans


if __name__ == '__main__':
    filepath = "customer_segmentation.csv"
    df = pd.read_csv(filepath)

    df_norm = (df - df.min()) / (df.max() - df.min())

    num_clusters = 3
    model = KMeans(n_clusters=num_clusters)
    model.fit(df_norm)
    df_norm['Cluster'] = model.predict(df_norm)

    for cls in range(num_clusters):
        cluster_data = df_norm[df_norm['Cluster'] == cls]
        plt.scatter(cluster_data['Age'],
        cluster_data['Annual_Income']
    )

    plt.xlabel('Age')
    plt.ylabel('Annual Income')
    plt.show()
