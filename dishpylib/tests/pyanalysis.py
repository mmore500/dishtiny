#!/bin/python3

from collections import Counter
import itertools as it
import unittest

from dishpylib.pyanalysis import count_hands_with_k_or_more_sets

class TestCountHandsWithKOrMoreSets(unittest.TestCase):

    def test_set_size_one(self):

        # k = 1
        # num_sets = 12
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=12,
                set_size=1,
                k=1,
            ),
            sum(
                sum(hand) >= 1
                for hand in it.combinations(
                    [1] * 12,
                    5,
                )
            ),
        )

        # k = 1
        # num_sets = 6
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=6,
                set_size=1,
                k=1,
            ),
            sum(
                sum(hand) >= 1
                for hand in it.combinations(
                    [0] * 6 + [1] * 6,
                    5,
                )
            ),
        )

        # k = 1
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=2,
                set_size=1,
                k=1,
            ),
            sum(
                sum(hand) >= 1
                for hand in it.combinations(
                    [0] * 10 + [1] * 2,
                    5,
                )
            ),
        )

        # k = 1
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=0,
                set_size=1,
                k=1,
            ),
            sum(
                sum(hand) >= 1
                for hand in it.combinations(
                    [0] * 12,
                    5,
                )
            ),
        )

        # k = 2
        # num_sets = 12
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=12,
                set_size=1,
                k=2,
            ),
            sum(
                sum(hand) >= 2
                for hand in it.combinations(
                    [1] * 12,
                    5,
                )
            ),
        )

        # k = 2
        # num_sets = 6
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=6,
                set_size=1,
                k=2,
            ),
            sum(
                sum(hand) >= 2
                for hand in it.combinations(
                    [0] * 6 + [1] * 6,
                    5,
                )
            ),
        )

        # k = 2
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=2,
                set_size=1,
                k=2,
            ),
            sum(
                sum(hand) >= 2
                for hand in it.combinations(
                    [0] * 10 + [1] * 2,
                    5,
                )
            ),
        )

        # k = 2
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=0,
                set_size=1,
                k=2,
            ),
            sum(
                sum(hand) >= 2
                for hand in it.combinations(
                    [0] * 12,
                    5,
                )
            ),
        )

        # k = 6
        # (not enough space in hand)
        # num_sets = 12
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=12,
                set_size=1,
                k=6,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 3
                for hand in it.combinations(
                    [1] * 12,
                    5,
                )
            ),
        )

        # k = 6
        # (not enough space in hand)
        # num_sets = 6
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=6,
                set_size=1,
                k=6,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 3
                for hand in it.combinations(
                    [0] * 6 + [1] * 6,
                    5,
                )
            ),
        )

        # k = 6
        # (not enough space in hand)
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=0,
                set_size=1,
                k=6,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 3
                for hand in it.combinations(
                    [0] * 12,
                    5,
                )
            ),
        )


    def test_set_size_two(self):

        # k = 1
        # num_sets = 6
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=6,
                set_size=2,
                k=1,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0,0,1,1,2,2,3,3,4,4,5,5],
                    5,
                )
            ),
        )

        # k = 1
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=2,
                set_size=2,
                k=1,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0,0,1,1,2,3,4,5,6,7,8,9],
                    5,
                )
            ),
        )

        # k = 1
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=0,
                set_size=2,
                k=1,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    range(12),
                    5,
                )
            ),
        )

        # k = 2
        # num_sets = 6
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=6,
                set_size=2,
                k=2,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    [0,0,1,1,2,2,3,3,4,4,5,5],
                    5,
                )
            ),
        )

        # k = 2
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=2,
                set_size=2,
                k=2,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    [0,0,1,1,2,3,4,5,6,7,8,9],
                    5,
                )
            ),
        )

        # k = 2
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=0,
                set_size=2,
                k=2,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    range(12),
                    5,
                )
            ),
        )

        # k = 3
        # (not enough space in hand)
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=5,
                num_sets=6,
                set_size=2,
                k=3,
            ),
            sum(
                sum(val == 2 for val in Counter(hand).values()) >= 3
                for hand in it.combinations(
                    [0,0,1,1,2,2,3,3,4,4,5,5],
                    5,
                )
            ),
        )


    def test_set_size_three(self):

        # k = 1
        # num_sets = 4
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=4,
                set_size=3,
                k=1,
            ),
            sum(
                sum(val == 3 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0,0,0,1,1,1,2,2,2,3,3,3],
                    6,
                )
            ),
        )

        # k = 1
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=2,
                set_size=3,
                k=1,
            ),
            sum(
                sum(val == 3 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0,0,0,1,1,1,2,3,4,5,6,7],
                    6,
                )
            ),
        )

        # k = 1
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=0,
                set_size=3,
                k=1,
            ),
            sum(
                sum(val == 3 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    range(12),
                    6,
                )
            ),
        )

        # k = 2
        # num_sets = 4
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=4,
                set_size=3,
                k=2,
            ),
            sum(
                sum(val == 3 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    [0,0,0,1,1,1,2,2,2,3,3,3],
                    6,
                )
            ),
        )

        # k = 2
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=2,
                set_size=3,
                k=2,
            ),
            sum(
                sum(val == 3 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    [0,0,0,1,1,1,2,3,4,5,6,7],
                    6,
                )
            ),
        )

        # k = 2
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=0,
                set_size=3,
                k=2,
            ),
            sum(
                sum(val == 3 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    range(12),
                    6,
                )
            ),
        )

        # k = 3
        # (not enough space in hand)
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=4,
                set_size=3,
                k=3,
            ),
            sum(
                sum(val == 3 for val in Counter(hand).values()) >= 3
                for hand in it.combinations(
                    [0,0,0,1,1,1,2,2,2,3,3,3],
                    6,
                )
            ),
        )


    def test_set_size_four(self):

        # k = 1
        # num_sets = 3
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=3,
                set_size=4,
                k=1,
            ),
            sum(
                sum(val == 4 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0,0,0,0,1,1,1,1,2,2,2,2],
                    6,
                )
            ),
        )

        # k = 1
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=2,
                set_size=4,
                k=1,
            ),
            sum(
                sum(val == 4 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0,0,0,0,1,1,1,1,2,3,4,5],
                    6,
                )
            ),
        )

        # k = 1
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=0,
                set_size=4,
                k=1,
            ),
            sum(
                sum(val == 4 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    range(12),
                    6,
                )
            ),
        )

        # k = 2
        # num_sets = 3
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=3,
                set_size=4,
                k=2,
            ),
            sum(
                sum(val == 4 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    [0,0,0,0,1,1,1,1,2,2,2,2],
                    6,
                )
            ),
        )

        # k = 2
        # num_sets = 2
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=2,
                set_size=4,
                k=2,
            ),
            sum(
                sum(val == 4 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    [0,0,0,0,1,1,1,1,2,3,4,5],
                    6,
                )
            ),
        )

        # k = 2
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=0,
                set_size=4,
                k=2,
            ),
            sum(
                sum(val == 4 for val in Counter(hand).values()) >= 2
                for hand in it.combinations(
                    range(12),
                    6,
                )
            ),
        )

        # k = 3
        # (not enough space in hand)
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=6,
                num_sets=3,
                set_size=4,
                k=3,
            ),
            sum(
                sum(val == 4 for val in Counter(hand).values()) >= 3
                for hand in it.combinations(
                    [0,0,0,0,1,1,1,1,2,2,2,2],
                    6,
                )
            ),
        )

    def test_set_size_eleven(self):

        # k = 1
        # num_sets = 1
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=11,
                num_sets=1,
                set_size=11,
                k=1,
            ),
            sum(
                sum(val == 11 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0] * 11 + [1],
                    11,
                )
            ),
        )

        # k = 1
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=11,
                num_sets=0,
                set_size=11,
                k=1,
            ),
            sum(
                sum(val == 11 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    range(12),
                    11,
                )
            ),
        )

    def test_set_size_twelve(self):

        # k = 1
        # num_sets = 1
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=12,
                num_sets=1,
                set_size=12,
                k=1,
            ),
            sum(
                sum(val == 12 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    [0] * 12,
                    12,
                )
            ),
        )

        # k = 1
        # num_sets = 0
        self.assertEqual(
            count_hands_with_k_or_more_sets(
                deck_size=12,
                hand_size=12,
                num_sets=0,
                set_size=12,
                k=1,
            ),
            sum(
                sum(val == 12 for val in Counter(hand).values()) >= 1
                for hand in it.combinations(
                    range(12),
                    12,
                )
            ),
        )


if __name__ == '__main__':
    unittest.main()
