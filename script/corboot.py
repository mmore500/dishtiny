import numpy as np
from scipy import stats
from astropy.stats import bootstrap
from tqdm import tqdm

# a: two series to inspect for correlation
#    of the form dataframe[['trait1','trait2']]
# n: number of bootstrap samplings to perform s
def corboot(a, n=100000):
    # reshape to form [[x_1, y_1],[x_2,y_2],...]
    # draw len(dataframe) x,y pairs with replacement n times
    bootresult = np.swapaxes(bootstrap(np.array(a), n),2,1)
    # then perform a linear regression on each of the n sets
    # grab the slope value
    bootstats = [stats.linregress(b[0], b[1])[0] for b in tqdm(bootresult)]
    # print out slope at X percentile
    print("99.99% {}".format(np.percentile(bootstats,99.99)))
    print("99.9% {}".format(np.percentile(bootstats,99.9)))
    print("99% {}".format(np.percentile(bootstats,99)))
    print("95% {}".format(np.percentile(bootstats,95)))
    print("5% {}".format(np.percentile(bootstats,5)))
    print("1% {}".format(np.percentile(bootstats,1)))
    print("0.1% {}".format(np.percentile(bootstats,0.1)))
    print("0.01% {}".format(np.percentile(bootstats,0.01)))
    # return the n linear regression slopes
    return bootstats
