# Resources:
    
# https://code.likeagirl.io/finding-dominant-colour-on-an-image-b4e075f98097

# Import required libraries
import cv2
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

def find_histogram(clt):
    """
    create a histogram with k clusters
    :param: clt
    :return:hist
    """
    numLabels = np.arange(0, len(np.unique(clt.labels_)) + 1)
    (hist, _) = np.histogram(clt.labels_, bins=numLabels)

    hist = hist.astype("float")
    hist /= hist.sum()

    return hist

def plot_colors2(hist, centroids):
    bar = np.zeros((50, 300, 3), dtype="uint8")
    startX = 0

    for (percent, color) in zip(hist, centroids):
        # plot the relative percentage of each cluster
        endX = startX + (percent * 300)
        cv2.rectangle(bar, (int(startX), 0), (int(endX), 50),
                      color.astype("uint8").tolist(), -1)
        startX = endX

    # return the bar chart
    return bar

plt.ion()
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
camera = cv2.VideoCapture(0)


while(1):
    # Camera reading
    _, img = camera.read()
    imgCopy = img # copy of img
    
    # Dimensions 
    width = 640
    height = 480

    # Rectangle object
    cv2.rectangle(img, (int(width*0.24), int(height*0.24)), (int(width*0.76), int(height*0.76)), (255,255,255),2)

    # Show camera frame
    cv2.imshow('frame', img)

    # New dimensions
    newHeight1 = int(height*0.2)
    newHeight2 = int(height*0.7)
    newWidth1 = int(width*0.2)
    newWidth2 = int(width*0.7)
    img = imgCopy[newHeight1:newHeight2, int(newWidth1*0.5):int(newWidth2*0.5)]
    
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img = img.reshape((img.shape[0] * img.shape[1],3))
    
    # Cluster finder
    clt = KMeans(n_clusters = 5) # Shows the 5 most dominant colors
    clt.fit(img)

    # Histogram generation
    histogram = find_histogram(clt)
    
    # Plotting
    bar = plot_colors2(histogram, clt.cluster_centers_)
    plt.imshow(bar)

    fig.canvas.draw()
    fig.canvas.flush_events()
    
    # To kill
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break
cv2.destroyAllWindows()
