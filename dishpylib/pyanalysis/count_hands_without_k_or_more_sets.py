import math

from .count_hands_with_k_or_more_sets import count_hands_with_k_or_more_sets

def count_hands_without_k_or_more_sets(**kwargs):
    return math.comb(
        kwargs['deck_size'],
        kwargs['hand_size']
    ) - count_hands_with_k_or_more_sets(**kwargs)
