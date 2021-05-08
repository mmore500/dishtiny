import math

def count_hands_with_k_or_more_sets(
    *, deck_size, hand_size, num_sets, set_size, k
):

    assert set_size

    num_fixed = set_size * k
    deck_free = deck_size - num_fixed
    hand_free = hand_size - num_fixed

    if k > num_sets or k > hand_size or deck_free < 0 or hand_free < 0:
        return 0

    if set_size == 1:
        return sum(
            math.comb(num_sets, k_)
            * math.comb(deck_size - num_sets, hand_size - k_)
            for k_ in range(k, num_sets + 1)
            if hand_size >= k_
        )

    return (
        math.comb(deck_free, hand_free)
        * math.comb(num_sets, k)
    ) - count_hands_with_k_or_more_sets(
        deck_size=deck_size,
        hand_size=hand_size,
        num_sets=num_sets,
        set_size=set_size,
        k=k+1,
    )
